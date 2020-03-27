//========================================================================================
// Parthenon performance portable AMR framework
// Copyright(C) 2020 The Parthenon collaboration
// Licensed under the 3-clause BSD License, see LICENSE file for details
//========================================================================================
// (C) (or copyright) 2020. Triad National Security, LLC. All rights reserved.
//
// This program was produced under U.S. Government contract 89233218CNA000001
// for Los Alamos National Laboratory (LANL), which is operated by Triad
// National Security, LLC for the U.S. Department of Energy/National Nuclear
// Security Administration. All rights in the program are reserved by Triad
// National Security, LLC, and the U.S. Department of Energy/National Nuclear
// Security Administration. The Government is granted for itself and others
// acting on its behalf a nonexclusive, paid-up, irrevocable worldwide license
// in this material to reproduce, prepare derivative works, distribute copies to
// the public, perform publicly and display publicly, and to permit others to do
// so.
//========================================================================================

#include <catch2/catch.hpp>

#include "kokkos_abstraction.hpp"
#include "parthenon_arrays.hpp"

using parthenon::ParArrayND;
using Real = double;

TEST_CASE("ParArrayND","[ParArrayND],[Kokkos]") {

  GIVEN("A ParArrayND with some dimensions") {
    constexpr int N1 = 2;
    constexpr int N2 = 3;
    constexpr int N3 = 4;
    ParArrayND<Real> a("test",N3,N2,N1);
    WHEN("We fill it with increasing integers") {
      auto view = a.Get();
      auto mirror = a.GetMirror();
      int n = 0;
      int sum_host = 0;
      for (int k = 0; k < N3; k++) {
        for (int j = 0; j < N2; j++) {
          for (int i = 0; i < N1; i++) {
            mirror(0,0,0,k,j,i) = n;
            sum_host += n;
            n++;
          }
        }
      }
      Kokkos::deep_copy(view,mirror);
      THEN("the sum of the lower three indices is correct") {
        int sum_device;
        using policy = Kokkos::MDRangePolicy<Kokkos::Rank<3>>;
        Kokkos::parallel_reduce(policy({0,0,0}, {N3,N2,N1}),
                                KOKKOS_LAMBDA(const int k, const int j,
                                              const int i,
                                              int& update) {
                                  update += a(k,j,i);
                                },
                                sum_device);
        REQUIRE( sum_host == sum_device );
      }
      THEN("the sum of the lower TWO indices is correct") {
        int sum_host = 0;
        int n = 0;
        for (int j = 0; j < N2; j++) {
          for (int i = 0; i < N1; i++) {
            sum_host += n;
            n++;
          }
        }
        int sum_device;
        using policy = Kokkos::MDRangePolicy<Kokkos::Rank<2>>;
        Kokkos::parallel_reduce(policy({0,0}, {N2,N1}),
                                KOKKOS_LAMBDA(const int j, const int i,
                                              int& update) {
                                  update += a(j,i);
                                },
                                sum_device);
        REQUIRE( sum_host == sum_device );
        AND_THEN("We can get a raw 2d subview and it works the same way.") {
        auto v2d = a.Get<2>();
        int sum_device;
        using policy = Kokkos::MDRangePolicy<Kokkos::Rank<2>>;
        Kokkos::parallel_reduce(policy({0,0}, {N2,N1}),
                                KOKKOS_LAMBDA(const int j, const int i,
                                              int& update) {
                                  update += v2d(j,i);
                                },
                                sum_device);
        REQUIRE( sum_host == sum_device );
        }
      }
      THEN("slicing is possible") {
        // auto b = a.SliceD(std::make_pair(1,3),3);
        auto b = a.SliceD<3>(std::make_pair(1,3));
        AND_THEN("slices have correct values.") {
          int total_errors;
          using policy = Kokkos::MDRangePolicy<Kokkos::Rank<3>>;
          Kokkos::parallel_reduce(policy({0,0,0}, {2,N2,N1}),
                                  KOKKOS_LAMBDA(const int k,
                                                const int j,
                                                const int i,
                                                int& update) {
                                    update += (b(k,j,i) == a(k+1,j,i)) ? 0 : 1;
                                  },
                                  total_errors);
          REQUIRE( total_errors == 0 );
        }
      }
    }
  }
}
