# x64BareBones - starting a 64 Bits OS almost from scratch

x64BareBones is a basic setup to develop operating systems for the Intel 64 bits architecture.

This project is based on a UNIX based operating system where we implement many key OS features.

## Environment setup: 

1- Install the following packages before building the Toolchain and Kernel:

    user@linux:$ nasm qemu gcc make

2- Start the Docker container, in Bash simply run

    docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -ti --name dockerSO agodio/itba-so:1.0
from the directory where you have the x64Barebones project.

***Inside the Docker container***
3- Compile the Toolchain
    
    user@linux:$ cd root/x64Barebones/Toolchain
    user@linux:~/x64Barebones/Toolchain$ make all 
    
4- Compile the Kernel 
    
    user@linux:~/x64Barebones/Toolchain$ cd ..
    user@linux:~/x64Barebones$ make all 

***From Bash terminal***
5- Go inside the x64barebones directory and run:

    user@linux:~/x64barebones$ ./run.sh

