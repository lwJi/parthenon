//========================================================================================
// (C) (or copyright) 2020-2024. Triad National Security, LLC. All rights reserved.
//
// This program was produced under U.S. Government contract 89233218CNA000001 for Los
// Alamos National Laboratory (LANL), which is operated by Triad National Security, LLC
// for the U.S. Department of Energy/National Nuclear Security Administration. All rights
// in the program are reserved by Triad National Security, LLC, and the U.S. Department
// of Energy/National Nuclear Security Administration. The Government is granted for
// itself and others acting on its behalf a nonexclusive, paid-up, irrevocable worldwide
// license in this material to reproduce, prepare derivative works, distribute copies to
// the public, perform publicly and display publicly, and to permit others to do so.
//========================================================================================

#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "basic_types.hpp"
#include "interface/metadata.hpp"
#include "interface/state_descriptor.hpp"
#include "utils/error_checking.hpp"

namespace parthenon {

void Packages_t::Add(const std::shared_ptr<StateDescriptor> &package) {
  const auto &name = package->label();
  PARTHENON_REQUIRE_THROWS(packages_.count(name) == 0,
                           "Package name " + name + " must be unique.");
  packages_[name] = package;
  return;
}

class VariableProvider {
 public:
  virtual void AddPrivate(const std::string &package, const std::string &base_name,
                          const Metadata &metadata) = 0;
  virtual void AddProvides(const std::string &package, const std::string &base_name,
                           const Metadata &metadata) = 0;
  virtual void AddOverridable(const std::string &base_name, Metadata &metadata) = 0;
};

// Helper class for ResolvePackages
class DependencyTracker {
 public:
  bool Provided(const std::string &base_name) {
    return provided_vars.count(base_name) > 0;
  }

  void Categorize(const std::string &package, const std::string &base_name,
                  const Metadata &metadata, VariableProvider *pvp) {
    auto dependency = metadata.Role();
    if (dependency == Metadata::Private) {
      pvp->AddPrivate(package, base_name, metadata);
    } else if (dependency == Metadata::Provides) {
      if (Provided(base_name)) {
        PARTHENON_THROW("Variable " + base_name + " provided by multiple packages");
      }
      provided_vars.insert(base_name);
      pvp->AddProvides(package, base_name, metadata);
    } else if (dependency == Metadata::Requires) {
      depends_vars.insert(base_name);
    } else if (dependency == Metadata::Overridable) {
      if (overridable_meta.count(base_name) == 0) {
        overridable_meta[base_name] = {metadata};
      }
      // only update overridable_vars count once
      if (overridable_meta.at(base_name).size() == 1) {
        overridable_vars[base_name] += 1; // using value initialization of ints = 0
      }
    } else {
      PARTHENON_THROW("Unknown dependency");
    }
  }

  template <typename Collection>
  void CategorizeCollection(const std::string &package, const Collection &c,
                            VariableProvider *pvp) {
    for (auto &pair : c) {
      const auto &base_name = pair.first;
      auto &metadata = pair.second;
      Categorize(package, base_name, metadata, pvp);
    }
  }

  void CheckRequires() {
    for (auto &v : depends_vars) {
      if (!Provided(v) && overridable_vars.count(v) == 0) {
        std::stringstream ss;
        ss << "Variable " << v
           << " registered as required, but not provided by any package!" << std::endl;
        PARTHENON_THROW(ss);
      }
    }
  }

  void CheckOverridable(VariableProvider *pvp) {
    std::unordered_set<std::string> cache;
    for (auto &pair : overridable_vars) {
      const auto &base_name = pair.first;
      auto &count = pair.second;
      if (!Provided(base_name)) {
        if (count > 1) {
          std::stringstream ss;
          ss << "Variable " << base_name
             << " registered as overridable multiple times, but never provided."
             << " This results in undefined behaviour as to which package will provide"
             << " it." << std::endl;
          PARTHENON_DEBUG_WARN(ss);
        }
        auto &mvec = overridable_meta[base_name];
        for (auto &metadata : mvec) {
          pvp->AddOverridable(base_name, metadata);
        }
      }
    }
  }

