#include "server.hpp"

namespace server
{
    namespace
    {

        // Build instance of map in string type
        std::string GameServerImpl::BuildMap() const noexcept
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

        bool GameServerImpl::isWon() const noexcept
        {
            bool won = false;
            size_t o = 0, x = 0;

            // Horizontal check
            for (const auto &i : m_map)
            {
                x = std::count(i.begin(), i.end(), 'x');
                o = std::count(i.begin(), i.end(), 'o');
                if (x == i.size() || o == i.size())
                    return true;
            }

            // Vertical check
            for (size_t j = 0; j < m_size; ++j)
            {
                x = 0;
                o = 0;
                for (size_t i = 0; i < m_size; ++i)
                {
                    if (m_map[i][j] == 'x')
                        ++x;
                    if (m_map[i][j] == 'o')
                        ++o;
                }
                if (x == m_size || o == m_size)
                    return true;
            }

            // LeftDiagonal check
            x = 0;
            o = 0;
            for (size_t i = 0; i < m_size; ++i)
            {
                if (m_map[i][i] == 'x')
                    ++x;
                if (m_map[i][i] == 'o')
                    ++o;
            }
            if (x == m_size || o == m_size)
                return true;

            // RightDiagonal check
            x = 0;
            o = 0;
            for (int i = m_size - 1; i >= 0; --i)
            {
                if (m_map[i][i] == 'x')
                    ++x;
                if (m_map[i][i] == 'o')
                    ++o;
            }
            if (x == m_size || o == m_size)
                return true;

            return false;
        }

        GameServerImpl::GameServerImpl()
            : m_lastTurn(2)
        {
            for (size_t i = 0; i < m_size; ++i)
            {
                m_map.push_back({});
                for (size_t j = 0; j < m_size; ++j)
                    m_map[i].push_back(' ');
            }
        }

        // Network methods
        Status GameServerImpl::StartGame(ServerContext *context, const ReadyRequest *request, ReadyResponse *response)
        {
            if (m_players > 2)
                return {grpc::StatusCode::UNAVAILABLE, "Two player are connected."};

            response->set_map(BuildMap());
            response->set_id(m_players);

            std::cout << YEL << "Player " << m_players << " is connected." << NC << std::endl;
            ++m_players;

            return Status::OK;
        }

        Status GameServerImpl::MakeStep(ServerContext *context, const StepRequest *request, StepResponse *response)
        {
            if (request->id() == m_lastTurn)
                return {grpc::StatusCode::PERMISSION_DENIED, "Not you turn."};

            auto x = request->x();
            auto y = request->y();

            if (m_map[y][x] != ' ' or x >= m_size or y >= m_size)
                return {grpc::StatusCode::PERMISSION_DENIED, "Block is filled or out of range."};

            m_map[y][x] = (request->id() % 2 ? 'x' : 'o');

            response->set_win(isWon());
            response->set_map(BuildMap());

            m_lastTurn = request->id();

            return Status::OK;
        }

        Status GameServerImpl::GetState(ServerContext *context, const StateRequest *request, StateResponse *response)
        {
            if (request->id() == m_lastTurn)
            {
                response->set_map("");
                return Status::OK;
            }

            response->set_map(BuildMap());

            return Status::OK;
        }
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

        std::cout << GRN << "Stop listening" << NC << std::endl;
    }
};