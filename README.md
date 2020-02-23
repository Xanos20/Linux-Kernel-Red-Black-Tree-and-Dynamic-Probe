# Linux-Kernel-Red-Black-Tree-and-Dynamic-Probe

Kamal Nadesan
1209035928



1) To Compile:
- Run 'make' in the directory with Mydriver.c
  -> This will generate the x86 binary userspace (main) file 
  -> Will also generate the Mydriver.ko file

2) Now run make
- Run 'make' in the Kernel_Probe directory with Driver_RB_Probe.c
  -> This will generate the Driver_RB_Probe.ko object
  -> It will also generate an extraneous main object file which is not used

NOTE
- Each makefile uses -g -Wall -O0 flags 
- Each ko file as well as the main object file should be a 32 bit binary

3) Next 
- Import both kernel objects and the userspace main object file (from the directory with Mydriver.ko) to the Galileo board

4) Next Run
- sudo insmod Mydriver.ko
- sudo insmod Driver_RB_Probe.ko

5) Verify all devices are in /dev 

6) To run the userspace program
- sudo ./main

7)
- One thread in the userspace program can read in user input for creating/removing kprobes
- From Professor Lee -> The user is fully responsible for placing the kprobes at the correct x86 instruction 
- From TA Gao -> he recommended using flag=1 for writing the kprobe to the rbtree_write fun
	and flag=2 for writing the kprobe to the rbtree_read fun
- User will pass an int flag and unsigned int offset for writing/removing kprobes

NOTE
- An output of a previous run is displayed below around line 222







NOTE: When using a test program in userspace for interacting with a device the driver interfaces with, the userspace binary should be run with: sudo <bin>











Sample Tree Output

[31919.203177] Display Tree In Ascending Order
[31919.203178] key=0
[31919.203178] data=36
[31919.203179] key=1
[31919.203179] data=1
[31919.203180] key=2
[31919.203181] data=2
[31919.203181] key=3
[31919.203181] data=3
[31919.203182] key=4
[31919.203182] data=43
[31919.203183] key=5
[31919.203183] data=5
[31919.203184] key=6
[31919.203184] data=40
[31919.203185] key=7
[31919.203185] data=7
[31919.203186] key=8
[31919.203186] data=8
[31919.203187] key=9
[31919.203187] data=9
[31919.203188] key=10
[31919.203188] data=42
[31919.203189] key=11
[31919.203189] data=22
[31919.203190] key=12
[31919.203190] data=12
[31919.203191] key=13
[31919.203191] data=13
[31919.203191] key=14
[31919.203192] data=14
[31919.203192] key=15
[31919.203193] data=15
[31919.203193] key=16
[31919.203194] data=82
[31919.203194] key=17
[31919.203195] data=17
[31919.203195] key=18
[31919.203195] data=18
[31919.203196] key=19
[31919.203196] data=19
[31919.203197] key=20
[31919.203197] data=20
[31919.203198] key=21
[31919.203198] data=21
[31919.203198] key=22
[31919.203199] data=22
[31919.203199] key=23
[31919.203200] data=23
[31919.203200] key=24
[31919.203200] data=37
[31919.203201] key=25
[31919.203201] data=25
[31919.203202] key=26
[31919.203202] data=26
[31919.203203] key=27
[31919.203203] data=27
[31919.203203] key=28
[31919.203204] data=28
[31919.203204] key=29
[31919.203205] data=29
[31919.203205] key=30
[31919.203206] data=30
[31919.203206] key=31
[31919.203207] data=31
[31919.203207] key=32
[31919.203207] data=32
[31919.203208] key=33
[31919.203209] data=33
[31919.203209] key=34
[31919.203209] data=34
[31919.203210] key=35
[31919.203210] data=35
[31919.203211] key=36
[31919.203211] data=36
[31919.203212] key=37
[31919.203212] data=37
[31919.203212] key=38
[31919.203213] data=38
[31919.203213] key=39
[31919.203214] data=11
[31919.203214] key=40
[31919.203215] data=22
[31919.203215] key=41
[31919.203215] data=41
[31919.203216] key=42
[31919.203216] data=64
[31919.203217] key=43
[31919.203217] data=70
[31919.203218] key=44
[31919.203218] data=26
[31919.203219] key=45
[31919.203219] data=45
[31919.203219] key=46
[31919.203220] data=46
[31919.203220] key=47
[31919.203221] data=47
[31919.203221] key=48
[31919.203222] data=72
[31919.203222] key=49
[31919.203223] data=49
[31919.203223] key=50
[31919.203224] data=50
[31919.203224] key=60
[31919.203225] data=63
[31919.203225] key=67
[31919.203225] data=5
[31919.203226] key=71
[31919.203226] data=73
[31919.203227] key=76
[31919.203227] data=42
[31919.203228] key=84
[31919.203228] data=58
[31919.203229] key=86
[31919.203229] data=69
[31919.203230] key=95
[31919.203230] data=26
[31919.203231] key=99
[31919.203231] data=79








































	linux@linux-VirtualBox:~/CSE530/Linux_Kernel_Tree/Linux-Kernel-Red-Black-Tree-and-Dynamic-Probe$ sudo ./main
