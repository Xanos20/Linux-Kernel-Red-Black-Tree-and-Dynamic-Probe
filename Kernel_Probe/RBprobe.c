/* ----------------------------------------------- DRIVER DriverRB_PROBE --------------------------------------------------
 
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

#include <linux/DriverRB_PROBE.h>
#include <linux/spinlock.h>


#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#define DEVICE_NAME                 "RBprobe"  // device name to be created and registered


/* SOURCES 
https://www.infradead.org/~mchehab/kernel_docs/unsorted/DriverRB_PROBE.html
*/

/* per device structure */
//struct DriverRB_PROBE_dev {
//	struct cdev cdev;               /* The cdev structure */
//	char name[20];                  /* Name of device*/
//	char in_string[256];			/* buffer for the input string */
//	int current_write_pointer;
//} *DriverRB_PROBE_devp;

struct DriverRB_PROBE_dev {
	struct cdev cdev;               /* The cdev structure */
	char name[20];                  /* Name of device*/
	char in_string[256];			/* buffer for the input string */
	int current_write_pointer;
    struct rb_root mytree;
    spinlock_t spinlockDevice;
    int readOrderDevice;
} *DriverRB_PROBE_devp;


static dev_t DriverRB_PROBE_dev_number;      /* Allotted device number */
struct class *DriverRB_PROBE_dev_class;          /* Tie with the device model */
static struct device *DriverRB_PROBE_dev_device;

// Global spinlock to protect tree writes and reads
//spinlock_t SPINLOCK;
// For the ioctl
int READ_ORDER;

static char *user_name = "Kamal Nadesan";  /* the default user name, can be replaced if a new name is attached in insmod command */

module_param(user_name,charp,0000);	//to get parameter from load.sh script to greet the user

/*
* Open DriverRB_PROBE driver
*/
int DriverRB_PROBE_driver_open(struct inode *inode, struct file *file)
{
	struct DriverRB_PROBE_dev *DriverRB_PROBE_devp;

	/* Get the per-device structure that contains this cdev */
	DriverRB_PROBE_devp = container_of(inode->i_cdev, struct DriverRB_PROBE_dev, cdev);


	/* Easy access to cmos_devp from rest of the entry points */
	file->private_data = DriverRB_PROBE_devp;
	printk("\n%s is openning \n", DriverRB_PROBE_devp->name);
	return 0;
}

/*
 * Release DriverRB_PROBE driver
 */
int DriverRB_PROBE_driver_release(struct inode *inode, struct file *file)
{
	struct DriverRB_PROBE_dev *DriverRB_PROBE_devp = file->private_data;
	
	printk("\n%s is closing\n", DriverRB_PROBE_devp->name);
	
	return 0;
}

/*
 * Write to DriverRB_PROBE driver
 */

/*
write: 
if the input object of rb_object_t has a non-zero data field, a node is created and added to 
the rbt530. 
If an object with the same key already exists in the tree, it should be replaced with the 
new one. If the data field is 0, any existing object with the input key is deleted from the table.

*/
// user just passes in struct with key and vaue







ssize_t DriverRB_PROBE_driver_write(struct file *file, const char *buf,
           size_t count, loff_t *ppos)
{
	// User should provide rb_object
	struct DriverRB_PROBE_dev *DriverRB_PROBE_devp = file->private_data;
	// LOCK

	spin_lock(&(DriverRB_PROBE_devp->spinlockDevice));
	// TODO: Test count
	printk("Original Count = %zu\n", count);
	// subtract one because of the strlen)+1 parameter from userspace
	
	// UNLOCK
	spin_unlock(&(DriverRB_PROBE_devp->spinlockDevice));
	return 0;
}

















/*
 * Read to DriverRB_PROBE driver
 */




ssize_t DriverRB_PROBE_driver_read(struct file *file, char *buf,
           size_t count, loff_t *ppos)
{
	// TODO: Write object back to user
	int bytes_read = 0;
	struct DriverRB_PROBE_dev *DriverRB_PROBE_devp = file->private_data;
	
	// LOCK (in case someone wants to remove driver)
	spin_lock(&(DriverRB_PROBE_devp->spinlockDevice));
	printk("COUNT READ = %zu", count);


	// UNLOCK
	spin_unlock(&(DriverRB_PROBE_devp->spinlockDevice));
	// Most read functions return the number of bytes put into the buffer
	return bytes_read;

}



/* File operations structure. Defined in linux/fs.h */
static struct file_operations DriverRB_PROBE_fops = {
    .owner		= THIS_MODULE,           /* Owner */
    .open		= DriverRB_PROBE_driver_open,        /* Open method */
    .release	= DriverRB_PROBE_driver_release,     /* Release method */
    .write		= DriverRB_PROBE_driver_write,       /* Write method */
    .read		= DriverRB_PROBE_driver_read,        /* Read method */
    .unlocked_ioctl      = DriverRB_PROBE_driver_unlocked_ioctl
};
/*
struct DriverRB_PROBE_dev {
	struct cdev cdev;               
	char name[20];                  
	char in_string[256];			
	int current_write_pointer;
	struct rb_root mytree;
} *DriverRB_PROBE_devp;


*/



