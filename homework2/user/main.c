#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 9999
#define NUMS 2

int main(void) {
    char test[BUFFER_SIZE] = "Hello_World!";
    char test2[BUFFER_SIZE] = "Today is bad day QQ...";
    char response[BUFFER_SIZE];
    char response2[BUFFER_SIZE];
    char fd[NUMS];
    int i;

    system("sudo rm -r /dev/mycdev_*");
    system("sudo mknod /dev/mycdev_1 c 42 0");
    system("sudo mknod /dev/mycdev_2 c 42 1");
    fd[0] = open("/dev/mycdev_1", O_RDWR);
    fd[1] = open("/dev/mycdev_2", O_RDWR);

    for (i = 0; i < NUMS; i++) if (*(fd + i) < 0) printf("FD[%hhd] not open\n", i);

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

    for (i = 0; i < NUMS; i++) close(fd[i]);
    return 0;
}