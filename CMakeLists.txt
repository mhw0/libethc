cmake_minimum_required(VERSION 3.0)
project(
  ethc
  LANGUAGES C
  VERSION 0.8.0)

include(FindPkgConfig)
include(GNUInstallDirs)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(ETHC_SOURCE_FILES
    src/abi.c
    src/account.c
    src/address.c
    src/ecdsa.c
    src/bloom.c
    src/internals.c
    src/unit.c
    src/utf8.c
    src/hex.c
    src/rlp.c
    src/keccak256.c)

option(ENABLE_TESTING "Enable testing" OFF)

pkg_check_modules(GMP REQUIRED gmp)
pkg_check_modules(SECP256K1 REQUIRED libsecp256k1)

add_library(${PROJECT_NAME} SHARED ${ETHC_SOURCE_FILES})

set_target_properties(
  ${PROJECT_NAME} PROPERTIES C_STANDARD 90 SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries(${PROJECT_NAME} keccak m ${GMP_LINK_LIBRARIES} ${SECP256K1_LINK_LIBRARIES})
target_include_directories(
  ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                          ${GMP_INCLUDE_DIRS} ${SECP256K1_INCLUDE_DIRS})

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
