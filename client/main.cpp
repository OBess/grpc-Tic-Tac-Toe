// C++
#include <iostream>

// gRPC
#include "include/client.hpp"

int main(int argc, char const *argv[])
{
    client::Run("localhost:4030");
    return EXIT_SUCCESS;
}
