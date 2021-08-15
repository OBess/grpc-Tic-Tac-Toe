#pragma once

// C++
#include <iostream>
#include <memory>
#include <string>

// Proto
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

// Service
#include "game_service.grpc.pb.h"

// Usings
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using GameService::MapResponse;
using GameService::ReadyRequest;
using GameService::ReadyResponse;
using GameService::StepRequest;

class GameServerImpl final : public GameService::GameService::Service
{
private:
    // Map of game (3x3)
    std::string m_map{};

    // Count players
    int m_players{};

public:
    GameServerImpl() = default;
    ~GameServerImpl() = default;

    // Block to copy
    GameServerImpl(const GameServerImpl &) = delete;
    GameServerImpl(GameServerImpl &&) = delete;

    // Main method of start
    void Run() noexcept
    {
    }

    // Network methods
    Status StartGame(ServerContext *context, const ReadyRequest *request, ReadyResponse *response) override
    {
        return Status::OK;
    }

    Status MakeStep(ServerContext *context, const StepRequest *request, MapResponse *response) override
    {
        response->set_map(m_map);
        return Status::OK;
    }

    // Block to copy
    GameServerImpl &operator=(const GameServerImpl &) = delete;
    GameServerImpl &operator=(const GameServerImpl &&) = delete;
};