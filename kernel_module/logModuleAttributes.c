#include "logModuleMain.h"

/* Создание аттрибута для периода времени */

static ssize_t timeForLogShow(struct kobject *kobj, struct kobj_attribute *attr,
                              char *buf){
    unsigned int val;

    mutex_lock(&logModuleData.mutex);
    val = logModuleData.timeForLog;
    mutex_unlock(&logModuleData.mutex);

    return sprintf(buf, "%u\n", val);
}

static ssize_t timeForLogStore(struct kobject *kobj,
                               struct kobj_attribute *attr,
                               const char *buf, size_t count){
    unsigned int val;

    if (kstrtouint(buf, 10, &val) != 0)
        return -EINVAL;

    if (val == 0)
        return -EINVAL;

    mutex_lock(&logModuleData.mutex);
    logModuleData.timeForLog = val;
    mutex_unlock(&logModuleData.mutex);

    return count;
}

struct kobj_attribute lm_timeForLogAttribute = __ATTR(timeForLog, 0660, timeForLogShow, (void *)timeForLogStore);


/* Создание аттрибута для полного пути файла для записи */

static ssize_t filenameShow(struct kobject *kobj, struct kobj_attribute *attr,
                            char *buf){
    ssize_t val;

    mutex_lock(&logModuleData.mutex);
    val = sprintf(buf, "%s\n", logModuleData.logFilename);
    mutex_unlock(&logModuleData.mutex);

    return val;
}

static ssize_t filenameStore(struct kobject *kobj, struct kobj_attribute *attr,
                             const char *buf, size_t count){
    size_t len;

    if (count >= sizeof(logModuleData.logFilename))
        return -EINVAL;

    mutex_lock(&logModuleData.mutex);

    len = strnlen(buf, count);
    strncpy(logModuleData.logFilename, buf, len);
    logModuleData.logFilename[len] = '\0';

    if (len > 0 && logModuleData.logFilename[len - 1] == '\n')
        logModuleData.logFilename[len - 1] = '\0';

    mutex_unlock(&logModuleData.mutex);

    return count;
}

struct kobj_attribute lm_filenameAttribute = __ATTR(logFilename, 0660, filenameShow, (void *)filenameStore);