 private:
  std::unordered_set<std::string> provided_vars;
  std::unordered_set<std::string> depends_vars;

  std::unordered_map<std::string, int> overridable_vars;
  std::unordered_map<std::string, std::vector<Metadata>> overridable_meta;
};

// Helper functions for adding vars
// closures by reference
class FieldProvider : public VariableProvider {
 public:
  explicit FieldProvider(Packages_t &packages, std::shared_ptr<StateDescriptor> &sd)
      : packages_(packages), state_(sd) {}
  void AddPrivate(const std::string &package, const std::string &base_name,
                  const Metadata &metadata) {
    bool added = false;
    const std::string new_name = package + internal_varname_seperator + base_name;
    auto pkg = packages_.Get(package);
    if (metadata.IsSet(Metadata::Sparse)) {
      const auto &src_pool = pkg->GetSparsePool(base_name);
      added = state_->AddSparsePool(new_name, src_pool);
    } else {
      auto controller = packages_.Get(package)->GetFieldController(base_name);
      added = state_->AddField(new_name, metadata, controller);
    }

    PARTHENON_REQUIRE_THROWS(added, "Couldn't add private field '" + base_name +
                                        "' to resolved state");
  }
  void AddProvides(const std::string &package, const std::string &base_name,
                   const Metadata &metadata) {
    bool added = false;
    auto pkg = packages_.Get(package);
    if (metadata.IsSet(Metadata::Sparse)) {
      const auto &pool = pkg->GetSparsePool(base_name);
      added = state_->AddSparsePool(pool);
    } else {
      auto controller = packages_.Get(package)->GetFieldController(base_name);
      added = state_->AddField(base_name, metadata, controller);
    }

    PARTHENON_REQUIRE_THROWS(added, "Couldn't add provided field '" + base_name +
                                        "' to resolved state");
  }
  void AddOverridable(const std::string &base_name, Metadata &metadata) {
    // we don't know which package this pool is coming from, so we need to search for it
    std::shared_ptr<StateDescriptor> pkg;
    bool found = false;
    for (auto &pair : packages_.AllPackages()) {
      pkg = pair.second;
      if (pkg->SparseBaseNamePresent(base_name) || pkg->FieldPresent(base_name)) {
        found = true;
        break;
      }
    }
    PARTHENON_REQUIRE_THROWS(found, "Cound't find overridable field " + base_name);
    bool added = false;
    if (metadata.IsSet(Metadata::Sparse)) {
      const auto &pool = pkg->GetSparsePool(base_name);
      added = state_->AddSparsePool(pool);
    } else {
      for (auto &pair : packages_.AllPackages()) {
        auto &package = pair.second;
        if (package->FieldPresent(base_name)) {
          auto controller = package->GetFieldController(base_name);
          added = state_->AddField(base_name, metadata, controller);
          break;
        }
      }
    }

    PARTHENON_REQUIRE_THROWS(added, "Couldn't add overridable field '" + base_name +
                                        "' to resolved state");
  }

 private:
  Packages_t &packages_;
  std::shared_ptr<StateDescriptor> &state_;
};

class SwarmProvider : public VariableProvider {
 public:
  SwarmProvider(Packages_t &packages, std::shared_ptr<StateDescriptor> &sd)
      : packages_(packages), state_(sd) {}
  void AddPrivate(const std::string &package, const std::string &label,
                  const Metadata &metadata) {
    AddSwarm_(packages_.Get(package).get(), label,
              package + internal_varname_seperator + label, metadata);
  }
  void AddProvides(const std::string &package, const std::string &label,
                   const Metadata &metadata) {
    AddSwarm_(packages_.Get(package).get(), label, label, metadata);
  }
  void AddOverridable(const std::string &label, Metadata &metadata) {
    state_->AddSwarm(label, metadata);
    for (auto &pair : packages_.AllPackages()) {
      auto &package = pair.second;
      if (package->SwarmPresent(label)) {
        for (auto &pair : package->AllSwarmValues(label)) {
          state_->AddSwarmValue(pair.first, label, pair.second);
        }
        return;
      }
    }
  }

