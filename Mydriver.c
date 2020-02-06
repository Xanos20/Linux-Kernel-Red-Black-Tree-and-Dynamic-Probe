/* ----------------------------------------------- DRIVER rbtree --------------------------------------------------
 
 Basic driver example to show skelton methods for several file operations.
 
 ----------------------------------------------------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/jiffies.h>

#include<linux/init.h>
#include<linux/moduleparam.h>

#include <linux/rbtree.h>
#include <linux/spinlock.h>

#define DEVICE_NAME                 "rbtree"  // device name to be created and registered

/* per device structure */
//struct rbtree_dev {
//	struct cdev cdev;               /* The cdev structure */
//	char name[20];                  /* Name of device*/
//	char in_string[256];			/* buffer for the input string */
//	int current_write_pointer;
//} *rbtree_devp;

struct rbtree_dev {
	struct cdev cdev;               /* The cdev structure */
	char name[20];                  /* Name of device*/
	char in_string[256];			/* buffer for the input string */
	int current_write_pointer;
    struct rb_root mytree;
} *rbtree_devp;


static dev_t rbtree_dev_number;      /* Allotted device number */
struct class *rbtree_dev_class;          /* Tie with the device model */
static struct device *rbtree_dev_device;

//struct semaphore *SEM;
spinlock_t SPINLOCK;

static char *user_name = "Kamal Nadesan";  /* the default user name, can be replaced if a new name is attached in insmod command */

module_param(user_name,charp,0000);	//to get parameter from load.sh script to greet the user

/*
* Open rbtree driver
*/
int rbtree_driver_open(struct inode *inode, struct file *file)
{
	struct rbtree_dev *rbtree_devp;

	/* Get the per-device structure that contains this cdev */
	rbtree_devp = container_of(inode->i_cdev, struct rbtree_dev, cdev);


	/* Easy access to cmos_devp from rest of the entry points */
	file->private_data = rbtree_devp;
	printk("\n%s is openning \n", rbtree_devp->name);
	return 0;
}

/*
 * Release rbtree driver
 */
int rbtree_driver_release(struct inode *inode, struct file *file)
{
	struct rbtree_dev *rbtree_devp = file->private_data;
	
	printk("\n%s is closing\n", rbtree_devp->name);
	
	return 0;
}

/*
 * Write to rbtree driver
 */
ssize_t rbtree_driver_write(struct file *file, const char *buf,
           size_t count, loff_t *ppos)
{

	struct rbtree_dev *rbtree_devp = file->private_data;
	// LOCK
	spin_lock(&SPINLOCK);
	
	while (count) {	
		// TODO: Does get_user need error checking?????
		get_user(rbtree_devp->in_string[rbtree_devp->current_write_pointer], buf++);
		count--;
		if(count){
			rbtree_devp->current_write_pointer++;
			if( rbtree_devp->current_write_pointer == 256)
				rbtree_devp->current_write_pointer = 0;
		}
	}
	printk("Writing -- %d %s \n", rbtree_devp->current_write_pointer, rbtree_devp->in_string);
	// UNLOCK
	spin_unlock(&SPINLOCK);
	return 0;
}
/*
 * Read to rbtree driver
 */

ssize_t rbtree_driver_read(struct file *file, char *buf,
           size_t count, loff_t *ppos)
{
	
	int bytes_read = 0;
	struct rbtree_dev *rbtree_devp = file->private_data;

	// LOCK (in case someone wants to remove driver)
	spin_lock(&SPINLOCK);

	/*
	 * If we're at the end of the message, 
	 * return 0 signifying end of file 
	 */
	if (rbtree_devp->in_string[0] == 0)
		return 0;

	/* 
	 * Actually put the data into the buffer 
	 */
	while (count && rbtree_devp->in_string[bytes_read]) {

		// TODO: ADD error checking
		put_user(rbtree_devp->in_string[bytes_read], buf++);
		count--;
		bytes_read++;
	}
	printk("Reading -- '%s'\n",rbtree_devp->in_string);
	/* 
	 * Most read functions return the number of bytes put into the buffer
	 */

	// UNLOCK
	spin_unlock(&SPINLOCK);
	return bytes_read;

}

