
#ifdef Q_OS_LINUX
#include <asm/ioctl.h>
#endif
/*usbkey设备描述结构体*/
struct usbkey_dev                                     
{                                                        
  char *data;                      
  unsigned long size;       
};
#define USBKEYDEV_MAJOR 120
#define USBKEYDEV_NR_DEVS 1

#define ENOUSBKEY 19
#define USBKEYDEV_SIZE 100
/* 定义幻数 */
#define USBKEYDEV_IOC_MAGIC  'k'

/* 定义命令 */
#define USBKEYDEV_IOCPRINT   _IO(USBKEYDEV_IOC_MAGIC, 1)
#define USBKEYDEV_IOCGETDATA _IOR(USBKEYDEV_IOC_MAGIC, 2, int)
#define USBKEYDEV_IOCSETDATA _IOW(USBKEYDEV_IOC_MAGIC, 3, int)

#define USBKEYDEV_IOC_MAXNR 3

