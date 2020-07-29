cmake_minimum_required(VERSION 2.8)

message("httpclient")
#message(${PROJECT_SOURCE_DIR})
#message(${PROJECT_BINARY_DIR})

include_directories(${PROJCT_SOURCE_DIR})

#set(SRC main.cpp http_client.cpp ../common/mongoose.c)
aux_source_directory(${PROJECT_SOURCE_DIR}/common COMMON_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/httpclient CLIENT_SRC)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(http_client ${COMMON_SRC} ${CLIENT_SRC})