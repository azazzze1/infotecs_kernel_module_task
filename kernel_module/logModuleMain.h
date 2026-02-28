#ifndef LOG_MODULE_MAIN_H
#define LOG_MODULE_MAIN_H

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

struct log_module_data {
    struct kobject *kobj;
    struct delayed_work work;
    struct mutex mutex;
    int timeForLog;
    char logFilename[256];
    unsigned long logNumber;
};

extern struct log_module_data logModuleData;

extern struct kobj_attribute lm_timeForLogAttribute;
extern struct kobj_attribute lm_filenameAttribute;

void lm_logWorkHandler(struct work_struct *work);

#endif /* LOG_MODULE_MAIN_H */