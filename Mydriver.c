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


#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#define DEVICE_NAME                 "rbt530_dev"  // device name to be created and registered


/* SOURCES 
https://www.infradead.org/~mchehab/kernel_docs/unsorted/rbtree.html
*/

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
    spinlock_t spinlockDevice;
    int readOrderDevice;
} *rbtree_devp;


static dev_t rbtree_dev_number;      /* Allotted device number */
struct class *rbtree_dev_class;          /* Tie with the device model */
static struct device *rbtree_dev_device;

// Global spinlock to protect tree writes and reads
//spinlock_t SPINLOCK;
// For the ioctl
int READ_ORDER;

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

/*
write: 
if the input object of rb_object_t has a non-zero data field, a node is created and added to 
the rbt530. 
If an object with the same key already exists in the tree, it should be replaced with the 
new one. If the data field is 0, any existing object with the input key is deleted from the table.

*/
// user just passes in struct with key and vaue

typedef struct keydata_to_read {
	int key;
	int data;
} keydata_to_read_t;

typedef struct rb_keydata {
	char key;
	char data;
} rb_keydata_t;

typedef struct rb_object{
	int key;
	int data;
	struct rb_node node;
} rb_object_t;



struct rb_object *my_search(struct rb_root *root, int key)
  {
  	struct rb_node *node = root->rb_node;

  	while (node) {
  		struct rb_object *curr = container_of(node, struct rb_object, node);
  		//printk("Iterating\n");
  		//printk("Key = %d\n", curr->key);
  		//printk("Data = %d\n", curr->data);

		if(key < curr->key) {
			node = node->rb_left;
		} 
		else if(key > curr->key) {
        	node = node->rb_right;
        } 
        else {
        	// found node
        	printk("Found Node to Delete\n");
            return curr;
        }
	}
	// node does not exist in tree
	return NULL;
  }


int my_insert(struct rb_root *root, struct rb_object *nodeToInsert) {

	// if nodeToInsert->data == 0
	//printk("Key = %d\n", nodeToInsert->key);
	//printk("Data = %d\n", nodeToInsert->data);
	if(nodeToInsert->data == 0) {
		struct rb_object *nodeToDelete = my_search(root, nodeToInsert->key);
  		if (nodeToDelete != NULL) {
  			rb_erase(&(nodeToDelete->node), root);
  			kfree(nodeToDelete);
  			printk("Removed Node\n");
  			return 0;
  		}
	}
    
    struct rb_node **new = &(root->rb_node), *parent = NULL;
    // Figure out where to put new node 
    while (*new) {
        struct rb_object *this = container_of(*new, struct rb_object, node);
        //int result = strcmp(nodeToInsert->keystring, this->keystring);
        parent = *new;
        if(nodeToInsert->key < this->key) {
        	new = &((*new)->rb_left);
        } else if(nodeToInsert->key > this->key) {
        	new = &((*new)->rb_right);
        } else {
        	// node already exists in tree
        	// replace data
        	this->data = nodeToInsert->data;
            return 1;
        }
            
    }

      // Add new node and rebalance tree. 
      rb_link_node(&nodeToInsert->node, parent, new);
      rb_insert_color(&nodeToInsert->node, root);

      return 0;
}

// 

