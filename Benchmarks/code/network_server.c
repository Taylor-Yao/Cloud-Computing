#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


//port number
#define PORT 8222
//buffer size to be received
#define SIZE_TO_RECEIVE 30000000

//function to handle error
void Handle_err(int return_value, char *location)
{
	if(return_value == -1)
	{
		fprintf(stdout, "Error: %s\n", location);
		exit(-1);
	}
}
//server TCP
void *Ser_TCP(void *th_id)
{
	int tid = (int)(long)th_id;
	int ser_sockfd, cli_sockfd;
	int err, size;
	struct sockaddr_in ser_addr;
	char *buffer;
	socklen_t addrlen;
	
	//create socket
	ser_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	Handle_err(ser_sockfd, "socket");
	
	//set server address and port
	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//may use multi ports at the same time
	ser_addr.sin_port = htons(PORT + tid);
	//bind port to the socket
	err = bind(ser_sockfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
	Handle_err(err, "bind");
	
	//start listening the port
	err = listen(ser_sockfd, 5);
	Handle_err(err, "listen");

	printf("Server is listening...\n");

	addrlen = sizeof(struct sockaddr);
	//when a client require to connect, server accept it
	cli_sockfd = accept(ser_sockfd, (struct sockaddr *)&cli_sockfd, &addrlen);
	Handle_err(cli_sockfd, "accept");

	buffer = (char*)malloc(SIZE_TO_RECEIVE);
	size = SIZE_TO_RECEIVE;

	//if size is 0, all messages have been sent
	while(size > 0)
	{
		size = recv(cli_sockfd, buffer, SIZE_TO_RECEIVE, 0);
		Handle_err(size, "recv");
	}
	close(cli_sockfd);
	//close socket
	close(ser_sockfd);
}

//server UDP
void *Ser_UDP(void *th_id)
{
	int sockfd;
	int err, size;
	struct sockaddr_in addr_server, addr_client;
	char *buffer;
	socklen_t addrlen;

	int tid = (int)(long)th_id;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	Handle_err(sockfd, "socket");
	
	//configure server 
	memset(&addr_server, 0 ,sizeof(addr_server));
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_server.sin_port = htons(PORT + tid);
	
	addrlen = sizeof(struct sockaddr);
	err = bind(sockfd, (struct sockaddr *)&addr_server, sizeof(addr_server));
	Handle_err(err, "bind");

	printf("Server is receiving...\n");

	//set the receiving buffer
	buffer = (char*)malloc(SIZE_TO_RECEIVE);
	memset(buffer, 0, SIZE_TO_RECEIVE);
		
	//receive messages from client
	size = recvfrom(sockfd, buffer, SIZE_TO_RECEIVE, 0, (struct sockaddr *)&addr_client, &addrlen);
	Handle_err(size, "recvfrom");
	//close socket
	close(sockfd);
}

int main(int argc, char *argv[])
{
	//check input parameters
	if(argc != 3)
	{
		printf("usage: %s <type of connection> <number of threads>\n", argv[0]);
		printf("connect type: \ntcp       udp\n");
		exit(-1);
	}

	int i;
	//number of threads
	int numThread = atoi(argv[2]);
	pthread_t tid[numThread];
	
	//identify the type of connection
	if(strcmp(argv[1], "tcp") == 0)
	{
		//Maybe need two threads
		for (i = 0; i < numThread; i++)
		{
			pthread_create(&tid[i], NULL, Ser_TCP, (void*)(long)i);
		}
	}

	else if(strcmp(argv[1], "udp") == 0)
	{
		
		for(i = 0; i < numThread; i++)
		{
			pthread_create(&tid[i], NULL, Ser_UDP, (void*)(long)i);
		}
	}
	else
	{
		fprintf(stdout, "Error connect type\n");
		exit(-1);
	}

	//wait for all threads ending
	for (i = 0; i < numThread; i++)
	{
		pthread_join(tid[i], NULL);
	}

	return 0;
}
