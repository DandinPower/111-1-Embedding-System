#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10000                                                                    // buffer大小
#define NUMS 2                                                                               // 要開啟的device數量

int main(void) {
    char test[BUFFER_SIZE] = "Hello_World!";                                                 // 要寫進device的第一組測資
    char test2[BUFFER_SIZE] = "Today is bad day QQ...";                                      // 要寫進device的第二組測資
    char response[BUFFER_SIZE];                                                              // 從device讀出來的空間
    char response2[BUFFER_SIZE];                                                             // 從device讀出來的空間
    char fd[NUMS];                                                                           // 開啟device的char
    int i;

    system("sudo rm -r /dev/mycdev_*");                                                      // 將過去建立起來的device刪掉
    system("sudo mknod /dev/mycdev_1 c 42 0");                                               // 創建1個device
    system("sudo mknod /dev/mycdev_2 c 42 1");                                               // 創建1個device                              
    fd[0] = open("/dev/mycdev_1", O_RDWR);                                                   // 開啟device
    fd[1] = open("/dev/mycdev_2", O_RDWR);                                                   // 開啟device

    for (i = 0; i < NUMS; i++) if (*(fd + i) < 0) printf("FD[%hhd] not open\n", i);          // 檢查是否開啟成功

    write(fd[0], test, strlen(test)+1);                                                      // 將測資1寫進device1
    write(fd[1], test2, strlen(test2)+1);                                                    // 將測資2寫進device2
    read(fd[0], response, strlen(test)+1);                                                   // 把從device1讀出來的放到response
    read(fd[1], response2, strlen(test2)+1);                                                 // 把從device2讀出來的放到response2
    printf("%s\n", response);                                                                // 把response結果印出來
    printf("%s\n", response2);                                                               // 把response2結果印出來  

    for (i = 0; i < NUMS; i++) close(fd[i]);                                                 // 關掉device
    return 0;
}