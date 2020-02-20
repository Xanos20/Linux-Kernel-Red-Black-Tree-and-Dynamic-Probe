/* ----------------------------------------------- DRIVER rbKprobe --------------------------------------------------
 
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

//#include <linux/rbKprobe.h>
#include <linux/spinlock.h>
#include <linux/rbtree.h>
//#include <linux/rbtree.h>
#include <linux/spinlock.h>

#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

//#include <linux/kprobes.h>
#include <linux/kprobes.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <asm/msr.h>

#include <linux/unistd.h>

//#include <linux/kernel/tsc.h>

#define DEVICE_NAME                 "RBprobe"  // device name to be created and registered


/* SOURCES 
https://www.infradead.org/~mchehab/kernel_docs/unsorted/rbKprobe.html
*/

/* per device structure */
//struct rbKprobe_dev {
//	struct cdev cdev;               /* The cdev structure */
//	char name[20];                  /* Name of device*/
//	char in_string[256];			/* buffer for the input string */
//	int current_write_pointer;
//} *rbKprobe_devp;



/*struct kprobeHead {
	struct kprobeNode* next;
	int size;
};
*/
typedef struct retrieveFromUser {
	int flag;
	unsigned long addr;

} retrieveFromUser_t;

struct args_from_user {
	char flag;
	char addr;
};

// for generating the path to the current node in the rb driver
struct keydata {
	int key;
	int data;

};
struct path_to_cursor {
	int pathLength;
	struct keydata path[10];
};


// for declarations from rb driver
typedef struct rb_object{
	int key;
	int data;
	struct rb_node node;
} rb_object_t;

struct rbtree_dev {
	struct cdev cdev;               /* The cdev structure */
	char name[20];                  /* Name of device*/
	char in_string[256];
	struct rb_node* treeCursor;
    struct rb_root mytree;
    spinlock_t spinlockDevice;
    int readOrderDevice;
} *rbtree_devp;


// the kprobe buffer
typedef struct probe_data {
	void* addr;
	pid_t pid;
	unsigned long long tsc;
	struct path_to_cursor path;
} probe_data_t;





// kmalloc the node but not the list
struct kprobeNode {
	struct list_head list;
	struct kprobe kp;

};


struct rbKprobe_dev {
	struct cdev cdev;               /* The cdev structure */
	char name[20];                  /* Name of device*/
	char in_string[256];			/* buffer for the input string */
	int current_write_pointer;
    struct rb_root mytree;
    spinlock_t spinlockDevice;
    int readOrderDevice;
    struct kprobe kp;
    //struct kprobeHead head;
   // struct kprobeNode* curr;
    struct list_head head;
    struct kprobeNode testNode;
    struct probe_data probe_buffer;

} *rbKprobe_devp;


/*
 the address of the kprobe, the pid of the running process that hits 
the probe, time stamp (x86 TSC), and all 
rb_object 
objects 


*/
// TODO copy lines 114-121
//static __



typedef struct toReadToUser{
	int key;
	int data;
} toReadToUser_t;


/*
struct kprobe {
    struct hlist_node hlist;                    // Internal 
    kprobe_opcode_t addr;                       // Address of probe 
    kprobe_pre_handler_t pre_handler;           // Address of pre-handler 
    kprobe_post_handler_t post_handler;         // Address of post-handler 
    kprobe_fault_handler_t fault_handler;       // Address of fault handler 
    kprobe_break_handler_t break_handler;       // Internal 
    kprobe_opcode_t opcode;                     // Internal         
    kprobe_opcode_t insn[MAX_INSN_SIZE];        // Internal 
};
*/

static dev_t rbKprobe_dev_number;      /* Allotted device number */
struct class *rbKprobe_dev_class;          /* Tie with the device model */
static struct device *rbKprobe_dev_device;

// Global spinlock to protect tree writes and reads
//spinlock_t SPINLOCK;
// For the ioctl
int READ_ORDER;

static char *user_name = "Kamal Nadesan";  /* the default user name, can be replaced if a new name is attached in insmod command */

module_param(user_name,charp,0000);	//to get parameter from load.sh script to greet the user

/*
* Open rbKprobe driver
*/
int rbKprobe_driver_open(struct inode *inode, struct file *file)
{
	struct rbKprobe_dev *rbKprobe_devp;

	/* Get the per-device structure that contains this cdev */
	rbKprobe_devp = container_of(inode->i_cdev, struct rbKprobe_dev, cdev);


	/* Easy access to cmos_devp from rest of the entry points */
	file->private_data = rbKprobe_devp;
	printk("\n%s is openning \n", rbKprobe_devp->name);
	return 0;
}

