#include <iostream>

#include "Alarm.h"

//ssh -4 -f -N -M -L 8000:192.168.1.204:8000 Jesus

int main(int count, char **args) {
    if (count != 5) {
        std::cout << "Args error: ip port login pass" << std::endl;
        return 1;
    }

    const char *ip = args[1];
    const char *port = args[2];
    const char *login = args[3];
    const char *pass = args[4];

    Run_Alarm(ip, port, login, pass);
    return 0;
}

