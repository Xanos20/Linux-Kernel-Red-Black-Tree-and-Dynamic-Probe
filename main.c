/*   A test program for /dev/kbuf
		To run the program, enter "kbuf_tester show" to show the current contend of the buffer.
				enter "kbuf_tester write <input_string> to append the <input_string> into the buffer

*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <linux/ioctl.h>
#include <linux/rtc.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
//#include <linux/kprobes.h>
//#include <sodium.h>
#include <sys/ioctl.h>
#include <pthread.h>



// for generating the path to the current node in the rb driver
struct keydata {
	int key;
	int data;

};
struct path_to_cursor {
	int pathLength;
	struct keydata path[10];
};

// the kprobe buffer
typedef struct probe_data {
	unsigned long addr;
	pid_t pid;
	unsigned long long tsc;
	struct path_to_cursor path;
} probe_data_t;





typedef struct pair {
	int key;
	int data;
} pair_t;



typedef struct kprobe_pair {
	int flag;
	unsigned int offsetInsideFunction;

} kprobe_pair_t;


int write_to_kprobe_driver_remove(int probe_fd) {
	struct kprobe_pair kpair;
	kpair.flag = -1;

	//00000000000004d0 ffffffff9e684690
	//0x00000500
	kpair.offsetInsideFunction = 0x0;
	printf("ADDR Passed = %x\n", kpair.offsetInsideFunction);

	int kprobeWrite;
	kprobeWrite = write(probe_fd, &kpair, sizeof(struct kprobe_pair));
	if(kprobeWrite < 0) {
		printf("Can not write to device file.\n");		
		exit(-1);

	}
	return 0;

}

int write_to_kprobe_driver(int probe_fd) {
	struct kprobe_pair kpair;
	kpair.flag = 1;

	//00000000000004d0 ffffffff9e684690
	//0x00000500

	kpair.offsetInsideFunction = 0x0;
	printf("ADDR Passed = %x\n", kpair.offsetInsideFunction);

	int kprobeWrite;
	kprobeWrite = write(probe_fd, &kpair, sizeof(struct kprobe_pair));
	if(kprobeWrite < 0) {
		printf("Can not write to device file.\n");		
		exit(-1);

	}
	return 0;

}














int write_to_kprobe_driver_removeRead(int probe_fd) {
	struct kprobe_pair kpair;
	kpair.flag = -2;

	//00000000000004d0 ffffffff9e684690
	//0x00000500
	kpair.offsetInsideFunction = 0x0;
	printf("ADDR Passed = %x\n", kpair.offsetInsideFunction);

	int kprobeWrite;
	kprobeWrite = write(probe_fd, &kpair, sizeof(struct kprobe_pair));
	if(kprobeWrite < 0) {
		printf("Can not write to device file.\n");		
		exit(-1);

	}
	return 0;

}



int write_to_kprobe_driverRead(int probe_fd) {
	struct kprobe_pair kpair;
	kpair.flag = 2;

	//00000000000004d0 ffffffff9e684690
	//0x00000500

	kpair.offsetInsideFunction = 0x0;
	printf("ADDR Passed = %x\n", kpair.offsetInsideFunction);

	int kprobeWrite;
	kprobeWrite = write(probe_fd, &kpair, sizeof(struct kprobe_pair));
	if(kprobeWrite < 0) {
		printf("Can not write to device file.\n");		
		exit(-1);

	}
	return 0;

}



/*

struct keydata {
	int key;
	int data;

};

// the kprobe buffer
typedef struct probe_data {
	void* addr;
	pid_t pid;
	unsigned long long tsc;
	struct path_to_cursor path;
} probe_data_t;

struct path_to_cursor {
	int pathLength;
	struct keydata path[10];
};
*/


int read_from_kprobe_driver(int probe_fd) {


	char buff[sizeof(struct probe_data)];
	memset(buff, 0, sizeof(struct probe_data));
	
	int res;
	res = read(probe_fd, buff, sizeof(struct probe_data));
	if(res < 0) {
		printf("Read from tree failed\n");
		return -1;
	}
	
	struct probe_data* fromKernel;
	fromKernel = (struct probe_data*) buff;
	printf("Addr = %lx\n",  fromKernel->addr);
	printf("PID = %d\n", fromKernel->pid);
	printf("TSC = %lld\n", fromKernel->tsc);
	printf("PATH LENGTH = %d\n", fromKernel->path.pathLength);
	int i = 0;
	i = fromKernel->path.pathLength;
	for(int j = 0; j < i; j++) {
		printf("Path Key = %d\n", fromKernel->path.path[j].key);
		printf("Path Data = %d\n", fromKernel->path.path[j].data);
	}
	
	return 0;
} 



