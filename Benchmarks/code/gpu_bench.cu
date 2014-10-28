
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


                                                     

// this is the max iterations decide to do in the loop
unsigned long MAX_OPS = 20000000;

__global__ void gpu_iops(unsigned long max_ops) {


    
    int ab=1;
    int bb=1;
    int cb=1;
    int db=1;
    int eb=1;
    int fb=1;
    int gb=1;
    int hb=1;
    int ib=1;
    int jb=1;
    int kb=1;
    int lb=1;
    int mb=1;
    int nb=1;
    int ob=1;
    int pb=1;
    int qb=1;
    int rb=1;
    int sb=1;
    int tb=1;
    int ub=1;
    int vb=1;
    int wb=1;
    int xb=1;
   // 24 interger calculation 
    for(unsigned long count=0; count<max_ops; count++)
    {
        ab=ab+12;
        bb=bb+22;
        cb=cb+32;
        db=db+42;
        eb=eb+52;
        fb=fb+62;
        gb=gb+72;
        hb=hb+82;
        ib=ib+92;
        jb=jb+10;
        kb=kb+11;
        lb=lb+12;
        mb=mb*13;
        nb=nb*14;
        ob=ob*15;
        pb=pb*16;
        qb=qb*17;
        rb=rb*18;
        sb=sb*19;
        tb=tb*20;
        ub=ub*21;
        vb=vb*22;
        wb=wb*23;
        xb=xb*24;
    }
}

__global__ void gpu_flops(unsigned long max_ops) {


    float ae=0.2;
    float be=0.2;
    float ce=0.2;
    float de=0.2;
    float ee=0.2;
    float fe=0.2;
    float ge=0.2;
    float he=0.2;
    float ie=0.2;
    float je=0.2;
    float ke=0.2;
    float le=0.2;
    float me=0.2;
    float ne=0.2;
    float oe=0.2;
    float pe=0.2;
    float qe=0.2;
    float re=0.2;
    float se=0.2;
    float te=0.2;
    float ue=0.2;
    float ve=0.2;
    float we=0.2;
    float xe=0.2;

//24  floating point calculation
    for(unsigned long count=0; count<max_ops; count++)
    {
        ae=ae+1.11;
        be=be+2.22;
        ce=ce+3.33;
        de=de+4.44;
        ee=ee+5.52;
        fe=fe+6.61;
        ge=ge+7.72;
        he=he+8.83;
        ie=ie+9.94;
        je=je+10.10;
        ke=ke+11.11;
        le=le+12.12;
        me=me*13.13;
        ne=ne*14.14;
        oe=oe*15.15;
        pe=pe*16.16;
        qe=qe*17.17;
        re=re*18.18;
        se=se*19.19;
        te=te*20.20;
        ue=ue*21.21;
        ve=ve*22.22;
        we=we*23.23;
        xe=xe*24.24;
    }
}

int main(int argc, char *argv[]) {
//decide the character  
    char c; 

//decide the maximum thread 
    int threads = 1024;
    char test = 'I';

//get the parameter
    while ( (c = getopt(argc, argv, "n:l:t:") ) != -1) 
    {
        switch (c) 
        {
            case 'n':
                threads = atoi(optarg);
                break;
            case 'l':
                MAX_OPS = atol(optarg);
                break;
            case 't':
                test = optarg[0];
                break;
            default:
                printf("Usage\n");
                return -1;
        }
    }
   
//set the time parameter
    struct timeval tv;
    long long start, stop;
    double secs;

    if(test == 'I')
    {
      gettimeofday(&tv, NULL);
//get the start time
      start = tv.tv_sec*1000000LL + tv.tv_usec;   
      gpu_iops<<< ceil(threads/1024), 1024 >>>(MAX_OPS);        
      cudaThreadSynchronize(); 
      gettimeofday(&tv, NULL);
//get the stop time
      stop = tv.tv_sec*1000000LL + tv.tv_usec;
//get the totally time cost 
      secs = (stop-start)/1000000.0;
      printf("Time taken: %lf\n", secs);
      printf("IOPS\t%lf\n", (MAX_OPS*24.*threads)/(secs*10000000000.)); 
    }
    else if(test == 'F')
    {
      gettimeofday(&tv, NULL);
//get the start time
      start = tv.tv_sec*1000000LL + tv.tv_usec;
      
      gpu_flops<<< ceil(threads/1024), 1024 >>>(MAX_OPS);        
      cudaThreadSynchronize();
// get the stop time 
      gettimeofday(&tv, NULL);
      stop = tv.tv_sec*1000000LL + tv.tv_usec;
//get the totally time cost 
      secs = (stop-start)/1000000.0;
      printf("Time taken: %lf\n", secs);
      printf("GFLOPS\t%lf\n", (MAX_OPS*24.*threads)/(secs*10000000000.)); 
    }
}
