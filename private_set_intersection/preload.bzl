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

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def psi_preload():
    if "rules_proto" not in native.existing_rules():
        http_archive(
            name = "rules_proto",
            sha256 = "602e7161d9195e50246177e7c55b2f39950a9cf7366f74ed5f22fd45750cd208",
            strip_prefix = "rules_proto-97d8af4dc474595af3900dd85cb3a29ad28cc313",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
                "https://github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
            ],
        )

    if "io_bazel_rules_go" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_go",
            sha256 = "7b9bbe3ea1fccb46dcfa6c3f3e29ba7ec740d8733370e21cdc8937467b4a4349",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v0.22.4/rules_go-v0.22.4.tar.gz",
                "https://github.com/bazelbuild/rules_go/releases/download/v0.22.4/rules_go-v0.22.4.tar.gz",
            ],
        )

    if "bazel_gazelle" not in native.existing_rules():
        BAZEL_GAZELLE_VER = "0.20.0"
        BAZEL_GAZELLE_SHA256 = "d8c45ee70ec39a57e7a05e5027c32b1576cc7f16d9dd37135b0eddde45cf1b10"
        http_archive(
            name = "bazel_gazelle",
            sha256 = BAZEL_GAZELLE_SHA256,
            urls = [
                "https://storage.googleapis.com/bazel-mirror/github.com/bazelbuild/bazel-gazelle/releases/download/v%s/bazel-gazelle-v%s.tar.gz" % (BAZEL_GAZELLE_VER, BAZEL_GAZELLE_VER),
                "https://github.com/bazelbuild/bazel-gazelle/releases/download/v%s/bazel-gazelle-v%s.tar.gz" % (BAZEL_GAZELLE_VER, BAZEL_GAZELLE_VER),
            ],
        )

    if "rules_pkg" not in native.existing_rules():
        RULES_PKG_VER = "0.2.6"
        RULES_PKG_SHA256 = "aeca78988341a2ee1ba097641056d168320ecc51372ef7ff8e64b139516a4937"
        http_archive(
            name = "rules_pkg",
            urls = [
                "https://github.com/bazelbuild/rules_pkg/releases/download/%s-1/rules_pkg-%s.tar.gz" % (RULES_PKG_VER, RULES_PKG_VER),
                "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/%s/rules_pkg-%s.tar.gz" % (RULES_PKG_VER, RULES_PKG_VER),
            ],
            sha256 = RULES_PKG_SHA256,
        )

    if "pybind11_bazel" not in native.existing_rules():
        http_archive(
            name = "pybind11_bazel",
            strip_prefix = "pybind11_bazel-203508e14aab7309892a1c5f7dd05debda22d9a5",
            urls = ["https://github.com/pybind/pybind11_bazel/archive/203508e14aab7309892a1c5f7dd05debda22d9a5.zip"],
            sha256 = "75922da3a1bdb417d820398eb03d4e9bd067c4905a4246d35a44c01d62154d91",
        )

    if "pybind11" not in native.existing_rules():
        http_archive(
            name = "pybind11",
            build_file = "@pybind11_bazel//:pybind11.BUILD",
            strip_prefix = "pybind11-2.5.0",
            urls = ["https://github.com/pybind/pybind11/archive/v2.5.0.zip"],
            sha256 = "1859f121837f6c41b0c6223d617b85a63f2f72132bae3135a2aa290582d61520",
        )

    if "rules_python" not in native.existing_rules():
        http_archive(
            name = "rules_python",
            url = "https://github.com/bazelbuild/rules_python/archive/a0fbf98d4e3a232144df4d0d80b577c7a693b570.zip",
            strip_prefix = "rules_python-a0fbf98d4e3a232144df4d0d80b577c7a693b570",
            sha256 = "98c9b903f6e8fe20b7e56d19c4822c8c49a11b475bd4ec0ca6a564e8bc5d5fa2",
        )

    RULES_PYTHON_EXTERNAL_VERSION = "3aacabb928a710b10bff13d0bde49ceaade58f15"
    if "rules_python_external" not in native.existing_rules():
        http_archive(
            name = "rules_python_external",
            sha256 = "5a1d7e6e4bab49dcdd787694f0f5d52ac5debdfc1852981a89cc414e338d60dc",
            strip_prefix = "rules_python_external-{version}".format(version = RULES_PYTHON_EXTERNAL_VERSION),
            url = "https://github.com/dillon-giacoppo/rules_python_external/archive/{version}.zip".format(version = RULES_PYTHON_EXTERNAL_VERSION),
        )

    RULES_JVM_EXTERNAL_TAG = "3.3"
    RULES_JVM_EXTERNAL_SHA = "d85951a92c0908c80bd8551002d66cb23c3434409c814179c0ff026b53544dab"
    if "rules_jvm_external" not in native.existing_rules():
        http_archive(
            name = "rules_jvm_external",
            strip_prefix = "rules_jvm_external-%s" % RULES_JVM_EXTERNAL_TAG,
            sha256 = RULES_JVM_EXTERNAL_SHA,
            url = "https://github.com/bazelbuild/rules_jvm_external/archive/%s.zip" % RULES_JVM_EXTERNAL_TAG,
        )
