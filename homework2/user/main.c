#include <linux/kernel.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BUFFER_SIZE 100

int main(void)
{

    char test[BUFFER_SIZE] = "Hello_World!";
	char test2[BUFFER_SIZE] = "Today is bad day QQ...";
    char response[BUFFER_SIZE];
	char response2[BUFFER_SIZE];
    char fd[5];
	int i;

    system("sudo rm -r /dev/mycdev_*");
	system("sudo mknod /dev/mycdev_1 c 42 0");
	fd[0] = open("/dev/mycdev_1", O_RDWR);
	system("sudo mknod /dev/mycdev_2 c 42 1");
	fd[1] = open("/dev/mycdev_2", O_RDWR);
	system("sudo mknod /dev/mycdev_3 c 42 2");
	fd[2] = open("/dev/mycdev_3", O_RDWR);
	system("sudo mknod /dev/mycdev_4 c 42 3");
	fd[3] = open("/dev/mycdev_4", O_RDWR);
	system("sudo mknod /dev/mycdev_5 c 42 4");
	fd[4] = open("/dev/mycdev_5", O_RDWR);
	
	for (i = 0; i < 5; i++) {
		if (*(fd + i) < 0) {
			/* handle error */
			printf("FD[%hhd] not open\n", i);
		}
	}

    write(fd[0], test, strlen(test)+1);
	write(fd[1], test2, strlen(test2)+1);
    read(fd[0], response, strlen(test)+1);
	read(fd[1], response2, strlen(test2)+1);
	printf("%s\n", response);
	printf("%s\n", response2);	
	write(fd[0], test2, strlen(test2)+1);
	write(fd[1], test, strlen(test)+1);
	read(fd[0], response, strlen(test2)+1);
	read(fd[1], response2, strlen(test)+1);
	printf("%s\n", response);
	printf("%s\n", response2);	
	for (i = 0; i < 5; i++) close(fd[i]);
	return 0;
}