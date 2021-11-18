project(ethc-run-tests)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
add_definitions(${GLIB_CFLAGS_OTHER})

add_test(NAME ethc-run-tests COMMAND ethc-run-tests)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
  --force-new-ctest-process
  --verbose
  --output-on-failure
  --progress
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(ethc-run-tests run-tests.c)

target_link_libraries(ethc-run-tests ethc)
target_link_libraries(ethc-run-tests ${GLIB_LIBRARIES})
