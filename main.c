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

typedef struct pair {
	int key;
	int data;
} pair_t;

int main(int argc, char **argv)
{
	int fd, res;
	char buff[sizeof(struct pair)];
	int i = 0;
	/*
	if(argc == 1){
		return 0;
	}
	*/

	/* open devices */
	//fd = open("/dev/rbt530_dev", O_RDWR);
	fd = open("/dev/rbt530_dev", O_RDWR);
	if (fd < 0 ){
		fprintf(stderr, "fopen() failed: %s\n");
		printf("Can not open device file.\n");		
		return 0;
	}
	printf("File REACHED\n");


	
	printf("Buffer\n");
	/*
	for(int i = 0; i < strlen(buff); i++) {
		printf("%d\n", buff[i]);
	}
	*/
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

	/*
	printf("WRITE\n");
		if(res == strlen(buff)+1){
			printf("Can not write to the device file.\n");		
			return 0;
		}
		*/	
	
	printf("READ\n");
	res = read(fd, buff, sizeof(struct pair));
	printf("READ Result = %d\n", res);
	struct pair* validate;
	validate = (struct pair*) buff;
	printf("Struct Validate Key = %d", validate->key);
	printf("Struct Validate Data = %d", validate->data);

	res = read(fd, buff, sizeof(struct pair));
	printf("READ Result = %d\n", res);
	//struct pair* validate;
	validate = (struct pair*) buff;
	printf("Struct Validate Key = %d", validate->key);
	printf("Struct Validate Data = %d", validate->data);
		/*
	if(strcmp("show", argv[1]) == 0) {
			memset(buff, 0, 1024);
			res = read(fd, buff, 256);
			sleep(1);
			// printf("'%s'\n", buff);
	}
	*/
	
	close(fd);



	/*
	else{
		if(strcmp("show", argv[1]) == 0){
			memset(buff, 0, 1024);
			res = read(fd, buff, 256);
			sleep(1);
			// printf("'%s'\n", buff);
		}else if(strcmp("write", argv[1]) == 0){
			memset(buff, 0, 1024);
			if(argc >= 3){
				sprintf(buff,"%s", argv[2]);
				for(i = 3; i < argc; i++)
					sprintf(buff,"%s %s",buff,argv[i]);
			}
			//printf("'%s'\n", buff);
			res = write(fd, buff, strlen(buff)+1);
			if(res == strlen(buff)+1){
				printf("Can not write to the device file.\n");		
				return 0;
			}	
		}
		// close devices 
		close(fd);
	}
	*/
	return 0;
}
