#pragma once

// C++
#include <iostream>
#include <memory>
#include <string>

// gRPC
#include <grpcpp/grpcpp.h>

// Service
#include "game_service.grpc.pb.h"

// Usings
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using GameService::MapResponse;
using GameService::ReadyRequest;
using GameService::ReadyResponse;
using GameService::StepRequest;

// Defines
#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define CYN "\e[0;36m"
#define REDB "\e[41m"

namespace client
{
    class GameClient final
    {
    private:
        std::unique_ptr<GameService::GameService::Stub> m_stub;
        int m_number;

    public:
        GameClient(std::shared_ptr<Channel> channel)
            : m_stub(GameService::GameService::NewStub(channel))
        {
        }

        ~GameClient() = default;

        // Block to copy
        GameClient(const GameClient &) = delete;
        GameClient(GameClient &&) = delete;

        // Main method for start
        void Ready() noexcept
        {
            ReadyRequest request;
            request.set_ready(true);

            ReadyResponse response;

            ClientContext context;

            Status status = m_stub->StartGame(&context, request, &response);

            if (status.ok())
                m_number = response.side();
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;
        }

        // Block to copy
        GameClient &operator=(const GameClient &) = delete;
        GameClient &operator=(const GameClient &&) = delete;
    };
}