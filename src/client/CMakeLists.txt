add_library(client client.cpp)

add_executable(kvclient main.cpp)
target_link_libraries(kvclient client page disk buffer util ${PROJECT_SOURCE_DIR}/third_party/rpclib/build/librpc.a pthread)
