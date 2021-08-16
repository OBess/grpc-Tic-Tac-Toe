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
        std::pair<ReadyResponse, bool> GameClient::Ready() noexcept
        {
            ReadyRequest request;
            request.set_ready(true);

            ReadyResponse stepResponse;
            ClientContext context;

            Status status = m_stub->StartGame(&context, request, &stepResponse);

            if (status.ok())
            {
                m_id = stepResponse.id();
                m_side = m_id % 2 ? 'x' : 'o';
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;

            return {stepResponse, status.ok()};
        }

        // Make step
        StepResponse GameClient::Step(unsigned x, unsigned y) noexcept
        {
            StepRequest request;
            request.set_id(m_id);
            request.set_x(x - 1);
            request.set_y(y - 1);

            StepResponse stepResponse;
            ClientContext context;

            Status status = m_stub->MakeStep(&context, request, &stepResponse);

            if (status.ok())
            {
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;

            return stepResponse;
        }

        // Make step
        std::string GameClient::GetState() noexcept
        {
            StateRequest request;
            request.set_id(m_id);

            StateResponse stepResponse;
            ClientContext context;

            Status status = m_stub->GetState(&context, request, &stepResponse);

            if (status.ok())
            {
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;

            return stepResponse.map();
        }

        char GameClient::getSide() const noexcept
        {
            return m_side;
        }
    }

    // Main method of start
    void Run(const std::string &ip)
    {
        client::GameClient clt(grpc::CreateChannel(ip, grpc::InsecureChannelCredentials()));

        auto readyResponse = clt.Ready();
        if (!readyResponse.second)
            return;

        int x, y;
        bool isWinner;
        std::string map = readyResponse.first.map();

        std::cout << YEL << "\tTic-Tac-Toe" << std::endl;
        std::cout << "First is \'x\'!" << std::endl;
        std::cout << "You are " << clt.getSide() << std::endl
                  << std::endl
                  << NC;
        std::cout << "Board: " << std::endl
                  << map << std::endl;

        do
        {
            if (clt.GetState() != "")
            {
                std::cout << "Enter x and y [1; 3]: ";

                std::cin >> x >> y;
                auto stepResponse = clt.Step(x, y);
                isWinner = stepResponse.win();
                map = stepResponse.map();

                std::cout << "Board: " << std::endl;
                std::cout << map << std::endl;
                std::cout << "Turn other player." << std::endl;
            }

            do
            {
                map = clt.GetState();
            } while (map == "");

            std::cout << "Board: " << std::endl;
            std::cout << map << std::endl;

        } while (true);

        std::cout << YEL << "End of game" << NC << std::endl;
    }
};