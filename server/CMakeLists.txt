
add_executable(server main.cpp inlcude/server.cpp ../protos/include/game_service.pb.cc ../protos/include/game_service.grpc.pb.cc)
target_include_directories(server PUBLIC "../protos/include")
target_link_libraries(server gRPC::grpc++)