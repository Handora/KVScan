add_library(server server.cpp)
target_link_libraries(page)

add_executable(kvserver main.cpp)
