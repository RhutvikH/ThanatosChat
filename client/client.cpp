#include <bits/stdc++.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "constants.h"
#include "errs.h"
#include "startwsa.h"

using namespace std;

int main(int argc, char **argv)
{

    int ires;
    int rbuflen = DEF_BUF_LEN;

    WSAData wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL, *ptr = NULL, hints;
    char recvbuf[DEF_BUF_LEN];
    char msg[DEF_BUF_LEN];

    // WSA startup
    ires = wsa_startup();
    if (wsastartup_errcheck(ires) != 0)
    {
        genericError();
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    cout << "Connecting to port: " << DEFAULT_PORT << " on the server address: " << argv[1] << endl;

    ires = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (getaddrinfo_errcheck(ires) != 0)
    {
        genericError();
        return 1;
    }

    ptr = result;

    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (ConnectSocket_errcheck(ConnectSocket, result) != 0)
    {
        genericError();
        return 1;
    }

    ires = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (ires == SOCKET_ERROR)
    {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (invalidCliSock_errcheck(ConnectSocket) != 0)
    {
        genericError();
        return 1;
    }
    char name[DEF_BUF_LEN];
    cout << "Enter the name you want your messages to be displayed as: ";
    cin.getline(name, DEF_BUF_LEN);

    ires = send(ConnectSocket, name, strlen(name), 0);
    if (send_errcheck(ires, ConnectSocket) != 0)
    {
        genericError();
        return 1;
    }

    ires = recv(ConnectSocket, recvbuf, rbuflen, 0);
    if (recv_errcheck(ires, ConnectSocket) != 0)
    {
        genericError();
        return 1;
    }
    std::string str = recvbuf;
    cout << "Server" << ": " << str.substr(0, ires) << endl;


    ires = send(ConnectSocket, "init()", 6, 0);
    if (send_errcheck(ires, ConnectSocket) != 0)
    {
        genericError();
        return 1;
    }


    cout << "Bytes sent: " << ires << endl;

    auto fn = [ConnectSocket, rbuflen] () {
        int ires;
        char recvbuf[DEF_BUF_LEN];
        std::string str = recvbuf;
        do
        {
            ires = recv(ConnectSocket, recvbuf, rbuflen, 0);
            if (ires > 0) {
                str = recvbuf;
                cout << "Server" << ": " << str.substr(0, ires) << endl;
                cout << "its working!" << endl;
            }
            else{
                if (ires == 0)
                    cout << "Client: Connection closed" << endl;
                else
                    cout << "Client: Recv failed: " << WSAGetLastError() << endl;
            }
        } while (ires > 0);

    };

    std::thread th_obj(fn);
    
    do
    {
        // ires = recv(ConnectSocket, recvbuf, rbuflen, 0);

            try
            {
                cin.getline(msg, DEF_BUF_LEN);
            }
            catch (std::exception e)
            {
                cout << "Client: Number of characters entered was above the character limit. Sending message to admin." << endl;
                memset(msg, 0, 511);
                strcpy(msg, "Reached character limit");
                ires = send(ConnectSocket, msg, strlen(msg), 0);
                continue;
            }

            ires = send(ConnectSocket, msg, strlen(msg), 0);

            if (send_errcheck(ires, ConnectSocket) != 0)
            {
                genericError();
                return 1;
            }
        

        cout << "Bytes sent: " << ires << endl;

    } while (ires > 0);

    th_obj.join();

    ires = shutdown(ConnectSocket, SD_SEND);
    if (shutdown_errcheck(ires, ConnectSocket) != 0)
    {
        genericError();
        return 1;
    }

    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}