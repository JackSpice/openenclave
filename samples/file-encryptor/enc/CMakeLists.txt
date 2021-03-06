# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

# Use the edger8r to generate C bindings from the EDL file.
add_custom_command(OUTPUT fileencryptor_t.h fileencryptor_t.c
  DEPENDS ${CMAKE_SOURCE_DIR}/fileencryptor.edl
  COMMAND openenclave::oeedger8r --trusted ${CMAKE_SOURCE_DIR}/fileencryptor.edl)

add_executable(enclave ecalls.cpp encryptor.cpp keys.cpp ${CMAKE_CURRENT_BINARY_DIR}/fileencryptor_t.c)

target_compile_definitions(enclave PUBLIC OE_API_VERSION=2)

target_include_directories(enclave PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR} # Needed for #include "../shared.h"
  ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(enclave openenclave::oeenclave openenclave::oelibcxx)
