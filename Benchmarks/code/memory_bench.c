#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// Set memory space
int memSpace;

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

/* Sequential access to the memory
 * Input: blockSize
 */
void *seq_Access(void *block_Sizes)
{
	
	int i;
	int size = (int)(long)block_Sizes;
	char *low = (char*)malloc(sizeof(char) * memSpace);
			char *temp = (char*)malloc(sizeof(char) * size);
	//copy memory block by block to new allocated memories
	for(i = 0; i < memSpace/size; i++)
	{		
		//char *temp = (char*)malloc(sizeof(char) * size);
		memcpy(temp, low + i * size, size);
	}
	free(low);
	//free(temp);	
	
}

/* Random access to the memory
 * Input: blockSize
 */
void *ran_Access(void *block_Sizes)
{
	int i, ran;
	int size = (int)(long)block_Sizes;
	char *low = (char*)malloc(sizeof(char) * memSpace);
	
	srand((unsigned)time(NULL));
			char *temp = (char*)malloc(sizeof(char) * size);
	//copy memory randomly to new allocated memories
	for(i = 0; i < memSpace/size; i++)
	{
		ran = rand()%(memSpace/size);
		//char *temp = (char*)malloc(sizeof(char) * size);
		memcpy(temp, low + ran * size, size);
	}
	free(low);
	//free(temp);
}


int main(int argc, char* argv[]){
	if(argc != 5)
	{
		printf("Please input right command format:\n");
        	printf("Usage: %s\n", argv[0]);
        	printf("Access type: Ran(Random) or Seq(Sequential)\n");
        	printf("block_Sizes: 1B, 1KB or 1MB\n");
        	printf("Concurrency: 1, or 2 threads\n");
		printf("Input memory space (MB)\n");
		exit(0);
	}

        // Initialization
	char *accessType = argv[1];
	char *block = argv[2];
	int num_Thread = atoi(argv[3]);
	int len = atoi(argv[4]);	
	memSpace = 1024*1024*len;
	//printf("%lld\n", memSpace);

	int blockSize;
	pthread_t thread[num_Thread];
	struct timeval startTime, endTime;
	unsigned long long ms_Start, ms_End;
	float totalTime;
	
	// Get block size
	int block_Size(char *block);
	blockSize = block_Size(block);
	//printf("%d\n",blockSize);

	// Start time
	gettimeofday(&startTime, NULL);

	// Choose which method to run
	int i;
	if(strcmp(argv[1], "Seq") == 0)
		for (i = 0; i < num_Thread; i++)
		{
			pthread_create(&thread[i], NULL, seq_Access, (void*)(long)blockSize);
		}	
	else if(strcmp(argv[1], "Ran") == 0)
		for (i = 0; i < num_Thread; i++)
		{
			pthread_create(&thread[i], NULL, ran_Access, (void*)(long)blockSize);
		}
	else
	{
		printf("Error\n");
		exit(0);
	}	

	for (i = 0; i < num_Thread; i++)
	{
		pthread_join(thread[i], NULL);
	}
	
	// End time
	gettimeofday(&endTime, NULL);
	
	// Calculate the run time
	float run_Time(struct timeval startTime, struct timeval endTime);
	float runTime = run_Time(startTime, endTime);

	float throughPut = (float)num_Thread * memSpace / runTime * 1000 / 1048576;
	float latency = (float)(runTime*1000)/(memSpace*num_Thread*8);

	printf("Memory testing: %s\t, %s\t, %s\t", argv[1], argv[2], argv[3]);
	printf("Run time: %.3f ms, ThroughPut: %.3f MB/sec, Latency: %f ms/bit\n", runTime, throughPut,latency);
	//printf("Latency: %.3f ms/bit\n", latency);
	return 0;

}
