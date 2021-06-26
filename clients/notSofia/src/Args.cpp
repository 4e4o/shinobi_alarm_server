#include "Args.hpp"

Args* Args::g_args = nullptr;

using std::string;

Args::Args(int count, char **argv) :
    port(0) {
    if (count != 3)
        return;

    ip = string(argv[1]);
    port = std::stoi(argv[2]);

    g_args = this;
}

Args* Args::args() {
    return g_args;
}