/*
 * Release rbKprobe driver
 */
int rbKprobe_driver_release(struct inode *inode, struct file *file)
{
	struct rbKprobe_dev *rbKprobe_devp = file->private_data;
	
	printk("\n%s is closing\n", rbKprobe_devp->name);
	
	return 0;
}





/*
 * Write to rbKprobe driver
 */

/*
write: 
if the input object of rb_object_t has a non-zero data field, a node is created and added to 
the rbt530. 
If an object with the same key already exists in the tree, it should be replaced with the 
new one. If the data field is 0, any existing object with the input key is deleted from the table.

*/
// user just passes in struct with key and vaue

void Post_Handler(struct kprobe *probe, struct pt_regs *regs, unsigned long flags) {
	printk("Post_Handler\n");
	return;
}





/*
struct path_to_cursor {
	int pathLength;
	struct keydata path[10];
};
*/


/*
 the address of the kprobe, 
 the pid of the running process that hits the probe,
 time stamp (x86 TSC), 
 and all rb_object objects 
*/








// Assembly code to get the Time Stamp Counter
static __always_inline unsigned  long long get_native_read_tsc(void) {
	DECLARE_ARGS(val, low, high);
	asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));
	return EAX_EDX_VAL(val, low, high);
}


struct path_to_cursor generate_path(struct rb_root *root, int key) {

  	struct rb_node *node = root->rb_node;

  	struct path_to_cursor PathToCursor;
  	PathToCursor.pathLength = 0;

  	while (node) {

  		struct rb_object *curr = container_of(node, struct rb_object, node);

  		//printk("Key = %d\n", curr->key);
  		//printk("Data = %d\n", curr->data);

  		if(PathToCursor.pathLength >= 10) {
  			// Only return the key and data of the first [0,9] nodes in path
  			return PathToCursor;
  		}

  		// Store current node in the path
  		PathToCursor.path[PathToCursor.pathLength ].key = curr->key;
		PathToCursor.path[PathToCursor.pathLength ].data = curr->data;
		// Increment Counter
		PathToCursor.pathLength += 1;

		// Transverse the rb tree
		if(key < curr->key) {
			node = node->rb_left;
		} 
		else if(key > curr->key) {
        	node = node->rb_right;
        } 
        else {
        	printk("Found Node\n");
            return PathToCursor;
        }
	}

	// Node does not exist in tree and the path length is less than 10
	return PathToCursor;
}


int Pre_Handler(struct kprobe *probe, struct pt_regs *regs) {
	//struct probe_data toReturnToRead;

	// Retrieve and store the corresponding address of the kprobe
	rbKprobe_devp->probe_buffer.addr = (void*) probe->addr;
	//kprobe_opcode_t* addrToReturn = (void*) probe->addr;

	// Get Time Stamp Counter
	unsigned long long tsc = get_native_read_tsc();
	//unsigned long long tsc = rdtsc();
	printk("TSC = %lld\n", tsc);
	//toReturnToRead.tsc = tsc;
	rbKprobe_devp->probe_buffer.tsc = tsc;

	// Get PID
	pid_t processID = current->pid;
	//toReturnToRead.pid = processID;
	rbKprobe_devp->probe_buffer.pid = processID;
	printk("Pid = %d\n", processID);

	// TODO: Get all rb objects from the file pointer should be in the eax register
	struct file *file = (struct file*) regs->ax;

	// Get device data of the file
	struct rbtree_dev *rbtree_devp = file->private_data;

	// Get the root of the corresponding rb tree
	struct rb_root mytree = rbtree_devp->mytree;

	// The current node in the rb tree
	struct rb_node* node = rbtree_devp->treeCursor;
	struct rb_object *treeCursorKeyDataPair = container_of(node, struct rb_object, node);
	int key = treeCursorKeyDataPair->key;

	// generate the path by reading first 10 elements to the path to treeCursor
	rbKprobe_devp->probe_buffer.path = generate_path(&mytree, key);

	return 0;
}




