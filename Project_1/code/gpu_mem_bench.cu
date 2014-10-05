#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>



//define the check err
#define CHECK_ERR(x)                                    \
  if (x != cudaSuccess) {                               \
    fprintf(stderr,"%s in %s at line %d\n",             \
	    cudaGetErrorString(err),__FILE__,__LINE__);	\
    exit(-1);						\
  }                                                     \

//define the maximum operations for the loop 
unsigned long MAX_OPERATIONS = 40000000;

//size for 1MB
const long MEGABYTE = 1048576;

__global__ void gpu_iops(unsigned long max_ops) {


    
}

//get the parameter
int main(int argc, char *argv[]) {
  
    char c;  
    char test = 'B';
    char rw = 'R';
    while ( (c = getopt(argc, argv, "r:t:") ) != -1)
    {
        switch (c) 
        {
            case 'r':
                rw = optarg[0];
                break;
            case 't':
                test = optarg[0];
                break;
            default:
                printf("nothing\n");
                return -1;
        }
    }
//define the time variables
    struct timeval tv;
    long long startTime, stopTime;
    double seconds;
	
//define the cuda err
    cudaError_t err;
 
//define the memory pointer
 
    unsigned char *d_mem_pointer;
    unsigned char *mem_pointer;
	
    cudaMemcpyKind dir = cudaMemcpyHostToDevice;
	
	//direction for read and write
    if(rw == 'R')
    {
      dir = cudaMemcpyDeviceToHost;
    }
	
    else if(rw == 'W')
    {
      dir - cudaMemcpyHostToDevice;
    }
    
    //test the 1B size block
    if(test == 'B')
    {
      
      err = cudaMalloc((void **) &d_mem_pointer, sizeof(unsigned char)*MEGABYTE);
      CHECK_ERR(err);
      mem_pointer = (unsigned char *)malloc(sizeof(unsigned char)*1);
      gettimeofday(&tv, NULL);
      startTime = tv.tv_sec*1000000LL + tv.tv_usec;
    
      for(unsigned long i = 0; i<MEGABYTE; i++)
      {
//write operation
        if(rw == 'W')
          err = cudaMemcpy((void *)&d_mem_pointer[i], (void *)mem_pointer, 1, dir);
//read operation  
      else if(rw == 'R')
          err = cudaMemcpy((void *)mem_pointer, (void *)&d_mem_pointer[i], 1, dir);
        CHECK_ERR(err);
      }
      
      gettimeofday(&tv, NULL);
//record the stop time 
      stopTime = tv.tv_sec*1000000LL + tv.tv_usec;
	  
//totally time cost 
 
      seconds = (stopTime-startTime)/1000000.0;
      printf("Operation: %c\tMessage Size:1%c\tBandwidth: %lf MB/S \n", rw, test, 1.0/(seconds)); 
    }
//test the 1K block size
    else if(test == 'K')
    {
      err = cudaMalloc((void **) &d_mem_pointer, sizeof(unsigned char)*256*MEGABYTE);
      CHECK_ERR(err);
      mem_pointer = (unsigned char *)malloc(sizeof(unsigned char)*1024);
      gettimeofday(&tv, NULL);
      startTime = tv.tv_sec*1000000LL + tv.tv_usec;
    
      for(unsigned long i = 0; i<256*MEGABYTE/1024; i++)
      {
// test the write operation 
        if(rw == 'W')
          err = cudaMemcpy((void *)&d_mem_pointer[i*1024], (void *)mem_pointer, 1024, dir);
//test the read operation 
        else if(rw == 'R')
          err = cudaMemcpy((void *)mem_pointer, (void *)&d_mem_pointer[i*1024], 1024, dir);
        CHECK_ERR(err);
      }
      
      gettimeofday(&tv, NULL);
//record the stop time
      stopTime = tv.tv_sec*1000000LL + tv.tv_usec;
// totally time cost 
      seconds = (stopTime-startTime)/1000000.0;
      printf("Operation:%c\tMessage Size:1%cB\tBandwidth: %lf MB/S \n", rw, test, (256.0/1024.0)/(seconds)); 
    }
//test 1MB size Block
    else if(test == 'M')
    {
      err = cudaMalloc((void **) &d_mem_pointer, sizeof(unsigned char)*512*MEGABYTE);
      CHECK_ERR(err);
      mem_pointer = (unsigned char *)malloc(sizeof(unsigned char)*MEGABYTE);
      gettimeofday(&tv, NULL);
      startTime = tv.tv_sec*1000000LL + tv.tv_usec;
    
      for(unsigned long i = 0; i<512*10; i++)
      {
//write operation
        if(rw == 'W')
          err = cudaMemcpy((void *)&d_mem_pointer[(i*MEGABYTE)%(512*MEGABYTE)], (void *)mem_pointer, MEGABYTE, dir);
//read operation 
        else if(rw == 'R')
          err = cudaMemcpy((void *)mem_pointer, (void *)&d_mem_pointer[(i*MEGABYTE)%(512*MEGABYTE)], MEGABYTE, dir);
        CHECK_ERR(err);
      }
      
      gettimeofday(&tv, NULL);
// record the stop time
      stopTime = tv.tv_sec*1000000LL + tv.tv_usec;
//ttoally time cost 
      seconds = (stopTime-startTime)/1000000.0;
      printf("Operation:%c\tMessage Size:1%cB \tBandwidth:%lf MB/S\n", rw, test, (512*10)/(seconds)); 
    }
    err = cudaFree(d_mem_pointer);
    CHECK_ERR(err);
}
