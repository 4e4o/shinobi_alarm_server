#include "Server.hpp"
#include "ClientConnection.hpp"

#include <boost/bind/bind.hpp>

#include <iostream>
#include <string>
#include <cstdlib>

#include "Args.hpp"
#include "ActivityWatcher.hpp"

using boost::asio::ip::tcp;
using boost::asio::ip::address;

TCPServer::TCPServer(boost::asio::io_context& io_context)
    : m_ioContext(io_context),
      m_acceptor(io_context, tcp::endpoint(tcp::v4(), 0)),
      m_clientAccepted(0),
      m_clientThread(nullptr) {
    ActivityWatcher::get()->add(this);
    boost::asio::ip::tcp::endpoint le = m_acceptor.local_endpoint();
    m_port = le.port();
    startAccept();
    startHikClient();
}

TCPServer::~TCPServer() {
    ActivityWatcher::get()->remove(this);

    if (m_clientThread != nullptr) {
        m_clientThread->join();
        delete m_clientThread;
    }
}

void TCPServer::startHikClient() {
    m_clientThread = new std::thread([this]() {
        Args *args = Args::args();
        const std::string cmd = "hikvision_client 127.0.0.1 " +
                std::to_string(m_port) + " " + args->login + " " +
                args->pass;
        std::system(cmd.c_str());
    });
}

void TCPServer::startAccept() {
    m_clientAccepted++;

    if (m_clientAccepted > 2) {
        return;
    }

    auto new_connection =
            ClientConnection::create(m_ioContext);

    m_acceptor.async_accept(new_connection->socket(),
                            boost::bind(&TCPServer::handleAccept, this, new_connection,
                                        boost::asio::placeholders::error));
}

void TCPServer::close() {
    try {
        m_acceptor.close();
    }  catch (...) {
    }
}

void TCPServer::handleAccept(boost::shared_ptr<ClientConnection> new_connection,
                             const boost::system::error_code& error) {
    if (!error)
        new_connection->start();

    startAccept();
}
