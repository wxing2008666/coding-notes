cmake_minimum_required(VERSION 2.8)

message("httpserver")
#message(${PROJECT_SOURCE_DIR})
#message(${PROJECT_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR})

#set(SRC main.cpp http_server.cpp ../common/mongoose.c)
aux_source_directory(${PROJECT_SOURCE_DIR}/common COMMON_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/httpserver SERVER_SRC)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

file(COPY ${PROJECT_BINARY_DIR}/httpserver/web DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_executable(http_server ${COMMON_SRC} ${SERVER_SRC})