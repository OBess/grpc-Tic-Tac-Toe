// C++
#include <iostream>

// gRPC
#include "include/server.hpp"

int main()
{
    std::cout << BLU << "Start of server!" << NC << std::endl;
    server::Run("localhost:4030");
    std::cout << BLU << "End of server!" << NC << std::endl;
    return EXIT_SUCCESS;
}
