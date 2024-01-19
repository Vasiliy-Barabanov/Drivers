#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 
#include <linux/fs.h>


static int device_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "I am opened %d\n");
   return 0;
}
static int device_release(struct inode *inode, struct file *file){
	printk(KERN_INFO "I am released %d\n");
  return 0;
}

dev_t Major;
int Device_Open = 0;
int signal_cnt = 0;
char msg[6] = "hello";
ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset)
{
	
	if(length > 6)
		length = 6;
	
	copy_to_user(buffer, msg, length);
	printk(KERN_INFO "I send %d\n", msg);
	return length;
}


const struct file_operations fops = {
   .read = device_read,
//   .write = device_write,
   .open = device_open,
   .release = device_release
};

unsigned int Major;
int init_module(void) {

   pr_info("Hello world 1.\n");

   Major = register_chrdev(0, "func", &fops);
   if (Major < 0) {
       printk(KERN_ALERT "Registering char device failed with %d\n", Major);
       return Major;
   }
   printk(KERN_INFO "Major %d\n", Major);
    /* A non 0 return means init_module failed; module can't be loaded. */
    return 0;
}

void cleanup_module(void) { 
    pr_info("Goodbye world 1.\n"); 
   unregister_chrdev(Major, "func");

} 
 
MODULE_LICENSE("GPL");
