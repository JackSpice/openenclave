# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

set(PLATFORM_SRC "")

CHECK_SYMBOL_EXISTS(getopt_long getopt.h HAVE_GETOPT_LONG)

if (NOT ${HAVE_GETOPT_LONG})
  set(PLATFORM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/getopt_long.c)
endif()

add_executable(oesign main.c oedump.c ${PLATFORM_SRC})

if (NOT ${HAVE_GETOPT_LONG})
  target_include_directories(oesign PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

target_link_libraries(oesign oehost)

# assemble into proper collector dir
set_property(TARGET oesign PROPERTY RUNTIME_OUTPUT_DIRECTORY ${OE_BINDIR})

# install rule
install(TARGETS oesign EXPORT openenclave-targets DESTINATION ${CMAKE_INSTALL_BINDIR})
