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

int main(int argc, char **argv)
{
	int fd, res;
	int buff_size = 4;
	char buff[buff_size];
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


	buff[0] = 1;
	buff[1] = 2;
	buff[2] = 3;
	buff[3] = 4;
	int sz = strlen(buff);
	printf("Buff Size = %d\n", sz);
	res = write(fd, buff, strlen(buff)+1);
	printf("WRITE\n");
		if(res == strlen(buff)+1){
			printf("Can not write to the device file.\n");		
			return 0;
		}	
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
