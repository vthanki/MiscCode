#
# Add GoogleTest target for the source files given by including required
# headers and linking required google test libraries
#
# Parameters:
# TARGET:           Name of the target
# SOURCES:          List of source files
# GTEST_DIR:        Optional - path of googltest installation which will be used to extract headers and libraries
# 
# Example:
# add_gtest(MyFirstGtest 
#       SOURCES
#           main_test.cpp
#           src1.cpp 
#       GTEST_DIR
#           /path/to/googletest
# )

# TODO:
#   * Check whether google test directory has a build directory
#   * Obtain the library names based on platform (e.g. gmockd v/s gmock)

include(CMakeParseArguments)

function(add_gtest TARGET)
    set(_SINGLE_VALUE_ARGS GTEST_DIR)
	set(_MULTI_VALUE_ARGS SOURCES)
	cmake_parse_arguments(_ARG "${_OPTION_ARGS}" "${_SINGLE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})
    
    message(STATUS "Adding google tests for target ${TARGET}")

    add_executable(${TARGET}
        ${_ARG_SOURCES}
    )

    if (_ARG_GTEST_DIR)
        target_include_directories(${TARGET}
            PRIVATE
                ${_ARG_GTEST_DIR}/googlemock/include
                ${_ARG_GTEST_DIR}/googletest/include
        )

        find_library(_LIBGMOCK
               gmockd ${_ARG_GTEST_DIR}/build/googlemock
        )

        find_library(_LIBGTEST
               gtestd ${_ARG_GTEST_DIR}/build/googlemock/gtest
        )

        target_link_libraries(${TARGET}
            ${_LIBGMOCK}
            ${_LIBGTEST}
        )
    else()
        target_link_libraries(${TARGET}
            gmockd
            gtestd
        )
    endif()

endfunction()
