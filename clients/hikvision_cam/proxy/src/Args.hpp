#ifndef ARGS_H
#define ARGS_H

#include <string>

struct Args {
    Args(int count, char **argv);

    static Args* args();

    std::string ip;
    int port;
    std::string login;
    std::string pass;
    int cam_id;

private:
    static Args* g_args;
};

#endif /* ARGS_H*/
