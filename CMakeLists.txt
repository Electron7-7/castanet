cmake_minimum_required(VERSION 3.30.0..4.3.0)
project(castanet
    VERSION 2.2.0
    LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS "Generate a compile_commands.json file" ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS AND EXISTS ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json)
    file(COPY_FILE ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CASTANET_USE_ADDRESS_SANITIZER "Compile with address sanitizer" ON)
endif()

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(Sources.cmake)

add_executable(castanet ${CASTANET_SOURCES} ${GETARGS_SOURCES})
target_compile_options(castanet
    PUBLIC $<$<CONFIG:Debug>:-Wall -O0>
    PUBLIC $<$<BOOL:${CASTANET_USE_ADDRESS_SANITIZER}>:-g -fsanitize=address>)
target_link_options(castanet
    PUBLIC $<$<BOOL:${CASTANET_USE_ADDRESS_SANITIZER}>:-g -fsanitize=address>)
target_include_directories(castanet
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty>)

if(MSVC)
    target_compile_features(castanet PUBLIC cxx_std_23)
    set_property(TARGET castanet PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    target_compile_features(castanet PUBLIC cxx_std_26)
endif()
