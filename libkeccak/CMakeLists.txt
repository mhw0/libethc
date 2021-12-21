project(keccak LANGUAGES C)

cmake_minimum_required(VERSION 3.0)

set(KECCAK_SOURCE_FILES KeccakHash.c KeccakSponge.c KeccakSponge.inc KeccakP-1600-compact64.c)

add_library(keccak STATIC ${KECCAK_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_POSITION_INDEPENDENT_CODE ON)

target_include_directories(keccak
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# https://github.com/XKCP/XKCP/blob/master/lib/LowLevel.build#L147
#set(CMAKE_C_FLAGS "-O2 -g0 -march=native -mtune=native")

target_compile_definitions(keccak PUBLIC XKCP_has_Sponge_Keccak XKCP_has_FIPS202 XKCP_has_KeccakP1600)
