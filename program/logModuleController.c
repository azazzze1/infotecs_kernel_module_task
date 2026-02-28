#include "logModuleController.h"

static struct option longOpt[] = {
    {"time", required_argument, 0, 't'},
    {"file", required_argument, 0, 'f'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0}
};


int writeToFile(const char* path, const char* val){
    FILE *file = fopen(path, "w");
    if(!file){
        fprintf(stderr, "Error: failed to open file %s\n", path);
        return -1;
    }

    if(fprintf(file, "%s", val) < 0){
        fprintf(stderr, "Error: failed to write to fiel %s\n", path);
        fclose(file);
        return -1;
    }

    fclose(file); 
    return 0; 
}

void cmdInfo(const char *prog){
    printf("Usage: %s [OPTIONS]\n", prog);
    printf("Options:\n");
    printf("\t-t, --time <seconds>      Set timer period in seconds\n");
    printf("\t-f, --file <filename>     Set log filename (full path)\n");
    printf("\t-h, --help                Show this help\n");
}

int checkModuleLoaded(){
    struct stat st;
    if (stat(SYSFS_PATH, &st) != 0){
        fprintf(stderr, "Error: logModule not loaded or path %s not found\n", SYSFS_PATH);
        return -1;
    }
    return 0;
}

int parseArguments(int argc, char* argv[], struct module_cfg* cfg){
    int opt, option_index = 0;
    memset(cfg, 0, sizeof(*cfg));

    while((opt = getopt_long(argc, argv, "t:f:h", longOpt, &option_index)) != -1){
        switch(opt){
            case 't':
                if(strlen(optarg) >= MAX_TIME_LENGTH){
                    fprintf(stderr, "Error: time value too long\n");
                    return -1;
                }
                snprintf(cfg->timeVal, sizeof(cfg->timeVal), "%s", optarg);
                cfg->setTime = 1;
                break;
            case 'f':
                if(strlen(optarg) >= MAX_FILENAME_LENGTH){
                    fprintf(stderr, "Error: filename value too long\n");
                    return -1;
                }
                snprintf(cfg->filenameVal, sizeof(cfg->filenameVal), "%s", optarg);
                cfg->setFilename = 1;
                break;
            case 'h': 
                cmdInfo(argv[0]);
                exit(0); 
            default:
                cmdInfo(argv[0]);
                return -1; 
        }
    }

    if(!cfg->setTime && !cfg->setFilename){
        cmdInfo(argv[0]);
        return -1; 
    }

    return 0; 
}

int applyAttributes(const struct module_cfg *cfg){
    if(cfg->setTime){
        printf("Setting time period to %s seconds..,\n", cfg->timeVal);
        if(writeToFile(SYSFS_TIME_FILE, cfg->timeVal)){
            return -1; 
        } 
        printf("Done!\n");
    }

    if(cfg->setFilename){
        printf("Setting filename to %s..,\n", cfg->filenameVal);
        if(writeToFile(SYSFS_FILENAME_FILE, cfg->filenameVal)){
            return -1; 
        } 
        printf("Done!\n");
    }

    printf("Configuration updated: success!!!\n");
    return 0; 
}


