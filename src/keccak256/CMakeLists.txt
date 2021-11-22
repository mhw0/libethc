project(keccak256 LANGUAGES C)

set(KECCAK256_SOURCE_FILES KeccakSponge.c KeccakSponge.inc FIPS202/KeccakHash.c KeccakP-1600/KeccakP-1600-compact64.c)
set(KECCAK256_HEADER_FILES FIPS202/ KeccakP-1600/ KeccakP-1600/common common/ .)

add_library(keccak256 SHARED ${KECCAK256_SOURCE_FILES})
target_include_directories(keccak256 PRIVATE ${KECCAK256_HEADER_FILES})

# is this ok ?
target_include_directories(ethc PRIVATE ${KECCAK256_HEADER_FILES})
target_include_directories(ethc-run-tests PRIVATE ${KECCAK256_HEADER_FILES})
target_link_libraries(ethc keccak256)
