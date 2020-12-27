#include <linux/input.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/init.h>

/*本测试代码基于ubuntu16.04,有的函数可能版本不匹配*/
static struct input_dev *event_input_dev = NULL;
static struct timer_list timer;

static int event_open(struct input_dev *dev)
{
    printk(KERN_INFO "event_open\n");  
    return 0; 
}

static void event_close(struct input_dev *dev)
{
    printk(KERN_INFO "event_close\n");   
}


static void   input_event_timer_callback(struct timer_list * list)
{
    printk(KERN_INFO "input_event\n");
    static int value = 20;
    input_report_rel(event_input_dev,REL_X,value++);
    input_report_rel(event_input_dev,REL_Y,value++);
    input_sync(event_input_dev);
    mod_timer(&timer,jiffies + msecs_to_jiffies(1000));
}

static int __init event_input_init(void)
{
    int error;
    event_input_dev = input_allocate_device();
    if (!event_input_dev) {
	    printk(KERN_ERR "Not enough memory\n");
	    error = -ENOMEM;
    }
    event_input_dev->open = event_open;
    event_input_dev->close = event_close;
    event_input_dev->name  = "event_test";

     __set_bit(EV_REL,event_input_dev->evbit);
     __set_bit(REL_X,event_input_dev->relbit);
     __set_bit(REL_Y,event_input_dev->relbit);
     error = input_register_device(event_input_dev);
     if (error) {
	     printk(KERN_ERR "Failed to register device\n");
	     goto err_free_dev;
     }
     timer_setup(&timer,input_event_timer_callback,0);
     add_timer(&timer);
     mod_timer(&timer,jiffies + msecs_to_jiffies(1000));

	return 0;

err_free_dev:
	input_free_device(event_input_dev);
	return error;
}

static void __exit event_input_exit(void)
{
    del_timer(&timer);
    input_unregister_device(event_input_dev);
}

module_init(event_input_init);
module_exit(event_input_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Carl");