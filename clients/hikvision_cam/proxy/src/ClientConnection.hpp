#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include "StolenConnection.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "IClosable.hpp"

class HikConnection;

class ClientConnection
        : public boost::enable_shared_from_this<ClientConnection>,
        public IClosable {
public:
    typedef boost::shared_ptr<ClientConnection> pointer;

    static pointer create(boost::asio::io_context& io_context);

    void start();

    ~ClientConnection();

    boost::asio::ip::tcp::socket& socket();

private:
    void receiveFromHik();

    ClientConnection(boost::asio::io_context& io_context);

    void handleRead(const boost::system::error_code& error,
                    size_t bytes_transferred0);
    void startRead();
    void close() override final;

    boost::asio::ip::tcp::socket m_socket;
    boost::shared_ptr<HikConnection> m_hikCon;
    boost::asio::io_context& m_ioContext;
    std::vector<boost::uint8_t> m_recvBuffer;
};

#endif /* CLIENT_CONNECTION_HPP */
