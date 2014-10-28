# include<stdio.h>
# include<stdlib.h>
# include<time.h>
# include<string.h>
# include<pthread.h>
# include<sys/types.h>
# include<sys/stat.h>
# include<fcntl.h>
# include<unistd.h>

long fileLength;
/* Get the block size
 * Transfer the command to block size
 * 1B == 1  1KB == 1024  1MB == 1048576  default == 1024 (byte)
 */
int block_Size(char *block)
{
	int blockSize;	
	// Get block_Sizes
	//int blockSize;
	if(strcmp(block, "1B") == 0)
	{
		blockSize = 1;
	}
	else if(strcmp(block, "1KB") == 0)
	{
		blockSize = 1024;
	}
	else if(strcmp(block, "1MB") == 0)
	{
		// 1024*1024 = 1048576
		blockSize = 1048576;
	}
	else 
	{
		// Set default block size to 1048576
		blockSize = 1048576;
		printf("The default block size is 1MB!");
	}
	return blockSize;
}

/* Get the code of the command
 * Transfer the command to code number
 * 1. Read Seq  2. Read Ran
 * 3. Write Seq 4. Write Ran 
 */
int code(char *operation, char *accessType)
{
	int codeNum;
	if(strcmp(operation, "Read") == 0)
	{
		// Sequential Access
		if(strcmp(accessType, "Seq") == 0)
		{
			codeNum = 1;
			
		}
		else if(strcmp(accessType, "Ran") == 0)
		{
			codeNum = 2;
		}
	}
	else if(strcmp(operation, "Write") == 0)
	{
		if(strcmp(accessType, "Seq") == 0)
		{
			codeNum = 3;
		}
		else if(strcmp(accessType, "Ran") == 0)
		{
			codeNum = 4;
		}
	}
	return codeNum;
}

/* Calculate runtime of operation
 */
float run_Time(struct timeval startTime, struct timeval endTime)
{
	float runTime;
	unsigned long long ms_Start, ms_End;
	ms_Start = (unsigned long long)startTime.tv_sec * 1000000 + startTime.tv_usec;
	ms_End = (unsigned long long)endTime.tv_sec * 1000000 + endTime.tv_usec;

	runTime = (float)(ms_End - ms_Start) / 1000;
	return runTime;
}

// Random read file from disk
void *ran_Read(void *block_Sizes)
{
	int i, r;
	int size = (int)(long)block_Sizes;
	srand((unsigned)time(NULL));
	
	int fd;
	// Open test file from local disk
	fd = open("test.bat", O_RDONLY, 0666);
	if(fd < 0)
	{
		printf("Open test file failed!\n");
		printf("Please creat test file first!\n");
		exit(0);
	}
	
	// Randomly choose file indicator to read test file
	i = fileLength;
	while(i>0)
	{
		r = rand()%(fileLength / size);	
		char *buffer = (char*)malloc(sizeof(char) * size);
		// Set the positon of the file stream to the given random offset
		lseek(fd, r * size, SEEK_SET);
		// Read block size of file from the disk
		read(fd, buffer, size);
		i = i - size;
		free(buffer);
	}
	
	close(fd);

}

// Sequential read file from disk
void *seq_Read(void *block_Sizes)
{
	int i;
	int size = (int)(long)block_Sizes;
	
	// Open file from the disk
	int fd;
	fd = open("test.bat", O_RDONLY, 0666);
	if(fd < 0)
	{
		printf("Open test file failed!\n");
		printf("Please creat test file first!\n");
		exit(0);
	}
	
	
	// Sequential read data from the disk
	i = fileLength;
	while(i>0)
	{
		// Read block size of file from the disk
		char *buffer = (char*)malloc(sizeof(char) * size);
		read(fd, buffer, size);
		i = i - size;
		free(buffer);
	}
	
	close(fd);

}

