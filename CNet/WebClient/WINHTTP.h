#pragma once
#include <stdio.h>
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "Winhttp.lib")

#ifndef WINHTTP_DOT_H
#define WINHTTP_DOT_H

int winhttp(const char* ip, const char* method);

#endif
