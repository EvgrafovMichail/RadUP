# URead

Программа для чтения данных с радиолокатора кругового обзора.

## Как собрать

**Требования:**

- g++ v12.0.1;    
- Python interpretor v3.11.2;

**Сборка:**

1. Установить библиотеки Python в глобальное вертуальное окружение (пока так):
    ```cmd
    pip install -r dev-requirements.txt
    ```   
2. Переходим в папку src, где лежит файл `task.py`:
    ```cmd
    cd src
    ```
3. Запускаем сборку с помощью утилиты `invoke`:
    ```cmd
    invoke build
    ```
4. Запускаем исполняемый файл `main.exe`;
