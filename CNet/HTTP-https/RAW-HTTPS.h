#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "Http.h"

// include order is important.
using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#pragma comment (lib, "crypt32")
#pragma comment (lib, "libcrypto.lib")
#pragma comment (lib, "libssl.lib")


#ifndef RAWHTTPS_DOT_H
#define RAWHTTPS_DOT_H

int rawhttps(const char* ip, const char* method);

#endif 
