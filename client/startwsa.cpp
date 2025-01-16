#include "startwsa.h"

int wsa_startup() {
    WSADATA wsaData;
    int ires;
    ires = WSAStartup(MAKEWORD(2, 2), &wsaData);

    return ires;
}