project(ethc-run-tests)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBTAP REQUIRED tap)

set(TEST_SOURCE_FILES run-tests.c test-eth-keccak256.c test-eth-hex.c test-eth-address.c)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${TEST_SOURCE_FILES})

set(TEST_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include ${LIBTAP_INCLUDE_DIRS})

target_include_directories(
  ${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${TEST_INCLUDE_DIRECTORIES}>")
target_link_directories(${PROJECT_NAME} PUBLIC ${LIBTAP_LIBRARY_DIRS})

target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME} ${LIBTAP_LIBRARIES})
