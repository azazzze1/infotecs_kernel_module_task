#include "logModuleMain.h"

MODULE_AUTHOR("Shushkov Egor");
MODULE_DESCRIPTION("LogModule for Infotecs");
MODULE_LICENSE("GPL");

struct log_module_data logModuleData = {
    .timeForLog = 5,
    .logFilename = "/var/tmp/test_module/log.txt",
    .logNumber = 0,
};

static int __init logModule_init(void)
{
    int error = 0;

    pr_info("logModule: init\n");

    mutex_init(&logModuleData.mutex);

    logModuleData.kobj = kobject_create_and_add("logModule", kernel_kobj);
    if (!logModuleData.kobj)
        return -ENOMEM;

    error = sysfs_create_file(logModuleData.kobj, &lm_timeForLogAttribute.attr);
    if (error){
        pr_info("Failed to create timeLog file\n");
        kobject_put(logModuleData.kobj);
        return error;
    }

    error = sysfs_create_file(logModuleData.kobj, &lm_filenameAttribute.attr);
    if (error){
        pr_info("Failed to create filename file\n");
        sysfs_remove_file(logModuleData.kobj, &lm_timeForLogAttribute.attr);
        kobject_put(logModuleData.kobj);
        return error;
    }

    INIT_DELAYED_WORK(&logModuleData.work, lm_logWorkHandler);
    schedule_delayed_work(&logModuleData.work,
                          msecs_to_jiffies(logModuleData.timeForLog * 1000));

    return error;
}

static void __exit logModule_exit(void)
{
    pr_info("logModule: exit\n");

    cancel_delayed_work_sync(&logModuleData.work);

    sysfs_remove_file(logModuleData.kobj, &lm_timeForLogAttribute.attr);
    sysfs_remove_file(logModuleData.kobj, &lm_filenameAttribute.attr);

    kobject_put(logModuleData.kobj);
    mutex_destroy(&logModuleData.mutex);
}

module_init(logModule_init);
module_exit(logModule_exit);