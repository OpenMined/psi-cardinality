package(default_visibility = ["//visibility:public"])

load("@io_bazel_rules_rust//rust:rust.bzl", "rust_doc", "rust_library", "rust_test")
load("@io_bazel_rules_rust//proto:proto.bzl", "rust_proto_library")
load("@io_bazel_rules_rust//proto:toolchain.bzl", "PROTO_COMPILE_DEPS")

rust_library(
    name = "rust_psi",
    srcs = [
        "src/client.rs",
        "src/lib.rs",
        "src/server.rs",
    ],
    deps = [
        "//private_set_intersection/c:c_psi_server",
        "//private_set_intersection/c:c_psi_client",
        "//private_set_intersection/c:c_package",
        "//third_party/cargo:libc",
        "//third_party/cargo:protobuf",
        "//private_set_intersection/proto:psi_rust_proto",
    ] + PROTO_COMPILE_DEPS,
)

rust_test(
    name = "rust_psi_unit_test",
    crate = ":rust_psi",
    deps = ["//third_party/cargo:semver"],
)

rust_test(
    name = "rust_psi_test",
    srcs = ["tests/client_server_test.rs"],
    deps = [":rust_psi"],
)

rust_doc(
    name = "rust_psi_doc",
    dep = ":rust_psi",
)
