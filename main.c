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
#include <sched.h>

#define MAX_RW_OPS 50

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
		fprintf(stderr, "Can not write to device file.\n");		
		exit(-1);

	}
	return 0;

}

int write_to_kprobe_driver(int probe_fd, int flag, unsigned int offset) {
	struct kprobe_pair kpair;


	kpair.flag = flag;

	kpair.offsetInsideFunction = offset;
	printf("ADDR Passed = %x\n", kpair.offsetInsideFunction);

	int kprobeWrite;
	kprobeWrite = write(probe_fd, &kpair, sizeof(struct kprobe_pair));
	if(kprobeWrite < 0) {
		fprintf(stderr, "Can not write to device file, error = %d\n", kprobeWrite);		
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
		fprintf(stderr, "Can not write to device file.\n");		
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
		fprintf(stderr, "Can not write to device file.\n");		
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
		fprintf(stderr, "Read from tree failed\n");
		exit(-1);
	}
	
	struct probe_data* fromKernel;
	fromKernel = (struct probe_data*) buff;
	printf("Addr = %lx\n",  fromKernel->addr);
	printf("PID = %d\n", fromKernel->pid);
	printf("TSC = %lld\n", fromKernel->tsc);
	printf("PATH LENGTH = %d\n", fromKernel->path.pathLength);
	int i = 0;
	i = fromKernel->path.pathLength;
	int j = 0;
	for(; j < i; j++) {
		printf("Path Key = %d\n", fromKernel->path.path[j].key);
		printf("Path Data = %d\n", fromKernel->path.path[j].data);
	}
	
	return 0;
} 



// Write to an RB device (rand generator inside)
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



// Write to an RB Device (provide key and data)
int write_pair_to_tree(int tree_fd, int key, int data) {

	struct pair toDriver;
	memset(&toDriver, 0, sizeof(struct pair));
	
	toDriver.key = key;
	toDriver.data = data;
	int res;
	//printf("Sending to Kernel Write\n");
	res = write(tree_fd, &toDriver, sizeof(struct pair));
	if(res == -1) {
		fprintf(stderr, "Write to tree failed with code = %d\n", res);
		exit(-1);;
	}
	return 0;
}







/*
int read_from_tree_iterative(int tree_fd) {
	char buff[sizeof(struct pair)];
	int res;
	res = read(tree_fd, buff, sizeof(struct pair));
	if(res < 0) {
		// change read order instead of exiting
		fprintf(stderr, "Read from tree failed with code = %d\n", res);
		return(-1);
	}
	struct pair* validate;
	validate = (struct pair*) buff;
	printf("Validate Key Read From Tree Display = %d\n", validate->key);
	printf("Validate Data Read From Tree Display = %d\n", validate->data);
	return 0;
}
*/



/*
int display_tree(int tree_fd) {
	printf("In Display Tree\n");
	int ret;
	ret = ioctl(tree_fd, 2);
	if(ret != 0) {
		fprintf(stderr, "Ioctl Command Did Not Work With Code = %d\n", ret);
		exit(-1);
	}
	int rest = 0;
	while(rest != -1) {
		rest = read_from_tree_iterative(tree_fd);
	}


	printf("Displayed Contents\n");
	return 0;
}
*/

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
		fprintf(stderr, "Ioctl Command Did Not Work With Code = %d\n", ret);
		exit(-1);
	}
	return 0;
}

// Read the key and data of the cursor node in the tree (for simulation purposes return -1 if error and change read direction)
int read_from_tree(int tree_fd) {
	char buff[sizeof(struct pair)];
	int res;
	res = read(tree_fd, buff, sizeof(struct pair));
	if(res < 0) {
		// change read order instead of exiting
		fprintf(stderr, "Read from tree failed with code = %d\n", res);
		return(-1);
	}
	struct pair* validate;
	validate = (struct pair*) buff;
	printf("Validate Key Read From Tree = %d\n", validate->key);
	printf("Validate Data Read From Tree = %d\n", validate->data);
	return 0;
}








