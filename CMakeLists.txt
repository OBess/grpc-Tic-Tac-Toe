cmake_minimum_required(VERSION 3.15)
project (ticTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
include("/usr/src/environment/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

add_subdirectory(server)
add_subdirectory(client)