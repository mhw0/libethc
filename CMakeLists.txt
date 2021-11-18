project(ethc LANGUAGES C)

cmake_minimum_required(VERSION 3.21)

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GMP REQUIRED gmp)

include_directories(${GLIB_INCLUDE_DIRS} ${GMP_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS} ${GMP_LIBRARY_DIRS})
add_definitions(${GLIB_CFLAGS_OTHER} ${GMP_CFLAGS_OTHER})

set(ETHC_SOURCE_FILES src/main.c)

add_library(ethc SHARED ${ETHC_SOURCE_FILES})

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND DEFINED BUILD_ETHC_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()

target_link_libraries(ethc ${GLIB_LIBRARIES} ${GMP_LIBRARIES})
target_include_directories(ethc PRIVATE include/)

# install the dylib
install(TARGETS ethc LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
# install the header files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION . FILES_MATCHING PATTERN "*.h")

install(FILES ${CMAKE_BINARY_DIR}/ethc.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

configure_file(ethc.pc.in ethc.pc @ONLY)
