add_library(server server.cpp)

add_executable(kvserver main.cpp )
target_link_libraries(kvserver server page disk buffer util rpc pthread)
