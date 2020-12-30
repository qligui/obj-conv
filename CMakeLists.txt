
cmake_minimum_required(VERSION 3.10.0)

project(sw-json)
#set(CMAKE_CXX_COMPILER "/usr/local/gcc/bin/g++")

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS "-std=c++14")
set(THRID_PARTY_DIR ./thirdparty)

include_directories(
    ${THRID_PARTY_DIR}
)

link_directories(

)
file(GLOB SW_JSON_LIST_FILES "./sw-json/*.hpp" "./*.cpp")
aux_source_directory(./sw-protocol SW_PROTOCOL_LIST_FILES)
aux_source_directory(./ SW_LIST_FILES)
add_executable(${PROJECT_NAME} 
    ${SW_PROTOCOL_LIST_FILES}
    ${SW_LIST_FILES}
)
target_link_libraries(${PROJECT_NAME}

)