/*
int registerkProbe(struct rbKprobe_dev *rbKprobe_devp, const char *buf, size_t count) {

	// TODO: Buff should not include flag

	if(rbKprobe_devp->head.next == NULL) {
		// new node
		rbKprobe_devp->head.next = kmalloc(sizeof(struct kprobeNode), GFP_KERNEL);	
		if (!(rbKprobe_devp->head.next)) {
			printk("Bad Kmalloc\n"); return -ENOMEM;
		}

		// put in new element
		rbKprobe_devp->head.next->kp.addr = (kprobe_opcode_t*) buf;
		rbKprobe_devp->head.next->kp.pre_handler = Pre_Hander;
		// account for next node
		rbKprobe_devp->head.next->next = NULL;
		// register kprobe
		int verifyRegister = register_kprobe(&(rbKprobe_devp->head.next->kp));
		if(verifyRegister != 0) {
			printk("kprobe register failed\n");
			return -1;
		}
		return 0;
	} else {
		// TODO: append new node to end of list
		struct kprobeNode* curr = rbKprobe_devp->head.next;
		while(curr->next != NULL) {
			// find the end of the list
			curr = curr->next;
		}

		// make new node
		curr->next = kmalloc(sizeof(struct kprobeNode), GFP_KERNEL);
		if(!(curr->next)) {
			printk("Bad Kmalloc\n"); return -ENOMEM;
		}
		// fill in new node fields
		curr->next->kp.addr = (kprobe_opcode_t*) buf;
		curr->next->kp.pre_handler = Pre_Hander;
		
		// update next ptr in new node
		curr->next->next = NULL;

		// register kprobe in new node
		int verifyRegister = register_kprobe(&(curr->next->kp));
		if(verifyRegister != 0) {
			printk("kprobe register failed\n");
			kfree(curr->next);
			// dont want use after free
			curr->next = NULL;
			return -1;
		}

		// update size
		rbKprobe_devp->head.size += 1;
		return 0;
	}
	
}





int deregisterkProbe(struct rbKprobe_dev *rbKprobe_devp, const char *buf, size_t count) {

	// TODO: Buff should not include flag


	return 0;
	
}

*/

