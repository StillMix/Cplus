#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <sys/stat.h>
#include "helper.h"

using namespace std;

// Проверка наличия команды в системе
bool isCommandAvailable(const string& command) {
    string commandCheck = "which " + command + " > /dev/null 2>&1";
    return system(commandCheck.c_str()) == 0;
}

// Проверка, является ли путь директорией
bool isDirectory(const string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    return false;
}

// Установка plocate
void installPlocate() {
    cout << "Инструмент 'locate' не найден. Устанавливаем plocate..." << endl;
    system("sudo apt update && sudo apt install -y plocate");
    cout << "Обновляем базу данных plocate..." << endl;
    system("sudo updatedb");
    cout << "Установка завершена. База данных обновлена." << endl;
}

// Очистка экрана
void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

// Показ сообщения с задержкой
void showMessage(const string& message, int delaySeconds) {
    cout << message << endl;
    this_thread::sleep_for(chrono::seconds(delaySeconds));
    clearScreen();
}