// TREE
int write_to_tree(int tree_fd) {

	struct pair toDriver;
	//toDriver.key = randombytes_uniform(10);
	//toDriver.data = randombytes_uniform(10);
	toDriver.key = rand() % 100;
	toDriver.data = rand() % 100;
	int res = 0;
	printf("Sending to Kernel RBTree Write\n");
	res = write(tree_fd, &toDriver, sizeof(struct pair));
	if(res == -1) {
		printf("Write to tree failed\n");
		return -1;
	}
	return 0;
}

// TREE
int write_to_tree_hardcode2(int tree_fd) {

	struct pair toDriver;
	//toDriver.key = randombytes_uniform(10);
	//toDriver.data = randombytes_uniform(10);
	toDriver.key = 2;
	toDriver.data = 3;
	int res = 0;
	printf("Sending to Kernel Write\n");
	res = write(tree_fd, &toDriver, sizeof(struct pair));
	if(res == -1) {
		printf("Write to tree failed\n");
		return -1;
	}
	return 0;
}

int write_to_tree_hardcode4(int tree_fd) {

	struct pair toDriver;
	//toDriver.key = randombytes_uniform(10);
	//toDriver.data = randombytes_uniform(10);
	toDriver.key = 4;
	toDriver.data = 5;
	int res;
	printf("Sending to Kernel Write\n");
	res = write(tree_fd, &toDriver, sizeof(struct pair));
	if(res == -1) {
		printf("Write to tree failed\n");
		return -1;
	}
	return 0;
}


int write_to_tree_hardcode6(int tree_fd) {

	struct pair toDriver;
	//toDriver.key = randombytes_uniform(10);
	//toDriver.data = randombytes_uniform(10);
	toDriver.key = 6;
	toDriver.data = 7;
	int res;
	printf("Sending to Kernel Write\n");
	res = write(tree_fd, &toDriver, sizeof(struct pair));
	if(res == -1) {
		printf("Write to tree failed\n");
		return -1;
	}
	return 0;
}



int write_to_tree_hardcode8(int tree_fd) {

	struct pair toDriver;
	//toDriver.key = randombytes_uniform(10);
	//toDriver.data = randombytes_uniform(10);
	toDriver.key = 8;
	toDriver.data = 9;
	int res;
	printf("Sending to Kernel Write\n");
	res = write(tree_fd, &toDriver, sizeof(struct pair));
	if(res == -1) {
		printf("Write to tree failed\n");
		return -1;
	}
	return 0;
}



int write_pair_to_tree(int tree_fd, int key, int data) {

	struct pair toDriver;
	memset(&toDriver, 0, sizeof(struct pair));
	
	toDriver.key = key;
	toDriver.data = data;
	int res;
	printf("Sending to Kernel Write\n");
	res = write(tree_fd, &toDriver, sizeof(struct pair));
	if(res == -1) {
		printf("Write to tree failed\n");
		return -1;
	}
	return 0;
}



/*
change_tree_read_order: 
The command “read_order” to set up ascending or descending order that objects are to be 
read

If read_order is 0, read calls retrieve the objects in ascending order starting from the object 
with  the  minimal  key. 

If read_order  is  1,  read  calls  get  the  objects  in  descending  order  star
ting  from  the object with the maximal key. 

Otherwise, -1 is returned in the kernel and errno is set to EINVAL.

*/
int change_tree_read_order(int tree_fd, int read_order) {
	int ret;
	ret = ioctl(tree_fd, read_order);
	if(ret != 0) {
		printf("Ioctl Command Did Not Work\n");
		return -1;
	}
	return 0;
}


int read_from_tree(int tree_fd) {
	char buff[sizeof(struct pair)];
	int res;
	res = read(tree_fd, buff, sizeof(struct pair));
	if(res < 0) {
		printf("Read from tree failed\n");
		return -1;
	}
	struct pair* validate;
	validate = (struct pair*) buff;
	printf("Validate Key Read From Tree = %d\n", validate->key);
	printf("Validate Data Read From Tree = %d\n", validate->data);
	return 0;
}



// File descriptors for tree1, tree2, kprobe driver
int tree1_fd, tree2_fd, probe_fd;



// Populate the first tree
pthread_t Thread_Populate_Tree1;
pthread_attr_t Thread_Populate_Tree1_Attr;

// Populate the SECOND tree
pthread_t Thread_Populate_Tree2;
pthread_attr_t Thread_Populate_Tree2_Attr;


