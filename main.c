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
// the kprobe buffer
typedef struct probe_data {
	void* addr;
	pid_t pid;
	unsigned long long tsc;
	struct path_to_cursor path;
} probe_data_t;
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
	
	return 0;
} 



// TREE
int write_to_tree(int tree_fd) {

	struct pair toDriver;
	//toDriver.key = randombytes_uniform(10);
	//toDriver.data = randombytes_uniform(10);
	toDriver.key = rand() % 10;
	toDriver.data = rand() % 10;
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







int simulate_reads() {
	return 0;

}





int main(int argc, char **argv)
{
	//int tree1_fd, res;
	//char buff[sizeof(struct pair)];
	int tree1_fd, tree2_fd, probe_fd;
	/*
	if(argc == 1){
		return 0;
	}
	*/

	/* open devices */
	//fd = open("/dev/rbt530_dev", O_RDWR);
	tree1_fd = open("/dev/rbt530_dev", O_RDWR);
	if (tree1_fd < 0 ){
		printf("Can not open device file.\n");		
		return 0;
	}
	/*
	tree2_fd = open("/dev/rbt530_dev2", O_RDWR);
	if (tree2_fd < 0 ){
		printf("Can not open device file.\n");		
		return 0;
	}
	*/

	probe_fd = open("/dev/RBprobe", O_RDWR);
	if (probe_fd < 0 ){
		printf("Can not open device file.\n");		
		return 0;
	}

	printf("Files REACHED\n");
	

	printf("write to tree first time\n");
	write_to_tree(tree1_fd);

	/*
	printf("Write to kprobe driver\n");
	write_to_kprobe_driver(probe_fd);
	//write_to_kprobe_driverRead(probe_fd);
	*/

	printf("read from tree\n");
	read_from_tree(tree1_fd);
	/*
	printf("Write to Tree 2nd Time\n");
	write_to_tree(tree1_fd);

	printf("Retrieving Data From kprobe\n");
	read_from_kprobe_driver(probe_fd);

	printf("Write to tree third time\n");
	write_to_tree(tree1_fd);

	printf("Remove the kprobe\n");
	write_to_kprobe_driver_remove(probe_fd);
	//write_to_kprobe_driver_removeRead(probe_fd);
	*/
	
	/*
	printf("Write to kprobe driver To Remove Kprobe\n");
	write_to_kprobe_driver_remove(probe_fd);
	*/
	

	/*
	
	struct pair toDriver;
	toDriver.key = 1;
	toDriver.data = 2;

	//int sz = strlen(buff);
	//printf("Buff Size = %d\n", sz);
	//res = write(fd, buff, strlen(buff)+1);
	printf("Key = %d\n", toDriver.key);
	printf("Data = %d\n", toDriver.data);
	printf("SizeStruct = %ld\n", sizeof(struct pair));
	// write one every time
	res = write(fd, &toDriver, sizeof(struct pair));

	toDriver.key = 3;
	toDriver.data = 4;

	res = write(fd, &toDriver, sizeof(struct pair));


	toDriver.key = 5;
	toDriver.data = 6;
	res = write(fd, &toDriver, sizeof(struct pair));
	*/
	//printf("Call ioctl\n");
	//change_tree_read_order(tree1_fd, 0);

	
	/*printf("READ\n");

	read_from_tree(tree1_fd);
	read_from_tree(tree1_fd);
	read_from_tree(tree1_fd);
	*/
	/*
	res = read(fd, buff, sizeof(struct pair));
	printf("READ Result = %d\n", res);
	struct pair* validate;
	validate = (struct pair*) buff;
	printf("Struct Validate Key = %d\n", validate->key);
	printf("Struct Validate Data = %d\n", validate->data);

	//struct pair* validate;

	res = read(fd, buff, sizeof(struct pair));
	printf("READ Result = %d\n", res);
	//struct pair* validate;
	validate = (struct pair*) buff;
	printf("Struct Validate Key = %d\n", validate->key);
	printf("Struct Validate Data = %d\n", validate->data);


	//validate = (struct pair*) buff;
	res = read(fd, buff, sizeof(struct pair));
	printf("Struct Validate Key = %d\n", validate->key);
	printf("Struct Validate Data = %d\n", validate->data);
	*/
		/*
	if(strcmp("show", argv[1]) == 0) {
			memset(buff, 0, 1024);
			res = read(fd, buff, 256);
			sleep(1);
			// printf("'%s'\n", buff);
	}
	*/
	printf("Close\n");
	close(tree1_fd);
	//close(tree2_fd);
	close(probe_fd);
	return 0;
}