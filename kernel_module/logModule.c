#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>

MODULE_AUTHOR("Shushkov Egor");
MODULE_DESCRIPTION("LogModule for Infotecs");
MODULE_LICENSE("GPL");

static struct kobject *logModule; 

static int timeForLog = 0;

static ssize_t timeForLogShow(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf){
    return sprintf(buf, "%d\n", timeForLog); 
}

static ssize_t timeForLogStore(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf, size_t count){
    sscanf(buf, "%du", &timeForLog); 
    return count;
}

static struct kobj_attribute timeForLogAttribute =
    __ATTR(timeForLog, 0660, timeForLogShow, (void*)timeForLogStore); 

static int __init logModule_init(void){
    int error = 0;

    pr_info("logModule: init\n");

    logModule = kobject_create_and_add("logModule", kernel_kobj); 
    if (!logModule)
        return -ENOMEM;
    
    error = sysfs_create_file(logModule, &timeForLogAttribute.attr);
    if (error){
        pr_info("Failed to create log\n");
    }

    return error;
}

static void __exit logModule_exit(void){
    pr_info("logModule: exit\n");
    kobject_put(logModule);
}

module_init(logModule_init);
module_exit(logModule_exit);