ssize_t rbtree_driver_write(struct file *file, const char *buf,
           size_t count, loff_t *ppos)
{
	// User should provide rb_object
	struct rbtree_dev *rbtree_devp = file->private_data;
	// LOCK

	spin_lock(&(rbtree_devp->spinlockDevice));
	// TODO: Test count
	printk("Original Count = %zu\n", count);
	// subtract one because of the strlen)+1 parameter from userspace
	
	/*if(count % 2 == 1) {
		count = count - 1;
	}
	// divide by two because each while loop iteration takes two value from parameter buf
	count = count / 2;
	*/
	struct rb_keydata intermediate;

	//unsigned long errChkStruct = copy_from_user(&intermediate, buf, sizeof(struct rb_keydata));
	int errChkKey = get_user(intermediate.key, (buf));
		if(errChkKey == -EFAULT) {
			printk("Unknown KEY  From Buffer Detected\n");
			spin_unlock(&(rbtree_devp->spinlockDevice));
			return PTR_ERR(buf);
		}
		// add 4 because we want the next int and int has size 4. Adding one is only for chars
		buf = buf + 4;
		int errChkData = get_user(intermediate.data, (buf));
		if(errChkData == -EFAULT) {
			printk("Unknown KEY  From Buffer Detected\n");
			spin_unlock(&(rbtree_devp->spinlockDevice));
			return PTR_ERR(buf);
		}
	printk("KEY = %d\n", intermediate.key);
	printk("DATA = %d\n", intermediate.data);
		/*
	if( copy_from_user(&intermediate, buf, sizeof(struct rb_keydata)) ) {
		spin_unlock(&(rbtree_devp->spinlockDevice));
		return -EFAULT;
	}
	*/
	/*
		if(errChkStruct != 0) {
			printk("Unknown Value From Buffer Detected\n");
			spin_unlock(&(rbtree_devp->spinlockDevice));
			return PTR_ERR(buf);

		}
		*/


	
	while (count > 0) {	
		//get_user(rbtree_devp->in_string[rbtree_devp->current_write_pointer], buf++);
		//printk("BUF = %s\n", buf[count]);
		
		// Get Key
		/*
		char toInsertKey = '\0';
		int errChkKey = get_user(toInsertKey, (buf++));
		if(errChkKey == -EFAULT) {
			printk("Unknown KEY  From Buffer Detected\n");
			spin_unlock(&(rbtree_devp->spinlockDevice));
			return PTR_ERR(buf);

		}
		printk("KEY = %d\n", toInsertKey);
		int keyToInsert = (int) toInsertKey;

		// Get Data
		char toInsertData = '\0';
		int errChkData = get_user(toInsertData, (buf++));
		if(errChkData == -EFAULT) {
			printk("Unknown DATA  From Buffer Detected\n");
			spin_unlock(&(rbtree_devp->spinlockDevice));
			return PTR_ERR(buf);
		}
		printk("DATA = %d\n", toInsertData);
		int dataToInsert = (int) toInsertData;
		printk("COUNT = %zu\n", count);
		*/



		//struct rb_node *node;
		//struct rb_keydata intermediate;

		//char KEY;
		/*
		int errChkKey = get_user(intermediate.key, (buf++));
		if(errChkKey == -EFAULT) {
			printk("Unknown KEY  From Buffer Detected\n");
			spin_unlock(&(rbtree_devp->spinlockDevice));
			return PTR_ERR(buf);
		}
		int errChkData = get_user(intermediate.data, (buf++));
		if(errChkData == -EFAULT) {
			printk("Unknown KEY  From Buffer Detected\n");
			spin_unlock(&(rbtree_devp->spinlockDevice));
			return PTR_ERR(buf);
		}
		*/
		
		/*unsigned long errChkStruct = copy_from_user(&intermediate, buf, sizeof(struct rb_keydata));
		if(errChkStruct != 0) {
			printk("Unknown Value From Buffer Detected\n");
			spin_unlock(&(rbtree_devp->spinlockDevice));
			return PTR_ERR(buf);

		}
		*/

		int keyToInsert = (int) intermediate.key;
		int dataToInsert = (int) intermediate.data;

		

		
		if(dataToInsert == 0) {
			// If (data==0) remove corresponding node
			struct rb_object* nodeToDelete = my_search(&(rbtree_devp->mytree), keyToInsert);
			if (nodeToDelete) {
				// the node exists in the tree and now should be deleted
      			rb_erase(&nodeToDelete->node, &(rbtree_devp->mytree));
      			kfree(nodeToDelete);
      			printk("Node Deleted\n");
			} else {
				// Node does not exist in tree and no action should be taken in this while loop iteration
			}

		} else {
			// Insert new node or replace data field of existing node in tree
			struct rb_object* nodeToInsert = kmalloc(sizeof(struct rb_object), GFP_KERNEL);
		    if (!nodeToInsert) {
			    printk("Bad Kmalloc\n"); 
			    spin_unlock(&(rbtree_devp->spinlockDevice));
			    return -ENOMEM;
		    }
		    // fill in node 
		    nodeToInsert->key = keyToInsert;
			nodeToInsert->data = dataToInsert;
			int result = my_insert(&(rbtree_devp->mytree), nodeToInsert);
			if(result == 0) {
				printk("Node Inserted\n");
			} else {
				printk("Node Exists in Tree\n");
				kfree(nodeToInsert);
			}
		}
				
		// count should deincrement regardless of if else choice
		count--;
	}
	
    	
	/*for (node = rb_first(&(rbtree_devp->mytree)); node; node = rb_next(node))
	    printk("data=%d\n", rb_entry(node, struct rb_object, node)->data);
	    */
	struct rb_node *node;
	printk("Entering Loop\n");
    /*for (node = rb_first(&(rbtree_devp->mytree)); node != NULL; node = rb_next(node)) {
    	printk("key=%d\n", rb_entry(node, struct rb_object, node)->key);
    	if(rb_next(node) == NULL) {
    		break;
    	}

    }
    */
    printk("Entering Reverse Loop\n");
    for (node = rb_last(&(rbtree_devp->mytree)); node != NULL; node = rb_prev(node)) {
    	printk("key=%d\n", rb_entry(node, struct rb_object, node)->key);
    	if(rb_prev(node) == NULL) {
    		printk("RB_NEXT == NULL\n");
    		break;
    	}

    }
	
	
	// UNLOCK
	spin_unlock(&(rbtree_devp->spinlockDevice));
	return 0;
}

