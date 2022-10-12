#include <Windows.h>
#include <WinInet.h>

#include <cstdio>

using namespace std;

#pragma comment(lib, "wininet.lib")

int wininet(const char* ip,const char* isSSL, const char* method)
{
	int iErrorLevel = 0;

	HINTERNET hSession = NULL;

	hSession = InternetOpenA("Agent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hSession)
	{
		iErrorLevel = GetLastError();
		printf("InternetOpenA() Error: %d\n", iErrorLevel);
	}

	HINTERNET hConnect = NULL;
	if (!iErrorLevel)
	{	
		if (isSSL == "Yes")
			hConnect = InternetConnectA(hSession, (LPCSTR)ip, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL); // for HTTPS
		else
			hConnect = InternetConnectA(hSession, (LPCSTR)ip, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);

		if (!hConnect)
		{
			iErrorLevel = GetLastError();
			printf("InternetConnectA() Error: %d\n", iErrorLevel);
		}
	}

	HINTERNET hRequest = NULL;
	if (!iErrorLevel)
	{
		if (isSSL == "Yes" && method == "GET")
			hRequest = HttpOpenRequestA(hConnect, "GET", "/", NULL, NULL, NULL, INTERNET_FLAG_SECURE, NULL); // for HTTPS
		else if (isSSL == "No" && method == "GET")
			hRequest = HttpOpenRequestA(hConnect, "GET", "/", NULL, NULL, NULL, 0, NULL);
		else if (isSSL == "Yes" && method == "POST")
			hRequest = HttpOpenRequestA(hConnect, "POST", "/", NULL, NULL, NULL, INTERNET_FLAG_SECURE, NULL); // for HTTPS
		else if(isSSL == "No" && method == "POST")
			hRequest = HttpOpenRequestA(hConnect, "POST", "/", NULL, NULL, NULL, 0, NULL);

		if (!hRequest)
		{
			iErrorLevel = GetLastError();
			printf("HttpOpenRequestA() Error: %d\n", iErrorLevel);
		}
	}

	if (!iErrorLevel)
	{
		//HttpSendRequest() Error: 12045
		//Resolve 12045 - ERROR_INTERNET_INVALID_CA
		DWORD dwFlags = 0;
		DWORD dwBuffLen = sizeof(dwFlags);
		if (InternetQueryOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, &dwBuffLen))
		{
			dwFlags |= SECURITY_SET_MASK;
			InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));
		}

		//Send the request
		if (!HttpSendRequestA(hRequest, NULL, 0, NULL, 0))
		{
			iErrorLevel = GetLastError();
			printf("HttpSendRequestA() Error: %d\n", iErrorLevel);
		}
	}

	if (!iErrorLevel)
	{
		char aBuffer[BUFSIZ]{};
		DWORD dwBufferSize = BUFSIZ;
		DWORD dwIndex = 0;
		if (!HttpQueryInfoA(hRequest, HTTP_QUERY_STATUS_CODE, aBuffer, &dwBufferSize, &dwIndex))
		{
			iErrorLevel = GetLastError();
			printf("HttpQueryInfoA() Error: %d\n", iErrorLevel);
		}
	}

	if (!iErrorLevel)
	{
		char aBuffer[BUFSIZ]{};
		for (;;)
		{
			DWORD dwBytesRead = 0;
			if (!InternetReadFile(hRequest, aBuffer, BUFSIZ, &dwBytesRead))
			{
				iErrorLevel = GetLastError();
				printf("InternetReadFile() Error: %d\n", iErrorLevel);
				break;
			}

			if (!dwBytesRead)
			{
				break;
			}

			printf("%s", aBuffer);

			ZeroMemory(aBuffer, sizeof(aBuffer));
		}
	}

	if (hRequest)
	{
		if (!InternetCloseHandle(hRequest))
		{
			iErrorLevel = GetLastError();
			printf("InternetCloseHandle(hRequest) Error: %d\n", iErrorLevel);
		}
		else
		{
			hConnect = NULL;
		}
	}

	if (hConnect)
	{
		if (!InternetCloseHandle(hConnect))
		{
			iErrorLevel = GetLastError();
			printf("InternetCloseHandle(hConnect) Error: %d\n", iErrorLevel);
		}
		else
		{
			hConnect = NULL;
		}
	}

	if (hSession)
	{
		if (!InternetCloseHandle(hSession))
		{
			iErrorLevel = GetLastError();
			printf("InternetCloseHandle(hSession) Error: %d\n", iErrorLevel);
		}
		else
		{
			hSession = NULL;
		}
	}

	return iErrorLevel;
}