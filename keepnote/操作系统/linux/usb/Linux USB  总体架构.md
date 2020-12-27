# Linux USB  总体架构

linux usb 核心代码主要的目录为linux-xxx/drivers/usb/core，核心代码包括usb-core，usb hub ，usb hcd（USB Host Controller Driver framework），usb sysfs，usb urb（usb request block），usb buf（DMA memory management for framework level HCD code）等

核心架构图

![linux_usb_subsystem](/home/carl/learn/notebook/keepnote/操作系统/linux/usb/Linux USB Driver_files/linux_usb_subsystem.gif)

通过以上图示可以看出，usbcore 有以下几种功能

- 连接usb client dreiver 以及usb host controller driver，构建其二者之前沟通的桥梁。类似于很多的spi core，i2c core系统

- 面向usbfs，向应用层提供对应的接口来操作底层usb设备，为底层usb 设备编写应用层驱动，例如使用libusb来开发设备驱动

- 面向usb hub，对hub进行驱动

  

## hcd 驱动（USB host controller driver）

hcd 主要是用来驱动 usb host 控制器，负责数据的传输，以及相关usb 协议中的底层必要的硬件信号传输，类似phy，即链路层传输



`