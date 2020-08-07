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

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@io_bazel_rules_go//proto:def.bzl", "go_proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "psi_proto",
    srcs = ["psi.proto"],
)

cc_proto_library(
    name = "psi_cc_proto",
    deps = [":psi_proto"],
)

go_proto_library(
    name = "psi_go_proto",
    importpath = "github.com/openmined/psi/pb",
    protos = [":psi_proto"],
)

java_proto_library(
    name = "psi_java_proto",
    deps = [":psi_proto"]
)
