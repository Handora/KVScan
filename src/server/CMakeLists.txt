add_library(server server.cpp)

add_executable(kvserver main.cpp )
target_link_libraries(kvserver server page disk buffer util ${PROJECT_SOURCE_DIR}/third_party/rpclib/build/librpc.a pthread)
