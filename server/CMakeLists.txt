
add_executable(server main.cpp include/game_service.pb.cc include/game_service.grpc.pb.cc)
target_include_directories(server PUBLIC include)
target_link_libraries(server gRPC::grpc++)