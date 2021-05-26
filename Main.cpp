// Copyright Zhabitskaya A.
// 2021

#include "Proxi.h"

int main()
{

	  Proxi p;
	   p.open_log();

	   int     i, err, opt = 1;
	   int     sock, new_sock;
	   fd_set  active_set, read_set;
	   struct  sockaddr_in  addr;
	   struct  sockaddr_in  client;
	   char    buf[BUFLEN];
	   socklen_t  size;

	   //// »нициализаци€ windows sockets
	   WSADATA wsaData;
	   if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	   {
	   	printf("WSAStartup failed\n");
	   	return 1;
	   }

	   //// —оздаем TCP сокет дл€ приема запросов на соединение
	   sock = socket(PF_INET, SOCK_STREAM, 0);
	   if (sock < 0) 
	   {
	   	perror("Server: cannot create socket");
	   	exit(EXIT_FAILURE);
	   }
	   setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

	   //// «аполн€ем адресную структуру и
	   //// св€зываем сокет с любым адресом
	   addr.sin_family = AF_INET;
	   addr.sin_port = htons(PORT);
	   addr.sin_addr.s_addr = htonl(INADDR_ANY);
	   err = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	   if (err < 0) 
	   {
	   perror("Server: cannot bind socket");
	   WSACleanup();
	   	exit(EXIT_FAILURE);
	   }

	   // —оздаем очередь на 3 вход€щих запроса соединени€
	   err = listen(sock, 3);
	   if (err < 0) 
	   {
	   	perror("Server: listen queue failure");
	   	WSACleanup();
	   	exit(EXIT_FAILURE);
	   }

	   // ѕодготавливаем множества дескрипторов каналов ввода-вывода.
	    //ƒл€ простоты не вычисл€ем максимальное значение дескриптора,
	    //а далее будем провер€ть все дескрипторы вплоть до максимально
	   //возможного значени€ FD_SETSIZE.
	   FD_ZERO(&active_set);
	   FD_SET(sock, &active_set);

	   //// ќсновной бесконечный цикл проверки состо€ни€ сокетов
	   while (1) {
	   // ѕроверим, не по€вились ли данные в каком-либо сокете.
	   // ¬ нашем варианте ждем до фактического по€влени€ данных.
	   	read_set = active_set;
	   if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) 
	   	{
	   	perror("Server: select  failure");
	   	WSACleanup();
	   		exit(EXIT_FAILURE);
	   	}
	   	// fd_set в windows устроена немного иначе - как массив дескрипторов
	   	// ƒанные по€вились. ѕроверим в каком сокете.
	   	for (int j = 0; j < read_set.fd_count; j++) {
	   	i = read_set.fd_array[j];
	   	if (FD_ISSET(i, &read_set)) {
	   		if (i == sock) {
	   			// пришел запрос на новое соединение
	   			size = sizeof(client);
	   			new_sock = accept(sock, (struct sockaddr*)&client, &size);
	   			if (new_sock < 0) {
	   				perror("accept");
	   				WSACleanup();
	   				exit(EXIT_FAILURE);
	   			}
	   			fprintf(stdout, "Server: connect from host %s, port %hu.\n",
	   				inet_ntoa(client.sin_addr),
	   				ntohs(client.sin_port));
	   			FD_SET(new_sock, &active_set);
	   		}
	   		else {
	   			// пришли данные в уже существующем соединени
	   			err = p.read_from_client(i, buf);
	   	if (err < 0) {
	   				// ошибка или конец данных
	   			closesocket(i);
	   				FD_CLR(i, &active_set);
	   				}
	   			else {
	   				// а если это команда закончить работу?
	   				if (strstr(buf, "stop")) {
	   					closesocket(i);
	   					FD_CLR(i, &active_set);
	   				}
	   				else {
	   					// данные прочитаны нормально
	   					p.write_to_client(i, buf);
	   				}
	   			}
	   		}
	   	}
	   	}
	   }
	   WSACleanup();
	return 0;
}