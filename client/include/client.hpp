#pragma once

// C++
#include <iostream>
#include <memory>
#include <utility>
#include <string>

// gRPC
#include <grpcpp/grpcpp.h>

// Service
#include "game_service.grpc.pb.h"

// Usings
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using GameService::ConnectRequest;
using GameService::ConnectResponse;
using GameService::DisconnectRequest;
using GameService::DisconnectResponse;
using GameService::StateRequest;
using GameService::StateResponse;
using GameService::StepRequest;
using GameService::StepResponse;

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
    namespace
    {
        class GameClient final
        {
        private:
            std::unique_ptr<GameService::GameService::Stub> m_stub;
            int m_id;
            char m_side;

        public:
            GameClient(std::shared_ptr<Channel> channel);
            ~GameClient() = default;

            // Block to copy
            GameClient(const GameClient &) = delete;
            GameClient(GameClient &&) = delete;

            // Main method for start
            std::pair<ConnectResponse, bool> Connect() noexcept;

            // Main method for end
            void Disconnect() noexcept;

            // Make step
            StepResponse Step(unsigned x, unsigned y) noexcept;

            // Make step
            StateResponse GetState() noexcept;

            char getSide() const noexcept;

            // Block to copy
            GameClient &operator=(const GameClient &) = delete;
            GameClient &operator=(const GameClient &&) = delete;
        };
    }

    // Main method of start
    void Run(const std::string &ip);
};