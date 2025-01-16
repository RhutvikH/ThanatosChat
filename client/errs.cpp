#include "errs.h"

void genericError()
{
    cout << "An unexpected error occurred" << endl;
}

int wsastartup_errcheck(int ires)
{
    if (ires != 0)
    {
        cout << "WSAStartup failed: " << ires << endl;
        return 1;
    }
    else
    {
        cout << "Initialized the socket api" << endl;
    }
    return 0;
}

int getaddrinfo_errcheck(int ires)
{
    if (ires != 0)
    {
        cout << "Getaddrinfo failed: " << ires << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "getaddrinfo succeeded" << endl;
    }
    return 0;
}

int ConnectSocket_errcheck(SOCKET ConnectSocket, LPADDRINFO &result)
{
    if (ConnectSocket == INVALID_SOCKET)
    {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "Socket created for connection" << endl;
    }
    return 0;
}

int invalidCliSock_errcheck(SOCKET &ConnectSocket)
{
    if (ConnectSocket == INVALID_SOCKET)
    {
        cout << "Client: unable to connect to server!" << endl;
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "Client: Connected to server!" << endl;
    }
    return 0;
}

int recv_errcheck(int ires, SOCKET &ConnectSocket)
{
    if (ires <= 0)
    {
        cout << "Receive failed: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    return 0;
}

int send_errcheck(int ires, SOCKET &ConnectSocket)
{
    if (ires == SOCKET_ERROR)
    {
        cout << "Client: Send failed: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    return 0;
}

int shutdown_errcheck(int ires, SOCKET &ConnectSocket)
{
    if (ires == SOCKET_ERROR)
    {
        cout << "Client: Shutdown failed: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "Shutdown successful" << endl;
    }
    return 0;
}