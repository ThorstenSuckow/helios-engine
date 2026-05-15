# helios-engine

## CMake package usage

Build and install:

```bash
cmake -S . -B build -DHELIOS_ENGINE_ENABLE_PACKAGE=ON -DCMAKE_INSTALL_PREFIX="$PWD/install"
cmake --build build
cmake --install build
```

Consume from another project:

```cmake
find_package(helios-engine CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE helios::helios)
```

Configure a consumer against an installed prefix:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/helios-prefix"
cmake --build build
```


