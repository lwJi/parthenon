//========================================================================================
// (C) (or copyright) 2024. Triad National Security, LLC. All rights reserved.
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
#ifndef MESH_FOREST_RELATIVE_ORIENTATION_HPP_
#define MESH_FOREST_RELATIVE_ORIENTATION_HPP_

#include <array>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "basic_types.hpp"
#include "defs.hpp"
#include "mesh/forest/logical_location.hpp"
#include "utils/bit_hacks.hpp"
#include "utils/cell_center_offsets.hpp"
#include "utils/indexer.hpp"

namespace parthenon {
namespace forest {

struct RelativeOrientation {
  KOKKOS_INLINE_FUNCTION
  RelativeOrientation()
      : dir_connection{0, 1, 2}, dir_flip{false, false, false}, ncells{-1} {};

  void SetDirection(Direction origin, Direction neighbor, bool reversed = false) {
    dir_connection[static_cast<uint>(origin)] = static_cast<uint>(neighbor);
    dir_flip[static_cast<uint>(origin)] = reversed;
  }

  LogicalLocation Transform(const LogicalLocation &loc_in,
                            std::int64_t destination) const;
  LogicalLocation TransformBack(const LogicalLocation &loc_in, std::int64_t origin) const;

  using buf_indices_t = std::tuple<int, int, int, int, int, int, int>;

  KOKKOS_FORCEINLINE_FUNCTION
  buf_indices_t Transform(TopologicalType tt, int iel, std::array<int, 6> tuvkji_in) {
    // TODO(LFR): Non-scalar quantities should pick up a possible sign flip due to the
    // coordinate transformation,
    //            need to thread this through
    std::array<int, 6> tuvkji;
    for (int dir = 0; dir < 3; ++dir) {
      tuvkji[3 + dir] = dir_flip[dir] ? tuvkji_in[3 + dir_connection[dir]]
                                      : ncells - tuvkji_in[3 + dir_connection[dir]];
      tuvkji[dir] = tuvkji_in[dir];
    }
    if (tt == TopologicalType::Face || tt == TopologicalType::Edge)
      return {dir_connection[iel], tuvkji[0], tuvkji[1], tuvkji[2],
              tuvkji[3],           tuvkji[4], tuvkji[5]};
    return {0, tuvkji[0], tuvkji[1], tuvkji[2], tuvkji[3], tuvkji[4], tuvkji[5]};
  }

  bool use_offset = false;
  std::array<int, 3> offset;
  std::array<int, 3> dir_connection;
  std::array<bool, 3> dir_flip;
  int ncells;
};
} // namespace forest
} // namespace parthenon

#endif // MESH_FOREST_RELATIVE_ORIENTATION_HPP_
