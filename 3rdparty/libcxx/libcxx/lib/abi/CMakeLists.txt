
if (DEFINED TARGET_TRIPLE)
    # Ignore the major, minor, and patchlevel versions of the darwin
    # target.
    string(REGEX REPLACE "darwin([0-9]+)\\.([0-9]+)\\.([0-9]+)" "darwin"
      GENERIC_TARGET_TRIPLE "${TARGET_TRIPLE}")
endif()

# Detect if we are building in the same configuration used to generate
# the abilist files.
set(ABILIST_FILE "${CMAKE_CURRENT_LIST_DIR}/${GENERIC_TARGET_TRIPLE}.v${LIBCXX_ABI_VERSION}.abilist")
set(SYMDIFF_EXE "${LIBCXX_SOURCE_DIR}/utils/sym_diff.py")
if (EXISTS "${ABILIST_FILE}"
    AND TARGET cxx_shared
    AND ("${LIBCXX_CXX_ABI_LIBNAME}" STREQUAL "libcxxabi" OR
         (APPLE AND "${LIBCXX_CXX_ABI_LIBNAME}" STREQUAL "default"))
    AND NOT LIBCXX_ABI_UNSTABLE)
    add_custom_target(check-cxx-abilist
            ${SYMDIFF_EXE} --only-stdlib-symbols --strict ${ABILIST_FILE}
            $<TARGET_SONAME_FILE:cxx_shared>
            DEPENDS cxx_shared
            COMMENT "Testing ABI compatibility...")
else()
    message(STATUS "there is no pre-generated ABI list for the requested libc++ configuration. "
            "check-cxx-abilist target is not supported")
endif()
