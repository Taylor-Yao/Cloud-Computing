CFLAGS =-std=gnu99
all: cpu_bench gpu_bench gpu_mem_bench memory_bench disk_bench network_server network_client 

cpu_bench:cpu_bench.o
	gcc -o cpu_bench $(CFLAGS) cpu_bench.c -lpthread

gpu_bench:
	nvcc -o gpu_bench gpu_bench.cu

gpu_mem_bench:
	nvcc -o gpu_mem_bench gpu_mem_bench.cu

memory_bench: memory_bench.o
	gcc -o memory_bench memory_bench.c -lpthread

disk_bench: disk_bench.o
	gcc -o disk_bench disk_bench.c -lpthread

network_server: network_server.o
	gcc -o network_server network_server.c -lpthread

network_client: network_client.o
	gcc -o network_client network_client.c -lpthread


clean:
	rm cpu_bench.o memory_bench.o disk_bench.o network_server.o network_client.o cpu_bench gpu_bench gpu_mem_bench disk_bench memory_bench network_server network_client 
