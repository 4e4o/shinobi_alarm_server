#ifndef HIK_CONNECTION_HPP
#define HIK_CONNECTION_HPP

#include "StolenConnection.hpp"
#include "Args.hpp"

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

class HikConnection
        : public boost::enable_shared_from_this<HikConnection>,
        public IClosable {
public:
    typedef boost::shared_ptr<HikConnection> pointer;

    static pointer create(boost::asio::io_context& io_context);

    template<class Completion>
    void connect(Completion&& c) {
        m_socket.async_connect(boost::asio::ip::tcp::endpoint(
                                   boost::asio::ip::address::from_string(Args::args()->ip), Args::args()->port),
                               std::forward<Completion>(c));
    }

    template<class Completion>
    void sendToHik(Completion&& c, std::vector<boost::uint8_t>& buffer, size_t bytes_transferred) {
        if (m_firstSend) {
            std::cout <<"m_firstSend " << m_firstSend << " " << bytes_transferred << " " << this << std::endl;

            if (bytes_transferred == 32)
                m_alarm_con = true;

            m_firstSend = false;
        }

        if (m_alarm_con) {
            boost::asio::async_write(m_socket, boost::asio::buffer(buffer, bytes_transferred),
                                     [this, bytes_transferred, c{std::move(c)}](const boost::system::error_code& error,
                                     size_t bytes_transferred0) {
                if (error)
                    return;

                if (bytes_transferred0 == bytes_transferred) {
                    auto p = StolenConnection::create(m_ioContext, m_socket.release());
                    p->receive();
                    return;
                }
            });
        } else {
            boost::asio::async_write(m_socket, boost::asio::buffer(buffer, bytes_transferred),
                                     std::forward<Completion>(c));
        }
    }

    template<class Completion>
    void receive(Completion&& c) {
        m_socket.async_read_some(boost::asio::buffer(m_recvBuffer),
                                 [this, c{std::move(c)}](const boost::system::error_code& error,
                                 size_t bytes_transferred0) {
            c(error, bytes_transferred0, m_recvBuffer);
        });
    }

    ~HikConnection();

private:
    HikConnection(boost::asio::io_context& io_context);
    void close() override final;

    boost::asio::ip::tcp::socket m_socket;
    std::vector<boost::uint8_t> m_recvBuffer;
    bool m_firstSend = true;
    bool m_alarm_con = false;
    boost::asio::io_context& m_ioContext;
};

#endif /* HIK_CONNECTION_HPP */
