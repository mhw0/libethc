PROJECT(ethc-run-tests)

SET(TEST_SOURCE_FILES
    run-tests.c
    test-eth-bloom.c
    test-eth-account.c
    test-eth-abi.c
    test-eth-ecdsa.c
    test-eth-unit.c
    test-eth-keccak256.c
    test-eth-hex.c
    test-eth-rlp.c
    test-eth-address.c)

ADD_TEST(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_SOURCE_FILES})

GET_FILENAME_COMPONENT(PARENT_DIR ../ ABSOLUTE)

SET(TEST_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include ${PARENT_DIR}/libtap)

TARGET_INCLUDE_DIRECTORIES(
  ${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${TEST_INCLUDE_DIRECTORIES}>")

TARGET_LINK_DIRECTORIES(${PROJECT_NAME} PUBLIC ${PARENT_DIR}/libtap)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_PROJECT_NAME} tap.a)