/*
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
*/

/*
ioctl: 
The command “read_order” to set up ascending or descending order that objects are to be 
read

If the argument is 0, read calls retrieve the objects in ascending order starting from the object 
with  the  minimal  key. 

It  if  is  1,  read  calls  get  the  objects  in  descending  order  star
ting  from  the object with the maximal key. 

Otherwise, -1 is returned and errno is set to EINVAL.

*/
// TODO: Use unlocked ioctl
long rbtree_driver_unlocked_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {
	// TODO: local read order
	struct rbtree_dev *rbtree_devp = file->private_data;


	if(ioctl_num == 0) {
		rbtree_devp->readOrderDevice = 0;
	} else if(ioctl_num == 1) {
		rbtree_devp->readOrderDevice = 1;
	} else {
		printk("ioctl must have 0 or 1 as a parameter\n");
		// TODO: search how to set up error call ioctl
		return -1;
	}
	return 0;

}
















/*
 * Read to rbtree driver
 */

/*
read: 
to retrieve the next object in either ascending or descending order (to be set by an ioctl call) from the RB tree. 
  If the RB tree is empty or the next object is null, -1 is returned and errno is set to EINVAL.

  struct rb_node *rb_first(struct rb_root *tree);
  struct rb_node *rb_last(struct rb_root *tree);
  struct rb_node *rb_next(struct rb_node *node);
  struct rb_node *rb_prev(struct rb_node *node);

  typedef struct rb_keydata {
	char key;
	char data;
} rb_keydata_t;


		struct rb_node *node;
		printk("Entering Loop\n");
    	for (node = rb_first(&(rbtree_devp->mytree)); node; node = rb_next(node)) {
    		if(node != NULL) {
    			printk("key=%d\n", rb_entry(node, struct rb_object, node)->key);
    		}

    	}
*/