/*
typedef struct retrieveFromUser {
	int flag;
	unsigned long addr;

} retrieveFromUser_t;
*/
kprobe_opcode_t* address;
ssize_t rbKprobe_driver_write(struct file *file, const char *buf,
           size_t count, loff_t *ppos)
{
	// User should provide rb_object
	struct rbKprobe_dev *rbKprobe_devp = file->private_data;
	printk("RBPROBE WRITE\n");

	// LOCK
	spin_lock(&(rbKprobe_devp->spinlockDevice));

	struct retrieveFromUser intermediate;

	unsigned long errChkStruct = copy_from_user(&intermediate, buf, sizeof(struct retrieveFromUser));
	if(errChkStruct != 0) {
		printk("Error in Struct From User\n");
		return -1;
	}
	/*
	
	int errChkKey = get_user(intermediate.flag, (buf));
		if(errChkKey == -EFAULT) {
			printk("Unknown KEY  From Buffer Detected\n");
			spin_unlock(&(rbKprobe_devp->spinlockDevice));
			return PTR_ERR(buf);
		}
		// add 4 because we want the next int and int has size 4. Adding one is only for chars
		buf = buf + 4;
		int errChkData = get_user(intermediate.addr, (buf));
		if(errChkData == -EFAULT) {
			printk("Unknown KEY  From Buffer Detected\n");
			spin_unlock(&(rbKprobe_devp->spinlockDevice));
			return PTR_ERR(buf);
		}
		*/

	printk("Flag = %d\n", intermediate.flag);
	printk("Addr = %lx\n", intermediate.addr);


	if((intermediate.flag) == 1) {
		// put probe on write function in mydriver.ko

		//unsigned long rbDriverWriteAddr = kallsyms_lookup_name("rbtree_driver_write");
		/*
		printk("Driver Write Address = %lx\n", rbDriverWriteAddr);
		if(rbDriverWriteAddr == 0) {
			printk("Address For Write Not Found\n");
			spin_unlock(&(rbKprobe_devp->spinlockDevice));
			return -1;
		}
		*/

		//rbKprobe_devp->kp.addr = (kprobe_opcode_t*) intermediate.addr;

		// Initialize kprobe struct
		rbKprobe_devp->kp.addr = (kprobe_opcode_t*) intermediate.addr;
		rbKprobe_devp->kp.pre_handler = Pre_Handler;
		rbKprobe_devp->kp.post_handler = Post_Handler;

		int verifyProbeRegister = register_kprobe(&(rbKprobe_devp->kp));
		if(verifyProbeRegister != 0) {
			printk("Kprobe was not registered\n");
			spin_unlock(&(rbKprobe_devp->spinlockDevice));
			return -1;
		}

	} else if((intermediate.flag) == 0) {
		// remove correspnding kprobe by address

		unregister_kprobe(&(rbKprobe_devp->kp));

	} else {
		printk("Invalid Flag\n");
		spin_unlock(&(rbKprobe_devp->spinlockDevice));
		return -1;
	}
	/*

	// buf will have the memory address
	if(flag == 1) {
		// add new kprobe node to list
		
		struct kprobeNode* nodeToAdd = kmalloc(sizeof(struct kprobeNode), GFP_KERNEL);
		if(!(nodeToAdd)) {
			printk("Bad Kmalloc\n"); 
			spin_unlock(&(rbKprobe_devp->spinlockDevice));
			return -ENOMEM;
		}
		

		nodeToAdd->kp.addr = &addrToInsert;
		nodeToAdd->kp.pre_handler = Pre_Handler;
		
		// register kprobe
		int verifyRegister = register_kprobe(&(nodeToAdd->kp));
		if(verifyRegister != 0) {
			printk("kprobe register failed\n");
			return -1;
		}
		// add to end of list
		list_add_tail(&(nodeToAdd->list), &(rbKprobe_devp->head));
		
		// Just for one kprobe
		rbKprobe_devp->kp.addr = &addrToInsert;
		rbKprobe_devp->kp.pre_handler = Pre_Handler;
		int verifyProbeRegister = register_kprobe(&(rbKprobe_devp->kp));
		if(verifyProbeRegister != 0) {
			printk("Kprobe was not registered\n");
			spin_unlock(&(rbKprobe_devp->spinlockDevice));
			return -1;
		}
		kfree(nodeToAdd);
		return 0;




	
		//int verifyNewNode = registerkProbe(rbKprobe_devp, buf, count);
		
	} else if(flag == 0) {
		// remove kprobe node from list

		// TODO USE THIS int verifyDeleteNode = deregisterkProbe(rbKprobe_devp, buf - sizeof(char), count - sizeof(char));
		//int verifyDeleteNode = deregisterkProbe(rbKprobe_devp, buf, count);
		unregister_kprobe(&(rbKprobe_devp->kp));
		spin_unlock(&(rbKprobe_devp->spinlockDevice));
		return 0;

	} else {
		printk("Invalid Flag\n");
		spin_unlock(&(rbKprobe_devp->spinlockDevice));
		return -1;

	}


	// TODO: Test count
	//printk("Original Count = %zu\n", count);
	// subtract one because of the strlen)+1 parameter from userspace
	*/
	// UNLOCK
	spin_unlock(&(rbKprobe_devp->spinlockDevice));
	return 0;
}




/*
 * Read to rbKprobe driver
 the address of the kprobe, the pid of the running process that hits 
the probe, time stamp (x86 TSC), and all 
rb_object 
objects 
 */
ssize_t rbKprobe_driver_read(struct file *file, char *buf,
           size_t count, loff_t *ppos)
{
	// Returns the probe_buffer to userspace

	int bytes_read = 0;
	struct rbKprobe_dev *rbKprobe_devp = file->private_data;
	
	// LOCK (in case someone wants to remove driver)
	spin_lock(&(rbKprobe_devp->spinlockDevice));

	int errChkKeyData = copy_to_user(buf, &(rbKprobe_devp->probe_buffer), sizeof(struct probe_data));
	if(errChkKeyData == -EFAULT) {
		printk("copy_to_user Error\n");
		spin_unlock(&(rbtree_devp->spinlockDevice));
		return PTR_ERR(buf);
	}

	bytes_read = bytes_read + sizeof(struct probe_data);

	// UNLOCK
	spin_unlock(&(rbKprobe_devp->spinlockDevice));

	// Most read functions return the number of bytes put into the buffer
	return bytes_read;

}



/* File operations structure. Defined in linux/fs.h */
static struct file_operations rbKprobe_fops = {
    .owner		= THIS_MODULE,           /* Owner */
    .open		= rbKprobe_driver_open,        /* Open method */
    .release	= rbKprobe_driver_release,     /* Release method */
    .write		= rbKprobe_driver_write,       /* Write method */
    .read		= rbKprobe_driver_read,        /* Read method */
};
/*
struct rbKprobe_dev {
	struct cdev cdev;               
	char name[20];                  
	char in_string[256];			
	int current_write_pointer;
	struct rb_root mytree;
} *rbKprobe_devp;


*/


