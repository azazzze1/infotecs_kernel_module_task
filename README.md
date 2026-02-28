# LogModule for Linux Kernel 6.12
**Задание на стажировку в ИнфоТеКС на должность "Разработчик Linux" — Шушков Егор**

Модуль ядра Linux, периодически записывающий логи в файл, и пользовательская утилита для настройки параметров.


## Требования
- Ядро Linux версии 6.12
- GCC, Make, linux-headers

## Установка и запуск
1. **Скачать репозиторий**
```bash
git clone https://github.com/azazzze1/infotecs_kernel_module_task.git
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

| Опция | Описание | Значение по умолчанию |
|-------|----------|----------------------|
| `-t, --time SEC` | Интервал записи лога (сек) | 5 |
| `-f, --file PATH` | Путь к файлу лога | `/var/tmp/test_module/log.txt` |
| `-h, --help` | Показать справку | — |

### Пример:

```bash
sudo ./logModuleController -t 5 -f /var/tmp/test_module/log.txt
```

> [!WARNING]
> Полный путь до файла должен включать название файла, а также директория должна существовать

## Выгрузка модуля
```bash
cd kernel_module
sudo make unload
```