ssize_t rbtree_driver_read(struct file *file, char *buf,
           size_t count, loff_t *ppos)
{
	// TODO: Write object back to user
	int bytes_read = 0;
	struct rbtree_dev *rbtree_devp = file->private_data;
	
	// LOCK (in case someone wants to remove driver)
	spin_lock(&(rbtree_devp->spinlockDevice));
	printk("COUNT READ = %zu", count);

	//
	// If we're at the end of the message, 
	// return 0 signifying end of file 

	//Actually put the data into the buffer 
	if(count < 8) {
		// copies data to user in 8 byte increments
		return -1;
	}

	struct keydata_to_read sendToUser;

	struct rb_node *node;
	printk("READ\n");
	if(rbtree_devp->readOrderDevice == 0) {
		// read in ascending order
		for (node = rb_first(&(rbtree_devp->mytree)); node != NULL && (count <= 8); node = rb_next(node)) {

    		printk("key=%d\n", rb_entry(node, struct rb_object, node)->key);
    		sendToUser.key = rb_entry(node, struct rb_object, node)->key;
    		sendToUser.data = rb_entry(node, struct rb_object, node)->data;
    		printk("KEY = %d\n", sendToUser.key);
    		printk("DATA = %d\n", sendToUser.data);
    		printk("COUNT READ = %zu", count);



    		//int errChkPutUserKey = put_user(rb_entry(node, struct rb_object, node)->key, buf++);
    		//int errChkPutUserKey = put_user(rb_entry(node, struct rb_object, node)->key, buf++);
    		int errChkKeyData = copy_to_user(buf, &sendToUser, sizeof(struct keydata_to_read));
			if(errChkKeyData == -EFAULT) {
				printk("Get_User Error\n");
				spin_unlock(&(rbtree_devp->spinlockDevice));
				return PTR_ERR(node);
			}
			// Update Variables
			count = count - sizeof(struct keydata_to_read);
			bytes_read = bytes_read + sizeof(struct keydata_to_read);
			//count--;
			//bytes_read++;
    		if(rb_next(node) == NULL) {
    			break;
    		}

   		}
   	} else if(rbtree_devp->readOrderDevice == 1) {
   		// read in descending order
   		printk("Entering Reverse Loop\n");
   		for (node = rb_last(&(rbtree_devp->mytree)); node != NULL; node = rb_prev(node)) {
    		
    		printk("key=%d\n", rb_entry(node, struct rb_object, node)->key);
    		sendToUser.key = rb_entry(node, struct rb_object, node)->key;
    		sendToUser.data = rb_entry(node, struct rb_object, node)->data;

    		int errChkKeyData = copy_to_user(buf, &sendToUser, sizeof(struct keydata_to_read));
    		printk("errChkKeyData = %d\n", errChkKeyData);
    		//int errChkPutUserKeyRev = put_user(rb_entry(node, struct rb_object, node)->key, buf++);
			if(errChkKeyData == -EFAULT) {
				printk("Get_User Error\n");
				spin_unlock(&(rbtree_devp->spinlockDevice));
				return PTR_ERR(node);
			}
    		// Update Variables
			count = count - sizeof(struct keydata_to_read);
			bytes_read = bytes_read + sizeof(struct keydata_to_read);
    		if(rb_prev(node) == NULL) {
    			break;
    		}
    	}
   	} else {
   		printk("Read Order Error\n");
		spin_unlock(&(rbtree_devp->spinlockDevice));
		return -1;

   	}
   	printk("COUNT READ outside loop = %zu", count);
	// UNLOCK
	spin_unlock(&(rbtree_devp->spinlockDevice));
	// Most read functions return the number of bytes put into the buffer
	return bytes_read;

}


