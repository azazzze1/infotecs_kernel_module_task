#ifndef LOG_MODULE_CONTROLLER_H
#define LOG_MODULE_CONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <errno.h>

/** Пути к sysfs интерфейсу модуля */
#define SYSFS_PATH "/sys/kernel/logModule/"
#define SYSFS_TIME_FILE SYSFS_PATH "timeForLog"
#define SYSFS_FILENAME_FILE SYSFS_PATH "logFilename"

/** Максимальная длина значений */
#define MAX_TIME_LENGTH 10
#define MAX_FILENAME_LENGTH 256

/**
 * struct module_config - Структура конфигурации модуля
 * @setTime: Флаг установки периода
 * @setFilename: Флаг установки имени файла
 * @timeVal: Значение периода в секундах
 * @filenameVal: Полный путь к файлу логов
 */
struct module_cfg{
    int setTime;
    int setFilename;
    char timeVal[MAX_TIME_LENGTH];
    char filenameVal[MAX_FILENAME_LENGTH];
};

/**
 * writeToFile() - Изменение значения аттрибута
 * благодаря записи в файл sysfs.  
 * 
 * @path: Полный путь до файла аттрибута
 * @val: Новое значение аттрибута
 * 
 * @return - [int]: 0 при успехе, -1 при ошибке. 
 */
int writeToFile(const char* path, const char* val);

/**
 * cmdInfo() - Вывод справки по использованию
 * @prog: Имя программы (argv[0])
 */
void cmdInfo(const char *prog); 

/**
 * checkModuleLoaded() - Проверка загружен ли модуль
 * 
 * Проверяет наличие директории /sys/kernel/logModule/
 * 
 * @return 0 если модуль загружен, -1 если нет
 */
int checkModuleLoaded();

/**
 * parseArguments() - Парсинг аргументов командной строки
 * @argc: Количество аргументов
 * @argv: Массив аргументов
 * @config: Структура для хранения конфигурации
 * 
 * @return 0 при успехе, -1 при ошибке
 * 
 * Поддерживает короткие (-t, -f) и длинные (--time, --file) опции.
 */
int parseArguments(int argc, char* argv[], struct module_cfg* cfg);

/**
 * applyConfiguration() - Применение конфигурации к модулю
 * @config: Структура с конфигурацией
 * 
 * @return 0 при успехе, -1 при неудаче
 */
int applyAttributes(const struct module_cfg *cfg);

#endif /* LOG_MODULE_CONTROLLER_H */
