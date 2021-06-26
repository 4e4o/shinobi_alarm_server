#include "Args.hpp"

Args* Args::g_args = nullptr;

using std::string;

Args::Args(int count, char **argv) :
    port(0) {
    if (count != 6)
        return;

    ip = string(argv[1]);
    port = std::stoi(argv[2]);
    login = string(argv[3]);
    pass = string(argv[4]);
    cam_id = std::stoi(string(argv[5]));

    g_args = this;
}

Args* Args::args() {
    return g_args;
}

std::string ip;
int port;
std::string login;
std::string pass;
std::string path;


