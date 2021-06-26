#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

class MessageQueue;

class ClientConnection
        : public boost::enable_shared_from_this<ClientConnection> {
public:
    typedef boost::shared_ptr<ClientConnection> pointer;

    static pointer create(boost::asio::io_context& io_context);

    void start();

    ~ClientConnection();

    boost::asio::ip::tcp::socket& socket();

private:
    void startPingTimer();
    void onPingTick(const boost::system::error_code& e);
    void startAcitivityTimer();
    void onActivityTick(const boost::system::error_code& e);

    ClientConnection(boost::asio::io_context& io_context);

    void handleRead(const boost::system::error_code& error,
                    size_t bytes_transferred0);
    void startRead();
    void close();
    bool onCommandLine(const std::string&);
    void sendCmd(const std::string&);

    boost::asio::steady_timer m_pingTimer;
    boost::asio::steady_timer m_activityTimer;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::io_context& m_ioContext;
    std::vector<boost::uint8_t> m_recvBuffer;
    std::string m_strRecv;
    int m_recvCount;
    std::unique_ptr<MessageQueue> m_queue;
};

#endif /* CLIENT_CONNECTION_HPP */
