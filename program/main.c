#include "logModuleController.h"

int main(int argc, char* argv[]){
    struct module_cfg cfg;

    if(checkModuleLoaded()){
        return 1;
    }

    if(parseArguments(argc, argv, &cfg)){
        return 1; 
    }

    if(applyAttributes(&cfg)){
        return 1; 
    }

    return 0; 
}