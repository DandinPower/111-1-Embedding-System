#include <stdio.h>
#include <stdlib.h>

int main(void){
    int fd = open("/dev/my_device", O_RDONLY);
    if (fd < 0) {
        /* handle error */
    }
    /* do work */
    //..
    close(fd);
    return 0;
}