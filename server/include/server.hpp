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
            int m_players{};
            const size_t m_size = 3;

            // Build instance of map in string type
            std::string BuildMap() const noexcept
            {
                std::string map{};
                const std::string horizontalLine{"-----"};

                auto map_push = [&](size_t i)
                {
                    for (size_t j = 0; j < m_map[i].size() - 1; ++j)
                    {
                        map.push_back(m_map[i][j]);
                        map.push_back('|');
                    }
                    map.push_back(m_map[i][m_map[i].size() - 1]);
                };

                for (size_t i = 0; i < m_map.size() - 1; ++i)
                {
                    map_push(i);
                    map.push_back('\n');
                    map.append(horizontalLine);
                    map.push_back('\n');
                }
                map_push(m_map.size() - 1);

                return map;
            }

        public:
            GameServerImpl()
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    m_map.push_back({});
                    for (size_t j = 0; j < m_size; ++j)
                        m_map[i].push_back(' ');
                }
            }

            ~GameServerImpl() = default;

            // Block to copy
            GameServerImpl(const GameServerImpl &) = delete;
            GameServerImpl(GameServerImpl &&) = delete;

            // Network methods
            Status StartGame(ServerContext *context, const ReadyRequest *request, ReadyResponse *response) override
            {
                if (m_players > 1)
                    return { grpc::StatusCode::PERMISSION_DENIED, "Two player are connected." };

                response->set_map(BuildMap());
                response->set_side(m_players);

                std::cout << YEL << "Player " << m_players << " is connected." << NC << std::endl;
                ++m_players;

                return Status::OK;
            }

            Status MakeStep(ServerContext *context, const StepRequest *request, MapResponse *response) override
            {
                response->set_map(BuildMap());
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