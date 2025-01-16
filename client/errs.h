#ifndef ERR_H
#define ERR_H

#include <bits/stdc++.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "constants.h"
#include "startwsa.h"

void genericError();

int wsastartup_errcheck(int);

int getaddrinfo_errcheck(int);

int ConnectSocket_errcheck(SOCKET, LPADDRINFO &);

int invalidCliSock_errcheck(SOCKET &);

int recv_errcheck(int, SOCKET &);

int send_errcheck(int, SOCKET &);

int shutdown_errcheck(int, SOCKET &);

#endif