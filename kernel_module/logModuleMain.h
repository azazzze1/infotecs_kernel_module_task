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

/**
 * struct log_module_data - Основная структура данных модуля
 * @kobj: Kobject для создания аттрибутов 
 * @work: Структура отложенной работы для реализации таймера 
 * @mutex: Мьютекс для защиты общих данных от гонки
 * @timeForLog: Период записи логов в секундах
 * @logFilename: Полный путь к файлу для записи логов
 * @logNumber: Счётчик записанных строк (порядковый номер)
 * 
 * Эта структура хранит всё состояние модуля. 
 */
struct log_module_data {
    struct kobject *kobj;
    struct delayed_work work;
    struct mutex mutex;
    int timeForLog;
    char logFilename[256];
    unsigned long logNumber;
};

/* Глобальный экземпляр структуры данных модуля */
extern struct log_module_data logModuleData;

/* Атрибуты для настройки параметров */
extern struct kobj_attribute lm_timeForLogAttribute;
extern struct kobj_attribute lm_filenameAttribute;

/**
 * lm_logWorkHandler() - Функция для отработки отложенных задач (таймер)
 * @work: Указатель на структуру work_struct
 * 
 * Вызывается по таймеру в контексте процесса. Записывает строку
 * в лог-файл и перепланирует следующее выполнение.
 */
void lm_logWorkHandler(struct work_struct *work);

#endif /* LOG_MODULE_MAIN_H */