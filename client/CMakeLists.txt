
add_executable(client main.cpp include/client.cpp ../protos/include/game_service.pb.cc ../protos/include/game_service.grpc.pb.cc)
target_include_directories(client PUBLIC "../protos/include")
target_link_libraries(client gRPC::grpc++)