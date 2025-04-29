#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> // Make sure this is included for the remove function

using namespace std;

// Функция для проверки наличия команды в системе
bool isCommandAvailable(const string& command) {
    string commandCheck = "which " + command + " > /dev/null 2>&1";
    return system(commandCheck.c_str()) == 0;
}

// Функция для установки plocate (если locate не найден)
void installPlocate() {
    cout << "Инструмент 'locate' не найден. Устанавливаем plocate..." << endl;
    system("sudo apt update && sudo apt install -y plocate");
    // После установки обновляем базу данных
    cout << "Обновляем базу данных plocate..." << endl;
    system("sudo updatedb");
    cout << "Установка завершена. База данных обновлена." << endl;
}

// Функция для поиска папок
vector<string> findFolders(const string& folderName) {
    vector<string> foundFolders;
    string command;

    if (isCommandAvailable("locate")) {
        command = "locate -r '.*" + folderName + "' 2>/dev/null";
    } else if (isCommandAvailable("plocate")) {
        command = "plocate -r '.*" + folderName + "' 2>/dev/null";
    } else {
        // Если ни locate, ни plocate нет, предложим установить plocate
        installPlocate();
        command = "plocate -r '.*" + folderName + "' 2>/dev/null";
    }

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return foundFolders;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        string folderPath(buffer);
        // Убираем лишние символы новой строки - ИСПРАВЛЕНО:
        if (!folderPath.empty() && folderPath.back() == '\n') {
            folderPath.pop_back();
        }

        // Исключаем папки из кэша браузера и другие ненужные директории
        if (folderPath.find(".cache") == string::npos) {
            foundFolders.push_back(folderPath);
        }
    }

    pclose(pipe);
    return foundFolders;
}

void openFolder(const string& folderPath) {
    #if defined(_WIN32) || defined(_WIN64)
        string command = "explorer \"" + folderPath + "\"";
    #else
        string command = "xdg-open \"" + folderPath + "\"";
    #endif
    system(command.c_str());
}

void showMenu() {
    cout << "==== Компьютерный помощник ====" << endl;
    cout << "1. Открыть браузер" << endl;
    cout << "2. Открыть текстовый редактор" << endl;
    cout << "3. Перезагрузить компьютер" << endl;
    cout << "4. Выход" << endl;
    cout << "5. Открыть папку" << endl;
    cout << "Выберите команду (1-5): ";
}

int main() {
    int select;
    string input;

    while (true) {
        showMenu();

        while (true) {
            cin >> input;

            if (input.length() == 1 && input[0] >= '1' && input[0] <= '5') {
                select = input[0] - '0'; 
                break;
            } else {
                cout << "Ошибка! Пожалуйста, введите цифру от 1 до 5: ";
            }
        }

        switch (select) {
            case 1:
#if defined(_WIN32) || defined(_WIN64)
                system("start https://ya.ru");
#else
                system("xdg-open https://ya.ru");
#endif
                cout << "Команда выполнена: Браузер открыт!" << endl;
                break;

            case 2:
#if defined(_WIN32) || defined(_WIN64)
                system("notepad");
#else
                if (system("which gedit > /dev/null") == 0) {
                    system("gedit");
                } else {
                    system("nano");
                }
#endif
                cout << "Команда выполнена: Текстовый редактор открыт!" << endl;
                break;

            case 3:
#if defined(_WIN32) || defined(_WIN64)
                system("shutdown /r /t 5");
#else
                cout << "Нужны root-права для перезагрузки. Введите пароль при запросе." << endl;
                system("sudo shutdown -r +1");
#endif
                cout << "Команда выполнена: Перезагрузка компьютера..." << endl;
                break;

            case 4:
                cout << "Выход из программы." << endl;
                return 0;

            case 5: {
                string folderName;
                cout << "Введите название папки для поиска: ";
                cin >> folderName;

                vector<string> foundFolders = findFolders(folderName);

                if (foundFolders.empty()) {
                    cout << "Папка с таким названием не найдена." << endl;
                } else if (foundFolders.size() == 1) {
                    cout << "Единственная найденная папка: " << foundFolders[0] << endl;
                    openFolder(foundFolders[0]);
                } else {
                    cout << "Найдено несколько папок с таким названием:" << endl;
                    for (size_t i = 0; i < foundFolders.size(); ++i) {
                        cout << i + 1 << ". " << foundFolders[i] << endl;
                    }

                    int choice;
                    cout << "Выберите номер папки для открытия: ";
                    cin >> choice;

                    if (choice > 0 && choice <= foundFolders.size()) {
                        openFolder(foundFolders[choice - 1]);
                    } else {
                        cout << "Некорректный выбор." << endl;
                    }
                }
                break;
            }

            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
        }

        // Задержка перед очисткой экрана и показом нового меню
        this_thread::sleep_for(chrono::seconds(2));  // Пауза 2 секунды

        // Очищаем экран в зависимости от ОС, только если папка была открыта
        #if defined(_WIN32) || defined(_WIN64)
            system("cls");
        #else
            system("clear");
        #endif
    }

    return 0;
}