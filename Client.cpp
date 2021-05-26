#include "Client.h"

Client::Client() {}

Client::Client(const Client& c) {}

const Client& Client::operator=(const Client& c) { return *this; }

int Client::write_to_server(int fd)
{
	int nbytes;
	char buf[BUFLEN];

	fprintf(stdout, "Send to server > ");
	if (fgets(buf, BUFLEN, stdin) == nullptr)
	{
		printf("error\n");
	}
	buf[strlen(buf) - 1] = 0;

	nbytes = send(fd, buf, strlen(buf) + 1, 0);
	if (nbytes < 0) { perror("write"); return -1; }
	if (strstr(buf, "stop")) return -1;
	return 0;
}

int Client::read_from_server(int fd)
{
	int nbytes;
	char buf[BUFLEN];

	nbytes = recv(fd, buf, BUFLEN, 0);
	if (nbytes < 0)
	{
		perror("read");
		return -1;
	}
	else if (nbytes == 0)
	{
		fprintf(stderr, "Client: no message\n");
	}
	else
	{
		fprintf(stdout, "Server's replay: %s\n", buf);
	}
	return 0;
}