// Read and Write and IOCTL Random Ops In Tree 1
pthread_t Thread_RW_Tree1_First;
pthread_attr_t Thread_RW_Tree1_First_Attr;

pthread_t Thread_RW_Tree1_Second;
pthread_attr_t Thread_RW_Tree1_Second_Attr;

// Read and Write and IOCTL Random Ops In TREE 2
pthread_t Thread_RW_Tree2_Third;
pthread_attr_t Thread_RW_Tree2_Third_Attr;

pthread_t Thread_RW_Tree2_Fourth;
pthread_attr_t Thread_RW_Tree2_Fourth_Attr;

// The kprobe thread directly controlled by a user
// Can read/write/ioctl in any tree
pthread_t Thread_Usr_Controlled_Probe;
pthread_attr_t Thread_Usr_Controlled_Probe_Attr;




void* T1simulate() {
	int fun_choice = 0;
	fun_choice = rand() % 13;

	// IOCTL Read Ascending
	if(fun_choice == 0) {
		change_tree_read_order(tree1_fd, 0);
	} 
	else if(fun_choice == 1) {
		change_tree_read_order(tree2_fd, 0);
	}

	// IOCTL READ DESCENDING
	else if(fun_choice == 2) {
		change_tree_read_order(tree1_fd, 1);
	} 
	else if(fun_choice == 3) {
		change_tree_read_order(tree2_fd, 1);
	}

	// RbTree Write
	else if(fun_choice == 4) {
		write_to_tree(tree1_fd);
	}
	else if(fun_choice == 5) {
		write_to_tree(tree2_fd);
	}

	// Read
	else if(fun_choice == 6) {
		read_from_tree(tree1_fd);
	}
	else if(fun_choice == 7) {
		read_from_tree(tree2_fd);
	}

	// Kprobe Write Write
	else if(fun_choice == 8) {
		write_to_kprobe_driver(probe_fd);
	}
	else if(fun_choice == 9) {
		write_to_kprobe_driver_remove(probe_fd);
	} 

	// Kprobe Read Read
	else if(fun_choice == 10) {
		write_to_kprobe_driverRead(probe_fd);
	} 
	else if(fun_choice == 11) {
		write_to_kprobe_driver_removeRead(probe_fd);
	}

	// Sleep
	else {
		sleep(1);
	}
	/*
	printf("Write to kprobe driver\n");
	write_to_kprobe_driver(probe_fd);
	write_to_tree_hardcode2(tree1_fd);
	write_to_tree_hardcode4(tree1_fd);
	read_from_tree(tree1_fd);
	read_from_kprobe_driver(probe_fd);
	write_to_tree_hardcode6(tree1_fd);
	read_from_tree(tree1_fd);
	write_to_tree_hardcode8(tree1_fd);
	read_from_kprobe_driver(probe_fd);
	*/
	

}

void* T2simulate() {

	int fun_choice = 0;
	fun_choice = rand() % 13;

	// IOCTL Read Ascending
	if(fun_choice == 0) {
		change_tree_read_order(tree1_fd, 0);
	} 
	else if(fun_choice == 1) {
		change_tree_read_order(tree2_fd, 0);
	}

	// IOCTL READ DESCENDING
	else if(fun_choice == 2) {
		change_tree_read_order(tree1_fd, 1);
	} 
	else if(fun_choice == 3) {
		change_tree_read_order(tree2_fd, 1);
	}

	// RbTree Write
	else if(fun_choice == 4) {
		write_to_tree(tree1_fd);
	}
	else if(fun_choice == 5) {
		write_to_tree(tree2_fd);
	}

	// Read
	else if(fun_choice == 6) {
		read_from_tree(tree1_fd);
	}
	else if(fun_choice == 7) {
		read_from_tree(tree2_fd);
	}

	// Kprobe Write Write
	else if(fun_choice == 8) {
		write_to_kprobe_driver(probe_fd);
	}
	else if(fun_choice == 9) {
		write_to_kprobe_driver_remove(probe_fd);
	} 

	// Kprobe Read Read
	else if(fun_choice == 10) {
		write_to_kprobe_driverRead(probe_fd);
	} 
	else if(fun_choice == 11) {
		write_to_kprobe_driver_removeRead(probe_fd);
	}

	// Sleep
	else {
		sleep(1);
	}

	/*
	printf("Write to kprobe driver\n");
	write_to_kprobe_driver(probe_fd);
	write_to_tree_hardcode2(tree1_fd);
	write_to_tree_hardcode4(tree1_fd);
	read_from_tree(tree1_fd);
	read_from_kprobe_driver(probe_fd);
	write_to_tree_hardcode6(tree1_fd);
	read_from_tree(tree1_fd);
	write_to_tree_hardcode8(tree1_fd);
	read_from_kprobe_driver(probe_fd);
	*/
	

}

