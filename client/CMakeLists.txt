
add_executable(client main.cpp include/game_service.pb.cc include/game_service.grpc.pb.cc)
target_include_directories(client PUBLIC include)
target_link_libraries(client gRPC::grpc++)