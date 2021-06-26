#ifndef STOLEN_CONNECTION_H
#define STOLEN_CONNECTION_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "IClosable.hpp"

class MessageQueue;

class StolenConnection
        : public boost::enable_shared_from_this<StolenConnection>,
        public IClosable {
public:
    typedef boost::shared_ptr<StolenConnection> pointer;

    static pointer create(boost::asio::io_context& io_context, int sock);

    void receive();
    ~StolenConnection();

private:
    StolenConnection(boost::asio::io_context& io_context, int sock);

    void close() override final;

    boost::asio::ip::tcp::socket m_socket;
    std::vector<boost::uint8_t> m_recvBuffer;
    std::unique_ptr<MessageQueue> m_queue;
};

#endif /* STOLEN_CONNECTION_H*/
