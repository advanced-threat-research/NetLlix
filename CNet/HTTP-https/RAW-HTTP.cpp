#include "RAW-HTTP.h"

locale local;
char buffer[10000];
string response_data;
int i = 0;

//****************************************************

string rawhttp(string url, const char* isget) {
    WSADATA wsaData;
    SOCKET Socket;
    SOCKADDR_IN SockAddr;
    int lineCount = 0;
    int rowCount = 0;
    struct hostent* host;
    string request;

    if(isget == "GET")
        request = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";
    else
        request = "POST / HTTP/1.1\r\nHost: " + url + "\r\nConnection:close\r\nContent-Length:9\r\nContent-Type:application/x-www-form-urlencoded\r\n\r\nname=nina";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed.\n";
        system("pause");
        //return 1;
    }

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    host = gethostbyname(url.c_str());

    SockAddr.sin_port = htons(80);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
        cout << "Could not connect";
        system("pause");
        //return 1;
    }

    send(Socket, request.c_str(), strlen(request.c_str()), 0);

    int nDataLength;
    while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

            response_data += buffer[i];
            i += 1;
        }
    }

    closesocket(Socket);
    WSACleanup();

    return response_data;
}