/////////////////////////////////////////////////////////////////////////
// File descriptors for tree1, tree2, kprobe driver
int tree1_fd, tree2_fd, probe_fd;


// Populate the first tree
pthread_t Thread_Populate_Tree1;
pthread_attr_t Thread_Populate_Tree1_Attr;

// Populate the SECOND tree
pthread_t Thread_Populate_Tree2;
pthread_attr_t Thread_Populate_Tree2_Attr;

// Sum of read and write ops for each tree (should be protected by corresponding mutex since two threads will update value)
pthread_mutex_t lock_tree1_rw_ops;
int num_tree1_rw_ops;
pthread_mutex_t lock_tree2_rw_ops;
int num_tree2_rw_ops;




// Read and Write and IOCTL Random Ops In Tree 1
pthread_t Thread_RW_Tree1_First;
pthread_attr_t Thread_RW_Tree1_First_Attr;

pthread_t Thread_RW_Tree1_Second;
pthread_attr_t Thread_RW_Tree1_Second_Attr;
struct sched_param param_second;




// Read and Write and IOCTL Random Ops In TREE 2
pthread_t Thread_RW_Tree2_Third;
pthread_attr_t Thread_RW_Tree2_Third_Attr;

pthread_t Thread_RW_Tree2_Fourth;
pthread_attr_t Thread_RW_Tree2_Fourth_Attr;


// The kprobe thread directly controlled by a user
// Can read/write/ioctl in any tree
pthread_t Thread_KProbe;
pthread_attr_t Thread_KProbe_Attr;


// Ensure userspace program can connect and disconnect from drivers
void open_file_descriptors() {
	tree1_fd = open("/dev/rbt530_dev", O_RDWR);
	if (tree1_fd < 0 ){
		fprintf(stderr, "Can not open device file, error = %d\n", tree1_fd);		
		exit(-1);
	}
	
	tree2_fd = open("/dev/rbt530_dev2", O_RDWR);
	if (tree2_fd < 0 ){
		fprintf(stderr, "Can not open device file, error = %d\n", tree2_fd);		
		exit(-1);
	}
	
	probe_fd = open("/dev/RBprobe", O_RDWR);
	if (probe_fd < 0 ){
		fprintf(stderr, "Can not open device file, error = %d\n", probe_fd);		
		exit(-1);
	}
	return;
}


// Close connections to all used devices
int close_file_descriptors() {

	int ret = 0;

	ret = close(tree1_fd);
	if(ret != 0) {
		fprintf(stderr, "tree1 not closed correctly, error = %d\n", ret);
		exit(-1);
	}

	ret = close(tree2_fd);
	if(ret != 0) {
		fprintf(stderr, "tree2 not closed correctly, error = %d\n", ret);
		exit(-1);
	}

	ret = close(probe_fd);
	if(ret != 0) {
		fprintf(stderr, "probe_fd not closed correctly, error = %d\n", ret);
		exit(-1);
	}

	printf("Closed All File Descriptors\n");

	return 0;
}


// Ensure Locks Are Initialized and Deinitialized
void init_locks() {
	int verify_mutex_created = -1;
	verify_mutex_created = pthread_mutex_init(&lock_tree1_rw_ops, NULL);
	if(verify_mutex_created != 0) {
		fprintf(stderr, "First lock not created, error = %d\n", verify_mutex_created);
		exit(-1);
	}
	verify_mutex_created = pthread_mutex_init(&lock_tree2_rw_ops, NULL);
	if(verify_mutex_created != 0) {
		fprintf(stderr, "Second lock not created, error = %d\n", verify_mutex_created);
	}
	return;

}


// Clear mutex locks 
void clear_locks() {
	int verify_mutex_cleared = -1;
	verify_mutex_cleared = pthread_mutex_destroy(&lock_tree1_rw_ops);
	if(verify_mutex_cleared != 0) {
		fprintf(stderr, "First pthread mutext not cleared, error = %d\n", verify_mutex_cleared);
		exit(-1);
	}
	verify_mutex_cleared = pthread_mutex_destroy(&lock_tree2_rw_ops);
	if(verify_mutex_cleared != 0) {
		fprintf(stderr, "Second pthread mutex not cleared , error = %d\n", verify_mutex_cleared);
		exit(-1);
	}
	return;
}


