#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>


//port number
#define PORT 8222
//buffer size to be sent
#define SIZE_TO_SEND 6000000
#define SIZE_TO_SEND_SMALL 60000

void Handle_err(int return_value, char *location)
{
	if(return_value == -1)
	{
		fprintf(stdout, "Error: %s\n", location);
		exit(-1);
	}
}
//Struct to store parameters
typedef struct Info
{
	int packet_size;
	int thread_id;
} data;

//client TCP
void *Cli_TCP(void *arg)
{
	int sockfd, err, i;
	struct sockaddr_in addr_ser;
	char *buffer;

	int SendBuf, tid;
	data *pInfo;
	pInfo = (data *)arg;
	SendBuf = pInfo->packet_size;
	tid = pInfo->thread_id;
	
	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	Handle_err(sockfd, "socket");

	//set TCP packet size according to the packet size
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&SendBuf, sizeof(int));
	
	//set server's address and port
	memset(&addr_ser, 0, sizeof(addr_ser));
	addr_ser.sin_family = AF_INET;
	addr_ser.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_ser.sin_port = htons(PORT + tid);
	//connect to the server
	err = connect(sockfd, (struct sockaddr *)&addr_ser, sizeof(addr_ser));
	Handle_err(err, "connect");
	
	//send messages to the server
	for(i = 0; i < 5; i++)
	{
		buffer = (char*)malloc(SIZE_TO_SEND);
		memset(buffer, 'a', SIZE_TO_SEND);

		send(sockfd, buffer, SIZE_TO_SEND, 0);
	}
	
	//close socket
	close(sockfd);
}

int Buffer_Size(char *block)
{
	int buffersize;	

	if(strcmp(block, "1b") == 0)
	{
		buffersize = 1;
	}
	else if(strcmp(block, "1kb") == 0)
	{
		buffersize = 1024;
	}
	else if(strcmp(block, "64kb") == 0)
	{
		
		buffersize = 65507;

	}
	else 
	{
		printf("The packet size is wrong!");
	}
	return buffersize;
}

//client UDP
void *Cli_UDP(void *arg)
{
	int sockfd, size, i;
	struct sockaddr_in addr_ser;
	char *buffer;
	socklen_t addrlen;

	int SendBuf, tid;
	data *pInfo;
	pInfo = (data *)arg;
	SendBuf = pInfo->packet_size;
	tid = pInfo->thread_id;
	
	//create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	Handle_err(sockfd, "socket");
	
	//set server's address and port
	memset(&addr_ser, 0, sizeof(addr_ser));
	addr_ser.sin_family = AF_INET;
	addr_ser.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_ser.sin_port = htons(PORT + tid);

	addrlen = sizeof(addr_ser);

	int SIZE_TO_SEND_;
	
	//start send messages to server
	for(i = 0; i < 5; i++)
	{
		
		SIZE_TO_SEND_ = SendBuf > 1 ? SIZE_TO_SEND : SIZE_TO_SEND_SMALL;
		buffer = (char*)malloc(SIZE_TO_SEND_);
		memset(buffer, 'a', SIZE_TO_SEND_);
		size = SIZE_TO_SEND_;
		int sendsize;
		//until all the buffers have been sent
		while(size > 0)
		{
			
			//packet size is specified at here, cause UDP uses datagram
			sendsize = sendto(sockfd, buffer, SendBuf, 0, (struct sockaddr *)&addr_ser, addrlen);
			size -= SendBuf;
			if(size == -1)
			{
				printf("Error: sendto: %s\n", strerror(errno));
				exit(-1);
			}
		}
	}
	
	//close socket
	close(sockfd);
}


int main(int argc, char *argv[])
{
	//check input socket
	if(argc != 4)
	{
		printf("usage: %s <connect type> <packet size> <num of thread>\n", argv[0]);
		printf("connect type: \ntcp    udp\n");
		printf("packet size: 1b  1kb  64kb\n");
		exit(-1);
	}

	int SendBuf, i;
	//number of threads
	int numThread = atoi(argv[3]);
	pthread_t tid[numThread];
	char *block = argv[2];
	SendBuf = Buffer_Size(block);
	
	//parameter structure passed in pthread
	data d[2];
	d[0].packet_size = SendBuf;
	d[1].packet_size = SendBuf;
	//variables to count the time
	struct timeval etstart, etstop;
	unsigned long long start, stop;
	
	//start time
	gettimeofday(&etstart, NULL);
	
	//TCP
	if(strcmp(argv[1], "tcp") == 0)
	{
		//run several clients at the same time
		for (i = 0; i < numThread; i++)
		{
			d[i].thread_id = i;
			pthread_create(&tid[i], NULL, Cli_TCP, &d[i]);
		}
	}
	//UDP
	else if(strcmp(argv[1], "udp") == 0)
	{
		//run several clients at the same time
		for(i = 0; i < numThread; i++)
		{
			d[i].thread_id = i;
			pthread_create(&tid[i], NULL, Cli_UDP, &d[i]);
		}
	}
	else
	{
		printf("Error connect type\n");
		exit(-1);
	}

	//wait until all the threads end
	for (i = 0; i < numThread; i++)
	{
		pthread_join(tid[i], NULL);
	}
	
	//end time
	gettimeofday(&etstop, NULL);
	start = (unsigned long long)etstart.tv_sec * 1000000 + etstart.tv_usec;
	stop = (unsigned long long)etstop.tv_sec * 1000000 + etstop.tv_usec;

	int tempSize = (SendBuf > 1 || argv[1] == "tcp") ? SIZE_TO_SEND : SIZE_TO_SEND_SMALL;
	float running_time = (float)(stop - start) / 1000;
	float through_put = (float)numThread * tempSize * 5 * 8 / running_time * 1000 / 1e6;
	float latency = (float)(running_time * 1000)/(tempSize * 5 * numThread * 8);

	printf("Running time %.3f ms, throughput is %.3f Mbits/sec, latency is %f ms/bit\n", running_time, through_put, latency);
	
	return 0;
}
