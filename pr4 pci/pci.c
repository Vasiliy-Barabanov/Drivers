#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/pci_regs.h>
#include <linux/init.h>
#include <linux/etherdevice.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

#define PCI_VENDOR_ID 0x10ec
#define PCI_DEVICE_ID 0x8168
#define DRV_NAME "MyPCI"
#define ETH_ALEN 6
#define DEVICE_NAME "chardev" 
#define BUF_LEN 1024
#define MY_IOCTL_CMD_READ_DATA _IOR(77, 1, char*)

MODULE_LICENSE("GPL");

unsigned int dm7820_PCIMem_real, dm7820_PCIMem_size, DM7820_NAME;
unsigned char dm7820_PCIMem_virtual;
  
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *); 
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static long my_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);
static int fooprobe (struct pci_dev *dm7820_pci, const struct pci_device_id *id);

unsigned char mac_addr[ETH_ALEN];
static char *mac_Ptr; 
static int Device_Open = 0;	
static int Major; /* Major-номер, для нашего устройства */


static struct pci_device_id ids[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID, PCI_DEVICE_ID) },
	{ 0, }
};


int get_mac_address(struct pci_dev *pdev, unsigned char *mac_addr) {
    void __iomem *mmio_addr;
    u8 *mac_ptr;

    // Map the PCI device memory into kernel space
    mmio_addr = ioremap(pci_resource_start(pdev, 0), pci_resource_len(pdev, 0));
    if (!mmio_addr) {
        return -ENOMEM;
    }

    // Read the MAC address from the memory-mapped region
    mac_ptr = (u8 *)(mmio_addr + 0x0000);
    memcpy(mac_addr, mac_ptr, ETH_ALEN);

    // Unmap the memory region
    iounmap(mmio_addr);

    return 0; }

static int fooprobe (struct pci_dev *dm7820_pci, const struct pci_device_id *id) {
	printk ("Get physics BAR0...");
	dm7820_PCIMem_real = pci_resource_start (dm7820_pci,0);
	dm7820_PCIMem_size = pci_resource_len (dm7820_pci,0);
	if ((dm7820_PCIMem_real==0)||(dm7820_PCIMem_size==0)) {
		printk ("failed.\n"); return -1;
	}
	else
		printk ("%u...OK.\n",(uint32_t) dm7820_PCIMem_real);
	printk ("Checks physics BAR0...");
	if (pci_resource_flags (dm7820_pci,0)&IORESOURCE_MEM)
		printk ("OK.\n");
	else {
		printk ("failed.\n"); return -1;
	}
	printk ("Get virtual BAR0...");
	dm7820_PCIMem_virtual=ioremap(dm7820_PCIMem_real,dm7820_PCIMem_size);
	if (dm7820_PCIMem_virtual==0) {
		printk ("failed.\n"); return -1;
	}
	else 
		printk ("%u...OK.\n",(uint32_t)dm7820_PCIMem_virtual);
	printk ("Request region BAR0...\t\t");
	if (request_mem_region (dm7820_PCIMem_real,dm7820_PCIMem_size,DM7820_NAME))
		printk ("OK.\n");
	else {
		printk ("failed.\n"); 
		return -1;
	}
	return 0;
}


static int my_probe(struct pci_dev *dev, const struct pci_device_id *id) {
	
	printk(KERN_INFO "*** inside probe  *** \n");
	
	pci_resource_start(dev,0);
	// Get the MAC address for the device
	if (get_mac_address(dev, mac_addr)) {
		printk(KERN_ERR "Unable to get MAC address\n");
		return -EINVAL;
	}

	printk(KERN_INFO "MAC address: %pM\n", mac_addr);
	
	return 0;
}

static void my_remove(struct pci_dev *dev)
{
	unregister_chrdev(Major, DEVICE_NAME);	
}

/*pci driver operation */
static struct pci_driver pci_driver = {
	.name = DRV_NAME,
	.id_table = ids,
	.probe = fooprobe,
	.remove = my_remove,
};

static struct file_operations fops = {
	.read = device_read,
	.unlocked_ioctl = my_ioctl,
	.open = device_open,
	.release = device_release
};


int init_module(void)
{	
	printk(KERN_INFO "*** inside init  *** \n");

	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Major < 0) { 
		printk(KERN_ALERT "Регистрация симв. устройства failed для %d\n", Major); 	
		return Major; 
	}
	printk(KERN_INFO "To talk to the driver, create a dev file with\n"); 
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major); 
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n"); 
	printk(KERN_INFO "the device file.\n"); 
	

	return pci_register_driver(&pci_driver);
}


void cleanup_module(void)
{
	printk(KERN_INFO "*** inside exit *** \n");
	pci_unregister_driver(&pci_driver);
}


static ssize_t device_read(struct file *filp, 
			   char *buffer, /* буфер для данных */ 
			   size_t length, /* длина буфера */ 
			   loff_t * offset) /* смещение */ {
	size_t bytes_to_copy = min(length, ETH_ALEN - *offset);
	if (bytes_to_copy <= 0){
		return 0;
	}
	if (copy_to_user(buffer, mac_addr + *offset, bytes_to_copy)) {
		return -EFAULT;
	}
	*offset += bytes_to_copy;
	return bytes_to_copy;
}

static int device_open(struct inode *inode, struct file *file) { 
	if (Device_Open) 
		return -EBUSY;

	Device_Open++; 
	mac_Ptr = mac_addr;
	printk(KERN_INFO "device_open MAC address: %pM\n", mac_addr); 
	try_module_get(THIS_MODULE);

	return 0;
}

static int device_release(struct inode *inode, struct file *file) { 
	Device_Open--; 
	module_put(THIS_MODULE);

	return 0;
}

static long my_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
	printk(KERN_INFO "*** inside ioctl*** \n");
	int data_size = 0;
	printk(KERN_INFO "IOCTL MAC address: %pM\n", mac_addr); 
        printk(KERN_INFO "IOCTL cmd: %d\n", cmd); 
    
    	switch (cmd) {
        	case MY_IOCTL_CMD_READ_DATA:
            	// copy data from kernel space to user space
            		data_size = strlen(mac_addr) + 1;
            		if (copy_to_user((char *)arg, mac_addr, data_size)) {
                		return -EFAULT;
            		}
			break;
        	default:
            		return -ENOTTY; // unsupported command
	}
	return 0;
}

