//program by ZHAOAN GUAN  A20307995 
// CPU_BENCHMARK

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

unsigned long MAX_OPERATION = 60000000;

void* cpuInt(void* arg)
{ //define the integer  
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
    
    for(unsigned long cNN=0; cNN<MAX_OPERATION; cNN++)
    {
//do the integer calculation 
        ab=ab+1;
        bb=bb+2;
        cb=cb+3;
        db=db+4;
        eb=eb+5;
        fb=fb+6;
        gb=gb+7;
        hb=hb+8;
        ib=ib+9;
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
    
    
    return NULL;
}

void* cpuFloat(void* arg)
{  

//define the floating point  
    float ac=1.1;
    float bc=1.1;
    float cc=1.1;
    float dc=1.1;
    float ec=1.1;
    float fc=1.1;
    float gc=1.1;
    float hc=1.1;
    float ic=1.1;
    float jc=1.1;
    float kc=1.1;
    float lc=1.1;
    float mc=1.1;
    float nc=1.1;
    float oc=1.1;
    float pc=1.1;
    float qc=1.1;
    float rc=1.1;
    float sc=1.1;
    float tc=1.1;
    float uc=1.1;
    float vc=1.1;
    float wc=1.1;
    float xc=1.1;
    for(unsigned long cNN=0; cNN<MAX_OPERATION; cNN++)
    {
//do the floating point operation 
        ac=ac+1.1;
        bc=bc+2.2;
        cc=cc+3.3;
        dc=dc+4.4;
        ec=ec+5.5;
        fc=fc+6.6;
        gc=gc+7.7;
        hc=hc+8.8;
        ic=ic+9.9;
        jc=jc+10.10;
        kc=kc+11.11;
        lc=lc+12.12;
        mc=mc*13.13;
        nc=nc*14.14;
        oc=oc*15.15;
        pc=pc*16.16;
        qc=qc*17.17;
        rc=rc*18.18;
        sc=sc*19.19;
        tc=tc*20.20;
        uc=uc*21.21;
        vc=vc*22.22;
        wc=wc*23.23;
        xc=xc*24.24;
    }
    
    return NULL;
}

int main(int argc, char** argv)
{
    char caa;

//one htread as default
    int threads = 1;	
//define the perameter
    while ( (caa = getopt(argc, argv, "n:l:") ) != -1) 
    {
        switch (caa) 
        {
            case 'n':
                threads = atoi(optarg);
                break;
            case 'l':
                MAX_OPERATION = atol(optarg);
                break;
            default:
                printf("useage");
                return -1;
                break;
        }
    }
    
    struct timeval tv;
    long long start, stop;
    double seconds;
    
    pthread_t cpu_threads[threads]; 
   
	printf("Totally Thread: %d\t\n", threads);
 
    gettimeofday(&tv, NULL);
//record the start time
    start = tv.tv_sec*1000000LL + tv.tv_usec;
//create multiple thread
    for(int i=0; i<threads; i++)
    {
       pthread_create(&cpu_threads[i], NULL, cpuInt, NULL);
    }
   
    for(int i=0; i<threads; i++)
    {
        pthread_join(cpu_threads[i], NULL);
    }
	  gettimeofday(&tv, NULL);
//record the stop time
    stop = tv.tv_sec*1000000LL + tv.tv_usec;
//totally time cost 
    seconds = (stop-start)/1000000.0;
    printf("Time taken: %lf\n", seconds);
    printf("%lf GIOPS\n", (MAX_OPERATION*24.*threads)/(seconds*100000000.)*2); 
 
    
    gettimeofday(&tv, NULL);
    start = tv.tv_sec*1000000LL + tv.tv_usec;

//create multiple thread
    for(int i=0; i<threads; i++)
    {
       pthread_create(&cpu_threads[i], NULL, cpuFloat, NULL);
    }
    
    for(int i=0; i<threads; i++)
    {
        pthread_join(cpu_threads[i], NULL);
    }
		gettimeofday(&tv, NULL);
//record the stop time
    stop = tv.tv_sec*1000000LL + tv.tv_usec;
//totally time cost 
    seconds = (stop-start)/1000000.0;
    printf("Time taken: %lf\n", seconds);
    printf("%lf GFLOPS\n", (MAX_OPERATION*24.*threads)/(seconds*100000000.)*2);  
    
    
    return 0;
}
