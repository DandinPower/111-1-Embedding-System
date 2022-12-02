#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h>
#include <linux/fs.h> 
#include<linux/slab.h>
#include <linux/cdev.h>

#define DRIVER_BUFFER_SIZE 9999
#define MY_MAJOR       42
#define MY_MAX_MINORS  5

MODULE_DESCRIPTION("A Simple Character device Driver"); 
MODULE_AUTHOR("Yong-Cheng Liao<tomhot246@gmail.com>"); 
MODULE_LICENSE("GPL"); 

struct charIO_device_data {
	struct cdev cdev;
};

struct charIO_device_data devs[MY_MAX_MINORS];

//char* buffer;
int i, j, err;

/*
void allocate_buffer(char *str){
    buffer = (char *)kmalloc( DRIVER_BUFFER_SIZE, GFP_KERNEL);
    for (i=0; i<strlen(str); i++){
        buffer[i] = str[i];
    }
}*/

static int char_open(struct inode *inode, struct file *file)
{
	struct charIO_device_data *my_data;

    my_data = container_of(inode->i_cdev, struct charIO_device_data, cdev);

    file->private_data = my_data;
	return 0;
}

static ssize_t char_read(struct file *file, char __user *user_buffer,
					size_t size, loff_t *offset)
{
	struct charIO_device_data *my_data;

    my_data = (struct charIO_device_data *) file->private_data;
	return 0;
}

static ssize_t  char_write(struct file *file, const char __user *user_buffer,
					size_t size, loff_t * offset)
{
	return 0;
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
        /* report error */
        return err;
    }

    for(i = 0; i < MY_MAX_MINORS; i++) {
        /* initialize devs[i] fields */
        cdev_init(&devs[i].cdev, &charIO_fops);
        cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
    }

    printk("init successfully\n");
    return 0;
} 
 
static void charIO_exit(void) 
{ 
    for(i = 0; i < MY_MAX_MINORS; i++) {
        /* release devs[i] fields */
        cdev_del(&devs[i].cdev);
    }
    unregister_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS);
    printk("remove successfully\n");
} 
module_init(charIO_init); 
module_exit(charIO_exit); 