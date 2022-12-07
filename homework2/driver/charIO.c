#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h>
#include <linux/fs.h> 
#include<linux/slab.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#define DRIVER_BUFFER_SIZE 9999
#define MY_MAJOR       42
#define MY_MAX_MINORS  5

MODULE_DESCRIPTION("A Simple Character device Driver"); 
MODULE_AUTHOR("Yong-Cheng Liao<tomhot246@gmail.com>"); 
MODULE_LICENSE("GPL"); 

typedef struct {
	struct cdev cdev;
    char *buffer;
    int size;
} charIO_device_data_t;

charIO_device_data_t devs[MY_MAX_MINORS];

//test
//char* buffer;
int i, j, err;

void allocate_buffer(charIO_device_data_t *dev){
    dev->size = DRIVER_BUFFER_SIZE;
    dev->buffer = (char *)kmalloc( dev->size, GFP_KERNEL);
}

void free_buffer(charIO_device_data_t *dev){
    kfree(dev->buffer);
}

static int char_open(struct inode *inode, struct file *file)
{
	charIO_device_data_t *my_data = container_of(inode->i_cdev, charIO_device_data_t, cdev);
    file->private_data = my_data;
    allocate_buffer(my_data);
	return 0;
}

static ssize_t char_read(struct file *file, char __user *user_buffer,
					size_t size, loff_t *offset)
{
    *offset = 0;
	charIO_device_data_t *my_data = file->private_data;
    ssize_t len = min(my_data->size - *offset, size);
    if (len <= 0)
        return 0;
    printk("read len:%d\n", len);
    printk("read offset:%d\n", *offset);
    printk("read:%s\n", my_data->buffer);
    if (copy_to_user(user_buffer, my_data->buffer + *offset, len))
        return -EFAULT;
    *offset += len;
    return len;
}

static ssize_t  char_write(struct file *file, const char __user *user_buffer,
					size_t size, loff_t * offset)
{
    charIO_device_data_t *my_data = file->private_data;
	ssize_t len = min(my_data->size - *offset, size);
    if (len <= 0)
        return 0;
    if (copy_from_user(my_data->buffer + *offset, user_buffer, len))
        return -EFAULT;
    *offset += len;
    return len;
}

static int  char_release(struct inode *inode , struct file *filp)
{
	return 0;
}

const struct file_operations charIO_fops = {
	.owner = THIS_MODULE,
	.open = char_open,
	.read = char_read,
	.write = char_write,
	.release = char_release,
};

static int charIO_init(void) 
{ 
    err = register_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS,
                                 "my_device_driver");
    if (err != 0) {
        return err;
    }

    for(i = 0; i < MY_MAX_MINORS; i++) {
        cdev_init(&devs[i].cdev, &charIO_fops);
        cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
    }

    printk("init successfully\n");
    return 0;
} 
 
static void charIO_exit(void) 
{ 
    for(i = 0; i < MY_MAX_MINORS; i++) {
        cdev_del(&devs[i].cdev);
    }
    unregister_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS);
    printk("remove successfully\n");
} 
module_init(charIO_init); 
module_exit(charIO_exit); 