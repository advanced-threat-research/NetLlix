#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale>
#include <sstream>

using namespace std;
#pragma comment(lib,"ws2_32.lib")

#ifndef RAWHTTP_DOT_H
#define RAWHTTP_DOT_H

string rawhttp(string url, const char* isget);

#endif 
