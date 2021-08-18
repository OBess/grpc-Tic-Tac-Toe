#include "../include/server.hpp"

namespace server
{
    class GameServerImpl final : public GameService::GameService::Service
    {
    private:
        // Map of game (3x3)
        std::vector<std::vector<char>> m_map;

        // Count players
        int m_players{1};
        const size_t m_size = 3;
        bool m_end = false;

        int m_lastTurn = 2;
        int m_winner = -1;

        std::mutex m_con_disMutex;

        // Build instance of map in string type
        std::string BuildMap() const noexcept;

        bool isWon() const noexcept;
        bool filled() const noexcept;

    public:
        GameServerImpl();
        ~GameServerImpl() = default;

        // Block to copy
        GameServerImpl(const GameServerImpl &) = delete;
        GameServerImpl(GameServerImpl &&) = delete;

        // Network methods
        Status Connect(ServerContext *context, const ConnectRequest *request, ConnectResponse *response) override;

        Status Disconnect(ServerContext *context, const DisconnectRequest *request, DisconnectResponse *response) override;

        Status MakeStep(ServerContext *context, const StepRequest *request, StepResponse *response) override;

        Status GetState(ServerContext *context, const StateRequest *request, StateResponse *response) override;

        // Block to copy
        GameServerImpl &operator=(const GameServerImpl &) = delete;
        GameServerImpl &operator=(const GameServerImpl &&) = delete;

        // Getters
        bool isEnd() const noexcept;
        int amoutOfPlayer() const noexcept;
    };

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
        for (int i = m_size - 1, j = 0; i >= 0; --i, ++j)
        {
            if (m_map[j][i] == 'x')
                ++x;
            if (m_map[j][i] == 'o')
                ++o;
        }
        if (x == m_size || o == m_size)
            return true;

        return false;
    }

    bool GameServerImpl::filled() const noexcept
    {
        size_t count{};
        for (const auto &i : m_map)
            for (const auto &j : i)
                if (j == 'x' || j == 'o')
                    ++count;

        return count == (m_size * m_size);
    }

    GameServerImpl::GameServerImpl()
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            m_map.push_back({});
            for (size_t j = 0; j < m_size; ++j)
                m_map[i].push_back(' ');
        }
    }

    // Network methods
    Status GameServerImpl::Connect(ServerContext *context, const ConnectRequest *request, ConnectResponse *response)
    {
        std::unique_lock lk(m_con_disMutex);
        if (m_players > 2)
            return {grpc::StatusCode::UNAVAILABLE, "Two player are connected."};

        response->set_map(BuildMap());
        response->set_id(m_players);

        std::cout << YEL << "Player " << m_players << " is connected." << NC << std::endl;
        ++m_players;

        return Status::OK;
    }

    Status GameServerImpl::Disconnect(ServerContext *context, const DisconnectRequest *request, DisconnectResponse *response)
    {
        std::unique_lock lk(m_con_disMutex);
        --m_players;
        std::cout << YEL << "Player disconnected." << NC << std::endl;

        return Status::OK;
    }

    Status GameServerImpl::MakeStep(ServerContext *context, const StepRequest *request, StepResponse *response)
    {
        if (request->id() == m_lastTurn)
            return {grpc::StatusCode::PERMISSION_DENIED, "Not you turn."};

        auto x = request->x();
        auto y = request->y();

        if (x >= m_size or y >= m_size or m_map[y][x] != ' ')
            return {grpc::StatusCode::PERMISSION_DENIED, "Block is filled or out of range."};

        m_map[y][x] = (request->id() % 2 ? 'x' : 'o');

        response->set_map(BuildMap());
        m_winner = isWon() ? request->id() : -1;

        m_lastTurn = request->id();

        return Status::OK;
    }

    Status GameServerImpl::GetState(ServerContext *context, const StateRequest *request, StateResponse *response)
    {
        m_end = m_winner != -1 or (m_winner == -1 and filled());
        response->set_isend(m_end);
        response->set_winner(m_winner);

        if (request->id() == m_lastTurn)
            response->set_map("");
        else
            response->set_map(BuildMap());

        return Status::OK;
    }

    bool GameServerImpl::isEnd() const noexcept
    {
        return m_end;
    }

    int GameServerImpl::amoutOfPlayer() const noexcept
    {
        return m_players;
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

        auto fut = std::async(
            [&]()
            {
                while (!service.isEnd() or service.amoutOfPlayer() != 1)
                {
                }
                server.get()->Shutdown();
            });

        server.get()->Wait();

        std::cout << GRN << "Stop listening" << NC << std::endl;
    }
};