Check Kprobe Thread
Check Attr Policy
Made kprobe pthread
Initialize Thread_Populate_Tree2
In Kprobe Thread
ADDR Passed = 0
Initialize Thread_Populate_Tree1
Check First Thread
Check Attr Policy
Made first pthread
Check Second Thread
IN SIMULATE
fun_choice = 1
total ops rw = 0
fun_choice = 4
total ops rw = 0
IN SIMULATE
fun_choice = 3
total ops rw = 0
Validate Key Read From Tree = 50
Validate Data Read From Tree = 50
fun_choice = 1
total ops rw = 1
fun_choice = 5
total ops rw = 1
ADDR Passed = 0
fun_choice = 1
total ops rw = 1
fun_choice = 4
total ops rw = 1
fun_choice = 0
total ops rw = 1
fun_choice = 3
total ops rw = 1
Read from tree failed with code = -1
Node is Empty, Change Read Order
fun_choice = 2
total ops rw = 2
Sending to Kernel RBTree Write
fun_choice = 1
total ops rw = 3
fun_choice = 5
total ops rw = 3
fun_choice = 4
total ops rw = 3
fun_choice = 0
total ops rw = 3
fun_choice = 0
total ops rw = 3
fun_choice = 4
total ops rw = 3
fun_choice = 4
total ops rw = 3
fun_choice = 5
total ops rw = 3
fun_choice = 2
total ops rw = 3
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 4
Sending to Kernel RBTree Write
fun_choice = 1
total ops rw = 5
fun_choice = 1
total ops rw = 5
fun_choice = 1
total ops rw = 5
fun_choice = 5
total ops rw = 5
fun_choice = 0
total ops rw = 5
fun_choice = 0
total ops rw = 5
fun_choice = 0
total ops rw = 5
fun_choice = 5
total ops rw = 5
fun_choice = 3
total ops rw = 5
Validate Key Read From Tree = 67
Validate Data Read From Tree = 29
fun_choice = 1
total ops rw = 6
fun_choice = 4
total ops rw = 6
fun_choice = 1
total ops rw = 6
fun_choice = 4
total ops rw = 6
Addr = ffffffffc08f787c
PID = 28610
TSC = 89103994982167
PATH LENGTH = 7
Path Key = 16
Path Data = 16
Path Key = 24
Path Data = 24
Path Key = 32
Path Data = 32
Path Key = 40
Path Data = 40
Path Key = 44
Path Data = 44
Path Key = 46
Path Data = 46
Path Key = 48
Path Data = 48
fun_choice = 3
total ops rw = 6
Validate Key Read From Tree = 50
Validate Data Read From Tree = 50
fun_choice = 3
total ops rw = 7
Validate Key Read From Tree = 49
Validate Data Read From Tree = 49
fun_choice = 3
total ops rw = 8
Validate Key Read From Tree = 48
Validate Data Read From Tree = 48
fun_choice = 5
total ops rw = 9
fun_choice = 2
total ops rw = 9
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 10
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 11
fun_choice = 4
total ops rw = 11
fun_choice = 1
total ops rw = 11
fun_choice = 2
total ops rw = 11
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 12
Sending to Kernel RBTree Write
fun_choice = 1
total ops rw = 13
fun_choice = 3
total ops rw = 13
Validate Key Read From Tree = 47
Validate Data Read From Tree = 47
fun_choice = 3
total ops rw = 14
Validate Key Read From Tree = 46
Validate Data Read From Tree = 46
fun_choice = 2
total ops rw = 15
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 16
fun_choice = 4
total ops rw = 16
fun_choice = 3
total ops rw = 16
Validate Key Read From Tree = 45
Validate Data Read From Tree = 45
fun_choice = 1
total ops rw = 17
fun_choice = 5
total ops rw = 17
fun_choice = 0
total ops rw = 17
fun_choice = 3
total ops rw = 17
Validate Key Read From Tree = 46
Validate Data Read From Tree = 46
fun_choice = 0
total ops rw = 18
fun_choice = 3
total ops rw = 18
Validate Key Read From Tree = 47
Validate Data Read From Tree = 47
fun_choice = 2
total ops rw = 19
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 20
Sending to Kernel RBTree Write
fun_choice = 1
total ops rw = 21
fun_choice = 0
total ops rw = 21
fun_choice = 0
total ops rw = 21
fun_choice = 2
total ops rw = 21
Sending to Kernel RBTree Write
fun_choice = 3
total ops rw = 22
Validate Key Read From Tree = 48
Validate Data Read From Tree = 48
fun_choice = 5
total ops rw = 23
fun_choice = 2
total ops rw = 23
Sending to Kernel RBTree Write
fun_choice = 0
total ops rw = 24
fun_choice = 2
total ops rw = 24
Sending to Kernel RBTree Write
fun_choice = 4
total ops rw = 25
fun_choice = 4
total ops rw = 25
fun_choice = 0
total ops rw = 25
fun_choice = 0
total ops rw = 25
fun_choice = 5
total ops rw = 25
fun_choice = 3
total ops rw = 25
Validate Key Read From Tree = 49
Validate Data Read From Tree = 49
fun_choice = 4
total ops rw = 26
fun_choice = 2
total ops rw = 26
Sending to Kernel RBTree Write
fun_choice = 3
total ops rw = 27
Validate Key Read From Tree = 50
Validate Data Read From Tree = 50
fun_choice = 5
total ops rw = 28
fun_choice = 2
total ops rw = 28
Sending to Kernel RBTree Write
fun_choice = 4
total ops rw = 29
fun_choice = 2
total ops rw = 29
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 30
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 31
fun_choice = 4
total ops rw = 31
fun_choice = 5
total ops rw = 31
fun_choice = 4
total ops rw = 31
fun_choice = 3
total ops rw = 31
Validate Key Read From Tree = 56
Validate Data Read From Tree = 73
fun_choice = 3
total ops rw = 32
Validate Key Read From Tree = 67
Validate Data Read From Tree = 34
fun_choice = 1
total ops rw = 33
fun_choice = 3
total ops rw = 33
Validate Key Read From Tree = 56
Validate Data Read From Tree = 73
fun_choice = 5
total ops rw = 34
fun_choice = 3
total ops rw = 34
Validate Key Read From Tree = 50
Validate Data Read From Tree = 50
fun_choice = 5
total ops rw = 35
fun_choice = 3
total ops rw = 35
Validate Key Read From Tree = 49
Validate Data Read From Tree = 49
fun_choice = 1
total ops rw = 36
fun_choice = 3
total ops rw = 36
Validate Key Read From Tree = 48
Validate Data Read From Tree = 48
fun_choice = 4
total ops rw = 37
fun_choice = 5
total ops rw = 37
fun_choice = 3
total ops rw = 37
Validate Key Read From Tree = 47
Validate Data Read From Tree = 47
fun_choice = 0
total ops rw = 38
fun_choice = 5
total ops rw = 38
fun_choice = 4
total ops rw = 38
fun_choice = 5
total ops rw = 38
fun_choice = 5
total ops rw = 38
fun_choice = 2
total ops rw = 38
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 39
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 40
fun_choice = 0
total ops rw = 40
fun_choice = 0
total ops rw = 40
fun_choice = 4
total ops rw = 40
fun_choice = 5
total ops rw = 40
fun_choice = 4
total ops rw = 40
fun_choice = 3
total ops rw = 40
Validate Key Read From Tree = 48
Validate Data Read From Tree = 48
fun_choice = 0
total ops rw = 41
fun_choice = 5
total ops rw = 41
fun_choice = 2
total ops rw = 41
Sending to Kernel RBTree Write
fun_choice = 3
total ops rw = 42
Validate Key Read From Tree = 49
Validate Data Read From Tree = 49
fun_choice = 1
total ops rw = 43
fun_choice = 5
total ops rw = 43
fun_choice = 5
total ops rw = 43
fun_choice = 4
total ops rw = 43
fun_choice = 2
total ops rw = 43
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 44
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 45
Sending to Kernel RBTree Write
fun_choice = 4
total ops rw = 46
fun_choice = 5
total ops rw = 46
fun_choice = 2
total ops rw = 46
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 47
fun_choice = 1
total ops rw = 47
fun_choice = 3
total ops rw = 47
Validate Key Read From Tree = 48
Validate Data Read From Tree = 48
fun_choice = 4
total ops rw = 48
fun_choice = 4
total ops rw = 48
fun_choice = 5
total ops rw = 48
fun_choice = 5
total ops rw = 48
Addr = ffffffffc08f787c
PID = 28610
TSC = 89157113184987
PATH LENGTH = 7
Path Key = 16
Path Data = 16
Path Key = 32
Path Data = 32
Path Key = 40
Path Data = 40
Path Key = 44
Path Data = 44
Path Key = 48
Path Data = 48
Path Key = 67
Path Data = 34
Path Key = 50
Path Data = 50
fun_choice = 4
total ops rw = 48
fun_choice = 5
total ops rw = 48
fun_choice = 5
total ops rw = 48
fun_choice = 0
total ops rw = 48
fun_choice = 1
total ops rw = 48
fun_choice = 2
total ops rw = 48
Sending to Kernel RBTree Write
fun_choice = 1
total ops rw = 49
fun_choice = 3
total ops rw = 49
Validate Key Read From Tree = 47
Validate Data Read From Tree = 47
fun_choice = 1
total ops rw = 50
fun_choice = 2
total ops rw = 50
Simulated First Tree With Two Thread
Check Third Thread
Check Third Attr Policy
Made third pthread
Make fourth pthread
Created fourth thread
IN SIMULATE Second
fun_choice = 2
total ops rw = 0
Sending to Kernel RBTree Write
fun_choice = 0
total ops rw = 1
fun_choice = 3
total ops rw = 1
Validate Key Read From Tree = 1
Validate Data Read From Tree = 1
fun_choice = 2
total ops rw = 2
Sending to Kernel RBTree Write
fun_choice = 1
total ops rw = 3
fun_choice = 5
total ops rw = 3
IN SIMULATE Second
fun_choice = 2
total ops rw = 3
Sending to Kernel RBTree Write
fun_choice = 4
total ops rw = 4
fun_choice = 4
total ops rw = 4
fun_choice = 0
total ops rw = 4
fun_choice = 0
total ops rw = 4
fun_choice = 3
total ops rw = 4
Validate Key Read From Tree = 2
Validate Data Read From Tree = 2
fun_choice = 3
total ops rw = 5
Validate Key Read From Tree = 3
Validate Data Read From Tree = 3
fun_choice = 5
total ops rw = 6
Addr = ffffffffc08f787c
PID = 28620
TSC = 89171088627439
PATH LENGTH = 3
Path Key = 16
Path Data = 16
Path Key = 8
Path Data = 8
Path Key = 4
Path Data = 43
fun_choice = 0
total ops rw = 6
fun_choice = 2
total ops rw = 6
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 7
fun_choice = 2
total ops rw = 7
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 8
Addr = ffffffffc08f7396
PID = 28620
TSC = 89173884365439
PATH LENGTH = 4
Path Key = 16
Path Data = 16
Path Key = 8
Path Data = 8
Path Key = 4
Path Data = 43
Path Key = 2
Path Data = 2
ADDR Passed = 0
ADDR Passed = 0
Kprobe Thread Finished
fun_choice = 4
total ops rw = 8
fun_choice = 0
total ops rw = 8
fun_choice = 5
total ops rw = 8
fun_choice = 5
total ops rw = 8
fun_choice = 2
total ops rw = 8
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 9
Sending to Kernel RBTree Write
fun_choice = 1
total ops rw = 10
fun_choice = 3
total ops rw = 10
Validate Key Read From Tree = 2
Validate Data Read From Tree = 2
fun_choice = 3
total ops rw = 11
Validate Key Read From Tree = 1
Validate Data Read From Tree = 1
fun_choice = 4
total ops rw = 12
fun_choice = 2
total ops rw = 12
Sending to Kernel RBTree Write
fun_choice = 0
total ops rw = 13
fun_choice = 5
total ops rw = 13
fun_choice = 3
total ops rw = 13
Validate Key Read From Tree = 2
Validate Data Read From Tree = 2
fun_choice = 4
total ops rw = 14
fun_choice = 2
total ops rw = 14
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 15
fun_choice = 4
total ops rw = 15
fun_choice = 3
total ops rw = 15
Validate Key Read From Tree = 3
Validate Data Read From Tree = 3
fun_choice = 1
total ops rw = 16
fun_choice = 1
total ops rw = 16
fun_choice = 0
total ops rw = 16
fun_choice = 4
total ops rw = 16
fun_choice = 5
total ops rw = 16
fun_choice = 0
total ops rw = 16
fun_choice = 2
total ops rw = 16
Sending to Kernel RBTree Write
fun_choice = 0
total ops rw = 17
fun_choice = 5
total ops rw = 17
fun_choice = 2
total ops rw = 17
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 18
Sending to Kernel RBTree Write
fun_choice = 0
total ops rw = 19
fun_choice = 2
total ops rw = 19
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 20
Sending to Kernel RBTree Write
fun_choice = 4
total ops rw = 21
fun_choice = 2
total ops rw = 21
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 22
fun_choice = 3
total ops rw = 22
Validate Key Read From Tree = 4
Validate Data Read From Tree = 43
fun_choice = 4
total ops rw = 23
fun_choice = 1
total ops rw = 23
fun_choice = 2
total ops rw = 23
Sending to Kernel RBTree Write
fun_choice = 1
total ops rw = 24
fun_choice = 5
total ops rw = 24
fun_choice = 5
total ops rw = 24
fun_choice = 4
total ops rw = 24
fun_choice = 3
total ops rw = 24
Validate Key Read From Tree = 3
Validate Data Read From Tree = 3
fun_choice = 3
total ops rw = 25
Validate Key Read From Tree = 2
Validate Data Read From Tree = 2
fun_choice = 1
total ops rw = 26
fun_choice = 0
total ops rw = 26
fun_choice = 2
total ops rw = 26
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 27
Sending to Kernel RBTree Write
fun_choice = 4
total ops rw = 28
fun_choice = 2
total ops rw = 28
Sending to Kernel RBTree Write
fun_choice = 0
total ops rw = 29
fun_choice = 1
total ops rw = 29
fun_choice = 3
total ops rw = 29
Validate Key Read From Tree = 1
Validate Data Read From Tree = 1
fun_choice = 0
total ops rw = 30
fun_choice = 1
total ops rw = 30
fun_choice = 0
total ops rw = 30
fun_choice = 5
total ops rw = 30
fun_choice = 2
total ops rw = 30
Sending to Kernel RBTree Write
fun_choice = 3
total ops rw = 31
Validate Key Read From Tree = 2
Validate Data Read From Tree = 2
fun_choice = 3
total ops rw = 32
Validate Key Read From Tree = 3
Validate Data Read From Tree = 3
fun_choice = 2
total ops rw = 33
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 34
fun_choice = 0
total ops rw = 34
fun_choice = 3
total ops rw = 34
Validate Key Read From Tree = 4
Validate Data Read From Tree = 43
fun_choice = 1
total ops rw = 35
fun_choice = 0
total ops rw = 35
fun_choice = 1
total ops rw = 35
fun_choice = 1
total ops rw = 35
fun_choice = 5
total ops rw = 35
fun_choice = 1
total ops rw = 35
fun_choice = 3
total ops rw = 35
Validate Key Read From Tree = 3
Validate Data Read From Tree = 3
fun_choice = 2
total ops rw = 36
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 37
Sending to Kernel RBTree Write
fun_choice = 0
total ops rw = 38
fun_choice = 5
total ops rw = 38
fun_choice = 2
total ops rw = 38
Sending to Kernel RBTree Write
fun_choice = 0
total ops rw = 39
fun_choice = 4
total ops rw = 39
fun_choice = 4
total ops rw = 39
fun_choice = 5
total ops rw = 39
fun_choice = 2
total ops rw = 39
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 40
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 41
Sending to Kernel RBTree Write
fun_choice = 3
total ops rw = 42
Validate Key Read From Tree = 4
Validate Data Read From Tree = 43
fun_choice = 2
total ops rw = 43
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 44
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 45
Sending to Kernel RBTree Write
fun_choice = 2
total ops rw = 46
Sending to Kernel RBTree Write
fun_choice = 3
total ops rw = 47
Validate Key Read From Tree = 5
Validate Data Read From Tree = 5
fun_choice = 4
total ops rw = 48
fun_choice = 0
total ops rw = 48
fun_choice = 1
total ops rw = 48
fun_choice = 2
total ops rw = 48
Sending to Kernel RBTree Write
fun_choice = 5
total ops rw = 49
fun_choice = 3
total ops rw = 49
Validate Key Read From Tree = 4
Validate Data Read From Tree = 79
fun_choice = 2
total ops rw = 50
fun_choice = 0
total ops rw = 50
All pthreads joined
Write to kprobe driver
ADDR Passed = 0
Addr = ffffffffc08f7396
PID = 28606
TSC = 89215869110179
PATH LENGTH = 6
Path Key = 16
Path Data = 16
Path Key = 32
Path Data = 32
Path Key = 40
Path Data = 40
Path Key = 44
Path Data = 44
Path Key = 48
Path Data = 48
Path Key = 46
Path Data = 46
Closed All File Descriptors
