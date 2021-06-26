#include <iostream>

#include "Server.hpp"
#include "Args.hpp"
#include "ActivityWatcher.hpp"

int main(int count, char **argv) {
    if (count != 6) {
        std::cout << "Args error: ip port login pass cam_id" << std::endl;
        return 1;
    }

    new Args(count, argv);

    while(1) {
        try {
            boost::asio::io_context io_context;
            ActivityWatcher aw(io_context);
            TCPServer server(io_context);
            io_context.run();
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }

        sleep(2);
    }

    return 0;
}