/* File operations structure. Defined in linux/fs.h */
static struct file_operations rbtree_fops = {
    .owner		= THIS_MODULE,           /* Owner */
    .open		= rbtree_driver_open,        /* Open method */
    .release	= rbtree_driver_release,     /* Release method */
    .write		= rbtree_driver_write,       /* Write method */
    .read		= rbtree_driver_read,        /* Read method */
};
/*
struct rbtree_dev {
	struct cdev cdev;               
	char name[20];                  
	char in_string[256];			
	int current_write_pointer;
	struct rb_root mytree;
} *rbtree_devp;
*/

/*
 * Driver Initialization
 */
int __init rbtree_driver_init(void)
{
	int ret;
	int time_since_boot;

	// Initialize global semaphore to prevent race conditions with the write method
	//void init_MUTEX(&SEM);
	spin_lock_init(&SPINLOCK);

	/* Request dynamic allocation of a device major number */
	if (alloc_chrdev_region(&rbtree_dev_number, 0, 1, DEVICE_NAME) < 0) {
			printk(KERN_DEBUG "Can't register device\n"); return -1;
	}

	/* Populate sysfs entries */
	rbtree_dev_class = class_create(THIS_MODULE, DEVICE_NAME);

	if(IS_ERR(rbtree_dev_class)) {
		// TODO: Add unregister_chrdev_region
		printk("class_create(...) ERROR\n");
		return(PTR_ERR(rbtree_dev_class));
	}

	/* Allocate memory for the per-device structure */
	rbtree_devp = kmalloc(sizeof(struct rbtree_dev), GFP_KERNEL);
		
	if (!rbtree_devp) {
		printk("Bad Kmalloc\n"); return -ENOMEM;
	}

	/* Request I/O region */
	sprintf(rbtree_devp->name, DEVICE_NAME);

	/* Connect the file operations with the cdev */
	cdev_init(&rbtree_devp->cdev, &rbtree_fops);
	rbtree_devp->cdev.owner = THIS_MODULE;

	memset(rbtree_devp->in_string, 0, 256);

	// initialize rb_tree
	rbtree_devp->mytree = RB_ROOT;

	/* Connect the major/minor number to the cdev */
	ret = cdev_add(&rbtree_devp->cdev, (rbtree_dev_number), 1);

	if (ret) {
		printk("Bad cdev\n");
		return ret;
	}

	/* Send uevents to udev, so it'll create /dev nodes */
	rbtree_dev_device = device_create(rbtree_dev_class, NULL, MKDEV(MAJOR(rbtree_dev_number), 0), NULL, DEVICE_NAME);
	
	if(IS_ERR(rbtree_dev_device)) {
		class_destroy(rbtree_dev_class);
		// TODO: add unregister_chrdev_region
		printk("device_create(...) ERROR\n");
		return PTR_ERR(rbtree_dev_device);
	}

	//since on some systems jiffies is a very huge uninitialized value at boot and saved.
	time_since_boot=(jiffies-INITIAL_JIFFIES)/HZ; 
	sprintf(rbtree_devp->in_string, "Hi %s, this machine has been on for %d seconds", user_name, time_since_boot);
	
	rbtree_devp->current_write_pointer = 0;

	printk("rbtree driver initialized.\n'%s'\n",rbtree_devp->in_string);
	//printk("rbtree driver initialized.\n'%s'\n",rbtree_devp->mytree);

	return 0;
}
/* Driver Exit */
void __exit rbtree_driver_exit(void)
{
	// device_remove_file(rbtree_dev_device, &dev_attr_xxx);
	/* Release the major number */
	unregister_chrdev_region((rbtree_dev_number), 1);

	/* Destroy device */
	device_destroy (rbtree_dev_class, MKDEV(MAJOR(rbtree_dev_number), 0));
	cdev_del(&rbtree_devp->cdev);
	kfree(rbtree_devp);
	
	/* Destroy driver_class */
	class_destroy(rbtree_dev_class);

	printk("rbtree driver removed.\n");
}

module_init(rbtree_driver_init);
module_exit(rbtree_driver_exit);
MODULE_LICENSE("GPL v2");















// USEFUL STUFF POTENTIALLY



