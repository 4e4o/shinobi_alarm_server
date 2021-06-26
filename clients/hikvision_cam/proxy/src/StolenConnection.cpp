#include "StolenConnection.hpp"
#include "ActivityWatcher.hpp"
#include "Args.hpp"
#include "../../../../src/MessageQueue.h"

#include <iostream>
#include <boost/bind/bind.hpp>

using boost::asio::ip::tcp;
namespace bpt = boost::posix_time;

StolenConnection::pointer StolenConnection::create(boost::asio::io_context& io_context, int sock) {
    return pointer(new StolenConnection(io_context, sock));
}

StolenConnection::StolenConnection(boost::asio::io_context& io_context, int sock)
    : m_socket(io_context),
      m_recvBuffer(4096),
      m_queue(new MessageQueue()) {
    m_socket.assign(boost::asio::ip::tcp::v4(), sock);
    ActivityWatcher::get()->add(this);
}

StolenConnection::~StolenConnection() {
    ActivityWatcher::get()->remove(this);
    std::cout << "~StolenConnection "<< this << std::endl;
}

void StolenConnection::receive() {
    pointer p = shared_from_this();
    m_socket.async_read_some(boost::asio::buffer(m_recvBuffer),
                             [this, p](const boost::system::error_code& error,
                             size_t bytes_transferred) {
        if (error)
            return;

        if (bytes_transferred > 0) {
            ActivityWatcher::get()->trigger();

            // motion detection
            if (bytes_transferred == 108)
                m_queue->send(Args::args()->cam_id);

            //            std::cout << "StolenConnection received "<< bytes_transferred << this << std::endl;
            p->receive();
        }
    });
}

void StolenConnection::close() {
    try {
        m_socket.close();
    }  catch (...) {
    }
}