/*
 * Driver Initialization
 */
int __init rbKprobe_driver_init(void)
{
	/* TODO: Create two devices!!!!!!!!!!!! */
	int ret;
	int time_since_boot;

	/* Request dynamic allocation of a device major number */
	if (alloc_chrdev_region(&rbKprobe_dev_number, 0, 1, DEVICE_NAME) < 0) {
			printk(KERN_DEBUG "Can't register device\n"); return -1;
	}

	/* Populate sysfs entries */
	rbKprobe_dev_class = class_create(THIS_MODULE, DEVICE_NAME);

	if(IS_ERR(rbKprobe_dev_class)) {
		// TODO: Add unregister_chrdev_region
		printk("class_create(...) ERROR\n");
		/* Release the major number */
	    unregister_chrdev_region((rbKprobe_dev_number), 1);
		return(PTR_ERR(rbKprobe_dev_class));
	}

	/* Allocate memory for the per-device structure */
	rbKprobe_devp = kmalloc(sizeof(struct rbKprobe_dev), GFP_KERNEL);
		
	if (!rbKprobe_devp) {
		printk("Bad Kmalloc\n"); return -ENOMEM;
	}

	// Initialize lock
	spin_lock_init(&(rbKprobe_devp->spinlockDevice));

	// Initialize linked list
	INIT_LIST_HEAD(&(rbKprobe_devp->head));

	//rbKprobe_devp->head.next = NULL;


	// set default read order value (can be changed with ioctl)
	rbKprobe_devp->readOrderDevice = 0;

	/* Request I/O region */
	sprintf(rbKprobe_devp->name, DEVICE_NAME);

	/* Connect the file operations with the cdev */
	cdev_init(&rbKprobe_devp->cdev, &rbKprobe_fops);
	rbKprobe_devp->cdev.owner = THIS_MODULE;

	memset(rbKprobe_devp->in_string, 0, 256);

	// Initialize rb_tree root
	rbKprobe_devp->mytree = RB_ROOT;

	/* Connect the major/minor number to the cdev */
	ret = cdev_add(&rbKprobe_devp->cdev, (rbKprobe_dev_number), 1);

	if (ret) {
		printk("Bad cdev\n");
		return ret;
	}

	/* Send uevents to udev, so it'll create /dev nodes */
	rbKprobe_dev_device = device_create(rbKprobe_dev_class, NULL, MKDEV(MAJOR(rbKprobe_dev_number), 0), NULL, DEVICE_NAME);
	
	if(IS_ERR(rbKprobe_dev_device)) {
		class_destroy(rbKprobe_dev_class);
		// TODO: add unregister_chrdev_region
		/* Release the major number */
	    unregister_chrdev_region((rbKprobe_dev_number), 1);
		printk("device_create(...) ERROR\n");
		return PTR_ERR(rbKprobe_dev_device);
	}

	//since on some systems jiffies is a very huge uninitialized value at boot and saved.
	time_since_boot=(jiffies-INITIAL_JIFFIES)/HZ; 
	sprintf(rbKprobe_devp->in_string, "Hi %s, this machine has been on for %d seconds", user_name, time_since_boot);
	
	printk("rbKprobe driver initialized.\n'%s'\n",rbKprobe_devp->in_string);
	//printk("rbKprobe driver initialized.\n'%s'\n",rbKprobe_devp->mytree);

	return 0;
}
/* Driver Exit */
void __exit rbKprobe_driver_exit(void)
{
	// device_remove_file(rbKprobe_dev_device, &dev_attr_xxx);
	/* Release the major number */
	unregister_chrdev_region((rbKprobe_dev_number), 1);



	// TODO deregister and free all kprobe nodes in list

	// unregister kprobe
	unregister_kprobe(&(rbKprobe_devp->kp));

	/* Destroy device */
	device_destroy (rbKprobe_dev_class, MKDEV(MAJOR(rbKprobe_dev_number), 0));
	cdev_del(&rbKprobe_devp->cdev);
	kfree(rbKprobe_devp);
	
	/* Destroy driver_class */
	class_destroy(rbKprobe_dev_class);

	printk("rbKprobe driver removed.\n");
	return;
}

module_init(rbKprobe_driver_init);
module_exit(rbKprobe_driver_exit);
MODULE_LICENSE("GPL v2");















// USEFUL STUFF POTENTIALLY



