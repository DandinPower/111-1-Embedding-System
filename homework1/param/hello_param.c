#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

static char *x = "World";
module_param(x, charp, 0);

static int hello_init(void){
    printk(KERN_INFO "Hello, %s!\n", x);
    return 0;
}

static int hello_exit(void){
    printk(KENO_INFO "Goodbye %s!\n", x);
}

module_init(hello_init);
module_exit(hello_exit);