/*
 * Driver Initialization
 */
int __init DriverRB_PROBE_driver_init(void)
{
	/* TODO: Create two devices!!!!!!!!!!!! */
	int ret;
	int time_since_boot;

	// Initialize global semaphore to prevent race conditions with the write method
	//void init_MUTEX(&SEM);
	//spin_lock_init(&(DriverRB_PROBE_devp->spinlockDevice));

	// set default read_order value
	//READ_ORDER = 0;

	/* Request dynamic allocation of a device major number */
	if (alloc_chrdev_region(&DriverRB_PROBE_dev_number, 0, 1, DEVICE_NAME) < 0) {
			printk(KERN_DEBUG "Can't register device\n"); return -1;
	}

	/* Populate sysfs entries */
	DriverRB_PROBE_dev_class = class_create(THIS_MODULE, DEVICE_NAME);

	if(IS_ERR(DriverRB_PROBE_dev_class)) {
		// TODO: Add unregister_chrdev_region
		printk("class_create(...) ERROR\n");
		/* Release the major number */
	    unregister_chrdev_region((DriverRB_PROBE_dev_number), 1);
		return(PTR_ERR(DriverRB_PROBE_dev_class));
	}

	/* Allocate memory for the per-device structure */
	DriverRB_PROBE_devp = kmalloc(sizeof(struct DriverRB_PROBE_dev), GFP_KERNEL);
		
	if (!DriverRB_PROBE_devp) {
		printk("Bad Kmalloc\n"); return -ENOMEM;
	}

	spin_lock_init(&(DriverRB_PROBE_devp->spinlockDevice));

	// set default read order value (can be changed with ioctl)
	DriverRB_PROBE_devp->readOrderDevice = 0;

	/* Request I/O region */
	sprintf(DriverRB_PROBE_devp->name, DEVICE_NAME);

	/* Connect the file operations with the cdev */
	cdev_init(&DriverRB_PROBE_devp->cdev, &DriverRB_PROBE_fops);
	DriverRB_PROBE_devp->cdev.owner = THIS_MODULE;

	memset(DriverRB_PROBE_devp->in_string, 0, 256);

	// Initialize rb_tree root
	DriverRB_PROBE_devp->mytree = RB_ROOT;

	/* Connect the major/minor number to the cdev */
	ret = cdev_add(&DriverRB_PROBE_devp->cdev, (DriverRB_PROBE_dev_number), 1);

	if (ret) {
		printk("Bad cdev\n");
		return ret;
	}

	/* Send uevents to udev, so it'll create /dev nodes */
	DriverRB_PROBE_dev_device = device_create(DriverRB_PROBE_dev_class, NULL, MKDEV(MAJOR(DriverRB_PROBE_dev_number), 0), NULL, DEVICE_NAME);
	
	if(IS_ERR(DriverRB_PROBE_dev_device)) {
		class_destroy(DriverRB_PROBE_dev_class);
		// TODO: add unregister_chrdev_region
		/* Release the major number */
	    unregister_chrdev_region((DriverRB_PROBE_dev_number), 1);
		printk("device_create(...) ERROR\n");
		return PTR_ERR(DriverRB_PROBE_dev_device);
	}

	//since on some systems jiffies is a very huge uninitialized value at boot and saved.
	time_since_boot=(jiffies-INITIAL_JIFFIES)/HZ; 
	sprintf(DriverRB_PROBE_devp->in_string, "Hi %s, this machine has been on for %d seconds", user_name, time_since_boot);
	
	DriverRB_PROBE_devp->current_write_pointer = 0;
	// Initialize root to be
	//DriverRB_PROBE_devp->mytree = NULL;

	printk("DriverRB_PROBE driver initialized.\n'%s'\n",DriverRB_PROBE_devp->in_string);
	//printk("DriverRB_PROBE driver initialized.\n'%s'\n",DriverRB_PROBE_devp->mytree);

	return 0;
}
/* Driver Exit */
void __exit DriverRB_PROBE_driver_exit(void)
{
	// device_remove_file(DriverRB_PROBE_dev_device, &dev_attr_xxx);
	/* Release the major number */
	unregister_chrdev_region((DriverRB_PROBE_dev_number), 1);

	/* Destroy device */
	device_destroy (DriverRB_PROBE_dev_class, MKDEV(MAJOR(DriverRB_PROBE_dev_number), 0));
	cdev_del(&DriverRB_PROBE_devp->cdev);
	kfree(DriverRB_PROBE_devp);
	
	/* Destroy driver_class */
	class_destroy(DriverRB_PROBE_dev_class);

	printk("DriverRB_PROBE driver removed.\n");
}

module_init(DriverRB_PROBE_driver_init);
module_exit(DriverRB_PROBE_driver_exit);
MODULE_LICENSE("GPL v2");















// USEFUL STUFF POTENTIALLY



