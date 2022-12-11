#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h>
#include <linux/fs.h> 
#include<linux/slab.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#define DRIVER_BUFFER_SIZE 10000                                                             // buffer的大小
#define MY_MAJOR       42                                                                    // driver的major number
#define MY_MAX_MINORS  2                                                                     // driver可以被開的minors number
                                                                                             
MODULE_DESCRIPTION("A Simple Character device Driver"); 
MODULE_AUTHOR("Yong-Cheng Liao<tomhot246@gmail.com>"); 
MODULE_LICENSE("GPL"); 

// 宣告一個結構來儲存character device以及kernel裡的buffer空間以及buffer的長度
typedef struct {
    struct cdev cdev;
    char *buffer;
    int size;
} charIO_device_data_t;

charIO_device_data_t devs[MY_MAX_MINORS];                                                    // 由於會有MY_MAX_MINORS數量的cdev可能會被開起來, 所以需要宣告MY_MAX_MINORS個物件出來
int i, err;

// 實作了open的system call
static int char_open(struct inode *inode, struct file *file) {
    charIO_device_data_t *my_data = container_of(inode->i_cdev, charIO_device_data_t, cdev); // 將inode裡的cdev拿出來放到我們上面宣告的結構裡
    file->private_data = my_data;                                                            // 由於open會拿到一個file物件 所以再將剛剛創建出來的結構物件放到file裡
    my_data->size = DRIVER_BUFFER_SIZE;
    my_data->buffer = (char *)kmalloc( my_data->size, GFP_KERNEL);                           // 宣告kernel buffer的記憶體空間
    return 0;
}

// 實作了read的system call
static ssize_t char_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    *offset = 0;                                                                             // 使其從kernel buffer的一開始開始讀
    charIO_device_data_t *my_data = file->private_data;                                      // 從傳進來的file找到之前使用的device結構物件
    ssize_t len = min(my_data->size - *offset, size);                                        // 計算可以讀取的長度
    if (len <= 0) return 0;                                                                  // 如果不超過0就可以直接回傳
    if (copy_to_user(user_buffer, my_data->buffer + *offset, len)) return -EFAULT;           // 其他時候就將kernel buffer裡的值根據長度及offset寫回到user buufer裡
    *offset += len;                                                                          // offfset紀錄讀取的長度
    return len;
}

// 實作了write的system call
static ssize_t  char_write(struct file *file, const char __user *user_buffer, size_t size, loff_t * offset) {
    *offset = 0;                                                                             // 使其從kernel buffer的一開始開始寫
    charIO_device_data_t *my_data = file->private_data;                                      // 從傳進來的file找到之前使用的device結構物件
    ssize_t len = min(my_data->size - *offset, size);                                        // 計算可以寫入的長度
    if (len <= 0) return 0;                                                                  // 如果不超過0就可以直接回傳
    if (copy_from_user(my_data->buffer + *offset, user_buffer, len)) return -EFAULT;         // 其他時候就將user buufer裡的值根據長度及offset寫到kernel buffer裡
    *offset += len;                                                                          // offfset紀錄讀取的長度
    return len;
}

// 實作了release的system call
static int  char_release(struct inode *inode , struct file *filp){
    charIO_device_data_t *my_data = filp->private_data;                                      // 從傳進來的file找到之前使用的device結構物件
    kfree(my_data->buffer);                                                                  // 清除掉kernel的buffer空間
    return 0;
}

// 實作了character device需要的幾個function並透過function pointer的方式來找到
const struct file_operations charIO_fops = {
    .owner = THIS_MODULE,
    .open = char_open,
    .read = char_read,
    .write = char_write,
    .release = char_release,
};

// kernel module init的funciton
static int charIO_init(void) { 
    err = register_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS,"charIO");                // 跟kernel註冊character device
    if (err != 0) return err;                                                                // 發生錯誤回傳
    for(i = 0; i < MY_MAX_MINORS; i++) {                                                     // traverse所有device結構物件
        cdev_init(&devs[i].cdev, &charIO_fops);                                              // 初始化結構物件
        cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);                                      // 將結構物件註冊到major number跟minors number上
    }
    return 0;
} 
 
// kernel module exit的funciton
static void charIO_exit(void) { 
    for(i = 0; i < MY_MAX_MINORS; i++) cdev_del(&devs[i].cdev);                              // 刪除cdev
    unregister_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS);                             // 解除major number跟minors number的註冊
} 
module_init(charIO_init); 
module_exit(charIO_exit); 