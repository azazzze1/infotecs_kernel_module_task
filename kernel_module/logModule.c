#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>
#include <linux/err.h>
#include <linux/cred.h>
#include <linux/workqueue.h>

MODULE_AUTHOR("Shushkov Egor");
MODULE_DESCRIPTION("LogModule for Infotecs");
MODULE_LICENSE("GPL");

static struct kobject *logModule; 
static struct delayed_work logWork;

static DEFINE_MUTEX(moduleMutex);

static int timeForLog = 5;
static char logFilename[256] = "/var/tmp/test_module/log.txt";
static unsigned long logNumber = 0;

static ssize_t timeForLogShow(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf){

    unsigned int val;

    mutex_lock(&moduleMutex);
    val = timeForLog; 
    mutex_unlock(&moduleMutex);

    return sprintf(buf, "%u\n", val); 
}

static ssize_t timeForLogStore(struct kobject *kobj, struct kobj_attribute *attr,
                            const char *buf, size_t count){
    unsigned int val;
    
    if(kstrtouint(buf, 10, &val) != 0){
        return -EINVAL;
    }

    if(val == 0){
        return -EINVAL;
    }

    mutex_lock(&moduleMutex);
    timeForLog = val; 
    mutex_unlock(&moduleMutex);

    return count;
}

static struct kobj_attribute timeForLogAttribute =
    __ATTR(timeForLog, 0660, timeForLogShow, (void*)timeForLogStore); 

static ssize_t filenameShow(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf){
    ssize_t val;

    mutex_lock(&moduleMutex);
    val = sprintf(buf, "%s\n", logFilename); 
    mutex_unlock(&moduleMutex);

    return val;
}

static ssize_t filenameStore(struct kobject *kobj, struct kobj_attribute *attr,
                            const char *buf, size_t count){
    size_t len; 
    
    if(count >= sizeof(logFilename))
        return -EINVAL;

    mutex_lock(&moduleMutex);

    len = strnlen(buf, count);
    strncpy(logFilename, buf, len); 
    logFilename[count] = '\0';

    if(count > 0 && logFilename[count-1] == '\n')
        logFilename[count] = '\0';

    mutex_unlock(&moduleMutex);

    return count; 
}

static struct kobj_attribute filenameAttribute =
    __ATTR(logFilename, 0660, filenameShow, (void*)filenameStore); 


static long handleOpenError(struct file *filp, const char *filename)
{
    long err_code = PTR_ERR(filp);
    
    switch (err_code) {
    case -ENOENT:
        pr_err("logModule: path does not exist: %s\n", filename);
        break;
    case -EPERM:
        pr_err("logModule: permission denied: %s (error %ld)\n", filename, err_code);
        break;
    default:
        pr_err("logModule: failed to open file: %s (error %ld)\n", filename, err_code);
        break;
    }
    
    return err_code;
}

static void writeLogToFile(void){

    pr_info("logModule: start to log with N = %lu\n", logNumber);

    struct file *filp;
    char msg[128];
    ssize_t ret; 
    char curFilename[128];
    unsigned long curLogNumber;

    mutex_lock(&moduleMutex);
    strncpy(curFilename, logFilename, sizeof(curFilename) - 1);
    curFilename[sizeof(curFilename) - 1] = '\0';
    curLogNumber = ++logNumber;
    mutex_unlock(&moduleMutex);

    
    int len = snprintf(msg, sizeof(msg), "Hello from kernel module (%lu)\n", curLogNumber);
    if (len <= 0){
        pr_err("logModule: error copy msg"); 
        return;
    }

    filp = filp_open(curFilename, O_WRONLY | O_CREAT | O_APPEND, 0644); 

    if(IS_ERR(filp)){
        handleOpenError(filp, curFilename);
        return; 
    }

    ret = kernel_write(filp, msg, len, &filp->f_pos); 

    if(ret < 0){
        pr_err("logModule: write error");
    }

    filp_close(filp, NULL); 
}

static void logWorkHandler(struct work_struct *work)
{
    unsigned int period;

    writeLogToFile();

    mutex_lock(&moduleMutex);
    period = timeForLog;
    mutex_unlock(&moduleMutex);

    schedule_delayed_work(
        &logWork,
        msecs_to_jiffies(period * 1000)
    );
}
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

    INIT_DELAYED_WORK(&logWork, logWorkHandler);

    schedule_delayed_work(&logWork, msecs_to_jiffies(timeForLog * 1000));

    return error; 
}

static void __exit logModule_exit(void){
    pr_info("logModule: exit\n");

    cancel_delayed_work_sync(&logWork);

    sysfs_remove_file(logModule, &timeForLogAttribute.attr);
    sysfs_remove_file(logModule, &filenameAttribute.attr);
    
    kobject_put(logModule);
}

module_init(logModule_init);
module_exit(logModule_exit);