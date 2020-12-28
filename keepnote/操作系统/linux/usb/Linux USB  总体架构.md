# Linux USB  总体架构

linux usb 核心代码主要的目录为linux-xxx/drivers/usb/core，核心代码包括usb-core，usb hub ，usb hcd（USB Host Controller Driver framework），usb sysfs，usb urb（usb request block），usb buf（DMA memory management for framework level HCD code）等

核心架构图

![linux_usb_subsystem](/home/carl/learn/notebook/keepnote/操作系统/linux/usb/Linux USB Driver_files/linux_usb_subsystem.gif)

通过以上图示可以看出，usbcore 有以下几种功能

- 连接usb client dreiver 以及usb host controller driver，构建其二者之前沟通的桥梁。类似于很多的spi core，i2c core系统

- 面向usbfs，向应用层提供对应的接口来操作底层usb设备，为底层usb 设备编写应用层驱动，例如使用libusb来开发设备驱动

- 面向usb hub，对hub进行驱动

   
  
  

## USB core

核心代码位于/drivers/usb/core/下在c文件中，初始化函数`usb_init`，完成了以下几点功能

- `usb_debugfs_init`，完成usb debugfs的初始化，主要是debugs根文件夹创建了usb的目录并创建devices的文件，并提供了其read以及poll函数，方便在user space上能够打印对应的usb总线上的devices

​       `debugfs on /sys/kernel/debug type debugfs (rw,relatime)`通过mount指令即可获得

- `bus_register(&usb_bus_type)` usb总线的注册，其中提供了match 函数，用来匹配设备与驱动，提供了uevent 的事件通知

  ```
  struct bus_type usb_bus_type = {
      .name =     "usb",
      .match =    usb_device_match,
      .uevent =   usb_uevent,
  };
  ```

- `bus_register_notifier(&usb_bus_type, &usb_bus_nb)`注册了总线在加入设备以及移除设备的回调函数

- `usb_major_init`注册了一个字符设备，usb major是`#define USB_MAJOR           180`

- `usb_register(&usbfs_driver);`注册一个用户空间可以访问的usb driver，

  









## hcd 驱动（USB host controller driver）

hcd 主要是用来驱动 usb host 控制器，负责数据的传输，以及相关usb 协议中的底层必要的硬件信号传输，类似phy，即链路层传输

`