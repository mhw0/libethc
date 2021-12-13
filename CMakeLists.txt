project(ethc LANGUAGES C)

cmake_minimum_required(VERSION 3.21)

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED gmp)

include_directories(${GMP_INCLUDE_DIRS})
link_directories(${GMP_LIBRARY_DIRS})
add_definitions(${GMP_CFLAGS_OTHER})

set(ETHC_SOURCE_FILES src/address.c src/hex.c src/keccak256.c)

add_library(ethc SHARED ${ETHC_SOURCE_FILES})

set_target_properties(ethc PROPERTIES C_STANDARD 99)

option(ENABLE_TESTING "Enable testing" ON)

if(DEFINED ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

add_subdirectory(src/keccak256)

target_link_libraries(ethc ${GMP_LIBRARIES})
target_include_directories(ethc PRIVATE include/)

# install the dylib
install(TARGETS ethc LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
# install the header files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION . FILES_MATCHING PATTERN "*.h")

install(FILES ${CMAKE_BINARY_DIR}/ethc.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

configure_file(ethc.pc.in ethc.pc @ONLY)
