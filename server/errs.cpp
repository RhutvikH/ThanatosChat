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

int listenSocket_errcheck(SOCKET ListenSocket, LPADDRINFO &result)
{
    if (ListenSocket == INVALID_SOCKET)
    {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "socket() function executed successfully" << endl;
    }
    return 0;
}

int bind_errcheck(SOCKET ires, LPADDRINFO &result, SOCKET &ListenSocket)
{
    if (ires == SOCKET_ERROR)
    {
        cout << "Bind failed with error: " << ires << endl;
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "Bind completed successfully" << endl;
    }

    return 0;
}

int listen_errcheck(int ires, SOCKET &ListenSocket)
{
    if (ires == SOCKET_ERROR)
    {
        cout << "Listen failed with error: " << WSAGetLastError() << endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "Server has started Listening" << endl;
    }
    return 0;
}

int accept_conn_errcheck(SOCKET &ClientSocket, SOCKET &ListenSocket)
{
    if (ClientSocket == INVALID_SOCKET)
    {
        cout << "Accept failed: " << WSAGetLastError() << endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "Accepted connection" << endl;
    }
    return 0;
}

int recv_errcheck(int ires, SOCKET &ClientSocket)
{
    if (ires <= 0)
    {
        cout << "Receive failed: " << WSAGetLastError() << endl;
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    return 0;
}

int send_errcheck(int iSendRes, SOCKET &ClientSocket)
{
    if (iSendRes == SOCKET_ERROR)
    {
        cout << "Send failed: " << WSAGetLastError() << endl;
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    return 0;
}

int disconnect_errcheck(int ires, SOCKET &ClientSocket)
{
    if (ires == SOCKET_ERROR)
    {
        cout << "Disconnect failed: " << WSAGetLastError() << endl;
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        cout << "Disconnect successful" << endl;
    }
    return 0;
}