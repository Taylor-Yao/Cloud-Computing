#!/bin/sh
make all

LOGFILE=benchmarks.log

echo Benchmarking Suite Log >> $LOGFILE
date >> $LOGFILE

echo Running CPU Benchmarks...

for a in 1 2 4 8
do
	./cpu_bench  -n $a >> $LOGFILE
done
echo >> $LOGFILE

echo Done!


echo Benchmarking Suite Log >> $LOGFILE
date >> $LOGFILE


echo Running GPU Benchmanrks...
for a in I F
do
  ./gpu_bench  -t $a >> $LOGFILE
done
echo >> $LOGFILE
echo Size RW  Throughput >> $LOGFILE
for a in R W
do
  for s in B K M
  do
    ./gpu_mem_bench  -r $a -t $s >> $LOGFILE
  done
done
echo >> $LOGFILE


mkdir memory
m=1B

for access in Seq Ran
	do
	for blocksize in 1B 1KB 1MB
	do
		if [ $blocksize != $m ]
		then
   			fileLen=1024
		else
   			fileLen=20
		fi

		for thread in 1 2
		do

			echo "access type: "$access	\
			", block size:"$blocksize", thread number: "$thread
			echo $thread >> memory"_"$access"_"$blocksize.txt
			./memory_bench $access $blocksize $thread $fileLen >>	\
			memory"_"$access"_"$blocksize.txt

		done
		mv memory"_"$access"_"$blocksize.txt memory
	done
done
echo Memory testing finish!

mkdir disk
m=1B
for opt in Write Read
do
	for access in Seq Ran
	do
		for blocksize in 1B 1KB 1MB
		do
			if [ $blocksize != $m ]
			then
   				fileLen=200
			else
   				fileLen=2
			fi

			for thread in 1 2 4
			do
				
				echo "operation type: "$opt", access type: "$access	\
				", block size:"$blocksize", thread number: "$thread
				echo $thread >> disk"_"$opt"_"$access"_"$blocksize.txt	
				./disk_bench $opt $access $blocksize $thread $fileLen >>	\
				disk"_"$opt"_"$access"_"$blocksize.txt
			done 
			
			mv disk"_"$opt"_"$access"_"$blocksize.txt disk
		done
	done
done

echo Disk testing finish!

mkdir network
for connect in tcp udp
do
	for packet in 1b 1kb 64kb
	do
		for thread in 1 2
		do		
			echo "connect type: "$connect", packet size:"$packet	\
			", thread number: "$thread
			echo $thread thread >> network"_"$connect"_"$packet.txt 		
			./network_server $connect $thread &	
			sleep 2	
			./network_client $connect $packet $thread >>	\
			network"_"$connect"_"$packet.txt
		done
		mv network"_"$connect"_"$packet.txt network
	done
done

echo Network testing finish!
make clean