// Random write file to disk
void *ran_Write(void *block_Sizes)
{
	int i, r;
	int size = (int)(long)block_Sizes;
	srand((unsigned)time(NULL));
	
	// Open file from the disk
	int fd;
	fd = open("test.bat", O_CREAT|O_TRUNC|O_WRONLY, 0666);
	if(fd < 0)
	{
		printf("Creat test file failed!\n");
		exit(0);
	}
	
	char *buffer = (char*)malloc(sizeof(char) * size);
	memset(buffer, 'a', size);
	// Randomly choose the file indicator to write the file
	i = fileLength;
	while(i>0)
	{
		r = rand()%(fileLength / size);	
		// Set the positon of the file stream to the given random offset
		lseek(fd, r * size, SEEK_SET);
		// Read block size of file from the disk
		write(fd, buffer, size);
		i = i - size;
	}
	free(buffer);
	close(fd);

}

// Sequential write file to disk
void *seq_Write(void *block_Sizes)
{
	int i;
	int size = (int)(long)block_Sizes;
	
	// Open file from the disk
	int fd;
	fd = open("test.bat", O_CREAT|O_TRUNC|O_WRONLY, 0666);
	if(fd < 0)
	{
		printf("Creat test file failed!\n");
		exit(0);
	}
	
	char *buffer = (char*)malloc(sizeof(char) * size);
	memset(buffer, 'a', size);
	//randomly write the disk
	i = fileLength;
	while(i>0)
	{
		write(fd, buffer, size);
		i = i - size;
	}
	free(buffer);
	close(fd);

}


int main(int argc, char *argv[])
{
	
	if(argc != 6)
	{
		printf("Please input right command format:\n");
        	printf("Usage: %s\n", argv[0]);
		printf("Operation: Read or Write\n");
        	printf("Access type: Ran(Random) or Seq(Sequential)\n");
        	printf("block_Sizes: 1B, 1KB or 1MB\n");
        	printf("Concurrency: 1, 2 or 4 threads\n");
		printf("Input file length (MB)\n");
		exit(0);
	}
	
	// Initialization
	char *operation = argv[1];
	char *accessType = argv[2];
	char *block = argv[3];
	int num_Thread = atoi(argv[4]);
	int len = atoi(argv[5]);	
	fileLength = 1024*1024*len;

	int blockSize;
	int codeNum;
	pthread_t thread[num_Thread];
	struct timeval startTime, endTime;
	unsigned long long ms_Start, ms_End;
	float totalTime;
	
	// Get block size
	int block_Size(char *block);
	blockSize = block_Size(block);
	//printf("%d\n",blockSize);
 
	int code(char *operation, char *accessType);
	codeNum = code(operation, accessType);
	//printf("%d\n",codeNum);

	//Start time
	gettimeofday(&startTime, NULL);
	int i;
	for(i = 0; i < num_Thread; i++)
	{
		switch(codeNum)
		{
		case 1:pthread_create(&thread[i], NULL, seq_Read, (void*)(long)blockSize); break;
		case 2:pthread_create(&thread[i], NULL, ran_Read, (void*)(long)blockSize); break;
		case 3:pthread_create(&thread[i], NULL, seq_Write, (void*)(long)blockSize); break;
		case 4:pthread_create(&thread[i], NULL, ran_Write, (void*)(long)blockSize); break;
		default: printf("Error\n");
		}
		 
	}
	
	for(i = 0; i < num_Thread; i++)
	{
		pthread_join(thread[i], NULL);
	}
	//End time
	gettimeofday(&endTime, NULL);

	float run_Time(struct timeval startTime, struct timeval endTime);
	float runTime = run_Time(startTime, endTime);

	float throughPut = (float)num_Thread * fileLength / runTime * 1000 / 1048576;
	float latency = (float)(runTime*1000)/(fileLength*num_Thread*8);

	//printf("Run time %.3f ms, %.3f MB/sec\n", runTime, throughPut);
	//printf("Latency: %.3f ms/bit\n", latency);
	//printf("Disk testing: %s\t, %s\t, %s\t, %s\t", argv[1], argv[2], argv[3], argv[4]);
	printf("Run time: %.3f ms, ThroughPut: %.3f MB/sec, Latency: %f ms/bit\n", runTime, throughPut,latency);
	return 0;
}
