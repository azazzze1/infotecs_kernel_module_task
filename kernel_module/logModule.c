#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/slab.h>

MODULE_AUTHOR("Shushkov Egor");
MODULE_DESCRIPTION("LogModule for Infotecs");
MODULE_LICENSE("GPL");

static struct kobject *logModule; 

static int timeForLog = 5;
static char logFilename[256] = "/var/tmp/test_module/log.txt";

static ssize_t timeForLogShow(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf){
    return sprintf(buf, "%d\n", timeForLog); 
}

static ssize_t timeForLogStore(struct kobject *kobj, struct kobj_attribute *attr,
                            const char *buf, size_t count){
    sscanf(buf, "%d", &timeForLog); 
    return count;
}

static struct kobj_attribute timeForLogAttribute =
    __ATTR(timeForLog, 0660, timeForLogShow, (void*)timeForLogStore); 


static ssize_t filenameShow(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf){
    return sprintf(buf, "%s\n", logFilename); 
}

static ssize_t filenameStore(struct kobject *kobj, struct kobj_attribute *attr,
                            const char *buf, size_t count){
    if(count >= sizeof(logFilename))
        return -EINVAL;

    strncpy(logFilename, buf, count); 
    logFilename[count] = '\0';

    if(count > 0 && logFilename[count-1] == '\n')
        logFilename[count] = '\0';
    
    return count; 
}

static struct kobj_attribute filenameAttribute =
    __ATTR(logFilename, 0660, filenameShow, (void*)filenameStore); 


static int __init logModule_init(void){
    int error = 0;

    pr_info("logModule: init\n");

    logModule = kobject_create_and_add("logModule", kernel_kobj); 
    if (!logModule)
        return -ENOMEM;
    
    error = sysfs_create_file(logModule, &timeForLogAttribute.attr);
    if (error){
        pr_info("Failed to create timeLog file\n");
        kobject_put(logModule); 
        return error; 
    }

    error = sysfs_create_file(logModule, &filenameAttribute.attr);
    if (error){
        pr_info("Failed to create filename file\n");
        sysfs_remove_file(logModule, &timeForLogAttribute.attr);
        kobject_put(logModule); 
    }

    return error; 
}

static void __exit logModule_exit(void){
    pr_info("logModule: exit\n");
    sysfs_remove_file(logModule, &timeForLogAttribute.attr);
    sysfs_remove_file(logModule, &filenameAttribute.attr);
    kobject_put(logModule);
}

module_init(logModule_init);
module_exit(logModule_exit);