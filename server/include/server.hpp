#pragma once

// C++
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <future>
#include <mutex>

// gRPC
#include <grpcpp/grpcpp.h>

// Service
#include "game_service.grpc.pb.h"

// Usings
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using GameService::ConnectRequest;
using GameService::ConnectResponse;
using GameService::DisconnectRequest;
using GameService::DisconnectResponse;
using GameService::StateRequest;
using GameService::StateResponse;
using GameService::StepRequest;
using GameService::StepResponse;

// Colors
constexpr auto NC =     "\e[0m";
constexpr auto RED =    "\e[0;31m";
constexpr auto GRN =    "\e[0;32m";
constexpr auto YEL =    "\e[0;33m";
constexpr auto BLU =    "\e[0;34m";
constexpr auto CYN =    "\e[0;36m";
constexpr auto REDB =   "\e[41m";

namespace server
{
    // Main method of start
    void Run(const std::string &ip) noexcept;
};