#ifndef LOG_MODULE_CONTROLLER_H
#define LOG_MODULE_CONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <errno.h>

#define SYSFS_PATH "/sys/kernel/logModule/"
#define SYSFS_TIME_FILE SYSFS_PATH "timeForLog"
#define SYSFS_FILENAME_FILE SYSFS_PATH "logFilename"

#define MAX_TIME_LENGTH 32
#define MAX_FILENAME_LENGTH 256


struct module_cfg{
    int setTime;
    int setFilename;
    char timeVal[MAX_TIME_LENGTH];
    char filenameVal[MAX_FILENAME_LENGTH];
};

int writeToFile(const char* path, const char* val);
void cmdInfo(const char *prog); 
int checkModuleLoaded(void);

int parseArguments(int argc, char* argv[], struct module_cfg* cfg);
int applyAttributes(const struct module_cfg *cfg);

#endif /* LOG_MODULE_CONTROLLER_H */
