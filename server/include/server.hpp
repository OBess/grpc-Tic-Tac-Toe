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
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using GameService::MapResponse;
using GameService::ReadyRequest;
using GameService::ReadyResponse;
using GameService::StepRequest;

// Defines
#define NC      "\e[0m"
#define RED     "\e[0;31m"
#define GRN     "\e[0;32m"
#define YEL     "\e[0;33m"
#define BLU     "\e[0;34m"
#define CYN     "\e[0;36m"
#define REDB    "\e[41m"

namespace server
{

    namespace
    {
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

            // Network methods
            Status StartGame(ServerContext *context, const ReadyRequest *request, ReadyResponse *response) override
            {
                std::cout << YEL << "Player " << m_players << " is ready." << NC << std::endl;
                
                response->set_side(m_players);
                ++m_players;

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
    }

    // Main method of start
    void Run(const std::string &ip) noexcept
    {
        GameServerImpl service;

        ServerBuilder builder;
        builder
            .AddListeningPort(ip, grpc::InsecureServerCredentials())
            .RegisterService(&service);

        auto server = builder.BuildAndStart();

        std::cout << GRN << "Server listening on " << ip << NC << std::endl;
        server.get()->Wait();
    }

};