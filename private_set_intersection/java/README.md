# PSI - Java

## Build and test

```
bazel build //private_set_intersection/java/...
```

```
bazel test //private_set_intersection/java/...
```

## Development

Generating C++ headers with `javac -h` won't work, because there are Java dependencies on
Java protobuf code that makes linking difficult. **The best option is to hand-write
the C++ headers.**

During development, the java code should first be written. Then, the C++ headers
should be updated. Finally, the C++ code should be written according to the C++
headers. The C++ code does most of the heavy-lifting, by converting Java objects
to their C++ counterparts and calling the C++ code in `//private_set_intersection/cpp/`.
