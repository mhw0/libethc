cmake_minimum_required(VERSION 3.0)
project(ethc LANGUAGES C VERSION 0.0.0)

include(GNUInstallDirs)

set(ETHC_SOURCE_FILES src/address.c src/hex.c src/keccak256.c)

option(ENABLE_TESTING "Enable testing" OFF)

add_library(${PROJECT_NAME} SHARED ${ETHC_SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 99
    SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries(${PROJECT_NAME} keccak)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

add_subdirectory(libkeccak)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION . FILES_MATCHING PATTERN "*.h")
install(FILES ${CMAKE_BINARY_DIR}/ethc.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

configure_file(ethc.pc.in ethc.pc @ONLY)
