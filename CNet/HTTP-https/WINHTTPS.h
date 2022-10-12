#pragma once
#include <stdio.h>
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "Winhttp.lib")

#ifndef WINHTTPS_DOT_H
#define WINHTTPS_DOT_H

int winhttps(const char* ip, const char* method);

#endif