// Thread fun for populating second tree
void* populate_second_tree() {
	printf("Initialize Thread_Populate_Tree2\n");

	int key = 1;
	for(; key < 50 + 1; key++) {
		// generate 50 key data pairs where the key=data
		int verify = 0;
		int data = key;
		
		verify = write_pair_to_tree(tree2_fd, key, data);
		if(verify != 0) {
			fprintf(stderr, "populate_second_tree failed with %d\n", verify);
			exit(-1);
		}
	}

	return (void*) 0;
}


// Thread fun for populating first tree
void* populate_first_tree() {
	printf("Initialize Thread_Populate_Tree1\n");
	int key = 1;
	for(; key < 50 + 1; key++) {
		// generate 50 key data pairs where the key=data
		int verify = 0;
		int data = key;

		verify = write_pair_to_tree(tree1_fd, key, data);
		if(verify != 0) {
			fprintf(stderr, "populate_first_tree failed with %d\n", verify);
			exit(-1);
		}
	}


	return (void*) 0;
}

/*
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

*/

/*
void* interact_with_probes() {
	char cmd;
	unsigned int offset;

	while(1) {
		printf("w = write\n");
		printf("r = read\n");
		printf("q = quit\n");

		scanf("%c", cmd);
		if(cmd == 'w') {
			printf("Provide Integer Flag \n");
			printf("1=rbtree_write, \n");
			printf("2=rbtree_read, \n");
			printf("-1 = remove probe at rbtree_write, \n");
			printf("-2 = remove probe at rbtree_read, \n");
			int flag = 0;
			scanf("%d", flag);
			printf("Input for Flag = %d\n", flag);
			printf("Provide Offset (Unsigned Int in Hex Format) \n");
			unsigned int offset = 0;
			scanf("%u", offset);
			printf("Input For Offset = %u\n", offset);
			write_to_kprobe_driver(probe_fd, flag, offset);
		} 
		else if(cmd == 'r') {
			printf("Provide Integer Flag \n");
			int read_flag = 0;
			scanf("%d", read_flag);
			read_from_kprobe_driver(probe_fd);
		} 
		else if(cmd == 'q') {
			return (void*) 0;
		}
		else {
			// repeat help message
		}

	}
	return (void*) 0;

}
*/


void* probes() {
	printf("In Kprobe Thread\n");
	write_to_kprobe_driver(probe_fd, 1, 0x0);
	write_to_kprobe_driverRead(probe_fd);
	sleep(5);
	read_from_kprobe_driver(probe_fd);
	sleep(20);
	read_from_kprobe_driver(probe_fd);
	sleep(4);
	read_from_kprobe_driver(probe_fd);
	sleep(1);
	read_from_kprobe_driver(probe_fd);

	// unregister
	write_to_kprobe_driver_removeRead(probe_fd);
	write_to_kprobe_driver_remove(probe_fd);

	printf("Kprobe Thread Finished\n");

	return (void*) 0;
}


