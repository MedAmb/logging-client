set(TEST_TARGET_NAME yaml_${PROJECT_NAME}_unit_test)

set(TEST_SOURCES
    yaml_config_parser_test.cpp
)

set(TEST_LIBS
    ${RELEASE_TARGET_NAME}
    gtest
    gtest_main
)

add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_TARGET_NAME} PRIVATE ${TEST_LIBS})

get_target_property(INCLUDE_DIRS ${RELEASE_TARGET_NAME} INCLUDE_DIRECTORIES)

target_include_directories(${TEST_TARGET_NAME} PRIVATE ${INCLUDE_DIRS})

add_test(NAME config_parser_unit_test COMMAND ${TEST_TARGET_NAME})

