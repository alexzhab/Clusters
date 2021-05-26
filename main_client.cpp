#include "Client.h"

int main(int argc, char** argv)
{
	Client c;

	const char* serverName;
	serverName = (argc < 2) ? "127.0.0.1" : argv[1];

	int err;
	int sock;
	struct sockaddr_in server_addr;
	//struct hostent *hostinfo;

	// инициализаци€ windows sockets
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed\n");
		return -1;
	}

	// ѕолучаем информацию о сервере по его DNS имени
	// или точечной нотации IP адреса.
	//hostinfo = gethostbyname(SERVER_NAME);
	//if (hostinfo == NULL) {
	// fprintf(stderr, "Unknown host %s.\n", SERVER_NAME);
	// exit(EXIT_FAILURE);
	//}
	// можно было бы использовать GetAddrInfo()

	// «аполн€ем адресную структуру дл€ последующего
	// использовани€ при установлении соединени€
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	//server_addr.sin_addr = *(struct in_addr*) hostinfo->h_addr;
	unsigned int iaddr;
	inet_pton(AF_INET, serverName, &iaddr);
	server_addr.sin_addr.s_addr = iaddr;

	// —оздаем TCP сокет.
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Client: socket was not created");
		exit(EXIT_FAILURE);
	}

	// ”станавливаем соединение с сервером
	err = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (err < 0) {
		perror("Client: connect failure");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "Connection is ready\n");

	// ќбмениваемс€ данными
	while (1)
	{
		if (c.write_to_server(sock) < 0) break;
		if (c.read_from_server(sock) < 0) break;
	}
	fprintf(stdout, "The end\n");

	// «акрываем socket
	closesocket(sock);
	WSACleanup();
	exit(EXIT_SUCCESS);
}