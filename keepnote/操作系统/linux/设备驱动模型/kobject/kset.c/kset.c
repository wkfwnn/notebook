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



static int filter(struct kset *kset, struct kobject *kobj)
{

	printk("filter\n");
	return 1;
}
static int uevent(struct kset *kset, struct kobject *kobj,
		      struct kobj_uevent_env *env)
{

	printk("uevent\n");
	return 0;
}


struct kset_uevent_ops test_uevent_ops =  {
    .filter = filter,
    .uevent = uevent
};

struct kset test_kset = {
    .kobj = {.name = "test_kset",.ktype = &test_kobj_type},
    .uevent_ops = &test_uevent_ops
};

struct kset test_kset_1 = {
    .kobj = {.name = "test_kset_1",.ktype = &test_kobj_type},
    .uevent_ops = &test_uevent_ops
};

static int __init test_kset_init(void)
{

    int ret = kset_register(&test_kset);
    if(ret < 0){printk("kset_register fail\n");}
    test_kset_1.kobj.kset = &test_kset;
    ret = kset_register(&test_kset_1);
    if(ret < 0){printk("kset1_register fail\n");}
    return ret;
}

static void __exit test_kset_clean(void)
{
	printk("exit_test_kset\n");
	kset_unregister(&test_kset);
        kset_unregister(&test_kset_1);
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Carl");
MODULE_DESCRIPTION("test for kset_register");
module_init(test_kset_init);
module_exit(test_kset_clean);