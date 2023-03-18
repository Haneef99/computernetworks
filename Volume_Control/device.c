#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Haneef");
MODULE_DESCRIPTION("For controlling the master volume of the device");

char* buffk;

/* init */

static int __init dinit(void);
dev_t d = 0;
static struct cdev mydev;
static struct class *device_class;



/* open */
static int dopen(struct inode *inode,struct file *file);

/* close */
static int dclose(struct inode* inode, struct file *file);

/* read */
static ssize_t read(struct file* flip,char __user *buff,size_t len,loff_t *loff);

/* write */
static ssize_t write(struct file *flip,const char *buff,size_t len,loff_t *loff);

/* exit */
static void __exit dexit(void);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = dopen,
	.release = dclose,
	.read  = read,
	.write = write
};

static int dopen(struct inode *inode,struct file *file){
	if((buffk == kmalloc(1000,GFP_KERNEL)) == 0){
		printk(KERN_ALERT"No enough Memory\n");
		return -1;
	}
	
	printk(KERN_INFO"Memory allocated sucessfully\n");
	return 0;
	
}

static int dclose(struct inode *inode,struct file *file){
	kfree(buffk);
	printk(KERN_INFO"Memory freed!\n");
	
	return 0;
}

static ssize_t read(struct file* flip,char __user *buff,size_t len,loff_t *loff){

	copy_to_user(buff,buffk,strlen(buffk));
	
	printk(KERN_INFO"Data read from user successfully\n");
	
	return strlen(buffk) - 1;
}

static ssize_t write(struct file *flip,const char *buff,size_t len,loff_t *loff){
	copy_from_user(buffk,buff,len);
	buffk[len] = '\0';
	printk(KERN_INFO"Data written by user successfully\n");
	return len;
}

static int __init dinit(void){

	if(alloc_chrdev_region(&d,0,1,"vol") < 0){
		printk(KERN_ALERT"Memory failed to allocate\n");
		return -1;
	}
	
	printk(KERN_INFO"Device intialised with major : %d minor : %d\n",MAJOR(d),MINOR(d));
	
	cdev_init(&mydev,&fops);
	
	if(cdev_add(&mydev,d,1) < 0){
		printk(KERN_ALERT"Device adding failed\n");
		return -1;
	}
	
	if((device_class = class_create(THIS_MODULE,"class")) == NULL){
		printk(KERN_ALERT"Class creation failed\n");
		return -1;
	}

	if(device_create(device_class,NULL,d,NULL,"vol") == NULL){
		printk(KERN_ALERT"Device creation failed\n");
		class_destroy(device_class);
		return -1;
	}

	printk(KERN_ALERT"Device Inserted sucessfully\n");
	return 0;
}

static void __exit dexit(void){
	device_destroy(device_class,d);
	class_destroy(device_class);
	cdev_del(&mydev);
	unregister_chrdev_region(d,1);
	printk(KERN_ALERT"Device removed sucessfully\n");
}

module_init(dinit);
module_exit(dexit);
