cmake_minimum_required(VERSION 3.0)
project(
  ethc
  LANGUAGES C
  VERSION 0.7.0)

include(FindPkgConfig)
include(GNUInstallDirs)

set(ETHC_SOURCE_FILES
    src/abi.c
    src/address.c
    src/bloom.c
    src/unit.c
    src/utf8.c
    src/hex.c
    src/keccak256.c)

option(ENABLE_TESTING "Enable testing" OFF)

pkg_check_modules(GMP REQUIRED gmp)

add_library(${PROJECT_NAME} SHARED ${ETHC_SOURCE_FILES})

set_target_properties(
  ${PROJECT_NAME} PROPERTIES C_STANDARD 99 SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries(${PROJECT_NAME} keccak m ${GMP_LINK_LIBRARIES})
target_include_directories(
  ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                          ${GMP_INCLUDE_DIRS})

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

add_subdirectory(libkeccak)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include
  DESTINATION .
  FILES_MATCHING
  PATTERN "*.h")
install(FILES ${CMAKE_BINARY_DIR}/libethc.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

configure_file(libethc.pc.in libethc.pc @ONLY)