/*
ssize_t rbtree_driver_read(struct file *file, char *buf,
           size_t count, loff_t *ppos)
{
	
	int bytes_read = 0;
	struct rbtree_dev *rbtree_devp = file->private_data;


	// LOCK (in case someone wants to remove driver)
	spin_lock(&SPINLOCK);

	//
	// If we're at the end of the message, 
	// return 0 signifying end of file 
	 //
	if (rbtree_devp->in_string[0] == 0)
		return 0;

	//Actually put the data into the buffer 
	 
	while (count && rbtree_devp->in_string[bytes_read]) {

		// TODO: ADD error checking
		put_user(rbtree_devp->in_string[bytes_read], buf++);
		count--;
		bytes_read++;
	}
	printk("Reading -- '%s'\n",rbtree_devp->in_string);


	// UNLOCK
	spin_unlock(&SPINLOCK);
	//Most read functions return the number of bytes put into the buffer
	return bytes_read;

}
*/

/*
int rbtree_driver_unlocked_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {

	if(ioctl_num == 0) {
		READ_ORDER = 0;
	} else if(ioctl_num == 1) {
		READ_ORDER = 1;
	} else {
		printk("ioctl must have 0 or 1 as a parameter\n");
		return -1;
	}
	return 0;

}

*/




/* File operations structure. Defined in linux/fs.h */
static struct file_operations rbtree_fops = {
    .owner		= THIS_MODULE,           /* Owner */
    .open		= rbtree_driver_open,        /* Open method */
    .release	= rbtree_driver_release,     /* Release method */
    .write		= rbtree_driver_write,       /* Write method */
    .read		= rbtree_driver_read,        /* Read method */
    .unlocked_ioctl      = rbtree_driver_unlocked_ioctl
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
	/* TODO: Create two devices!!!!!!!!!!!! */
	int ret;
	int time_since_boot;

	// Initialize global semaphore to prevent race conditions with the write method
	//void init_MUTEX(&SEM);
	//spin_lock_init(&(rbtree_devp->spinlockDevice));

	// set default read_order value
	//READ_ORDER = 0;

	/* Request dynamic allocation of a device major number */
	if (alloc_chrdev_region(&rbtree_dev_number, 0, 1, DEVICE_NAME) < 0) {
			printk(KERN_DEBUG "Can't register device\n"); return -1;
	}

	/* Populate sysfs entries */
	rbtree_dev_class = class_create(THIS_MODULE, DEVICE_NAME);

	if(IS_ERR(rbtree_dev_class)) {
		// TODO: Add unregister_chrdev_region
		printk("class_create(...) ERROR\n");
		/* Release the major number */
	    unregister_chrdev_region((rbtree_dev_number), 1);
		return(PTR_ERR(rbtree_dev_class));
	}

	/* Allocate memory for the per-device structure */
	rbtree_devp = kmalloc(sizeof(struct rbtree_dev), GFP_KERNEL);
		
	if (!rbtree_devp) {
		printk("Bad Kmalloc\n"); return -ENOMEM;
	}

	spin_lock_init(&(rbtree_devp->spinlockDevice));

	// set default read order value (can be changed with ioctl)
	rbtree_devp->readOrderDevice = 0;

	/* Request I/O region */
	sprintf(rbtree_devp->name, DEVICE_NAME);

	/* Connect the file operations with the cdev */
	cdev_init(&rbtree_devp->cdev, &rbtree_fops);
	rbtree_devp->cdev.owner = THIS_MODULE;

	memset(rbtree_devp->in_string, 0, 256);

	// Initialize rb_tree root
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
		/* Release the major number */
	    unregister_chrdev_region((rbtree_dev_number), 1);
		printk("device_create(...) ERROR\n");
		return PTR_ERR(rbtree_dev_device);
	}

	//since on some systems jiffies is a very huge uninitialized value at boot and saved.
	time_since_boot=(jiffies-INITIAL_JIFFIES)/HZ; 
	sprintf(rbtree_devp->in_string, "Hi %s, this machine has been on for %d seconds", user_name, time_since_boot);
	
	rbtree_devp->current_write_pointer = 0;
	// Initialize root to be
	//rbtree_devp->mytree = NULL;

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



