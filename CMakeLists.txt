cmake_minimum_required(VERSION 3.10.0)

#set(CMAKE_CXX_COMPILER "/usr/lib/gcc/x86_64-redhat-linux/10.2.0/bin/g++")
#set(CMAKE_C_COMPILER "/usr/local/gcc/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/local/gcc/bin/g++")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS "-std=c++17 -static-libgcc -static-libstdc++")
project(reflex-format)
set(THRID_PARTY_DIR  ${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(
${THRID_PARTY_DIR}
${PROJECT_SOURCE_DIR}
)
aux_source_directory(./reflex_format REFLEX_FORMAT_LIST_FILES)
aux_source_directory(. REFLEX_LIST_FILES)
add_executable(${PROJECT_NAME} 
    ${REFLEX_FORMAT_LIST_FILES}
    ${REFLEX_LIST_FILES}
)
target_link_libraries(${PROJECT_NAME} tinyxml2)
add_subdirectory(thirdparty/tinyxml)
add_subdirectory(unit_test)