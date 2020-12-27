

/*
      功能： 调用kobject函数,kobject_init_and_add,在/sys/ 目录下创建test_config的目录，目录下创建test_1以及test_2的文件，可以对这两个文件进行echo 以及cat操作。

*/

#include <linux/kernel.h>

#include <linux/module.h>

#include <linux/fs.h>

#include <linux/sched.h>

#include <linux/uaccess.h>

#include <linux/cdev.h>

#include <linux/kobject.h>

ssize_t	test_show(struct kobject *, struct attribute *, char *);
ssize_t	test_store(struct kobject *, struct attribute *, const char *, size_t);
void   test_release(struct kobject *kobj);

struct attribute test_1 = {
    .name = "test_1",
    .mode = S_IRWXUGO
};

struct attribute test_2 = {
    .name = "test_2",
    .mode = S_IRWXUGO
};

struct attribute *test_attribute_array[] = {
    &test_1,
    &test_2,
    NULL
};


struct sysfs_ops test_file_ops = {
    .show = test_show,
    .store = test_store
};

struct kobject test_kobj;
struct kobj_type test_kobj_type = {
    .release = test_release,
    .sysfs_ops = &test_file_ops,
    .default_attrs = test_attribute_array
};

void   test_release(struct kobject *kobj)
{
	printk("test_release\n");
}

ssize_t	test_show(struct kobject *obj, struct attribute *attr, char *buf)
{
    printk("test_show %s",attr->name);
    strncpy(buf,attr->name,strlen(attr->name));
    return strlen(attr->name);
}

ssize_t	test_store(struct kobject *obj, struct attribute *attr, const char *buf, size_t len)
{
    printk("test_store %s %s\n",buf,attr->name);
    return len;
}

static int __init test_kobject_init(void)
{

    int ret = kobject_init_and_add(&test_kobj,&test_kobj_type,NULL,"test_object");
    if(ret < 0){printk("kobject_init_and_add fail\n");}
    return ret;
}

static void __exit test_kobject_clean(void)
{
	printk("exit_test_kobject\n");
	kobject_del(&test_kobj);
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Carl");
MODULE_DESCRIPTION("test for kobject_init_and_add");
module_init(test_kobject_init);
module_exit(test_kobject_clean);