int open_file_descriptors() {
	tree1_fd = open("/dev/rbt530_dev", O_RDWR);
	if (tree1_fd < 0 ){
		printf("Can not open device file.\n");		
		return -1;
	}
	
	tree2_fd = open("/dev/rbt530_dev2", O_RDWR);
	if (tree2_fd < 0 ){
		printf("Can not open device file.\n");		
		return -1;
	}
	
	probe_fd = open("/dev/RBprobe", O_RDWR);
	if (probe_fd < 0 ){
		printf("Can not open device file.\n");		
		return -1;
	}
	return 0;
}

int close_file_descriptors() {
	printf("Close All File Descriptors\n");

	int ret = 0;

	ret = close(tree1_fd);
	if(ret != 0) {
		return ret;
	}

	ret = close(tree2_fd);
	if(ret != 0) {
		return ret;
	}

	ret = close(probe_fd);
	if(ret != 0) {
		return ret;
	}

	return 0;
}



void* populate_second_tree() {
	printf("Initialize Thread_Populate_Tree2\n");

	for(int key = 1; key < 50 + 1; key++) {
		// generate 50 key data pairs where the key=data
		int verify = 0;
		int data = key;
		
		verify = write_pair_to_tree(tree2_fd, key, data);
		if(verify != 0) {
			exit(-1);
		}
	}

	return (void*) 0;
}

void* populate_second_tree_finished() {
	printf("Second Tree Populated\n");
	return (void*) 0;
}


void* populate_first_tree() {
	printf("Initialize Thread_Populate_Tree1\n");

	for(int key = 1; key < 50 + 1; key++) {
		// generate 50 key data pairs where the key=data
		int verify = 0;
		int data = key;

		verify = write_pair_to_tree(tree1_fd, key, data);
		if(verify != 0) {
			exit(-1);
		}
	}

	return (void*) 0;
}

void* populate_first_tree_finished() {
	printf("First Tree Populated\n");
	return (void*) 0;
}




int main(int argc, char **argv)
{
	
	// Open Devices
	int open_verify = -1;
	open_verify = open_file_descriptors();
	if(open_verify != 0) {
		exit(-1);
	}

	printf("Files REACHED\n");

	/*
	int did_populate_first_tree = -1;
	did_populate_first_tree = pthread_create(&Thread_Populate_Tree1, NULL, populate_first_tree, populate_first_tree_finished);

	if(did_populate_first_tree != 0) {
		printf("A tree was not populated\n");
		exit(-1);
	}

	pthread_join(Thread_Populate_Tree1, NULL);
	*/


	int did_populate_second_tree = -1;
	did_populate_second_tree = pthread_create(&Thread_Populate_Tree2, NULL, populate_second_tree, populate_second_tree_finished);

	if(did_populate_second_tree != 0) {
		printf("Second tree was not populated\n");
		exit(-1);
	}
	pthread_join(Thread_Populate_Tree2, NULL);
	

	/*
	printf("Write to kprobe driver\n");
	write_to_kprobe_driver(probe_fd);
	write_to_tree_hardcode2(tree1_fd);
	write_to_tree_hardcode4(tree1_fd);
	read_from_tree(tree1_fd);
	read_from_kprobe_driver(probe_fd);
	write_to_tree_hardcode6(tree1_fd);
	read_from_tree(tree1_fd);
	write_to_tree_hardcode8(tree1_fd);
	read_from_kprobe_driver(probe_fd);
	
	
	printf("write to tree first time\n");
	write_to_tree(tree1_fd);

	
	printf("Write to kprobe driver\n");
	write_to_kprobe_driver(probe_fd);
	//write_to_kprobe_driverRead(probe_fd);
	

	printf("read from tree\n");
	read_from_tree(tree1_fd);
	
	printf("Write to Tree 2nd Time\n");
	write_to_tree(tree1_fd);

	printf("Retrieving Data From kprobe\n");
	read_from_kprobe_driver(probe_fd);

	printf("Write to tree third time\n");
	write_to_tree(tree1_fd);
	*/

	//printf("Remove the kprobe\n");
	//write_to_kprobe_driver_remove(probe_fd);
	//write_to_kprobe_driver_removeRead(probe_fd);
	
	
	return close_file_descriptors();
}