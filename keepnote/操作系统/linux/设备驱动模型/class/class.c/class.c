#include <linux/kernel.h>

#include <linux/module.h>

#include <linux/fs.h>

#include <linux/sched.h>

#include <linux/uaccess.h>

#include <linux/cdev.h>

#include <linux/kobject.h>

#include <linux/sysfs.h>

struct attribute test_1 = {
	.name = "test_1",
	.mode = S_IRWXUGO
};

struct attribute test_2 = {
	.name = "test_2",
	.mode = S_IRWXUGO
};

struct attribute *group_file_array[]  = {	
	&test_1,
	&test_2,
        NULL
};


const struct attribute_group test_group = {
    .name = "test_group",
    .attrs = group_file_array
};

const struct attribute_group *test_group_array[] = {
	&test_group,
        NULL
};


struct bus_type  test_bus = {
    .name = "test_serial",
    .bus_groups = test_group_array 
};


static int __init test_bus_init(void)
{
    int ret = bus_register(&test_bus);
    if(ret)
        bus_unregister(&test_bus); 
    return ret;
}

static void __exit test_bus_clean(void)
{
    bus_unregister(&test_bus);    
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Carl");
MODULE_DESCRIPTION("test for bus register");
module_init(test_bus_init);
module_exit(test_bus_clean);