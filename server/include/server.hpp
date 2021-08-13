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

using GameService::ReadyRequest;
using GameService::ReadyResponse;
using GameService::StepRequest;
using GameService::MapResponse;

class GameServerImpl final : public GameService::GameService::Service
{
private:
    std::string m_map{};

    int m_sides{};

public:
    GameServerImpl() = default;
    ~GameServerImpl() = default;

    Status StartGame(ServerContext* context, const ReadyRequest* request, ReadyResponse* response) override
    {
        return Status::OK;
    }

    Status MakeStep(ServerContext* context, const StepRequest* request, MapResponse* response) override
    {
        response->set_map(m_map);
        return Status::OK;
    }
};