Open Enclave SDK CMake Package
==============================

This [CMake Config-file package](https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html)
is provided for use with CMake's
[`find_package` command](https://cmake.org/cmake/help/latest/command/find_package.html).

In a CMake project which is using The Open Enclave SDK, you find the package the
following in your `CMakeLists.txt`:

```cmake
find_package(OpenEnclave CONFIG REQUIRED)
```

This will bring in the Open Enclave targets under the `openenclave::` namespace.

The targets relevant to users of the SDK are:

- `openenclave::oeedger8r`
- `openenclave::oesign`
- `openenclave::oeenclave`
- `openenclave::oelibcxx`
- `openenclave::oehostapp`

The remaining targets in `openenclave-targets.cmake` are automatically included
when needed by the above.

The package will also search for (and requires) the
[`Threads` package](https://cmake.org/cmake/help/latest/module/FindThreads.html).
The libraries `crypto` and `dl` will also need to be available on the system.

Examples
--------

### Running `oeedger8r`

The `openenclave::oeedger8r` target is an executable that generates trusted and
untrusted C bindings from an Enclave Definition Language file. Example usage is:

```cmake
# Trusted bindings for the enclave
add_custom_command(OUTPUT example_t.h example_t.c
  COMMAND openenclave::oeedger8r --trusted ${CMAKE_CURRENT_SOURCE_DIR}/example.edl)

# Untrusted bindings fo rthe host
add_custom_command(OUTPUT example_u.h example_u.c
  COMMAND openenclave::oeedger8r --untrusted ${CMAKE_CURRENT_SOURCE_DIR}/example.edl)
```

Note that `oeedger8r` emits files in the current working directory, which is
`${CMAKE_CURRENT_BINARY_DIR}` by default, but this can be changed with the
`WORKING_DIRECTORY` argument to
[`add_custom_command`](https://cmake.org/cmake/help/latest/command/add_custom_command.html).

To trigger the generation of the `OUTPUT` files, you will need to specify them
as input source files to another target (such as an enclave and a host).

Because both the enclave(s) and host need files generated by `oeedger8r`, you
probably want to use two separate commands with just the `--trusted` and
`--untrusted` flags respectively, even though they can be given simultaneously.
This is because the output of a CMake custom command can only be used as an
input to a CMake target in the same `CMakeLists.txt`, and your enclave and host
targets are probably in separate folders. See our samples for a complete build
structure example.

### Creating an unsigned enclave

Enclave targets in CMake should use `add_executable`, link to
`openenclave::oeenclave`, and include the trusted EDL code if `oeedger8r` was
used. An example is:

```cmake
add_executable(example_enclave example.c example_t.c)
target_link_libraries(example_enclave openenclave::oeenclave)
```

If the enclave uses C++ code, you also need to link `openenclave::oelibcxx`:

```cmake
add_executable(example_cpp_enclave example.cpp example_t.c)
target_link_libraries(example_cpp_enclave openenclave::oeenclave openenclave::oelibcxx)
```

Note that to opt-in to our latest API version, you should also use:

```cmake
target_compile_definitions(example_enclave OE_API_VERSION=2)
```

### Signing an enclave

The `openenclave::oesign` target is an executable that takes an unsigned
enclave, an enclave configuration file, and a private key, and outputs a signed
enclave. Example usage is:

```cmake
add_custom_command(OUTPUT example_enclave.signed
  COMMAND openenclave::oesign sign $<TARGET_FILE:example_enclave> ${CMAKE_CURRENT_SOURCE_DIR}/example.conf ${CMAKE_CURRENT_SOURCE_DIR}/key.pem)
```

Since `oesign` is unaware of CMake information, the
[CMake Generator Expression](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html)
`$<TARGET_FILE:example_enclave>` is used. Given that `example_enclave` is a
valid CMake target, the generator expression will replace itself with the
absolute path to the output file of that build target.

Note that `oesign` emits the signed enclave next to the unsigned enclave, which
typically is in the build directory corresponding to the source directory of the
enclave. The current working directory does not affect it.

Because this is a custom command, you will need to depend on
`example_encalve.signed` from a
[custom target](https://cmake.org/cmake/help/latest/command/add_custom_target.html).

```cmake
add_custom_target(signed_example_enclave ALL
  DEPENDS example_enclave.signed)
```

This adds a default target to the build (by the use of `ALL`) which ensures the
signing takes place. You do not want to use `add_custom_target(COMMAND ...)`
instead of `add_custom_command` because the former would run on every build, but
the latter sets up a dependency where it only signs if the enclave is rebuilt.

### Creating a host

Host targets in CMake should use `add_executable`, link to
`openenclave::oehostapp`, and include the untrusted EDL code if `oeedger8r` was
used. An example is:

```cmake
add_executable(example_host host.cpp example_u.c)
target_link_libraries(example_host openenclave::oehostapp)
```

### Running the enclave

Finally, you can run the enclave with:

```sh
./example_host /path/to/example_enclave.signed
```
