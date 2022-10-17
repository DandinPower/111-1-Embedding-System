#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/slab.h> 
#include <linux/unistd.h> 
#include <linux/sched.h> 
#include <linux/fs.h> 
#include <linux/file.h> 
#include <linux/mm.h> 
#include <asm/uaccess.h> 
 
MODULE_DESCRIPTION("read file in and write it out in inverse"); 
MODULE_AUTHOR("Yong-Cheng Liao<tomhot246@gmail.com>"); 
MODULE_ALIAS("read file & write file module");
MODULE_LICENSE("GPL"); 

int read_file(void){
    struct file *io;
    void *buf;
    size_t count;
    loff_t pos = 0;
    ssize_t rx;
    char tmp_char[100];
    memset(tmp_char, 0, sizeof(tmp_char));

    io = filp_open("input.txt", O_GREAT | O_RDONLY, 0);
    if (IS_ERR(io)){
        printk("open file error/n");
        return -1;
    }
    count = sizeof(tmp_char);
    buf = (void *)(&tmp_char);
    rx = kernel_read(io, buf, count, &pos);
    printk("%s, read result: tmp_char=%s\n", __func__, tmp_char);
    filp_close(io, NULL);
    return 0;
}

int write_file(void){
    struct file *io;
    void *buf;
    size_t count;
    loff_t pos = 0;
    ssize_t tx;
    char *tmp_char = "hello world";
    //memset(tmp_char, 0, sizeof(tmp_char));

    io = filp_open("output.txt", O_GREAT | O_RDWR, 0664);
    if (IS_ERR(io)){
        printk("open file error/n");
        return -1;
    }
    count = strlen(tmp_char) + 1;
    buf = (void *)(&tmp_char);
    tx = kernel_write(io, buf, count, &pos);
    //printk("%s, read result: tmp_char=%s\n", __func__, tmp_char);
    filp_close(io, NULL);
    return 0;
}

static int fileIO_init(void) 
{ 
    /*
    io = filp_open("output.txt", O_RDWR, 0664);
    if (IS_ERR(io)){
        printk("create file error/n");
        return -1;
    }
    */
    //int status = read_file();
    return write_file();
} 
 
static void fileIO_exit(void) 
{ 
    printk("remove successfully\n");
} 
module_init(fileIO_init); 
module_exit(fileIO_exit); 