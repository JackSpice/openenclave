# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

# read tests.supported, sanitize the c-file, and create the test-case
file(STRINGS "tests.supported" alltests)
add_subdirectory(host)
if (BUILD_ENCLAVES)
	add_subdirectory(enc)
endif()

foreach(testcase ${alltests})
    get_testcase_name(${testcase} name "3rdparty/libunwind/libunwind/tests/")
	add_test(tests/libunwind/${name} ./host/libunwind_host ./enc/libunwindtest-${name}_enc)
endforeach(testcase)
