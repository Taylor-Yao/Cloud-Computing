Required Tools
-------------------------------------------------
This benchmark program needs CUDA compiler and GNU compiler, while ¡°gcc¡± and ¡°nvcc¡± are also needed.
Building
--------------------------------------------------
To compile the files, type the following command:
make all
To remove the binary files, type the following command:
make clean
Usage
------------------------------------------------------------
Type ./start can run the script, which can automatically compile and run the tests of all the benchmarks.
Note that each of the tests can be run separately.
CPU:
Usage: ./cpu_bench -n [nmber of threads]
Example: ./cpu_bench -n 8
GPU:
Usage: ./gpu_bench -t [type of operation(I,F)]
Example:./gpu_bench -t F
Usage: ./gpu_mem_bench -r [(R)ead or (W)rite] -t [size type (B ,KB ,MB)]
Example:./gpu_mem_bench -r R -t B
Memory:
Example:
./memory_bench Ran 1KB 2 1024
Disk:
Example:
./disk_bench Write Seq 1KB 1 1024
Network benchmark:
The command for running network benchmark is:
./network_server [connection type] [number of threads]
Open another terminal,
./network_client [connection type] [buffer size] [number of threads]
Note that all the arguments are required.
Example:
./server tcp 2
Open another terminal.
./client tcp 1kb 2