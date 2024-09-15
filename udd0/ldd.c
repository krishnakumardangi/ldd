#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");  //*  It is required to get access of other module which are using same license, if it is missing then mak eprocess will fail.
MODULE_AUTHOR("InPyjama-Krishna"); //! Optional. 
MODULE_DESCRIPTION("Our first dynamically loadable kernal mmodule"); //! Optional.

static int pyjama_module_init(void)    // This is like constructor for Kernel. It is first function get executed.
{
    printk("Hello, World! (from Krishna!!)\n");
    return 0;
}

static void pyjama_module_exit(void)  // This is like destructor for Kernel. It execute at end of module.
{
    printk("Good Bye! (from Krishna!!)\n");
}

module_init(pyjama_module_init); // This is an example of Macros in C.
module_exit(pyjama_module_exit); // This is an example of Macros in C.

