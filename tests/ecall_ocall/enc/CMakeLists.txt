# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

oeedl_file(../ecall_ocall.edl enclave gen)

add_enclave(TARGET ecall_ocall_enc CXX SOURCES enc.cpp ${gen})
target_compile_features(ecall_ocall_enc PRIVATE cxx_auto_type)

target_include_directories(ecall_ocall_enc PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
