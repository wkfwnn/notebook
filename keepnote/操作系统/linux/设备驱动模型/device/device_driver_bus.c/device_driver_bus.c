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

static int test_serial_match(struct device *dev, struct device_driver *drv)
{
	return 1;
}

struct bus_type  test_bus = {
    .name = "test_serial",
    .bus_groups = test_group_array, 
    .match = test_serial_match,
};


static void my_device_release(struct device *dev)
{
    printk("my_device_release\n");
}

const struct  device_type my_device_type = {
    .name = "my_name_type",
    .release = my_device_release
};

struct device my_device  = {
    .init_name =  "my_device",
    .bus = &test_bus,
    .type = &my_device_type,
};

static int my_drv_probe (struct device *dev)
{
    printk("my_drv_probe\n");
    return 0;
}

struct device_driver my_drv_1 =  
{
    .name = "my_drv_1",
    .bus = &test_bus,
    .owner = THIS_MODULE, 
    .probe = my_drv_probe,
};

static int __init test_device_init(void)
{
    int ret = bus_register(&test_bus);
    if(ret)
        bus_unregister(&test_bus); 
    ret = device_register(&my_device);
    if(ret)
        device_unregister(&my_device);
    ret = driver_register(&my_drv_1);
    return ret;
}

static void __exit test_device_clean(void)
{
    bus_unregister(&test_bus);    
    device_unregister(&my_device);
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Carl");
MODULE_DESCRIPTION("test for bus device and driver register");
module_init(test_device_init);
module_exit(test_device_clean);