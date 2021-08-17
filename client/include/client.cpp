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
        std::pair<ConnectResponse, bool> GameClient::Connect() noexcept
        {
            ConnectRequest request;
            request.set_ready(true);

            ConnectResponse response;
            ClientContext context;

            Status status = m_stub->Connect(&context, request, &response);

            if (status.ok())
            {
                m_id = response.id();
                m_side = m_id % 2 ? 'x' : 'o';
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;

            return {response, status.ok()};
        }

        void GameClient::Disconnect() noexcept
        {
            DisconnectRequest request;
            DisconnectResponse response;
            ClientContext context;

            Status status = m_stub.get()->Disconnect(&context, request, &response);

            if (status.ok())
            {
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;
        }

        // Make step
        StepResponse GameClient::Step(unsigned x, unsigned y) noexcept
        {
            StepRequest request;
            request.set_id(m_id);
            request.set_x(x - 1);
            request.set_y(y - 1);

            StepResponse response;
            ClientContext context;

            Status status = m_stub->MakeStep(&context, request, &response);

            if (status.ok())
            {
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;

            return response;
        }

        // Make step
        StateResponse GameClient::GetState() noexcept
        {
            StateRequest request;
            request.set_id(m_id);

            StateResponse response;
            ClientContext context;

            Status status = m_stub->GetState(&context, request, &response);

            if (status.ok())
            {
            }
            else
                std::cout << RED << status.error_code() << ": " << status.error_message() << NC << std::endl;

            return response;
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

        auto readyResponse = clt.Connect();
        if (!readyResponse.second)
            return;

        StepResponse stepResponse;
        StateResponse stateResponse;

        int x, y;
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
            if (clt.GetState().map() != "")
            {
                std::cout << "Enter x and y [1; 3]: ";

                std::cin >> x >> y;
                stepResponse = clt.Step(x, y);
                map = stepResponse.map();

                std::cout << "Board: " << std::endl;
                std::cout << map << std::endl;
                std::cout << "Turn other player." << std::endl;
            }

            do
            {
                stateResponse = clt.GetState();
                map = stateResponse.map();
            } while (map == "" && !stateResponse.isend());

            std::cout << "Board: " << std::endl;
            std::cout << map << std::endl;

        } while (!stateResponse.isend());
        clt.Disconnect();

        stateResponse = clt.GetState();
        auto winner = stateResponse.winner();
        if (winner == -1)
            std::cout << REDB << "Draw!" << NC << std::endl;
        else
            std::cout << REDB << "The winner is " << (winner % 2 ? 'x' : 'o') << NC << std::endl;
        std::cout << YEL << "End of game" << NC << std::endl;
    }
};