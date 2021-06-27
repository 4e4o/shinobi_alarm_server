#include "ClientConnection.hpp"
#include "HikConnection.hpp"
#include "ActivityWatcher.hpp"

#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/asio/ip/address.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::address;

ClientConnection::pointer ClientConnection::create(boost::asio::io_context& io_context) {
    return pointer(new ClientConnection(io_context));
}

ClientConnection::ClientConnection(boost::asio::io_context& io_context)
    : m_socket(io_context), m_ioContext(io_context), m_recvBuffer(4096) {
    ActivityWatcher::get()->add(this);
    //std::cout << "ClientConnection::ClientConnection "<< this << std::endl;
}

tcp::socket& ClientConnection::socket() {
    return m_socket;
}

void ClientConnection::start() {
    pointer pthis = shared_from_this();

    m_hikCon = HikConnection::create(m_ioContext);
    std::cout << "m_hikCon assign " << this << " " << m_hikCon.get() << std::endl;

    m_hikCon->connect([this, pthis](boost::system::error_code ec) {
        if (ec) {
//            std::cout << ec.message()  << std::endl;
            return;
        }

        pthis->startRead();
        receiveFromHik();
    });
}

ClientConnection::~ClientConnection() {
    ActivityWatcher::get()->remove(this);
//    std::cout << "~ClientConnection "<< this << std::endl;
}

void ClientConnection::receiveFromHik() {
    pointer pthis = shared_from_this();

    m_hikCon->receive([this, pthis](const boost::system::error_code& error,
                      size_t bytes_transferred0,
                      std::vector<boost::uint8_t>& data) {
        if (error)
            return;

        if (bytes_transferred0 > 0) {
            std::cout << "receiveFromHik "<< bytes_transferred0 << this << std::endl;

            boost::asio::async_write(m_socket, boost::asio::buffer(data, bytes_transferred0),
                                     [bytes_transferred0, pthis](const boost::system::error_code& error,
                                     size_t bytes_transferred) {
                if (error)
                    return;

                if (bytes_transferred0 == bytes_transferred) {
                    pthis->receiveFromHik();
                }
            });
        }
    });
}

void ClientConnection::close() {
    try {
        m_socket.close();
    }  catch (...) {
    }
}

void ClientConnection::handleRead(const boost::system::error_code& error,
                                  size_t bytes_transferred0) {
    if (error)
        return;

    if (bytes_transferred0 > 0) {
        std::cout << "handle_read "<< bytes_transferred0 << this << std::endl;

        pointer pthis = shared_from_this();

        m_hikCon->sendToHik([pthis, bytes_transferred0](
                            const boost::system::error_code& ec,
                            size_t bytes_transferred) {
            if (ec) {
                std::cout << ec.message()  << std::endl;
                return;
            }

            if (bytes_transferred == bytes_transferred0)
                pthis->startRead();
        }, m_recvBuffer, bytes_transferred0);
    }
}

void ClientConnection::startRead() {
    m_socket.async_read_some(boost::asio::buffer(m_recvBuffer),
                             boost::bind(&ClientConnection::handleRead, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}
