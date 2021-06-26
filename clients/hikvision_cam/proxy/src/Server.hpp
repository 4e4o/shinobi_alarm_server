#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "IClosable.hpp"

class ClientConnection;

class TCPServer : public IClosable {
public:
    TCPServer(boost::asio::io_context& io_context);
    ~TCPServer();

private:
    void startAccept();
    void handleAccept(boost::shared_ptr<ClientConnection> new_connection,
                      const boost::system::error_code& error);
    void startHikClient();
    void close() override final;

    boost::asio::io_context& m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;
    int m_clientAccepted;
    int m_port;
    std::thread* m_clientThread;
};

#endif /* SERVER_HPP */
