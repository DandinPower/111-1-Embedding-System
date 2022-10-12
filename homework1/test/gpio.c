#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/string.h>
#define MAJOR_NUM 60
#define MODULE_NAME "demo"
static char userChar[100];
static int iCount = 0;

static ssize_t drv_read(struct file *filp, char *buf, size_t count, loff_t *ppos)
{
    printk("device read\n");
    printk("%d\n", iCount);
    copy_from_user(userChar, buf, count);
    userChar[count - 1] = '\0';
    printk("userChar(char):%s\n", userChar);
    char gpio[10] = {0};
    strncpy(gpio, userChar, 3);
    printk("gpio:%s,length:%d\n", gpio, strlen(gpio));
    struct file *io;
    loff_t pos = 0;
    mm_segment_t old_fs;
    old_fs = get_fs();
    set_fs(get_ds());
    //set-value
    char buff[1] = {'0'};
    if (strcmp(gpio, "255") == 0)
        io = filp_open("/sys/class/gpio/gpio255/value", O_RDONLY, 0);
    else if (strcmp(gpio, "396") == 0)
        io = filp_open("/sys/class/gpio/gpio396/value", O_RDONLY, 0);
    else if (strcmp(gpio, "466") == 0)
        io = filp_open("/sys/class/gpio/gpio466/value", O_RDONLY, 0);
    else if (strcmp(gpio, "429") == 0)
        io = filp_open("/sys/class/gpio/gpio429/value", O_RDONLY, 0);
    vfs_read(io, buff, 1, &pos);
    copy_to_user(buf, buff, 1);
    filp_close(io, NULL);
    pos = 0;
    set_fs(old_fs);
    iCount++;
    return count;
}

static ssize_t drv_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
    printk("device write\n");
    printk("%d\n", iCount);
    printk("W_buf_size: %d\n", (int)count);

    copy_from_user(userChar, buf, count);
    userChar[count - 1] = 0;
    printk("userChar(chr): %s\n", userChar);
    char gpiovalue[10] = {0};
    strncpy(gpiovalue, userChar, 3);
    printk("gpio: %s, length: %d\n", gpiovalue, strlen(gpiovalue));
    struct file *io;
    loff_t pos = 0;
    mm_segment_t old_fs;
    old_fs = get_fs();
    set_fs(get_ds());
    //export
    io = filp_open("/sys/class/gpio/export", O_WRONLY, 0);
    vfs_write(io, gpiovalue, strlen(gpiovalue), &pos);
    filp_close(io, NULL);

    pos = 0;
    //set-dir
    if (strcmp(gpiovalue, "396") == 0)
    {
        io = filp_open("/sys/class/gpio/gpio396/direction", O_WRONLY, 0);
    }
    else if (strcmp(gpiovalue, "466") == 0)
    {
        io = filp_open("/sys/class/gpio/gpio466/direction", O_WRONLY, 0);     
    }
    else if (strcmp(gpiovalue, "255") == 0)
    {
        io = filp_open("/sys/class/gpio/gpio255/direction", O_WRONLY, 0);  
    }
    else if (strcmp(gpiovalue, "429") == 0)
    {
        io = filp_open("/sys/class/gpio/gpio429/direction", O_WRONLY, 0);  
    }

    vfs_write(io, "out", 3, &pos);
    filp_close(io, NULL);
    pos = 0;
    //set-value
    if (strcmp(gpiovalue, "396") == 0)
    {
        io = filp_open("/sys/class/gpio/gpio396/value", O_WRONLY, 0);
    }
    else if (strcmp(gpiovalue, "466") == 0)
    {
        io = filp_open("/sys/class/gpio/gpio466/value", O_WRONLY, 0);     
    }
    else if (strcmp(gpiovalue, "255") == 0)
    {
        io = filp_open("/sys/class/gpio/gpio255/value", O_WRONLY, 0);  
    }
    else if (strcmp(gpiovalue, "429") == 0)
    {
        io = filp_open("/sys/class/gpio/gpio429/value", O_WRONLY, 0);  
    }
    vfs_write(io, &userChar[count - 2], 1, &pos);
    filp_close(io, NULL);
    pos = 0;
    set_fs(old_fs);
    iCount++;
    return count;
}

long drv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    printk("device ioctl\n");
    return 0;
}

static int drv_open(struct inode *inode, struct file *filp)
{
    printk("device open\n");
    return 0;
}

static int drv_release(struct inode *inode, struct file *filp)
{
    printk("device close\n");
    return 0;
}

struct file_operations drv_fops =
    {
        read : drv_read,
        write : drv_write,
        unlocked_ioctl : drv_ioctl,
        open : drv_open,
        release : drv_release,
    };

static int gpio_init(void)
{
    if (register_chrdev(MAJOR_NUM, "demo", &drv_fops) < 0)
    {
        printk("<1>%s: can't get major %d\n", MODULE_NAME, MAJOR_NUM);
        return (-EBUSY);
    }
    printk("<1>%s: started\n", MODULE_NAME);
    return 0;
}

static void gpio_exit(void)
{
    unregister_chrdev(MAJOR_NUM, "demo");
    printk("<1>%s: removed\n", MODULE_NAME);
}

module_init(gpio_init);
module_exit(gpio_exit);
