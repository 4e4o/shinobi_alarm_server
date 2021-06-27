#include "ClientConnection.hpp"
#include "Args.hpp"
#include "../../../src/MessageQueue.h"

#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/asio/ip/address.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::address;

#define PING_INTERVAL_SEC       4
#define ACTIVITY_INTERVAL_SEC   10
#define BUFFER_SIZE             4 * 1024
#define CMD_SEPARATOR           "\n"

ClientConnection::pointer ClientConnection::create(boost::asio::io_context& io_context) {
    return pointer(new ClientConnection(io_context));
}

ClientConnection::ClientConnection(boost::asio::io_context& io_context)
    : m_pingTimer(io_context, boost::asio::chrono::seconds(PING_INTERVAL_SEC)),
      m_activityTimer(io_context, boost::asio::chrono::seconds(ACTIVITY_INTERVAL_SEC)),
      m_socket(io_context), m_ioContext(io_context), m_recvBuffer(BUFFER_SIZE),
      m_recvCount(0),
      m_queue(new MessageQueue()) {
    //std::cout << "ClientConnection::ClientConnection "<< this << std::endl;
}

ClientConnection::~ClientConnection() {
    //std::cout << "~ClientConnection "<< this << std::endl;
}

tcp::socket& ClientConnection::socket() {
    return m_socket;
}

void ClientConnection::startPingTimer() {
    m_pingTimer.expires_from_now(boost::asio::chrono::seconds(PING_INTERVAL_SEC));
    m_pingTimer.async_wait(boost::bind(&ClientConnection::onPingTick,
                                       this,
                                       boost::asio::placeholders::error));
}

void ClientConnection::onPingTick(const boost::system::error_code &e) {
    if (e)
        return;

    sendCmd("PI");
}

void ClientConnection::startAcitivityTimer() {
    m_activityTimer.expires_from_now(boost::asio::chrono::seconds(ACTIVITY_INTERVAL_SEC));
    m_activityTimer.async_wait(boost::bind(&ClientConnection::onActivityTick,
                                           this,
                                           boost::asio::placeholders::error));
}

void ClientConnection::onActivityTick(const boost::system::error_code& e) {
    if (e)
        return;

    if (m_recvCount == 0) {
        close();
        std::cout << "No receive activity, closing" << std::endl;
    }

    m_recvCount = 0;
}

void ClientConnection::start() {
    pointer pthis = shared_from_this();

    m_socket.async_connect(boost::asio::ip::tcp::endpoint(
                               boost::asio::ip::address::from_string(Args::args()->ip), Args::args()->port),
                           [pthis](boost::system::error_code ec) {
        if (ec) {
            std::cout << ec.message()  << std::endl;
            pthis->close();
            return;
        }

        pthis->startRead();
        pthis->startPingTimer();
        pthis->startAcitivityTimer();
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
    if (error) {
        close();
        return;
    }

    if (bytes_transferred0 <= 0)
        return;

    if (m_strRecv.size() + bytes_transferred0 >= BUFFER_SIZE) {
        close();
        return;
    }

    m_recvCount++;
    m_strRecv.append((char*)m_recvBuffer.data(), bytes_transferred0);

    size_t index;
    std::string line;

    while ((index = m_strRecv.find(CMD_SEPARATOR)) != std::string::npos) {
        line = m_strRecv.substr(0, index);
        m_strRecv.erase(0, index + strlen(CMD_SEPARATOR));
        if (!onCommandLine(line)) {
            close();
            return;
        }
    }

    startRead();
}

void ClientConnection::sendCmd(const std::string& cmd) {
    const std::string data = cmd + CMD_SEPARATOR;
    boost::asio::async_write(m_socket, boost::asio::buffer(data, data.size()),
                             [](const boost::system::error_code&, size_t) {
    });
}

bool ClientConnection::onCommandLine(const std::string& cmd) {
    if (cmd == "PO") {
        startPingTimer();
        return true;
    }

    if (cmd.starts_with("MD ")) {
        int chId = std::stoi(cmd.substr(3));
        m_queue->send(chId);
//        std::cout << "ClientConnection::onCommandLine: md " << chId << std::endl;
    }

//    std::cout << "ClientConnection::onCommandLine: " << cmd << std::endl;
    return true;
}

void ClientConnection::startRead() {
    m_socket.async_read_some(boost::asio::buffer(m_recvBuffer),
                             boost::bind(&ClientConnection::handleRead, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}
