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

int listenSocket_errcheck(SOCKET, LPADDRINFO &);

int bind_errcheck(SOCKET, LPADDRINFO &, SOCKET &);

int listen_errcheck(int, SOCKET &);

int accept_conn_errcheck(SOCKET &, SOCKET &);

int recv_errcheck(int, SOCKET &);

int send_errcheck(int, SOCKET &);

int disconnect_errcheck(int, SOCKET &);

#endif