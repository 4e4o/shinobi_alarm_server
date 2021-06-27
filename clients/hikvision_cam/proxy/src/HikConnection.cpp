#include "HikConnection.hpp"
#include "ActivityWatcher.hpp"

using boost::asio::ip::tcp;
using boost::asio::ip::address;

HikConnection::pointer HikConnection::create(boost::asio::io_context& io_context) {
    return pointer(new HikConnection(io_context));
}

HikConnection::HikConnection(boost::asio::io_context& io_context)
    : m_socket(io_context),
      m_recvBuffer(4096),
      m_firstSend(true),
      m_alarm_con(false),
      m_ioContext(io_context) {
    ActivityWatcher::get()->add(this);
    //std::cout << "HikConnection::HikConnection "<< this << std::endl;
}

HikConnection::~HikConnection() {
    ActivityWatcher::get()->remove(this);
    //std::cout << "~HikConnection "<< this << std::endl;
}

void HikConnection::close() {
    try {
        m_socket.close();
    }  catch (...) {
    }
}
