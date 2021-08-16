#pragma once

// C++
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// gRPC
#include <grpcpp/grpcpp.h>

// Service
#include "game_service.grpc.pb.h"

// Usings
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using GameService::StateRequest;
using GameService::StateResponse;
using GameService::ReadyRequest;
using GameService::ReadyResponse;
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

namespace server
{

    namespace
    {
        class GameServerImpl final : public GameService::GameService::Service
        {
        private:
            // Map of game (3x3)
            std::vector<std::vector<char>> m_map;

            // Count players
            int m_players{1};
            const size_t m_size = 3;

            int m_lastTurn;

            // Build instance of map in string type
            std::string BuildMap() const noexcept;

            bool isWon() const noexcept;

        public:
            GameServerImpl();
            ~GameServerImpl() = default;

            // Block to copy
            GameServerImpl(const GameServerImpl &) = delete;
            GameServerImpl(GameServerImpl &&) = delete;

            // Network methods
            Status StartGame(ServerContext *context, const ReadyRequest *request, ReadyResponse *response) override;

            Status MakeStep(ServerContext *context, const StepRequest *request, StepResponse *response) override;

            Status GetState(ServerContext *context, const StateRequest *request, StateResponse *response) override;

            // Block to copy
            GameServerImpl &operator=(const GameServerImpl &) = delete;
            GameServerImpl &operator=(const GameServerImpl &&) = delete;
        };
    }

    // Main method of start
    void Run(const std::string &ip) noexcept;

};