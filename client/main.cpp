// C++
#include <iostream>

// gRPC
#include "include/client.hpp"

int main(int argc, char const *argv[])
{
    const std::string ip("localhost:4030");
    client::GameClient clt(grpc::CreateChannel(ip, grpc::InsecureChannelCredentials()));
    clt.Ready();
    return EXIT_SUCCESS;
}
