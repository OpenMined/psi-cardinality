#
# Copyright 2020 the authors listed in CONTRIBUTORS.md
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "package",
    srcs = ["package.cpp"],
    hdrs = ["package.h"],
    deps = [
        "//private_set_intersection/cpp:package",
    ],
)

cc_test(
    name = "package_test",
    srcs = ["package_test.cpp"],
    deps = [
        ":package",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "internal_utils",
    srcs = ["internal_utils.cpp"],
    hdrs = ["internal_utils.h"],
    #visibility = ["//visibility:private"],
    deps = [
        "@private_join_and_compute//util:status",
    ],
)

cc_library(
    name = "psi_client",
    srcs = ["psi_client.cpp"],
    hdrs = ["psi_client.h"],
    deps = [
        "//private_set_intersection/c:internal_utils",
        "//private_set_intersection/cpp:psi_client",
    ],
)

cc_test(
    name = "psi_client_test",
    srcs = ["psi_client_test.cpp"],
    deps = [
        ":psi_client",
        "//private_set_intersection/cpp/util:status_matchers",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@private_join_and_compute//crypto:ec_commutative_cipher",
    ],
)

cc_library(
    name = "psi_server",
    srcs = ["psi_server.cpp"],
    hdrs = ["psi_server.h"],
    deps = [
        "//private_set_intersection/c:internal_utils",
        "//private_set_intersection/cpp:psi_server",
    ],
)

cc_test(
    name = "psi_server_test",
    srcs = ["psi_server_test.cpp"],
    deps = [
        ":psi_client",
        ":psi_server",
        "//private_set_intersection/cpp/util:status_matchers",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_binary(
    name = "psi_benchmark",
    srcs = ["psi_benchmark.cpp"],
    deps = [
        ":psi_client",
        ":psi_server",
        "@com_google_absl//absl/strings",
        "@com_google_benchmark//:benchmark_main",
    ],
)
