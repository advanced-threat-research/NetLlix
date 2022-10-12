#include "WINHTTPS.h"

int winhttps(const char* ip, const char* method)
{
    //Convert const char* to LPCWSTR
    WCHAR attacker[100];
    MultiByteToWideChar(CP_ACP, 0, ip, -1, attacker, 100);
    LPCWSTR server = attacker;

    BOOL bResults = FALSE;
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    LPCWSTR postdata = L"IAMROOT                                                    ";

    printf("Go HTTPS! \n\n");

    hSession = WinHttpOpen(L"WINHTTPS", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (hSession)
        hConnect = WinHttpConnect(hSession, server, INTERNET_DEFAULT_HTTPS_PORT, 0);

    if (hConnect && method == "GET")
        hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    else 
        hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
        
    if (hRequest)
    {
        DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;

        WinHttpSetOption( hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

        if (hConnect && method == "GET")
            bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
        else
            bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID)postdata, wcslen(postdata), wcslen(postdata), 0);
    }
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