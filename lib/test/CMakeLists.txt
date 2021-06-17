# Distributed under the MIT License (See accompanying file /LICENSE )

# CMake build : library tests

#configure variables
set (TEST_MAIN "${LIB_NAME}Test")
set (TEST_RUNNER_PARAMS "")  # Any arguemnts to feed the test runner (change as needed).

#configure directories
set (TEST_MODULE_PATH "${LIBRARY_MODULE_PATH}/test")

#configure test directories
set (TEST_SRC_PATH  "${TEST_MODULE_PATH}/src" )

#set includes
include_directories (${LIBRARY_INCLUDE_PATH} ${TEST_THIRD_PARTY_INCLUDE_PATH} ${Boost_INCLUDE_DIRS})

#set test sources
file (GLOB TEST_SOURCE_FILES "${TEST_SRC_PATH}/*.cpp")

#set concepts TS
# add_definitions ( -std=c++17 )

#set target executable
add_executable (${TEST_MAIN} ${TEST_SOURCE_FILES})

#add the library
target_link_libraries (${TEST_MAIN} PRIVATE ${LIB_NAME} ${LIBS}  Threads::Threads)

# Turn on CMake testing capabilities
enable_testing()

#parse catch tests
## ParseAndAddCatchTests (${TEST_MAIN})
add_test (NAME ${TEST_MAIN} COMMAND ${TEST_MAIN} ${TEST_RUNNER_PARAMS})

# Adds a 'coverage' target.
include(CodeCoverage)
