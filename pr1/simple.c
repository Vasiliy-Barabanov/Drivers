#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");

static char *whom = "world";
module_param(whom, charp, 0);
static int howmany = 1;
module_param(howmany, int, 0);         // Передается пара параметров - сколько рaз и  кому передается привет 
static int __init hello_init(void) {
    int i;
    for (i = 0; i < howmany; i++)  	
      printk(KERN_ALERT "(%d) Hello, %s\n", i, whom);
    return 0;
}
static void __exit hello_exit(void) {
    printk(KERN_ALERT "Goodbye, cruel %s\n", whom);
}
module_init(hello_init);
module_exit(hello_exit);
