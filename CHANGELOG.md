# Changelog

## Current develop

### Added (new features/APIs/variables/...)
- [[PR 1148]](https://github.com/parthenon-hpc-lab/parthenon/pull/1148) Add `GetPackDimension` to `StateDescriptor` for calculating pack sizes before `Mesh` initialization
- [[PR 1143]](https://github.com/parthenon-hpc-lab/parthenon/pull/1143) Add tensor indices to VariableState, add radiation constant to constants, add TypeLists, allow for arbitrary containers for solvers
- [[PR 1140]](https://github.com/parthenon-hpc-lab/parthenon/pull/1140) Allow for relative convergence tolerance in BiCGSTAB solver.
- [[PR 1047]](https://github.com/parthenon-hpc-lab/parthenon/pull/1047) General three- and four-valent 2D forests w/ arbitrary orientations.
- [[PR 1130]](https://github.com/parthenon-hpc-lab/parthenon/pull/1130) Enable `parthenon::par_reduce` for MD loops with Kokkos 1D Range
- [[PR 1119]](https://github.com/parthenon-hpc-lab/parthenon/pull/1119) Formalize MeshData partitioning.
- [[PR 1128]](https://github.com/parthenon-hpc-lab/parthenon/pull/1128) Add cycle and nbtotal to hst
- [[PR 1099]](https://github.com/parthenon-hpc-lab/parthenon/pull/1099) Functionality for outputting task graphs in GraphViz format.
- [[PR 1091]](https://github.com/parthenon-hpc-lab/parthenon/pull/1091) Add vector wave equation example.
- [[PR 991]](https://github.com/parthenon-hpc-lab/parthenon/pull/991) Add fine fields.
- [[PR 1106]](https://github.com/parthenon-hpc-lab/parthenon/pull/1106) Add CMake options for turning on ASAN and HWASAN
- [[PR 1100]](https://github.com/parthenon-hpc-lab/parthenon/pull/1100) Custom refinement ops propagated to fluxes
- [[PR 1090]](https://github.com/parthenon-hpc-lab/parthenon/pull/1090) SMR with swarms
- [[PR 1079]](https://github.com/parthenon-hpc-lab/parthenon/pull/1079) Address XDMF/Visit Issues
- [[PR 1084]](https://github.com/parthenon-hpc-lab/parthenon/pull/1084) Properly free swarm boundary MPI requests
- [[PR 1020]](https://github.com/parthenon-hpc-lab/parthenon/pull/1020) Add bi- and trilinear interpolation routines
- [[PR 1081]](https://github.com/parthenon-hpc-lab/parthenon/pull/1081) Add GetSize and GetIndex to sparse pack
- [[PR 1026]](https://github.com/parthenon-hpc-lab/parthenon/pull/1026) Particle BCs without relocatable device code
- [[PR 1037]](https://github.com/parthenon-hpc-lab/parthenon/pull/1037) Add SwarmPacks
- [[PR 1068]](https://github.com/parthenon-hpc-lab/parthenon/pull/1068) Add ability to dump sparse pack contents as a string
- [[PR 1062]](https://github.com/parthenon-hpc-lab/parthenon/pull/1062) UserWorkBeforeRestartOutput #1062
- [[PR 1043]](https://github.com/parthenon-hpc-lab/parthenon/pull/1043) Unify flux correction with boundary communication, make fluxes variables, allow fluxes for non-cell fields
- [[PR 1060]](https://github.com/parthenon-hpc-lab/parthenon/pull/1060) Add the ability to request new MeshData/MeshBlockData objects by selecting variables by UID.
- [[PR 1039]](https://github.com/parthenon-hpc-lab/parthenon/pull/1039) Add ability to output custom coordinate positions for Visit/Paraview
- [[PR 1019]](https://github.com/parthenon-hpc-lab/parthenon/pull/1019) Enable output for non-cell-centered variables

### Changed (changing behavior/API/variables/...)
- [[PR 1105]](https://github.com/parthenon-hpc-lab/parthenon/pull/1105) Refactor parameter input for linear solvers
- [[PR 1078]](https://github.com/parthenon-hpc-lab/parthenon/pull/1078) Add reduction fallback in 1D. Add IndexRange overload for 1D par loops
- [[PR 1024]](https://github.com/parthenon-hpc-lab/parthenon/pull/1024) Add .outN. to history output filenames
- [[PR 1004]](https://github.com/parthenon-hpc-lab/parthenon/pull/1004) Allow parameter modification from an input file for restarts

### Fixed (not changing behavior/API/variables/...)
- [[PR 1152]](https://github.com/parthenon-hpc-lab/parthenon/pull/1152) Fix memory leak in task graph outputs related to `abi::__cxa_demangle`
- [[PR 1146]](https://github.com/parthenon-hpc-lab/parthenon/pull/1146) Fix an issue outputting >4GB single variables per rank
- [[PR 1144]](https://github.com/parthenon-hpc-lab/parthenon/pull/1144) Fix some restarts w/non-CC fields
- [[PR 1132]](https://github.com/parthenon-hpc-lab/parthenon/pull/1132) Fix regional dependencies for iterative task lists and make solvers work for arbirtrary MeshData partitioning
- [[PR 1139]](https://github.com/parthenon-hpc-lab/parthenon/pull/1139) only add --expt-relaxed-constexpr for COMPILE_LANGUAGE:CXX
- [[PR 1131]](https://github.com/parthenon-hpc-lab/parthenon/pull/1131) Make deallocation of fine and sparse fields work
- [[PR 1127]](https://github.com/parthenon-hpc-lab/parthenon/pull/1127) Add WithFluxes to IsRefined check
- [[PR 1111]](https://github.com/parthenon-hpc-lab/parthenon/pull/1111) Fix undefined behavior due to bitshift of negative number in LogicalLocation
- [[PR 1092]](https://github.com/parthenon-hpc-lab/parthenon/pull/1092) Updates to DataCollection and MeshData to remove requirement of predefining MeshBlockData
- [[PR 1113]](https://github.com/parthenon-hpc-lab/parthenon/pull/1113) Prevent division by zero
- [[PR 1112]](https://github.com/parthenon-hpc-lab/parthenon/pull/1112) Remove shared_ptr cycle in forest::Tree
- [[PR 1104]](https://github.com/parthenon-hpc-lab/parthenon/pull/1104) Fix reading restarts due to hidden ghost var
- [[PR 1098]](https://github.com/parthenon-hpc-lab/parthenon/pull/1098) Move to symmetrized logical coordinates and fix SMR bug
- [[PR 1095]](https://github.com/parthenon-hpc-lab/parthenon/pull/1095) Add missing include guards in hdf5 restart
- [[PR 1093]](https://github.com/parthenon-hpc-lab/parthenon/pull/1093) Fix forest size for symmetry dimensions
- [[PR 1089]](https://github.com/parthenon-hpc-lab/parthenon/pull/1089) Fix loading restart files without derefinement counter
- [[PR 1079]](https://github.com/parthenon-hpc-lab/parthenon/pull/1079) Address XDMF/Visit Issues
- [[PR 1088]](https://github.com/parthenon-hpc-lab/parthenon/pull/1088) Correctly fill fluxes for non-cell variables in SparsePacks
- [[PR 1083]](https://github.com/parthenon-hpc-lab/parthenon/pull/1083) Correctly fill VariableFluxPack for edge fluxes in 2D
- [[PR 1087]](https://github.com/parthenon-hpc-lab/parthenon/pull/1087) Make sure InnerLoopPatternTVR is resolved on device properly when it is the default loop pattern
- [[PR 1071]](https://github.com/parthenon-hpc-lab/parthenon/pull/1070) Fix bug in static mesh refinement related to redefinition of Mesh::root_level
- [[PR 1073]](https://github.com/parthenon-hpc-lab/parthenon/pull/1073) Fix bug in AMR and sparse restarts
- [[PR 1070]](https://github.com/parthenon-hpc-lab/parthenon/pull/1070) Correctly exclude flux vars from searches by default
- [[PR 1049]](https://github.com/parthenon-hpc-lab/parthenon/pull/1049) Catch task failures from threads
- [[PR 1058]](https://github.com/parthenon-hpc-lab/parthenon/pull/1058) Vector history not being output if no scalar history present
- [[PR 1057]](https://github.com/parthenon-hpc-lab/parthenon/pull/1057) Fix history output after restarts
- [[PR 1053]](https://github.com/parthenon-hpc-lab/parthenon/pull/1053) Set the correct root level on restart
- [[PR 1024]](https://github.com/parthenon-hpc-lab/parthenon/pull/1024) Add features to history output
- [[PR 1031]](https://github.com/parthenon-hpc-lab/parthenon/pull/1031) Fix bug in non-cell centered AMR

### Infrastructure (changes irrelevant to downstream codes)
- [[PR 1117]](https://github.com/parthenon-hpc-lab/parthenon/pull/1117) Enable CI pipelines on AMD GPUs with ROCM/HIP
- [[PR 1114]](https://github.com/parthenon-hpc-lab/parthenon/pull/1114) Enable sanitizers for extended CI host build
- [[PR 1123]](https://github.com/parthenon-hpc-lab/parthenon/pull/1123) Default initialize ProResInfo.dir
- [[PR 1121]](https://github.com/parthenon-hpc-lab/parthenon/pull/1121) Default initialize BndInfo.dir
- [[PR 1116]](https://github.com/parthenon-hpc-lab/parthenon/pull/1116) Fix NumPy 2.0 test script breakage
- [[PR 1055]](https://github.com/parthenon-hpc-lab/parthenon/pull/1055) Refactor mesh constructors
- [[PR 1066]](https://github.com/parthenon-hpc-lab/parthenon/pull/1066) Re-introduce default loop patterns and exec spaces
- [[PR 1064]](https://github.com/parthenon-hpc-lab/parthenon/pull/1064) Forbid erroneous edge case when adding MeshData on a partition
- [[PR 1035]](https://github.com/parthenon-hpc-lab/parthenon/pull/1035) Fix multigrid infrastructure to work with forest
- [[PR 1048]](https://github.com/parthenon-hpc-lab/parthenon/pull/1048) Tiny fixes to custom coords logic
- [[PR 1028]](https://github.com/parthenon-hpc-lab/parthenon/pull/1028) Internal reorganization of LogicalLocation files
- [[PR 1009]](https://github.com/parthenon-hpc-lab/parthenon/pull/1009) Move from a single octree to a forest of octrees


### Removed (removing behavior/API/varaibles/...)
- [[PR 1108]](https://github.com/parthenon-hpc-lab/parthenon/pull/1108) Remove NaN payload tags infrastructure

### Incompatibilities (i.e. breaking changes)
- [[PR 1135]](https://github.com/parthenon-hpc-lab/parthenon/pull/1135) Drivers now correctly return DriverStatus::timeout on hittig walltime limit
- [[PR 1128]](https://github.com/parthenon-hpc-lab/parthenon/pull/1128) Add cycle and nbtotal to hst
- [[PR 1108]](https://github.com/parthenon-hpc-lab/parthenon/pull/1108) Remove NaN payload tags infrastructure
- [[PR 1026]](https://github.com/parthenon-hpc-lab/parthenon/pull/1026) Particle BCs without relocatable device code
- [[PR 1037]](https://github.com/parthenon-hpc-lab/parthenon/pull/1037) Add SwarmPacks
- [[PR 1042]](https://github.com/parthenon-hpc-lab/parthenon/pull/1042) Use Offset class and clean up of NeighborBlock
- [[PR 1019]](https://github.com/parthenon-hpc-lab/parthenon/pull/1019) Remove support for file formats < 3


## Release 24.03
Date: 2024-03-21

### Added (new features/APIs/variables/...)
- [[PR 852]](https://github.com/parthenon-hpc-lab/parthenon/pull/852) Add Mesh version of UserWorkBeforeOutput
- [[PR 998]](https://github.com/parthenon-hpc-lab/parthenon/pull/998) tensor indices added to sparse pack
- [[PR 999]](https://github.com/parthenon-hpc-lab/parthenon/pull/999) Add a post-initialization hook
- [[PR 987]](https://github.com/parthenon-hpc-lab/parthenon/pull/987) New tasking infrastructure and capabilities
- [[PR 969]](https://github.com/parthenon-hpc-lab/parthenon/pull/969) New macro-based auto-naming of profiling regions and kernels
- [[PR 981]](https://github.com/parthenon-hpc-lab/parthenon/pull/981) Add IndexSplit
- [[PR 983]](https://github.com/parthenon-hpc-lab/parthenon/pull/983) Add Contains to SparsePack
- [[PR 968]](https://github.com/parthenon-hpc-lab/parthenon/pull/968) Add per package registration of boundary conditions
- [[PR 948]](https://github.com/parthenon-hpc-lab/parthenon/pull/948) Add solver interface and update Poisson geometric multi-grid example
- [[PR 996]](https://github.com/parthenon-hpc-lab/parthenon/pull/996) Remove dynamic allocations from swarm particle creation

### Changed (changing behavior/API/variables/...)
- [[PR 973]](https://github.com/parthenon-hpc-lab/parthenon/pull/973) Multigrid performance upgrades

### Fixed (not changing behavior/API/variables/...)
- [[PR1023]](https://github.com/parthenon-hpc-lab/parthenon/pull/1023) Fix broken param of a scalar bool
- [[PR1012]](https://github.com/parthenon-hpc-lab/parthenon/pull/1012) Remove accidentally duplicated code
- [[PR992]](https://github.com/parthenon-hpc-lab/parthenon/pull/992) Allow custom PR ops with sparse pools
- [[PR988]](https://github.com/parthenon-hpc-lab/parthenon/pull/988) Fix bug in neighbor finding routine for small, periodic, refined meshes
- [[PR986]](https://github.com/parthenon-hpc-lab/parthenon/pull/986) Fix bug in sparse boundary communication BndInfo cacheing
- [[PR978]](https://github.com/parthenon-hpc-lab/parthenon/pull/978) remove erroneous sparse check

### Infrastructure (changes irrelevant to downstream codes)
- [[PR 1027]](https://github.com/parthenon-hpc-lab/parthenon/pull/1027) Refactor RestartReader as abstract class
- [[PR 1017]](https://github.com/parthenon-hpc-lab/parthenon/pull/1017) Make regression tests more verbose on failure
- [[PR 1007]](https://github.com/parthenon-hpc-lab/parthenon/pull/1007) Split template instantiations for HDF5 Read/Write attributes to speed up compile times
- [[PR 990]](https://github.com/parthenon-hpc-lab/parthenon/pull/990) Partial refactor of HDF5 I/O code for readability/extendability
- [[PR 982]](https://github.com/parthenon-hpc-lab/parthenon/pull/982) add some gut check testing for parthenon-VIBE

### Incompatibilities (i.e. breaking changes)
- [[PR 987]](https://github.com/parthenon-hpc-lab/parthenon/pull/987) Change the API for what was IterativeTasks
- [[PR 974]](https://github.com/parthenon-hpc-lab/parthenon/pull/974) Change GetParentPointer to always return T*
- [[PR 996]](https://github.com/parthenon-hpc-lab/parthenon/pull/996) Remove dynamic allocations from swarm particle creation


## Release 23.11
Date: 2023-11-16

### Added (new features/APIs/variables/...)
- [[PR 962]](https://github.com/parthenon-hpc-lab/parthenon/pull/962) Add support for in-situ histograms/profiles
- [[PR 911]](https://github.com/parthenon-hpc-lab/parthenon/pull/911) Add infrastructure for geometric multi-grid
- [[PR 971]](https://github.com/parthenon-hpc-lab/parthenon/pull/971) Add UserWorkBeforeLoop
- [[PR 907]](https://github.com/parthenon-hpc-lab/parthenon/pull/907) PEP1: Allow subclassing StateDescriptor
- [[PR 932]](https://github.com/parthenon-hpc-lab/parthenon/pull/932) Add GetOrAddFlag to metadata
- [[PR 931]](https://github.com/parthenon-hpc-lab/parthenon/pull/931) Allow SparsePacks with subsets of blocks
- [[PR 921]](https://github.com/parthenon-hpc-lab/parthenon/pull/921) Add more flexible ways of adding and using MeshData/MeshBlockData objects to DataCollections
- [[PR 900]](https://github.com/parthenon-hpc-lab/parthenon/pull/900) Add Morton numbers and expand functionality of LogicalLocation
- [[PR 902]](https://github.com/parthenon-hpc-lab/parthenon/pull/902) Add ability to output NaNs for de-allocated sparse fields
- [[PR 887]](https://github.com/parthenon-hpc-lab/parthenon/pull/887) Add ability to dump more types of params and read them from restarts
- [[PR 884]](https://github.com/parthenon-hpc-lab/parthenon/pull/884) Add constant derivative BC and expose GenericBC
- [[PR 892]](https://github.com/parthenon-hpc-lab/parthenon/pull/892) Cost-based load balancing and memory diagnostics
- [[PR 889]](https://github.com/parthenon-hpc-lab/parthenon/pull/889) Add PreCommFillDerived
- [[PR 872]](https://github.com/parthenon-hpc-lab/parthenon/pull/872) Boundary communication for non-cell centered fields
- [[PR 877]](https://github.com/parthenon-hpc-lab/parthenon/pull/877) Add flat sparse packs
- [[PR 868]](https://github.com/parthenon-hpc-lab/parthenon/pull/868) Add block-local face, edge, and nodal fields and allow for packing

### Changed (changing behavior/API/variables/...)
- [[PR 977]](https://github.com/parthenon-hpc-lab/parthenon/pull/977) Change to CalVer from SemVer
- [[PR 975]](https://github.com/parthenon-hpc-lab/parthenon/pull/975) Construct staged integrators via arbitrary name
- [[PR 976]](https://github.com/parthenon-hpc-lab/parthenon/pull/976) Move UserWorkBeforeLoop to be after first output
- [[PR 965]](https://github.com/parthenon-hpc-lab/parthenon/pull/965) Allow leading whitespace in input parameters
- [[PR 926]](https://github.com/parthenon-hpc-lab/parthenon/pull/926) Internal refinement op registration
- [[PR 897]](https://github.com/parthenon-hpc-lab/parthenon/pull/897) Deflate compression filter is not called any more if compression is soft disabled
- [[PR 896]](https://github.com/parthenon-hpc-lab/parthenon/pull/896) Update Kokkos integration to support installed version. Use `serial` (flat MPI) host parallelization by default (instead of OpenMP)
- [[PR 894]](https://github.com/parthenon-hpc-lab/parthenon/pull/894) Demand that sparse pool order sparse ids
- [[PR 888]](https://github.com/parthenon-hpc-lab/parthenon/pull/888) Bump Kokkos submodule to 4.0.1
- [[PR 885]](https://github.com/parthenon-hpc-lab/parthenon/pull/885) Expose PackDescriptor and use uids in SparsePacks

### Fixed (not changing behavior/API/variables/...)
- [[PR 955]](https://github.com/parthenon-hpc-lab/parthenon/pull/955) Only permit rank0 to mkdir when -d flag specified
- [[PR 952]](https://github.com/parthenon-hpc-lab/parthenon/pull/954) Fix format string in sparse advection example
- [[PR 947]](https://github.com/parthenon-hpc-lab/parthenon/pull/947) Add missing ForceRemeshComm dependencies
- [[PR 928]](https://github.com/parthenon-hpc-lab/parthenon/pull/928) Fix boundary comms during refinement next to refined blocks
- [[PR 937]](https://github.com/parthenon-hpc-lab/parthenon/pull/937) Fix multiple line continuations
- [[PR 933]](https://github.com/parthenon-hpc-lab/parthenon/pull/933) Remove extraneous debug check
- [[PR 917]](https://github.com/parthenon-hpc-lab/parthenon/pull/917) Update Iterative Tasking Infrastructure
- [[PR 890]](https://github.com/parthenon-hpc-lab/parthenon/pull/890) Fix bugs in sparse communication and prolongation

### Infrastructure (changes irrelevant to downstream codes)
- [[PR 967]](https://github.com/parthenon-hpc-lab/parthenon/pull/967) Change INLINE to FORCEINLINE on par_for_inner overloads
- [[PR 938]](https://github.com/parthenon-hpc-lab/parthenon/pull/938) Restructure buffer packing/unpacking kernel hierarchical parallelism
- [[PR 944]](https://github.com/parthenon-hpc-lab/parthenon/pull/944) Move sparse pack identifier creation to descriptor
- [[PR 904]](https://github.com/parthenon-hpc-lab/parthenon/pull/904) Move to prolongation/restriction in one for AMR and communicate non-cell centered fields
- [[PR 918]](https://github.com/parthenon-hpc-lab/parthenon/pull/918) Refactor RegionSize
- [[PR 901]](https://github.com/parthenon-hpc-lab/parthenon/pull/901) Implement shared element ownership model

### Removed (removing behavior/API/varaibles/...)
- [[PR 930](https://github.com/parthenon-hpc-lab/parthenon/pull/930) Remove ParthenonManager::ParthenonInit as it is error-prone and the split functions are the recommended usage.


## Release 0.8.0
Date: 2023-05-26

### Added (new features/APIs/variables/...)
- [[PR 868]](https://github.com/parthenon-hpc-lab/parthenon/pull/868) Add block-local face, edge, and nodal fields and allow for packing
- [[PR 830]](https://github.com/parthenon-hpc-lab/parthenon/pull/830) Add particle output
- [[PR 840]](https://github.com/parthenon-hpc-lab/parthenon/pull/840) Generalized integrators infrastructure in a backwards compatible way
- [[PR 810]](https://github.com/parthenon-hpc-lab/parthenon/pull/810) Add suport for Ascent in-situ visualization
- [[PR 831]](https://github.com/parthenon-hpc-lab/parthenon/pull/831) Add set-based MetadataFlag logic
- [[PR 803]](https://github.com/parthenon-hpc-lab/parthenon/pull/803) Add skeleton for sphinx docs
- [[PR 774]](https://github.com/parthenon-hpc-lab/parthenon/pull/774) Add second order refinement criteria
- [[PR 699]](https://github.com/parthenon-hpc-lab/parthenon/pull/699) Add independent sparse thresholds and sparse control fields. Sparse bug fixes.
- [[PR 780]](https://github.com/parthenon-hpc-lab/parthenon/pull/780) Add Burgers' benchmark, GetVector<T> interface for ParameterInput
- [[PR 732]](https://github.com/parthenon-hpc-lab/parthenon/pull/732) Add `Metadata::ForceRemeshComm` flag
- [[PR 755]](https://github.com/parthenon-hpc-lab/parthenon/pull/755) Add archive_parameters option to output all parameters to text file at start
- [[PR 729]](https://github.com/parthenon-hpc-lab/parthenon/pull/729) Optional modifications to output format
- [[PR 717]](https://github.com/parthenon-hpc-lab/parthenon/pull/717) Add ghost zone plotting capability to phdf.py and movie2d.py
- [[PR 712]](https://github.com/parthenon-hpc-lab/parthenon/pull/712) Allow to add params from cmdline

### Changed (changing behavior/API/variables/...)
- [[PR 867]](https://github.com/parthenon-hpc-lab/parthenon/pull/867) Rename CellVariable to Variable in anticipation of face fields
- [[PR 870]](https://github.com/parthenon-hpc-lab/parthenon/pull/870) Update component label logic in output to include basename
- [[PR 853]](https://github.com/parthenon-hpc-lab/parthenon/pull/853) Add multiple features and improve the performance of the movie2d.py tool
- [[PR 775]](https://github.com/parthenon-hpc-lab/parthenon/pull/775) Reorganize some of the bvals and prolongation/restriction machinery
- [[PR 753]](https://github.com/parthenon-hpc-lab/parthenon/pull/753) Cleanup uniform Cartesian variable names
- [[PR 769]](https://github.com/parthenon-hpc-lab/parthenon/pull/769) Thread custom prolongation-restriction functions through infrastructure and to userspace
- [[PR 781]](https://github.com/parthenon-hpc-lab/parthenon/pull/781) Output variables with same shape as in parthenon
- [[PR 758]](https://github.com/parthenon-hpc-lab/parthenon/pull/758) Bump required C++ standard to C++17
- [[PR 710]](https://github.com/parthenon-hpc-lab/parthenon/pull/710) Remove data transpose in hdf5 and restart outputs
- [[PR 713]](https://github.com/parthenon-hpc-lab/parthenon/pull/713) Remove Coordinates stub in favor of Coordinates_t
- [[PR 711]](https://github.com/parthenon-hpc-lab/parthenon/pull/711) Rename flux correction routines.
- [[PR 663]](https://github.com/parthenon-hpc-lab/parthenon/pull/663) Change bvals_in_one to use sparse boundary buffers and add flux_correction in one.

### Fixed (not changing behavior/API/variables/...)
- [[PR 876]](https://github.com/parthenon-hpc-lab/parthenon/pull/876) Fix restarting from files which record ghost zones
- [[PR 878]](https://github.com/parthenon-hpc-lab/parthenon/pull/878) Catch2 target guard in CMake
- [[PR 873]](https://github.com/parthenon-hpc-lab/parthenon/pull/873) Prevent HDF5 from throwing a fit when a swarm has no particles
- [[PR 866]](https://github.com/parthenon-hpc-lab/parthenon/pull/866) Add missing guard for HDF5 on restart
- [[PR 861]](https://github.com/parthenon-hpc-lab/parthenon/pull/861) Fix filesystem include for experimental namespace
- [[PR 859]](https://github.com/parthenon-hpc-lab/parthenon/pull/859) fix off-by-one indexing error in Ascent ghost mask
- [[PR 841]](https://github.com/parthenon-hpc-lab/parthenon/pull/841) De-allocate MPI communicators when freeing `Reduction` objects
- [[PR 851]](https://github.com/parthenon-hpc-lab/parthenon/pull/851) Fix xdmf hyperslab definition for vectors
- [[PR 843]](https://github.com/parthenon-hpc-lab/parthenon/pull/843) Add guard rails to prolongation/restriction infrastructure
- [[PR 832]](https://github.com/parthenon-hpc-lab/parthenon/pull/833) Fix movie2d script after it broke due to change in HDF5 format
- [[PR 820]](https://github.com/parthenon-hpc-lab/parthenon/pull/820) Fix XDMF spec to conform to standard and handle scalar and vector variables
- [[PR 795]](https://github.com/parthenon-hpc-lab/parthenon/pull/795) Fix length-1 vectors in output format version >= 3
- [[PR 824]](https://github.com/parthenon-hpc-lab/parthenon/pull/824) Remove unsupported exception handling from device methods in UniformCartesian
- [[PR 818]](https://github.com/parthenon-hpc-lab/parthenon/pull/818) Fix boundary prolongation only occurring in base stage
- [[PR 805]](https://github.com/parthenon-hpc-lab/parthenon/pull/805) Add Crusher/Frontier machine file and bump Kokkos to 3.7.01
- [[PR 797]](https://github.com/parthenon-hpc-lab/parthenon/pull/797) Fix boundary flux correction boundary logic
- [[PR 800]](https://github.com/parthenon-hpc-lab/parthenon/pull/800) Fix rare and intermitted race condition to set allocation status
- [[PR 777]](https://github.com/parthenon-hpc-lab/parthenon/pull/784) Fix double-output of last file in rare cases
- [[PR 740]](https://github.com/parthenon-hpc-lab/parthenon/pull/740) Faster PHDF file load times in phdf.py
- [[PR 751]](https://github.com/parthenon-hpc-lab/parthenon/pull/751) Delete useless file in advection example
- [[PR 765]](https://github.com/parthenon-hpc-lab/parthenon/pull/765) Fix incorrect BC labeling in swarm
- [[PR 759]](https://github.com/parthenon-hpc-lab/parthenon/pull/759) Add metadata so Visit treats outputs as time series
- [[PR 743]](https://github.com/parthenon-hpc-lab/parthenon/pull/743) Add missing HDF5 type on MacOS
- [[PR 739]](https://github.com/parthenon-hpc-lab/parthenon/pull/739) Fix phdf.py for flattened vectors
- [[PR 724]](https://github.com/parthenon-hpc-lab/parthenon/pull/724) Fix failing CI on Darwin due to differing `OutputFormatVersion` attribute in hdf5 gold files.
- [[PR 725]](https://github.com/parthenon-hpc-lab/parthenon/pull/725) Fix improperly exited kokkos profiling region
- [[PR 719]](https://github.com/parthenon-hpc-lab/parthenon/pull/719) Fix type mismatch in swarm boundaries when host pinned memory enabled
- [[PR 716]](https://github.com/parthenon-hpc-lab/parthenon/pull/716) Remove unneeded assert from ParArrayND

### Infrastructure (changes irrelevant to downstream codes)
- [[PR 879]](https://github.com/parthenon-hpc-lab/parthenon/pull/879) Zero buffer allocation AMR
- [[PR 838]](https://github.com/parthenon-hpc-lab/parthenon/pull/838) Move packing logic to from strings to unique IDs
- [[PR 848]](https://github.com/parthenon-hpc-lab/parthenon/pull/848) Implement recursive mkdir using [`std::filesystem`](https://en.cppreference.com/w/cpp/filesystem) in **src/utils/change_rundir.cpp**
- [[PR 837]](https://github.com/parthenon-hpc-lab/parthenon/pull/837) Migrate docs to Sphinx
- [[PR 791]](https://github.com/parthenon-hpc-lab/parthenon/pull/791) Set KOKKOS_DISABLE_WARNINGS=TRUE
- [[PR 777]](https://github.com/parthenon-hpc-lab/parthenon/pull/777) New action: check PR dependencies & warn until requirements merged
- [[PR 772]](https://github.com/parthenon-hpc-lab/parthenon/pull/772) Trigger short CI only for PRs and remove old SpaceInstances test
- [[PR 757]](https://github.com/parthenon-hpc-lab/parthenon/pull/757) Move to flux correction in-one and unify with bvals
- [[PR 768]](https://github.com/parthenon-hpc-lab/parthenon/pull/768) Update CI image and move to new CI machine (short and extended tests)
- [[PR 766]](https://github.com/parthenon-hpc-lab/parthenon/pull/766) Remove IAS performance regression test
- [[PR 735]](https://github.com/parthenon-hpc-lab/parthenon/pull/735) Clean up HDF5 output
- [[PR 708]](https://github.com/parthenon-hpc-lab/parthenon/pull/708) Bump minimum version of Kokkos to 3.6

### Removed (removing behavior/API/varaibles/...)
- [[PR 829]](https://github.com/parthenon-hpc-lab/parthenon/pull/829) Remove dead output code
- [[PR 738]](https://github.com/parthenon-hpc-lab/parthenon/pull/738) Remove old incomplete face-centered variables


## Release 0.7.0
Date: 2022-08-04

### Added (new features/APIs/variables/...)
- [[PR 702]](https://github.com/parthenon-hpc-lab/parthenon/pull/702) Allow for ParArrayGeneric to accept enums
- [[PR 694]](https://github.com/parthenon-hpc-lab/parthenon/pull/690) Add C++11 implementation of concepts lite
- [[PR 692]](https://github.com/parthenon-hpc-lab/parthenon/pull/692) Add SparsePack machinery
- [[PR 690]](https://github.com/parthenon-hpc-lab/parthenon/pull/690) Use power9 partition for Darwin CI
- [[PR 689]](https://github.com/parthenon-hpc-lab/parthenon/pull/689) Add `Mesh::ProblemGenerator` (allows reductions during init)
- [[PR 667]](https://github.com/parthenon-hpc-lab/parthenon/pull/667) Add parallel scan
- [[PR 654]](https://github.com/parthenon-hpc-lab/parthenon/pull/654) Add option for returning FlatIdx when requested variable doesn't exist
- [[PR 653]](https://github.com/parthenon-hpc-lab/parthenon/pull/653) Allow for multi-D particle variables
- [[PR 622]](https://github.com/parthenon-hpc-lab/parthenon/pull/622) Extend reduction framework to support more general data types. Now uses PR 623.
- [[PR 619]](https://github.com/parthenon-hpc-lab/parthenon/pull/619) Sort particles by cell
- [[PR 605]](https://github.com/parthenon-hpc-lab/parthenon/pull/605) Add output triggering by signaling.
- [[PR 602]](https://github.com/parthenon-hpc-lab/parthenon/pull/602) Added tuning functionality for HDF5 output
- [[PR 586]](https://github.com/parthenon-hpc-lab/parthenon/pull/586) Implement true sparse capability with automatic allocation and deallocation of sparse

### Changed (changing behavior/API/variables/...)
- [[PR 682]](https://github.com/parthenon-hpc-lab/parthenon/pull/682) Add prolongate-in-one
- [[PR 685]](https://github.com/parthenon-hpc-lab/parthenon/pull/685) Add `*pmb` to `MeshBlockUserWorkBeforeOutput`. Remove unused `MeshBlockUserWorkInLoop`.
- [[PR 676]](https://github.com/parthenon-hpc-lab/parthenon/pull/662) Remove broken swarm user boundary check
- [[PR 662]](https://github.com/parthenon-hpc-lab/parthenon/pull/662) Remove SetPrecise
- [[PR 673]](https://github.com/parthenon-hpc-lab/parthenon/pull/673) Remove smallest meshblock case from advection_performance
- [[PR 655]](https://github.com/parthenon-hpc-lab/parthenon/pull/655) Enable user boundary conditions for particles
- [[PR 623]](https://github.com/parthenon-hpc-lab/parthenon/pull/623) Enable Params to optionally return non-const pointers
- [[PR 604]](https://github.com/parthenon-hpc-lab/parthenon/pull/604) Allow modification of SimTime in PreStepUserWorkInLoop
- [[PR 617]](https://github.com/parthenon-hpc-lab/parthenon/pull/617) Unify the coordinates API for MeshBlockPack and VariablePack

### Fixed (not changing behavior/API/variables/...)
- [[PR 688]](https://github.com/parthenon-hpc-lab/parthenon/pull/688) Restore component labels for multicomponent non-vector field
- [[PR 679]](https://github.com/parthenon-hpc-lab/parthenon/pull/679) Handle case of multidim var labeling for output
- [[PR 680]](https://github.com/lanl/partheon/pull/680) Fix hanging compilation for sort unit test
- [[PR 678]](https://github.com/lanl/partheon/pull/678) Fix FlatIdx packing for size-1 dimensions
- [[PR 677]](https://github.com/lanl/partheon/pull/677) Fix restart without `SparseInfo` object
- [[PR 670]](https://github.com/lanl/partheon/pull/670) Fix typo in `parse_value` for non-hdf5 builds
- [[PR 656]](https://github.com/lanl/partheon/pull/656) Extend CC bvars to 5-, 6-D ParArrays
- [[PR 629]](https://github.com/parthenon-hpc-lab/parthenon/pull/629) Fix HIP backend (config and tests) and extend build coverage
- [[PR 652]](https://github.com/parthenon-hpc-lab/parthenon/pull/652) Fix issue with hsize_t and size_t in utils parser
- [[PR 649]](https://github.com/parthenon-hpc-lab/parthenon/pull/649) Ensure LoadBalancing send buffers are filled and allow async recv LB
- [[PR 618]](https://github.com/parthenon-hpc-lab/parthenon/pull/618) Fix bug in variable pack performance test
- [[PR 616]](https://github.com/parthenon-hpc-lab/parthenon/pull/609) Restore sparse base names in PackIndexMap
- [[PR 609]](https://github.com/parthenon-hpc-lab/parthenon/pull/609) Fix bug where .final is not written if signal raised while writing regular output
- [[PR 595]](https://github.com/parthenon-hpc-lab/parthenon/pull/595) Fix build options so that non-MPI builds cannot be paired with an MPI HDF5 lib

### Infrastructure (changes irrelevant to downstream codes)
- [[PR 703]](https://github.com/parthenon-hpc-lab/parthenon/pull/703) Fixed mpi/serial logic in extended CI tests
- [[PR 700]](https://github.com/parthenon-hpc-lab/parthenon/pull/700) Moved CI testing from GitLab mirror to GitHub Actions
- [[PR 698]](https://github.com/parthenon-hpc-lab/parthenon/pull/698) Remove matplotlib from required python libraries and make desired instead
- [[PR 686]](https://github.com/parthenon-hpc-lab/parthenon/pull/686) Remove coverage CI stage and add key features to README
- [[PR 681]](https://github.com/parthenon-hpc-lab/parthenon/pull/681) Refactor ParArrayNDGeneric to work with arbitrary rank Kokkos::views and hold state.
- [[PR 669]](https://github.com/parthenon-hpc-lab/parthenon/pull/669) Bump clang-format version (and checks) to >=11.0
- [[PR 661]](https://github.com/parthenon-hpc-lab/parthenon/pull/661) Replaced ids in MPI tags with separate `MPI_Comms` for each variable/swarm
- [[PR 651]](https://github.com/parthenon-hpc-lab/parthenon/pull/651) Bump Catch2 version due to GCC11.2 incompatibility
- [[PR 646]](https://github.com/parthenon-hpc-lab/parthenon/pull/646) Add machine configuration file for Stony Brook's Ookami A64FX and OLCF's Spock AMD systems.


## Release 0.6.1
Date: 09/22/2021

### Added (new features/APIs/variables/...)
- [[PR 563]](https://github.com/parthenon-hpc-lab/parthenon/pull/563) Physical boundary options for particles
- [[PR 582]](https://github.com/parthenon-hpc-lab/parthenon/pull/582) Adding global reductions and basic functionality needed for solvers.
- [[PR 556]](https://github.com/parthenon-hpc-lab/parthenon/pull/556) Introduce iterative tasks and regionally dependent tasks
- [[PR 578]](https://github.com/parthenon-hpc-lab/parthenon/pull/578) Add some profiling regions to tasks in particles example
- [[PR 577]](https://github.com/parthenon-hpc-lab/parthenon/pull/577) Update invalid indices to allow for no-op loops
- [[PR 564]](https://github.com/parthenon-hpc-lab/parthenon/pull/564) Add EstimateTimestep to particles example task list
- [[PR 557]](https://github.com/parthenon-hpc-lab/parthenon/pull/557) Re-enable `InitMeshBlockUserData` so data can be set per-remeshing
- [[PR 509]](https://github.com/parthenon-hpc-lab/parthenon/pull/509) Add `elapsed_main`, `elapsed_cycle`, and `elapsed_LBandAMR` functions to `Driver` as static functions to enable access to timing information in output and restart files.
- [[PR 479]](https://github.com/parthenon-hpc-lab/parthenon/pull/479) Add `Update` function to `Params` to update the value of an existing key.
- [[PR 482]](https://github.com/parthenon-hpc-lab/parthenon/pull/482) Add support for package enrolled history outputs.
- [[PR 511]](https://github.com/parthenon-hpc-lab/parthenon/pull/511) Improvements/speed ups in phdf_diff, phdf_diff, analytic_compare.py
- [[PR 497]](https://github.com/parthenon-hpc-lab/parthenon/pull/497) Add tracer particles example.
- [[PR 404]](https://github.com/parthenon-hpc-lab/parthenon/pull/404) Add capability to communicate particles across meshblocks/MPI processes

### Changed (changing behavior/API/variables/...)
- [[PR 558]](https://github.com/parthenon-hpc-lab/parthenon/pull/558) Boundary bugfix(es) incl. regression tests and exposing FluxDiv_ interface
- [[PR 583]](https://github.com/parthenon-hpc-lab/parthenon/pull/583) Fix file numbering logic for writing outputs after restarting a simulation.
- [[PR 581]](https://github.com/parthenon-hpc-lab/parthenon/pull/581) Change return status of `ArgParse` so that `complete` is returned when passing in the help flag.
- [[PR 580]](https://github.com/parthenon-hpc-lab/parthenon/pull/580) Hid variable/meshblock pack keys from public API, added unit tests for `MeshBlockData`.
- [[PR 535]](https://github.com/parthenon-hpc-lab/parthenon/pull/535) Modify various interfaces connected with
  variables, `StateDescriptor`, variable packing to support proper sparse variables. Replace
  `SparseVariable` with `SparsePool`. Remove `MeshBlockDataIterator` and make
  `MeshBlockData::GetVariablesBy[Name|Flag]` public instead. Remove public `MeshBlockData::Add`
  interface and add `MeshBlockData::Initialize` instead.
- [[PR 553]](https://github.com/parthenon-hpc-lab/parthenon/pull/553) Avoid use of variable named restrict
- [[PR 476]](https://github.com/parthenon-hpc-lab/parthenon/pull/476) Update min. `CMake` version to 3.16 (matching `Kokkos`) and add option to compile with C++17 (`PARTHENON_ENABLE_CPP17` - default: off)
- [[PR 532]](https://github.com/parthenon-hpc-lab/parthenon/pull/532) Remove obsolete `Properties_t`, they have been replaced by `Packages_t`
- [[PR 508]](https://github.com/parthenon-hpc-lab/parthenon/pull/508) Modify `RestrictCellCenteredVariables` to support a restriction over meshblock packs.
- [[PR 524]](https://github.com/parthenon-hpc-lab/parthenon/pull/524) Enforce `Metadata` flags constraints and add new `Metadata::WithFluxes` flag. Note: `Metadata::Independent` will be set automatically unless `Metadata::Derived` is set
- [[PR 517]](https://github.com/parthenon-hpc-lab/parthenon/pull/517) Remove optional `dims` argument from `MeshBlockData::Add` and use the shape from the `Metadata` instead
- [[PR 492]](https://github.com/parthenon-hpc-lab/parthenon/pull/492) Modify advection example to have an arbitrary number of dense variables and to disable fill derived for profiling.
- [[PR 486]](https://github.com/parthenon-hpc-lab/parthenon/pull/486) Unify HDF5 output and restart file writing, add HDF5 compression support, add support for sparse fields in HDF5 output/restart files, add and rename some metadata in HDF5 files.
- [[PR 522]](https://github.com/parthenon-hpc-lab/parthenon/pull/522) Corrected ordering of `OutputDatasetNames` to match `ComponentNames`

### Fixed (not changing behavior/API/variables/...)
- [[PR 588]](https://github.com/parthenon-hpc-lab/parthenon/pull/588) Switch from nbmax to nneighbors in particles
- [[PR 572]](https://github.com/parthenon-hpc-lab/parthenon/pull/572) Fix meshblockpack issue coming from variatic template shadowing
- [[PR 569]](https://github.com/parthenon-hpc-lab/parthenon/pull/569) Fix path to nvcc_wrapper in README example
- [[PR 551]](https://github.com/parthenon-hpc-lab/parthenon/pull/551) Hotfix to make particles compile without MPI again
- [[PR 552]](https://github.com/parthenon-hpc-lab/parthenon/pull/552) Fix missing include for fstream
- [[PR 537]](https://github.com/parthenon-hpc-lab/parthenon/pull/538) Fix inconsistent treatment of coarse buffers.
- [[PR 539]](https://github.com/parthenon-hpc-lab/parthenon/pull/539) Fix restart indexing/hdf5 bugs
- [[PR 487]](https://github.com/parthenon-hpc-lab/parthenon/pull/487) Add default tiling matching `i` index range to MDRange loop pattern.
- [[PR 531]](https://github.com/parthenon-hpc-lab/parthenon/pull/531) Work around in parthenon_hdf5.cpp for GCC 7.3.0

### Infrastructure (changes irrelevant to downstream codes)
- [[PR 575]](https://github.com/parthenon-hpc-lab/parthenon/pull/575) Make file comparison more verbose, don't check File metadata
- [[PR 502]](https://github.com/parthenon-hpc-lab/parthenon/pull/502) Use subviews of a single view for fluxes
- [[PR 505]](https://github.com/parthenon-hpc-lab/parthenon/pull/505) Can also use buffer-pack-in-one function also in `Mesh::Initialize` (and thus during load balancing/mesh refinement). Breaks sparse variables with FillGhost. Enable with `PARTHENON_ENABLE_INIT_PACKING=ON` (default OFF).
- [[PR 493]](https://github.com/parthenon-hpc-lab/parthenon/pull/493) Use subviews of a single view for comm buffers
- [[PR 500]](https://github.com/parthenon-hpc-lab/parthenon/pull/500) Update docker file and CI environment (for Cuda 11.3 and latest `nsys`)
- [[PR 490]](https://github.com/parthenon-hpc-lab/parthenon/pull/490) Adjust block size in OverlappingSpace instance tests to remain within Cuda/HIP limits
- [[PR 488]](https://github.com/parthenon-hpc-lab/parthenon/pull/488) Update GitLab Dockerfile to use HDF5 version 1.10.7
- [[PR 510]](https://github.com/parthenon-hpc-lab/parthenon/pull/510) Fix calling noexistant logger in python performance regression app
- [[PR 527]](https://github.com/parthenon-hpc-lab/parthenon/pull/527) Fix problem with CI when rebase is used.
- [[PR 518]](https://github.com/parthenon-hpc-lab/parthenon/pull/518) Added MPI performance regression tests to CI performance app
- [[PR 530]](https://github.com/parthenon-hpc-lab/parthenon/pull/530) Fixed issue with CI plotting the oldest 5 commit metrics for each test, also cleaned up legend formatting.
- [[PR 536]](https://github.com/parthenon-hpc-lab/parthenon/pull/536) Updated to latest Kokkos release.
- [[PR 520]](https://github.com/parthenon-hpc-lab/parthenon/pull/520) Add black python formatter to github actions
- [[PR 519]](https://github.com/parthenon-hpc-lab/parthenon/pull/519) Add checksum to bash uploader script to verify file is trusted
- [[PR 549]](https://github.com/parthenon-hpc-lab/parthenon/pull/549) Add deep-code badge.
- [[PR 554]](https://github.com/parthenon-hpc-lab/parthenon/pull/554) Small fix to documentation related to python parthenon tools README
- [[PR 555]](https://github.com/parthenon-hpc-lab/parthenon/pull/555) Added documentation for darwin CI and scripts
- [[PR 560]](https://github.com/parthenon-hpc-lab/parthenon/pull/560) Rename `png_files_to_upload` to more generic `figure_files_to_upload`
- [[PR 561]](https://github.com/parthenon-hpc-lab/parthenon/pull/561) Adding documentation to help with adding new performance regression tests.

### Removed (removing behavior/API/varaibles/...)
- [[PR 498]](https://github.com/parthenon-hpc-lab/parthenon/pull/498) Cleanup unused user hooks and variables
- [[PR 481]](https://github.com/parthenon-hpc-lab/parthenon/pull/481) Cleanup unused/untested/not fully ported code (mostly OpenMP and reconstruction)


## Release 0.5.0
Date: 03/30/2021

### Added (new features/APIs/variables/...)
- [[PR 475]](https://github.com/parthenon-hpc-lab/parthenon/pull/475) Add update function `UpdateWithFluxDivergence` and `gamma` variables to integrator to support low-storage, two stage integrators.
- [[PR 463]](https://github.com/parthenon-hpc-lab/parthenon/pull/463) Add `PARTHENON_ENABLE_TESTING` and `PARTHENON_ENABLE_PYTHON_MODULE_CHECK` option and documentation on how to use the regression testing framework downstream.
- [[PR 461]](https://github.com/parthenon-hpc-lab/parthenon/pull/461) A negative `dt` in an output block disables it.
- [[PR 439]](https://github.com/parthenon-hpc-lab/parthenon/pull/439) Add split initialization of environment (`ParthenonInitEnv()`) and packages and mesh (`ParthenonInitPackagesAndMesh()`) to `ParthenonManager`
- [[PR 406]](https://github.com/parthenon-hpc-lab/parthenon/pull/406) Add `stochastic_subgrid` example that performs a random amount of work per cell (drawn from a power law distribution)
- [[PR 440]](https://github.com/parthenon-hpc-lab/parthenon/pull/440) Add abstraction for allocating a `unique_ptr` to an object in device memory
- [[PR 438]](https://github.com/parthenon-hpc-lab/parthenon/pull/438) More diagnostic runtime output (AMR/Loadbalance and mesh structure) controlled via `parthenon/time/ncycle_out_mesh` input parameter (default 0 - off)
- [[PR 412]](https://github.com/parthenon-hpc-lab/parthenon/pull/412) Add capability to use host (pinned) memory for communication buffers (via `PARTHENON_ENABLE_HOST_COMM_BUFFERS` - default OFF)
- [[PR 359]](https://github.com/parthenon-hpc-lab/parthenon/pull/359) MeshBlockPack support for buffer pack and unpack of CellCentered Variables

### Changed (changing behavior/API/variables/...)
- [[PR 451]](https://github.com/parthenon-hpc-lab/parthenon/pull/451) Remove custom ran2 interface and source
- [[PR 425]](https://github.com/parthenon-hpc-lab/parthenon/pull/425) Remove ambiguity in package names. `Packages_t` no longer has an `operator[]` method. This has been replaced with `Add` and `Get`.
- [[PR 359]](https://github.com/parthenon-hpc-lab/parthenon/pull/359) Templated inline reconstruction functions to support different types (e.g., `ParArray4D` or `ParArrayND`)

### Fixed (not changing behavior/API/variables/...)
- [[PR 468]](https://github.com/parthenon-hpc-lab/parthenon/pull/468) Fix extra `endl` in input CheckDesired
- [[PR 465]](https://github.com/parthenon-hpc-lab/parthenon/pull/465) Fix soft disable output for drivers without temporal evolution
- [[PR 453]](https://github.com/parthenon-hpc-lab/parthenon/pull/453) Fix array bounds for AMR hierarchy log and use vector instead of `unique_ptr`
- [[PR 441]](https://github.com/parthenon-hpc-lab/parthenon/pull/441) Fixed type in input parsing of `pack_size`

### Infrastructure (changes irrelevant to downstream codes)
- [[PR 436]](https://github.com/parthenon-hpc-lab/parthenon/pull/436) Update Summit build doc and machine file
- [[PR 435]](https://github.com/parthenon-hpc-lab/parthenon/pull/435) Fix ctest logic for parsing number of ranks in MPI tests
- [[PR 407]](https://github.com/parthenon-hpc-lab/parthenon/pull/407) More cleanup, removed old bash scripts for CI.
- [[PR 428]](https://github.com/parthenon-hpc-lab/parthenon/pull/428) Triad Copyright 2021
- [[PR 413]](https://github.com/parthenon-hpc-lab/parthenon/pull/413) LANL Snow machine configuration
- [[PR 390]](https://github.com/parthenon-hpc-lab/parthenon/pull/390) Resolve `@PAR_ROOT@` to parthenon root rather than the location of the current source directory
- [[PR 443]](https://github.com/parthenon-hpc-lab/parthenon/pull/443) Fix Darwin machine config - use spectrum mpi
- [[PR 444]](https://github.com/parthenon-hpc-lab/parthenon/pull/444) Writes performance metrics to file for advection test
- [[PR 452]](https://github.com/parthenon-hpc-lab/parthenon/pull/452) Disable copyright check and linting by default, add CI check for copyright
- [[PR 473]](https://github.com/parthenon-hpc-lab/parthenon/pull/473) Added documentation for forked pr

## Release 0.4.0
Date: 01/19/2021

### Added (new features/APIs/variables/...)
- [[PR 434]](https://github.com/parthenon-hpc-lab/parthenon/pull/434) Allow the number of ghost zones to be set via the input file
- [[PR 400]](https://github.com/parthenon-hpc-lab/parthenon/pull/400) Extend `StateDescriptor` for customizable output via user-customizable function pointers `PreStepDiagnosticsMesh` and `PostStepDiagnosticsMesh`
- [[PR 391]](https://github.com/parthenon-hpc-lab/parthenon/pull/391) Add `VariablePack<T>::GetSparseId` and `VariablePack<T>::GetSparseIndex` to return global sparse ids and pack-local sparse index, repsectively.
- [[PR 381]](https://github.com/parthenon-hpc-lab/parthenon/pull/381) Overload `DataCollection::Add` to build `MeshData` and `MeshBlockData` objects with a subset of variables.
- [[PR 378]](https://github.com/parthenon-hpc-lab/parthenon/pull/378) Add Kokkos profiling regions throughout the code to allow the collection characteristic application profiles
- [[PR 358]](https://github.com/parthenon-hpc-lab/parthenon/pull/358) Generalize code that interfaces with downstream apps to work with both `MeshData` and `MeshBlockData`.
- [[PR 335]](https://github.com/parthenon-hpc-lab/parthenon/pull/335) Support for project-relative `MACHINE_CFG` with `@PAR_ROOT@`
- [[PR 328]](https://github.com/parthenon-hpc-lab/parthenon/pull/328) New `MeshBlock` packing interface using `DataCollection`s of `MeshData` and `MeshBlockData`.
- [[PR 386]](https://github.com/parthenon-hpc-lab/parthenon/pull/386) Introduce `Private`, `Provides`, `Requires`, and `Overridable` variable metadata, allowing fine-grained control of conflict resolution between packages.

### Changed (changing behavior/API/variables/...)
- [[PR 393]](https://github.com/parthenon-hpc-lab/parthenon/pull/393) Small refactor to make driver code more flexible for downstream apps.
- [[PR 400]](https://github.com/parthenon-hpc-lab/parthenon/pull/400) Change `Mesh`, `ApplicationInput`, and `Driver` to suppport pre- and post- step user work
- [[PR 394]](https://github.com/parthenon-hpc-lab/parthenon/pull/394) Make `Params.Get` const-correct.
- [[PR 332]](https://github.com/parthenon-hpc-lab/parthenon/pull/332) Rewrote boundary conditions to work on GPUs with variable packs. Re-enabled user-defined boundary conditions via `ApplicationInput`.

### Fixed (not changing behavior/API/variables/...)
- [[PR 401]](https://github.com/parthenon-hpc-lab/parthenon/issues/401) Fix missing initial timestep for MeshData functions
- [[PR 387]](https://github.com/parthenon-hpc-lab/parthenon/pull/387) Add missing const that was needed
- [[PR 353]](https://github.com/parthenon-hpc-lab/parthenon/pull/353) Fixed small error in input\_parameter logic
- [[PR 352]](https://github.com/parthenon-hpc-lab/parthenon/pull/352) Code compiles cleanly (no warnings) with nvcc_wrapper

### Infrastructure (changes irrelevant to downstream codes)
- [[PR 392]](https://github.com/parthenon-hpc-lab/parthenon/pull/392) Fix C++ linting for when parthenon is a submodule
- [[PR 335]](https://github.com/parthenon-hpc-lab/parthenon/pull/335) New machine configuration file for LANL's Darwin cluster
- [[PR 200]](https://github.com/parthenon-hpc-lab/parthenon/pull/200) Adds support for running CI on POWER9 nodes.
- [[PR 347]](https://github.com/parthenon-hpc-lab/parthenon/pull/347) Speed up darwin CI by using pre installed spack packages from project space
- [[PR 368]](https://github.com/parthenon-hpc-lab/parthenon/pull/368) Fixes false positive in CI.
- [[PR 369]](https://github.com/parthenon-hpc-lab/parthenon/pull/369) Initializes submodules when running on darwin CI.
- [[PR 382]](https://github.com/parthenon-hpc-lab/parthenon/pull/382) Adds output on fail for fast CI implementation on Darwin.
- [[PR 362]](https://github.com/parthenon-hpc-lab/parthenon/pull/362) Small fix to clean regression tests output folder on reruns
- [[PR 403]](https://github.com/parthenon-hpc-lab/parthenon/pull/403) Cleanup Codacy warnings
- [[PR 377]](https://github.com/parthenon-hpc-lab/parthenon/pull/377) New machine configuration file for LLNL's RZAnsel cluster

### Removed (removing behavior/API/varaibles/...)
- [[PR 410]](https://github.com/parthenon-hpc-lab/parthenon/pull/410) Addresses issue of cpp linter calling python instead of python3

## Release 0.3.0
Date: 10/29/2020

### Added (new features/APIs/variables/...)
- [[PR 317]](https://github.com/parthenon-hpc-lab/parthenon/pull/317) Add initial support for particles (no MPI support)
- [[PR 311]](https://github.com/parthenon-hpc-lab/parthenon/pull/311) Bugfix::Restart. Fixed restart parallel bug and also restart bug for simulations with reflecting boundary conditions.  Added ability to write restart files with or without ghost cells by setting `ghost_zones` in the output block similar to other output formats.
- [[PR 314]](https://github.com/parthenon-hpc-lab/parthenon/pull/314) Generalized `par_for` abstractions to provide for reductions with a consistent interface.
- [[PR 308]](https://github.com/parthenon-hpc-lab/parthenon/pull/308) Added the ability to register and name `MeshBlockPack`s in the `Mesh` or in package initialization.
- [[PR 285]](https://github.com/parthenon-hpc-lab/parthenon/pull/285) Parthenon can now be linked in CMake as `Parthenon::parthenon` when used as a subdirectory, matching install.

### Changed (changing behavior/API/variables/...)
- [[PR 303]](https://github.com/parthenon-hpc-lab/parthenon/pull/303) Changed `Mesh::BlockList` from a `std::list<MeshBlock>` to a `std::vector<std::shared_ptr<MeshBlock>>`, making `FindMeshBlock` run in constant, rather than linear, time. Loops over `block_list` in application drivers must be cahnged accordingly.
- [[PR 300]](https://github.com/parthenon-hpc-lab/parthenon/pull/300): Changes to `AddTask` function signature. Requires re-ordering task dependency argument to front.
- [[PR 307]](https://github.com/parthenon-hpc-lab/parthenon/pull/307) Changed back-pointers in mesh structure to weak pointers. Cleaned up `MeshBlock` constructor and implemented `MeshBlock` factory function.

### Fixed (not changing behavior/API/variables/...)
- [[PR 293]](https://github.com/parthenon-hpc-lab/parthenon/pull/293) Changed `VariablePack` and related objects to use `ParArray1D` objects instead of `ParArrayND` objects under the hood to reduce the size of the captured objects.
- [[PR 313]](https://github.com/parthenon-hpc-lab/parthenon/pull/313) Add include guards for Kokkos in cmake.
- [[PR 321]](https://github.com/parthenon-hpc-lab/parthenon/pull/321) Make inner loop pattern tags constexpr

### Infrastructure (changes irrelevant to downstream codes)
- [[PR 336]](https://github.com/parthenon-hpc-lab/parthenon/pull/336) Automated testing now checks for extraneous HtoD or DtoH copies.
- [[PR 325]](https://github.com/parthenon-hpc-lab/parthenon/pull/325) Fixes regression in convergence tests with multiple MPI ranks.
- [[PR 310]](https://github.com/parthenon-hpc-lab/parthenon/pull/310) Fix Cuda 11 builds.
- [[PR 281]](https://github.com/parthenon-hpc-lab/parthenon/pull/281) Allows one to run regression tests with more than one cuda device, Also improves readability of regression tests output.
- [[PR 330]](https://github.com/parthenon-hpc-lab/parthenon/pull/330) Fixes restart regression test.


## Release 0.2.0
Date: 9/12/2020

### Added
- [[PR 250]](https://github.com/parthenon-hpc-lab/parthenon/pull/250) Feature::Restart. If output file format 'rst' is specified restart files are written using independent variables and those marked with Restart metadata flag.  Simulations can be restarted with a '-r \<restartFile\>' argument to the code.
- [[PR 263]](https://github.com/parthenon-hpc-lab/parthenon/pull/263) Added MeshBlockPack, a mechanism for looping over the whole mesh at once within a `Kokkos` kernel. See [documentation](docs/mesh/packing.md)
- [[PR 267]](https://github.com/parthenon-hpc-lab/parthenon/pull/267) Introduced TaskRegions and TaskCollections to allow for task launches on multiple blocks.
- [[PR 287]](https://github.com/parthenon-hpc-lab/parthenon/pull/287) Added machine configuration file for compile options, see [documentation](https://github.com/lanl/parthenon/blob/develop/docs/building.md#default-machine-configurations)
- [[PR 290]](https://github.com/parthenon-hpc-lab/parthenon/pull/290) Added per cycle performance output diagnostic.
- [[PR 298]](https://github.com/parthenon-hpc-lab/parthenon/pull/298) Introduced Partition, a tiny utility for partitioning STL containers. Used for MeshBlockPacks, to enable packing over a fraction of the mesh.

### Changed
- [[PR 68]](https://github.com/parthenon-hpc-lab/parthenon/issues/68) Moved default `par_for` wrappers to `MeshBlock`
- [[PR 243]](https://github.com/parthenon-hpc-lab/parthenon/pull/243) Automatically find/check Python version used in regression tests. Bumps CMake minimum version to 3.12
- [[PR 266]](https://github.com/parthenon-hpc-lab/parthenon/pull/266): It is no longer necessary to specify Kokkos_ENABLE_OPENMP this is by default enabled, to turn off one can specify PARTHENON_DISABLE_OPENMP.

### Fixed
- [[PR 271]](https://github.com/parthenon-hpc-lab/parthenon/issues/256): Fix setting default CXX standard.
- [[PR 262]](https://github.com/parthenon-hpc-lab/parthenon/pull/262) Fix setting of "coverage" label in testing. Automatically applies coverage tag to all tests not containing "performance" label.
- [[PR 276]](https://github.com/parthenon-hpc-lab/parthenon/pull/276) Decrease required Python version from 3.6 to 3.5.
- [[PR 283]](https://github.com/parthenon-hpc-lab/parthenon/pull/283) Change CI to extended nightly develop tests and short push tests.
- [[PR 291]](https://github.com/parthenon-hpc-lab/parthenon/pull/291) Adds Task Diagram to documentation.

### Removed
- [[PR 282]](https://github.com/parthenon-hpc-lab/parthenon/pull/282) Integrated MeshBlockPack and tasking in pi example
- [[PR 294]](https://github.com/parthenon-hpc-lab/parthenon/pull/294) Fix `IndexShape::GetTotal(IndexDomain)` - previously was returning opposite of expected domain result.


## Release 0.1.0
Date: 8/4/2020

Initial release of Parthenon AMR infrastructure.

### Changed
- [[PR 214]](https://github.com/parthenon-hpc-lab/parthenon/pull/214): The weak linked routines for user-specified parthenon behavior have been removed in favor of a more portable approach. See [the documentation](docs/README.md#user-specified-internal-functions).
