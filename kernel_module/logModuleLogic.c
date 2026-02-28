#include "logModuleMain.h"

static long handleOpenError(struct file *filp, const char *filename){
    long err_code = PTR_ERR(filp);

    switch (err_code){
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
    pr_info("logModule: start to log with N = %lu\n", logModuleData.logNumber);

    struct file *filp;
    char msg[128];
    ssize_t ret;
    char curFilename[256];
    unsigned long curLogNumber;

    mutex_lock(&logModuleData.mutex);
    strncpy(curFilename, logModuleData.logFilename, sizeof(curFilename) - 1);
    curFilename[sizeof(curFilename) - 1] = '\0';
    curLogNumber = ++logModuleData.logNumber;
    mutex_unlock(&logModuleData.mutex);

    int len = snprintf(msg, sizeof(msg), "Hello from kernel module (%lu)\n", curLogNumber);
    if (len <= 0){
        pr_err("logModule: error copy msg");
        return;
    }

    filp = filp_open(curFilename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (IS_ERR(filp)){
        handleOpenError(filp, curFilename);
        return;
    }

    ret = kernel_write(filp, msg, len, &filp->f_pos);
    if (ret < 0)
        pr_err("logModule: write error");

    filp_close(filp, NULL);
}

void lm_logWorkHandler(struct work_struct *work){
    unsigned int period;

    writeLogToFile();

    mutex_lock(&logModuleData.mutex);
    period = logModuleData.timeForLog;
    mutex_unlock(&logModuleData.mutex);

    schedule_delayed_work(&logModuleData.work,
                          msecs_to_jiffies(period * 1000));
}