void* simulate_RWI_Tree1() {
	printf("IN SIMULATE\n");
	
	int fun_choice = 0;
	//fun_choice = rand() % 5;

	while(1) {

		// protect the rand variable
		pthread_mutex_lock(&lock_tree1_rw_ops);
		fun_choice = rand() % 6;
		pthread_mutex_unlock(&lock_tree1_rw_ops);


		printf("fun_choice = %d\n", fun_choice);
		printf("total ops rw = %d\n", num_tree1_rw_ops);

		if(num_tree1_rw_ops >= MAX_RW_OPS) {
			//change_tree_read_order(tree1_fd, 2);
			return (void*) 0;
		}

		// IOCTL Read Ascending
		if(fun_choice == 0) {
			change_tree_read_order(tree1_fd, 0);
		} 

		// IOCTL READ DESCENDING
		else if(fun_choice == 1) {
			change_tree_read_order(tree1_fd, 1);
		} 
		
		// RbTree Write
		else if(fun_choice == 2) {
			// write tree 1
			write_to_tree(tree1_fd);

			pthread_mutex_lock(&lock_tree1_rw_ops);
			num_tree1_rw_ops += 1;
			pthread_mutex_unlock(&lock_tree1_rw_ops);
		}

		// RbTree Read
		else if(fun_choice == 3) {
			// read tree 1
			int chk_read = -1;
			chk_read = read_from_tree(tree1_fd);
			pthread_mutex_lock(&lock_tree1_rw_ops);
			num_tree1_rw_ops += 1;
			pthread_mutex_unlock(&lock_tree1_rw_ops);
			if(chk_read == -1) {
				printf("Node is Empty, Change Read Order\n");
				int j = 0;
				j = rand() % 2;
				if(j == 0) {
					change_tree_read_order(tree1_fd, 0);
				} else {
					change_tree_read_order(tree1_fd, 1);
				}
			}
		}
		
		// Sleep
		else {
			sleep(1);
		}
	}
	return (void*) 0;

}




void* simulate_RWI_Tree_Second() {
	printf("IN SIMULATE Second\n");
	
	int fun_choice = 0;
	//fun_choice = rand() % 5;

	while(1) {

		// protect the rand variable
		pthread_mutex_lock(&lock_tree2_rw_ops);
		fun_choice = rand() % 6;
		pthread_mutex_unlock(&lock_tree2_rw_ops);


		printf("fun_choice = %d\n", fun_choice);
		printf("total ops rw = %d\n", num_tree2_rw_ops);

		if(num_tree2_rw_ops >= MAX_RW_OPS) {
			//change_tree_read_order(tree1_fd, 2);
			return (void*) 0;
		}

		// IOCTL Read Ascending
		if(fun_choice == 0) {
			change_tree_read_order(tree2_fd, 0);
		} 

		// IOCTL READ DESCENDING
		else if(fun_choice == 1) {
			change_tree_read_order(tree2_fd, 1);
		} 
		
		// RbTree Write
		else if(fun_choice == 2) {
			// write tree 1
			write_to_tree(tree2_fd);

			pthread_mutex_lock(&lock_tree2_rw_ops);
			num_tree2_rw_ops += 1;
			pthread_mutex_unlock(&lock_tree2_rw_ops);
		}

		// RbTree Read
		else if(fun_choice == 3) {
			// read tree 1
			int chk_read = -1;
			chk_read = read_from_tree(tree2_fd);
			pthread_mutex_lock(&lock_tree2_rw_ops);
			num_tree2_rw_ops += 1;
			pthread_mutex_unlock(&lock_tree2_rw_ops);
			if(chk_read == -1) {
				printf("Node is Empty, Change Read Order\n");
				int j = 0;
				j = rand() % 2;
				if(j == 0) {
					change_tree_read_order(tree2_fd, 0);
				} else {
					change_tree_read_order(tree2_fd, 1);
				}
			}

			
		}
		
		// Sleep
		else {
			sleep(1);
		}
	}
	//printf("Print Tree\n");
	return (void*) 0;

}








