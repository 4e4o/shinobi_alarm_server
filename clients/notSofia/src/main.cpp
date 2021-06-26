#include <iostream>

#include "Args.hpp"
#include "ClientConnection.hpp"

int main(int count, char **argv) {
    if (count != 3) {
        std::cout << "Args error: ip port" << std::endl;
        return 1;
    }

    new Args(count, argv);

    while(1) {
        try {
            boost::asio::io_context io_context;
            auto c = ClientConnection::create(io_context);
            c->start();
            io_context.run();
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }

        sleep(2);
    }

    return 0;
}
