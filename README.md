# Linux-Kernel-Red-Black-Tree-and-Dynamic-Probe




1) To Compile:
- Run make in the directory with Mydriver.o
  -> This will generate the x86 binary userspace (main) file 
  -> Will also generate the Mydriver.ko file

2) Now Enter the Kernel_Probe Directory (located inside the previous directory)
- Run make in this directory
  -> This will generate the Driver_RB_Probe.ko object
  -> It will also generate an extraneous main object file which is not used

NOTE
- Each makefile uses -g -Wall -O0 flags 
- Each ko file should be a 32 bit binary

3) Next 
- Import both kernel objects and the userspace main object file (from the directory with Mydriver.ko) to the Galileo  board

4) Next Run
- sudo insmod Mydriver.ko
- sudo insmod Driver_RB_Probe.ko

5) Verify all devices are in /dev 

6) To run the userspace program
- sudo ./main

7)
- One thread in the userspace program can read in user input for creating kprobes








NOTE: When using a test program in userspace for interacting with a device the driver interfaces with, the userspace binary should be run with: sudo <bin>