int main(int argc, char **argv) {
	
	// Open Devices
	open_file_descriptors();

	// Initialize locks 
	init_locks();

	// Initialize current RW Ops for each tree
	num_tree1_rw_ops = 0;
	num_tree2_rw_ops = 0;

	
	// Populate first device
	int did_populate_first_tree = -1;
	did_populate_first_tree = pthread_create(&Thread_Populate_Tree1, NULL, populate_first_tree, NULL);

	if(did_populate_first_tree != 0) {
		fprintf(stderr, "First tree was not populated, error = %d\n", did_populate_first_tree);
		exit(-1);
	}

	/*
	pthread_t Thread_KProbe;
	pthread_attr_t Thread_KProbe_Attr;

	*/
	// Set up kprobe thread
	printf("Check Kprobe Thread\n");
	int kprobe_thd = -1;
	int verify_kprobe_attr = -1;
	
	verify_kprobe_attr = pthread_attr_init(&Thread_KProbe_Attr);
	if(verify_kprobe_attr != 0) {
		fprintf(stderr, "verify_kprobe_attr failed with code = %d\n", verify_kprobe_attr);
		exit(-1);
	}
	
	printf("Check Attr Policy\n");

	verify_kprobe_attr = pthread_attr_setschedpolicy(&Thread_KProbe_Attr, SCHED_FIFO);
	if(verify_kprobe_attr != 0) {
		fprintf(stderr, "verify_kprobe_attr with FIFO failed with code = %d\n", verify_kprobe_attr);
		exit(-1);
	}

	// Higher Priority Thread
	
	kprobe_thd = pthread_create(&Thread_KProbe, &Thread_KProbe_Attr, probes, NULL);
	if(kprobe_thd != 0) {
		fprintf(stderr, "kprobe_thd code = %d\n", kprobe_thd);
		exit(-1);
	}
	printf("Made kprobe pthread\n");




	/////////////////////////////////////////////////////////////////////////

	// Populate second device
	int did_populate_second_tree = -1;
	did_populate_second_tree = pthread_create(&Thread_Populate_Tree2, NULL, populate_second_tree, NULL);

	if(did_populate_second_tree != 0) {
		fprintf(stderr, "Second tree was not populated, error = %d\n", did_populate_second_tree);
		exit(-1);
	}

	// Join both populate threads
	pthread_join(Thread_Populate_Tree1, NULL);
	pthread_join(Thread_Populate_Tree2, NULL);

	//printf("Display Tree 1\n");
	//display_tree(tree1_fd);


	// Use random multithread operations on first tree
	/*
	pthread_t Thread_RW_Tree1_First;
	pthread_attr_t Thread_RW_Tree1_First_Attr;

	pthread_t Thread_RW_Tree1_Second;
	pthread_attr_t Thread_RW_Tree1_Second_Attr;

	*/
	printf("Check First Thread\n");
	int did_first_thread_rwi_tree = -1;
	int verify_first_attr = -1;
	
	verify_first_attr = pthread_attr_init(&Thread_RW_Tree1_First_Attr);
	if(verify_first_attr != 0) {
		fprintf(stderr, "verify_first_attr failed with code = %d\n", verify_first_attr);
		exit(-1);
	}
	
	printf("Check Attr Policy\n");

	verify_first_attr = pthread_attr_setschedpolicy(&Thread_RW_Tree1_First_Attr, SCHED_FIFO);
	if(verify_first_attr != 0) {
		fprintf(stderr, "verify_first_attr with FIFO failed with code = %d\n", verify_first_attr);
		exit(-1);
	}

	// Higher Priority Thread
	
	did_first_thread_rwi_tree = pthread_create(&Thread_RW_Tree1_First, &Thread_RW_Tree1_First_Attr, simulate_RWI_Tree1, NULL);
	if(did_first_thread_rwi_tree != 0) {
		fprintf(stderr, "did_first_thread_rwi_tree code = %d\n", did_first_thread_rwi_tree);
		exit(-1);
	}
	printf("Made first pthread\n");
	


	//////////////////////////////////////////////////////////////
	printf("Check Second Thread\n");
	// Set up the other thread

	int did_second_thread_rwi_tree = -1;
	int verify_second_attr = -1;



	// inherit scheduling attributes from the creating thread
	verify_second_attr = pthread_attr_setinheritsched(&Thread_RW_Tree1_Second_Attr, PTHREAD_INHERIT_SCHED);
	if(verify_second_attr != 0) {
		fprintf(stderr, "verify_second_attr inherit schedule failed with code = %d\n", verify_second_attr);
		exit(-1);
	}

	// set the new scheduling param
	/*
	param_second.sched_priority = 40;
	verify_second_attr = pthread_attr_setschedparam(&Thread_RW_Tree1_Second_Attr, &param_second);
	if(verify_second_attr != 0) {
		fprintf(stderr, "verify_second_attr for setschedparam failed with code = %d\n", verify_second_attr);
		exit(-1);
	}
	*/

	// Create Lower Priority Thread
	did_second_thread_rwi_tree = pthread_create(&Thread_RW_Tree1_Second, &Thread_RW_Tree1_Second_Attr, simulate_RWI_Tree1, NULL);
	if(did_second_thread_rwi_tree != 0) {
		fprintf(stderr, "did_second_thread_rwi_tree code = %d\n", did_second_thread_rwi_tree);
		exit(-1);
	}

	pthread_join(Thread_RW_Tree1_First, NULL);
	pthread_join(Thread_RW_Tree1_Second, NULL);

	printf("Simulated First Tree With Two Thread\n");




	// Create Thread For Second Tree
	/*
	simulate_RWI_Tree_Second

	// Read and Write and IOCTL Random Ops In TREE 2
	pthread_t Thread_RW_Tree2_Third;
	pthread_attr_t Thread_RW_Tree2_Third_Attr;

	*/
	
	printf("Check Third Thread\n");
	int did_third_thread = -1;
	int verify_third_attr = -1;
	
	verify_third_attr = pthread_attr_init(&Thread_RW_Tree2_Third_Attr);
	if(verify_third_attr != 0) {
		fprintf(stderr, "verify_third_attr failed with code = %d\n", verify_third_attr);
		exit(-1);
	}
	
	printf("Check Third Attr Policy\n");

	verify_third_attr = pthread_attr_setschedpolicy(&Thread_RW_Tree2_Third_Attr, SCHED_FIFO);
	if(verify_third_attr != 0) {
		fprintf(stderr, "verify_third_attr with FIFO failed with code = %d\n", verify_third_attr);
		exit(-1);
	}

	// Higher Priority Thread
	
	did_third_thread = pthread_create(&Thread_RW_Tree2_Third, &Thread_RW_Tree2_Third_Attr, simulate_RWI_Tree_Second, NULL);
	if(did_third_thread != 0) {
		fprintf(stderr, "did_third_thread code = %d\n", did_third_thread);
		exit(-1);
	}
	printf("Made third pthread\n");


	/////////////////////////////////////////////////////////////

	printf("Make fourth pthread\n");
	/*
	pthread_t Thread_RW_Tree2_Fourth;
	pthread_attr_t Thread_RW_Tree2_Fourth_Attr;

	*/


	int did_fourth_thread = -1;
	int verify_fourth_attr = -1;



	// inherit scheduling attributes from the creating thread
	verify_fourth_attr = pthread_attr_setinheritsched(&Thread_RW_Tree2_Fourth_Attr, PTHREAD_INHERIT_SCHED);
	if(verify_fourth_attr != 0) {
		fprintf(stderr, "verify_fourth_attr inherit schedule failed with code = %d\n", verify_fourth_attr);
		exit(-1);
	}


	// Create Lower Priority Thread
	did_fourth_thread = pthread_create(&Thread_RW_Tree2_Fourth, &Thread_RW_Tree2_Fourth_Attr, simulate_RWI_Tree_Second, NULL);
	if(did_fourth_thread != 0) {
		fprintf(stderr, "did_fourth_thread error code = %d\n", did_fourth_thread);
		exit(-1);
	}

	printf("Created fourth thread\n");


	// Join all remaining pthreads
	pthread_join(Thread_RW_Tree2_Third, NULL);
	pthread_join(Thread_RW_Tree2_Fourth, NULL);
	pthread_join(Thread_KProbe, NULL);

	printf("All pthreads joined\n");

	printf("Write to kprobe driver\n");

	write_to_kprobe_driver(probe_fd, 1, 0x0);
	write_pair_to_tree(tree1_fd, 200, 200);	
	read_from_kprobe_driver(probe_fd);

	// Clear mutexes
	clear_locks();
	
	return close_file_descriptors();
}











//printf("Remove the kprobe\n");
	//write_to_kprobe_driver_remove(probe_fd);
	//write_to_kprobe_driver_removeRead(probe_fd);



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