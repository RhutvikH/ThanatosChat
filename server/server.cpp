#include <bits/stdc++.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <thread>

#include "constants.h"
#include "startwsa.h"
#include "errs.h"
#include "mthread.h"

// class Server {
//     public:
//         static size_t currUsers;
//         std::vector<Client> onlineClients;

// };

// class Client {

// };

int cliThread(SOCKET ClientSocket)
{
    int ires;
    int iSendRes;
    int rbuflen{DEF_BUF_LEN};

    char recvbuf[DEF_BUF_LEN];

    std::string str;

    ires = recv(ClientSocket, recvbuf, rbuflen, 0);

    if (recv_errcheck(ires, ClientSocket) != 0)
    {
        genericError();
        return 1;
    }
    std::string name{recvbuf};
    name = name.substr(0, ires);
    cout << name << " joined the server" << endl;
    cout << name << " has been added to the server count" << endl;
    std::string welcomeMsg = name + ", Welcome to the server!";

    iSendRes = send(ClientSocket, welcomeMsg.c_str(), strlen(welcomeMsg.c_str()), 0);
    if (send_errcheck(iSendRes, ClientSocket) != 0)
    {
        genericError();
        return 1;
    }

    do
    {
        ires = recv(ClientSocket, recvbuf, rbuflen, 0);
        if (recv_errcheck(ires, ClientSocket) != 0)
        {
            genericError();
            return 1;
        }

        str = recvbuf;

        // cout << "Bytes received: " << ires << endl;

        if ((strncmp("disconnect()", recvbuf, 12) == 0) && (ires == 12))
        {
            cout << "Server: Disconnecting " << name << " from the server." << endl;
            break;
        }
        else if ((strncmp("init()", recvbuf, 6) == 0) && (ires == 6))
        {
            cout << "Added " << name << " to the admin list" << endl;
        }
        else
            cout << name << ": " << str.substr(0, ires) << endl;

        iSendRes = send(ClientSocket, "received", 9, 0);

        if (send_errcheck(iSendRes, ClientSocket) != 0)
        {
            genericError();
            return 1;
        }

        // cout << "Bytes sent: " << iSendRes << endl;

    } while (ires > 0);

    cout << name << " disconnected from the server" << endl;

    ires = shutdown(ClientSocket, SD_SEND);
    if (disconnect_errcheck(ires, ClientSocket) != 0)
    {
        genericError();
        return 1;
    }

    closesocket(ClientSocket);
    return 0;
}

int main()
{

    int ires;
    int iSendRes;
    int rbuflen{DEF_BUF_LEN};

    SOCKET ListenSocket = INVALID_SOCKET; // Listen socket for creation
    SOCKET ClientSocket = INVALID_SOCKET; // Client socket storage for communication

    WSAData wsadata;
    WORD mVersionReq = MAKEWORD(2, 2);
    std::string str{};

    struct addrinfo *result = NULL, *ptr = NULL, hints;
    char recvbuf[DEF_BUF_LEN];

    // WSA startup
    ires = wsa_startup();
    if (wsastartup_errcheck(ires) != 0)
    {
        genericError();
        return 1;
    }

    // Empty hints
    ZeroMemory(&hints, sizeof(hints));

    // Set hint parameters
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Get info about IP addresses
    ires = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (getaddrinfo_errcheck(ires) != 0)
    {
        genericError();
        return 1;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket_errcheck(ListenSocket, result) != 0)
    {
        genericError();
        return 1;
    }

    ires = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (bind_errcheck(ires, result, ListenSocket) != 0)
    {
        genericError();
        return 1;
    }

    freeaddrinfo(result);

    ires = listen(ListenSocket, SOMAXCONN);
    if (listen_errcheck(ires, ListenSocket) != 0)
    {
        genericError();
        return 1;
    }

    std::vector<std::thread> clients;
    int ct = 0;
    while (true)
    {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (accept_conn_errcheck(ClientSocket, ListenSocket) != 0)
        {
            genericError();
            return 1;
        }
        ct++;
        // std::thread client(cliThread, ClientSocket);
        clients.emplace_back(std::thread(cliThread, ClientSocket));
        if (ct >= 10)
            break;
    }

    for (std::thread &thread : clients)
    {
        thread.join();
    }

    WSACleanup();

    return 0;
}
