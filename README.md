# LogModule for Linux Kernel 6.12

Модуль ядра Linux, периодически записывающий логи в файл, и пользовательская утилита для настройки параметров.

## Требования
- Ядро Linux версии 6.12
- GCC, Make, linux-headers

## Установка и запуск
1. **Скачать репозиторий**
```bash
git clone git@github.com:azazzze1/infotecs_kernel_module_task.git
cd infotecs_kernel_module_task
```

2. **Создать директорию для логов по умолчанию:**
```bash
sudo mkdir -p /var/tmp/test_module
```

3. **Загрузить модуль:**
```bash
cd kernel_module
make
sudo make load
```

4. **Загрузить пользовательскую программу:**
```bash
cd ../program/
make
```

5. **Проверка загрузки:**
```bash
sudo dmesg --follow
```

## Использование

### Настройка с помощью пользовательского приложения
```bash
# Изменение времени таймера (в секундах):
    sudo ./logModuleController --time 10
    sudo ./logModuleController -t 10

# Установить новый путь до файла логов:
    sudo ./logModuleController --file /var/tmp/test_module/mylog.txt
    sudo ./logModuleController -f /var/tmp/test_module/mylog.txt

# Комбинированная команда: 
    sudo ./logModuleController -t 5 -f /var/tmp/test_module/log.txt
```

## Выгрузка модуля
```bash
cd kernel_module
sudo make unload
```


## Автор
Shushkov Egor

## Лицензия
GPL
