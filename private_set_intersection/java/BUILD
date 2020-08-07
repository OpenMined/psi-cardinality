package(default_visibility = ["//visibility:public"])

cc_library(
    name = "c_PsiServer",
    srcs = [
        "src/main/c/org_openmined_PsiServer.cpp",
    ],
    hdrs = [
        "src/main/c/org_openmined_PsiServer.h"
    ],
    deps = [
        "//private_set_intersection/c:internal_utils",
        "//private_set_intersection/cpp:psi_server",
        "@bazel_tools//tools/jdk:jni",
    ],
    alwayslink = True,
    linkstatic = True
)

cc_binary(
    name = "libc_PsiServer.dylib",
    linkshared = True,
    deps = [
        ":c_PsiServer"
    ]
)

java_library(
    name = "java_PsiServer",
    srcs = [
        "src/main/java/org/openmined/PsiServer.java"
    ],
    deps = [
        ":libc_PsiServer.dylib",
        "//private_set_intersection/proto:psi_java_proto"
    ]
)

java_test(
    name = "java_TestPsiServer",
    srcs = [
        "src/test/java/org/openmined/TestPsiServer.java"
    ],
    test_class = "org.openmined.TestPsiServer",
    deps = [
        ":java_PsiServer",
        "@maven//:junit_junit"
    ]
)
