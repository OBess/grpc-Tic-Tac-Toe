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

using GameService::MapRequest;
using GameService::MapResponse;
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

namespace client
{
    class GameClient final
    {
    private:
        std::unique_ptr<GameService::GameService::Stub> m_stub;
        int m_size;

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
        bool Ready() noexcept
        {
            ReadyRequest request;
            request.set_ready(true);

            ReadyResponse response;
            ClientContext context;

            Status status = m_stub->StartGame(&context, request, &response);

            if (status.ok())
            {
                m_size = response.side();
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;

            return status.ok();
        }

        // Make step
        bool Step(unsigned x, unsigned y) noexcept
        {
            StepRequest request;
            request.set_side(m_size);
            request.set_x(x);
            request.set_y(y);

            StepResponse response;
            ClientContext context;

            Status status = m_stub->MakeStep(&context, request, &response);

            if (status.ok())
            {
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;

            return response.win();
        }

        // Make step
        std::string GetMap() noexcept
        {
            MapRequest request;
            MapResponse response;
            ClientContext context;

            Status status = m_stub->GetMap(&context, request, &response);

            if (status.ok())
            {
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;

            return response.map();
        }

        char getSide() const noexcept
        {
            return m_size % 2 ? 'x' : 'o';
        }

        // Block to copy
        GameClient &operator=(const GameClient &) = delete;
        GameClient &operator=(const GameClient &&) = delete;
    };

    // Main method of start
    void Run(const std::string &ip)
    {
        client::GameClient clt(grpc::CreateChannel(ip, grpc::InsecureChannelCredentials()));

        if (!clt.Ready())
            return;

        int x, y;
        bool isWinner;
        std::string map;

        std::cout << "\tTic-Tac-Toe" << std::endl;
        std::cout << "First is X!" << std::endl
                  << std::endl;
        std::cout << "You are " << clt.getSide() << std::endl;

        do
        {
            std::cout << "Enter x and y [1; 3]: ";

            std::cin >> x >> y;
            isWinner = clt.Step(x - 1, y - 1);

            map = clt.GetMap();

            std::cout << "Board: " << std::endl;
            std::cout << map << std::endl;
            std::cout << "Turn other player." << std::endl;

            while (map != "")
                map = clt.GetMap();

            std::cout << "Board: " << std::endl;
            std::cout << clt.GetMap() << std::endl;

        } while (true);
    }
}