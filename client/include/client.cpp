#include "client.hpp"

namespace client
{
    namespace
    {
        GameClient::GameClient(std::shared_ptr<Channel> channel)
            : m_stub(GameService::GameService::NewStub(channel))
        {
        }

        // Main method for start
        bool GameClient::Ready() noexcept
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
        bool GameClient::Step(unsigned x, unsigned y) noexcept
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
        std::string GameClient::GetMap() noexcept
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

        char GameClient::getSide() const noexcept
        {
            return m_size % 2 ? 'x' : 'o';
        }
    }

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
};