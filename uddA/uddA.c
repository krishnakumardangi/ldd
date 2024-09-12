#include<linux/init.h>
#include<linux/module.h>
#include<linux/proc_fs.h>

MODULE_LICENSE("GPL");  //*  It is required to get access of other module which are using same license, if it is missing then mak eprocess will fail.
MODULE_AUTHOR("InPyjama-Krishna"); //! Optional. 
MODULE_DESCRIPTION("Our first dynamically loadable kernal mmodule"); //! Optional.

static struct proc_dir_entry *custom_proc_node; // This will help in creating a file in user space and where we will perform read and write operation in future.


// static ssize_t ldd_read(struct file* file_pointer,
//                         char *user_space_buffer,
//                         size_t count,
//                         loff_t* offset){
//     char msg[] = "Ack!\n";
//     size_t len = strlen(msg);

//     int result, result1;


//     if(*offset >= len)
//         return 0;

//     //copy_to_user(dst, src, no_of_bytes);
//     result = copy_to_user(user_space_buffer, msg, len);

//     size_t len1 = strlen(kmsg);
//     result1 = copy_to_user(user_space_buffer, kmsg, len1);
//     *offset += len;

//     printk("ldd_read\n");
    
//     return len;
// }


// Side note: this is the correct "len" you want.
char etx_array[] = "hello how are you?";
size_t len = sizeof(etx_array) / sizeof(char) - 1;

// ...

static ssize_t ldd_read(struct file *filp, 
                        char __user *buf, 
                        size_t size, 
                        loff_t *off){
    loff_t offset = *off;
    size_t remaining;

    pr_info("proc file read\n");
    
    if (offset < 0)
        return -EINVAL;

    if (offset >= len || size == 0)
        return 0;

    if (size > len - offset)
        size = len - offset;

    remaining = copy_to_user(buf, etx_array + offset, size);
    if (remaining == size) {
        pr_err("copy_to_user failed\n");
        return -EFAULT;
    }

    size -= remaining;
    *off = offset + size;
    return size;
}

static ssize_t ldd_write(struct file *filp, const char *buf, size_t size, loff_t *off)
{
    loff_t offset = *off;
    size_t remaining;
    
    pr_info("proc file write\n");

    if (offset < 0)
        return -EINVAL;

    if (offset >= len || size == 0)
        return 0;

    if (size > len - offset)
        size = len - offset;

    remaining = copy_from_user(etx_array + offset, buf, size);
    if (remaining == size) {
        pr_err("copy_from_user failed\n");
        return -EFAULT;
    }

    size -= remaining;
    *off = offset + size;
    return size;
}


struct proc_ops driver_proc_ops = {
    .proc_read = ldd_read,
    .proc_write = ldd_write
};


static int pyjama_module_init(void)    // This is like constructor for Kernel. It is first function get executed.
{
    printk("Hello, World! (from InPyjama!!)\n");
    //printf("This is Linux Device Driver Tutorial.\n");

    printk("pyjama_module_init: entry\n");

    custom_proc_node = proc_create("ldd_driver",
                                    0,
                                    NULL,
                                    &driver_proc_ops);
    
    if(custom_proc_node == NULL)
    {
        printk("There is some error in your user defined driver!");
        return -1;
    }

    printk("pyjama_module_init: exit\n");
    return 0;
}

static void pyjama_module_exit(void)  // This is like destructor for Kernel. It execute at end of module.
{
    printk("Good Bye! (from InPyjama!!)\n");

    printk("pyjama_module_exit: entry\n");

    proc_remove(custom_proc_node);

    printk("pyjama_module_exit: exit\n");
    
}

module_init(pyjama_module_init); // This is an example of Macros in C.
module_exit(pyjama_module_exit); // This is an example of Macros in C.