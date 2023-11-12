cmake_minimum_required(VERSION 3.15)
project(config_parser LANGUAGES CXX)


set(RELEASE_TARGET_NAME ${PROJECT_NAME}_release)
set(RELEASE_SOURCES
    src/yaml_config_parser.cpp
)
set(RELEASE_LIBS
    yaml-cpp
)
add_library(${RELEASE_TARGET_NAME} STATIC ${RELEASE_SOURCES})
target_link_libraries(${RELEASE_TARGET_NAME} PRIVATE ${RELEASE_LIBS})
target_include_directories(${RELEASE_TARGET_NAME} PUBLIC include PRIVATE src)


add_subdirectory(test)
