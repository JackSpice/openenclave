# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_custom_command(OUTPUT remoteattestation_u.h remoteattestation_u.c
  DEPENDS ${CMAKE_SOURCE_DIR}/remoteattestation.edl
  COMMAND openenclave::oeedger8r --untrusted ${CMAKE_SOURCE_DIR}/remoteattestation.edl)

add_executable(remote_attestation_host host.cpp ${CMAKE_CURRENT_BINARY_DIR}/remoteattestation_u.c)

target_include_directories(remote_attestation_host PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../ # For common/shared.h
  ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(remote_attestation_host openenclave::oehostapp)
