# PSI - Java

## Build and test

```
bazel build //private_set_intersection/java/...
```

```
bazel test //private_set_intersection/java/...
```

## Generating headers

Generate C++ headers from Java:
```
cd private_set_intersection/java/
javac -h src/main/c/ src/main/java/org/openmined/PsiServer.java
```
This produces `src/main/c/org_openmined_PsiServer.h`.

During development, the java code should first be written. Then, the C++ headers
should be generated. Finally, the C++ code should be written according to the C++
headers. The C++ code does most of the heavy-lifting, by converting Java objects
to their C++ counterparts and calling the C++ code in `//private_set_intersection/cpp/`.