 private:
  void AddSwarm_(StateDescriptor *package, const std::string &swarm,
                 const std::string &swarm_name, const Metadata &metadata) {
    Metadata newm(metadata);
    newm.Set(Metadata::Swarm);
    state_->AddSwarm(swarm_name, newm);
    for (auto &p : package->AllSwarmValues(swarm)) {
      auto &val_name = p.first;
      auto &val_meta = p.second;
      state_->AddSwarmValue(val_name, swarm_name, val_meta);
    }
  }

  Packages_t &packages_;
  std::shared_ptr<StateDescriptor> &state_;
};

bool StateDescriptor::AddSwarmValue(const std::string &value_name,
                                    const std::string &swarm_name, const Metadata &m) {
  // Swarm variables are always Metadata::Particle
  Metadata newm(m);
  newm.Set(Metadata::Particle);
  if (!newm.IsSet(GetMetadataFlag())) newm.Set(GetMetadataFlag());

  if (swarmMetadataMap_.count(swarm_name) == 0) {
    throw std::invalid_argument("Swarm " + swarm_name + " does not exist!");
  }
  if (swarmValueMetadataMap_[swarm_name].count(value_name) > 0) {
    throw std::invalid_argument("Swarm value " + value_name + " already exists!");
  }
  swarmValueMetadataMap_[swarm_name][value_name] = newm;

  return true;
}

bool StateDescriptor::AddFieldImpl(const VarID &vid, const Metadata &m_in,
                                   const VarID &control_vid) {
  Metadata m = m_in; // Force const correctness

  const std::string &assoc = m.getAssociated();
  if (m.getAssociated() == "") {
    m.Associate(vid.label());
  }
  if (FieldPresent(vid.label()) || SparseBaseNamePresent(vid.label())) {
    return false; // this field has already been added
  } else {
    if (m.IsSet(Metadata::WithFluxes) && m.GetFluxName() == "") {
      std::vector<MetadataFlag> mFlags = {Metadata::OneCopy, Metadata::Flux};
      if (m.IsSet(Metadata::Sparse)) mFlags.push_back(Metadata::Sparse);
      if (m.IsSet(Metadata::Fine)) mFlags.push_back(Metadata::Fine);
      if (m.IsSet(Metadata::Cell))
        mFlags.push_back(Metadata::Face);
      else if (m.IsSet(Metadata::Face))
        mFlags.push_back(Metadata::Edge);
      else if (m.IsSet(Metadata::Edge))
        mFlags.push_back(Metadata::Node);

      Metadata mf;
      if (m.GetRefinementFunctions().label().size() > 0) {
        // Propagate custom refinement ops to flux field
        mf = Metadata(mFlags, m.Shape(), std::vector<std::string>(), std::string(),
                      m.GetRefinementFunctions());
      } else {
        mf = Metadata(mFlags, m.Shape());
      }
      auto fId = VarID{internal_fluxname + internal_varname_seperator + vid.base_name,
                       vid.sparse_id};
      AddFieldImpl(fId, mf, control_vid);
      m.SetFluxName(fId.label());
    }
    metadataMap_.insert({vid, m});
    refinementFuncMaps_.Register(m, vid.label());
    allocControllerReverseMap_.insert({vid, control_vid});
    // Add this variable to the set of unique IDs at the
    // earliest possible time
    Variable<Real>::GetUniqueID(vid.label());
  }

  return true;
}

bool StateDescriptor::AddSparsePoolImpl(const SparsePool &pool) {
  if (pool.pool().size() == 0) {
    return false;
  }

  if (FieldPresent(pool.base_name()) || SparseBaseNamePresent(pool.base_name())) {
    // this sparse variable has already been added
    return false;
  }

  sparsePoolMap_.insert({pool.base_name(), pool});
  refinementFuncMaps_.Register(pool.shared_metadata(), pool.base_name());

  std::string controller_base = pool.controller_base_name();
  if (controller_base == "") controller_base = pool.base_name();
  // add all the sparse fields
  for (const auto itr : pool.pool()) {
    if (!AddFieldImpl(VarID(pool.base_name(), itr.first), itr.second,
                      VarID(controller_base, itr.first))) {
      // a field with this name already exists, this would leave the StateDescriptor in an
      // inconsistent state, so throw
      PARTHENON_THROW("Couldn't add sparse field " +
                      VarID(pool.base_name(), itr.first).label());
    }
  }

  return true;
}

bool StateDescriptor::FlagsPresent(std::vector<MetadataFlag> const &flags,
                                   bool matchAny) {
  for (auto &pair : metadataMap_)
    if (pair.second.FlagsSet(flags, matchAny)) return true;

  for (auto &pair : swarmMetadataMap_)
    if (pair.second.FlagsSet(flags, matchAny)) return true;

  // TODO(JL): What about swarmValueMetadataMap_?

  return false;
}

std::ostream &operator<<(std::ostream &os, const StateDescriptor &sd) {
  os << "# Package: " << sd.label() << "\n"
     << "# ---------------------------------------------------\n"
     << "# Variables:\n"
     << "# Name\tMetadata flags\n"
     << "# ---------------------------------------------------\n";
  for (auto &pair : sd.metadataMap_) {
    auto &metadata = pair.second;
    if (!metadata.IsSet(Metadata::Sparse))
      os << std::left << std::setw(25) << pair.first.label() << " " << metadata << "\n";
  }
  os << "# ---------------------------------------------------\n"
     << "# Sparse Variables:\n"
     << "# Name\tsparse id\tMetadata flags\n"
     << "# ---------------------------------------------------\n";
  for (auto &pair : sd.metadataMap_) {
    auto &metadata = pair.second;
    if (metadata.IsSet(Metadata::Sparse))
      os << std::left << std::setw(25) << pair.first.label() << " " << metadata << "\n";
  }
  os << "# ---------------------------------------------------\n"
     << "# Swarms:\n"
     << "# Swarm\tValue\tmetadata\n"
     << "# ---------------------------------------------------\n";
  for (auto &pair : sd.swarmValueMetadataMap_) {
    auto &swarm = pair.first;
    auto &svals = pair.second;
    os << swarm << "\n";
    for (auto &p2 : svals) {
      auto &val = p2.first;
      auto &metadata = p2.second;
      os << std::left << std::setw(25) << ("    \t" + val + " ") << metadata << "\n";
    }
  }
  return os;
}

// Take a map going from variable 1 -> variable that controls variable 1 and invert it
// to give control variable -> list of variables controlled by control variable.
// TODO(LFR): Currently, calling this repeatedly will just add a controlled variable
// to the vector of a controlling variable repeatedly. I think this shouldn't cause any
// issues since allocating or deallocating a variable multiple times in a row is the
// same as allocating it or deallocating it once. That being said, it could be switched
// to an unordered_set from a vector so that variable names can only show up once. Also,
// it is not clear to me exactly what behavior this should have if invert control map is
// called more than once (I think the normal use case would be for just a single call
// during resolution of the combined state descriptor). It may be that we should be
// calling allocControllerMap_.clear() before starting the for_each loop.
void StateDescriptor::InvertControllerMap() {
  std::for_each(allocControllerReverseMap_.begin(), allocControllerReverseMap_.end(),
                [this](const auto &pair) {
                  auto var = pair.first.label();
                  auto cont = pair.second.label();
                  auto iter = allocControllerMap_.find(cont);
                  if (iter == allocControllerMap_.end()) {
                    allocControllerMap_.emplace(
                        std::make_pair(cont, std::vector<std::string>{var}));
                  } else {
                    iter->second.push_back(var);
                  }
                });
}

// Takes all packages and combines them into a single state descriptor
// containing all variables with conflicts resolved.  Note the new
// state descriptor DOES not have any of its function pointers set.
std::shared_ptr<StateDescriptor>
StateDescriptor::CreateResolvedStateDescriptor(Packages_t &packages) {
  auto state = std::make_shared<StateDescriptor>("parthenon::resolved_state");

  // The workhorse data structure. Uses sets to cache which variables
  // are of what type.
  DependencyTracker field_tracker;
  DependencyTracker swarm_tracker;
  // closures that provide functions for DependencyTracker
  FieldProvider field_provider(packages, state);
  SwarmProvider swarm_provider(packages, state);

  // Add private/provides variables. Check for conflicts among those.
  // Track dependent and overridable variables.
  for (auto &pair : packages.AllPackages()) {
    const auto &name = pair.first;
    auto &package = pair.second;

    // make metadata dictionary of all dense fields and sparse pools (they are treated on
    // the same level for the purpose of dependency resolution)
    Dictionary<Metadata> field_dict;

    for (const auto itr : package->AllFields()) {
      if (!itr.second.IsSet(Metadata::Sparse)) {
        field_dict.insert({itr.first.label(), itr.second});
      }
    }

    for (const auto itr : package->AllSparsePools()) {
      field_dict.insert({itr.first, itr.second.shared_metadata()});
    }

    // sort
    field_tracker.CategorizeCollection(name, field_dict, &field_provider);
    swarm_tracker.CategorizeCollection(name, package->AllSwarms(), &swarm_provider);

    // Add package registered boundary conditions
    for (int i = 0; i < 6; ++i)
      state->UserBoundaryFunctions[i].insert(state->UserBoundaryFunctions[i].end(),
                                             package->UserBoundaryFunctions[i].begin(),
                                             package->UserBoundaryFunctions[i].end());
    for (int i = 0; i < 6; ++i)
      state->UserSwarmBoundaryFunctions[i].insert(
          state->UserSwarmBoundaryFunctions[i].end(),
          package->UserSwarmBoundaryFunctions[i].begin(),
          package->UserSwarmBoundaryFunctions[i].end());
  }

  // check that dependent variables are provided somewhere
  field_tracker.CheckRequires();
  swarm_tracker.CheckRequires();

  // Treat overridable vars:
  // If a var is overridable and provided, do nothing.
  // If a var is overridable and unique, add it to the state.
  // If a var is overridable and not unique, add one to the state
  // and optionally throw a warning.
  field_tracker.CheckOverridable(&field_provider);
  swarm_tracker.CheckOverridable(&swarm_provider);

  state->InvertControllerMap();

  return state;
}

// Build a list of variables in the following order
// 1. fields requested by name (if present), in the order they're requested
// 2. non-sparse fields picked up by the provided Metadata::FlagCollection
// 3. sparse fields picked up by metadata
// For sparse fields, only those ids in sparse_ids are included, unless
// sparse_ids is empty, in which case all ids are included.
// Variable names are only added to the list once the first time they
// are encountered.
std::vector<std::string>
StateDescriptor::GetVariableNames(const std::vector<std::string> &requested_names,
                                  const Metadata::FlagCollection &flags,
                                  const std::vector<int> &sparse_ids) {
  std::unordered_set<int> sparse_ids_set(sparse_ids.begin(), sparse_ids.end());
  std::unordered_set<std::string> names;
  std::vector<std::string> names_vec;
  // first add names that are present
  for (const auto &name : requested_names) {
    if (FieldPresent(name)) {
      if (metadataMap_[VarID(name)].IsSet(Metadata::Sparse)) {
        PARTHENON_THROW("Cannot ask for variables by name with specific sparse ids");
      }
      if (names.count(name) == 0) names_vec.push_back(name);
      names.insert(name);
    } else if (SparseBaseNamePresent(name)) {
      const auto &sparse_pool = GetSparsePool(name);
      for (const auto &s : sparse_pool.pool()) {
        if (sparse_ids_set.empty() || sparse_ids_set.count(s.first) > 0) {
          if (names.count(MakeVarLabel(name, s.first)) == 0)
            names_vec.push_back(MakeVarLabel(name, s.first));
          names.insert(MakeVarLabel(name, s.first));
        }
      }
    }
  }

  // now add by metadata flags
  if (!flags.Empty()) {
    // first handle non-sparse fields
    for (const auto &v : metadataMap_) {
      auto name = (v.first).label();
      auto &meta = v.second;
      if (MetadataUtils::MatchFlags(flags, meta)) {
        if (!meta.IsSet(Metadata::Sparse)) {
          if (names.count(name) == 0) names_vec.push_back(name);
          names.insert(name);
        }
      }
    }
    // now handle sparse fields
    for (const auto &p : sparsePoolMap_) {
      auto &name = p.first;
      auto &pool = p.second;
      auto &meta = pool.shared_metadata();
      if (MetadataUtils::MatchFlags(flags, meta)) {
        for (const auto &s : pool.pool()) {
          if (sparse_ids_set.empty() || sparse_ids_set.count(s.first) > 0) {
            if (names.count(MakeVarLabel(name, s.first)) == 0)
              names_vec.push_back(MakeVarLabel(name, s.first));
            names.insert(MakeVarLabel(name, s.first));
          }
        }
      }
    }
  }

  return names_vec;
}
std::vector<std::string>
StateDescriptor::GetVariableNames(const std::vector<std::string> &requested_names,
                                  const std::vector<int> &sparse_ids) {
  return GetVariableNames(requested_names, Metadata::FlagCollection(), sparse_ids);
}
std::vector<std::string>
StateDescriptor::GetVariableNames(const Metadata::FlagCollection &flags,
                                  const std::vector<int> &sparse_ids) {
  return GetVariableNames({}, flags, sparse_ids);
}
std::vector<std::string>
StateDescriptor::GetVariableNames(const std::vector<std::string> &requested_names) {
  return GetVariableNames(requested_names, Metadata::FlagCollection(), {});
}
std::vector<std::string>
StateDescriptor::GetVariableNames(const Metadata::FlagCollection &flags) {
  return GetVariableNames({}, flags, {});
}

// Get the total length of this StateDescriptor's variables when packed
int StateDescriptor::GetPackDimension(const std::vector<std::string> &req_names,
                                      const Metadata::FlagCollection &flags,
                                      const std::vector<int> &sparse_ids) {
  std::vector<std::string> names = GetVariableNames(req_names, flags, sparse_ids);
  int dimension = 0;
  for (auto name : names) {
    const auto &meta = metadataMap_[VarID(name)];
    // if meta.Shape().size() < 1, then 'accumulate' will return the initialization value,
    // which is 1. Otherwise, this multiplies all elements present in 'Shape' to obtain
    // total length
    dimension += std::accumulate(meta.Shape().begin(), meta.Shape().end(), 1,
                                 [](auto a, auto b) { return a * b; });
  }
  return dimension;
}
int StateDescriptor::GetPackDimension(const std::vector<std::string> &req_names,
                                      const std::vector<int> &sparse_ids) {
  return GetPackDimension(req_names, Metadata::FlagCollection(), sparse_ids);
}
int StateDescriptor::GetPackDimension(const Metadata::FlagCollection &flags,
                                      const std::vector<int> &sparse_ids) {
  return GetPackDimension({}, flags, sparse_ids);
}
int StateDescriptor::GetPackDimension(const std::vector<std::string> &req_names) {
  return GetPackDimension(req_names, Metadata::FlagCollection(), {});
}
int StateDescriptor::GetPackDimension(const Metadata::FlagCollection &flags) {
  return GetPackDimension({}, flags, {});
}

} // namespace parthenon
