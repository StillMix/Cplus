#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <sys/stat.h> // Для проверки директорий

using namespace std;

// Функция для проверки наличия команды в системе
bool isCommandAvailable(const string& command) {
    string commandCheck = "which " + command + " > /dev/null 2>&1";
    return system(commandCheck.c_str()) == 0;
}

// Функция для проверки, является ли путь директорией
bool isDirectory(const string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    return false;
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
        command = "locate -r '" + folderName + "$' 2>/dev/null";
    } else if (isCommandAvailable("plocate")) {
        command = "plocate -r '" + folderName + "$' 2>/dev/null";
    } else {
        // Если ни locate, ни plocate нет, предложим установить plocate
        installPlocate();
        command = "plocate -r '" + folderName + "$' 2>/dev/null";
    }

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return foundFolders;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        string folderPath(buffer);
        // Убираем лишние символы новой строки
        if (!folderPath.empty() && folderPath.back() == '\n') {
            folderPath.pop_back();
        }

        // Исключаем папки из кэша браузера и другие ненужные директории
        if (folderPath.find(".cache") == string::npos) {
            // Проверяем, что путь ведет к директории, а не к файлу
            if (isDirectory(folderPath)) {
                foundFolders.push_back(folderPath);
            }
        }
    }

    // Добавляем поиск архивов с таким именем
    command = "locate -r '" + folderName + "\\.zip$' 2>/dev/null";
    pipe = popen(command.c_str(), "r");
    if (pipe) {
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            string zipPath(buffer);
            if (!zipPath.empty() && zipPath.back() == '\n') {
                zipPath.pop_back();
            }
            foundFolders.push_back(zipPath);
        }
        pclose(pipe);
    }

    return foundFolders;
}

void openFolder(const string& folderPath) {
    #if defined(_WIN32) || defined(_WIN64)
        string command = "explorer \"" + folderPath + "\"";
    #else
        // Перенаправляем вывод в /dev/null, чтобы скрыть сообщения
        string command = "xdg-open \"" + folderPath + "\" > /dev/null 2>&1 &";
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
                
                // Задержка перед очисткой экрана
                this_thread::sleep_for(chrono::seconds(2));
                
                // Очищаем экран
                #if defined(_WIN32) || defined(_WIN64)
                    system("cls");
                #else
                    system("clear");
                #endif
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
                
                // Задержка перед очисткой экрана
                this_thread::sleep_for(chrono::seconds(2));
                
                // Очищаем экран
                #if defined(_WIN32) || defined(_WIN64)
                    system("cls");
                #else
                    system("clear");
                #endif
                break;

            case 3:
            #if defined(_WIN32) || defined(_WIN64)
                system("shutdown /r /t 5");
            #else
                cout << "Нужны root-права для перезагрузки. Введите пароль при запросе." << endl;
                system("sudo shutdown -r +1");
            #endif
                cout << "Команда выполнена: Перезагрузка компьютера..." << endl;
                
                // Задержка перед очисткой экрана
                this_thread::sleep_for(chrono::seconds(5));
                
                // Очищаем экран
                #if defined(_WIN32) || defined(_WIN64)
                    system("cls");
                #else
                    system("clear");
                #endif
                break;

            case 4:
                cout << "Выход из программы." << endl;
                return 0;

            case 5: {
                string folderName;
                cout << "Введите название папки для поиска: ";
                cin.ignore(); // Очищаем входной буфер от предыдущего ввода
                getline(cin, folderName); // Используем getline, чтобы считывать имя с пробелами
                
                // Если ввод пустой, сообщаем об ошибке
                if (folderName.empty()) {
                    cout << "Ошибка: имя папки не может быть пустым." << endl;
                    this_thread::sleep_for(chrono::seconds(2));
                    
                    // Очищаем экран
                    #if defined(_WIN32) || defined(_WIN64)
                        system("cls");
                    #else
                        system("clear");
                    #endif
                    break;
                }
                
                // Показываем пользователю, что идет поиск
                cout << "Выполняется поиск папок \"" << folderName << "\"..." << endl;
                
                // Выполняем поиск папок без показа промежуточного процесса пользователю
                cout << "Пожалуйста, подождите..." << endl;
                vector<string> foundFolders = findFolders(folderName);

                if (foundFolders.empty()) {
                    cout << "Папка с таким названием не найдена." << endl;
                    
                    // Задержка, чтобы пользователь смог прочитать сообщение
                    this_thread::sleep_for(chrono::seconds(2));
                    
                    // Очищаем экран
                    #if defined(_WIN32) || defined(_WIN64)
                        system("cls");
                    #else
                        system("clear");
                    #endif
                } else if (foundFolders.size() == 1) {
                    cout << "Единственная найденная папка: " << foundFolders[0] << endl;
                    cout << "Открываем папку..." << endl;
                    
                    // Открываем папку
                    openFolder(foundFolders[0]);
                    
                    // Даем пользователю время увидеть папку
                    this_thread::sleep_for(chrono::seconds(3));
                    
                    // Очищаем экран
                    #if defined(_WIN32) || defined(_WIN64)
                        system("cls");
                    #else
                        system("clear");
                    #endif
                } else {
                    cout << "Найдено несколько папок с таким названием:" << endl;
                    for (size_t i = 0; i < foundFolders.size(); ++i) {
                        cout << i + 1 << ". " << foundFolders[i] << endl;
                    }

                    int choice;
                    cout << "Выберите номер папки для открытия: ";
                    cin >> choice;

                    if (choice > 0 && choice <= foundFolders.size()) {
                        cout << "Открываем папку: " << foundFolders[choice - 1] << endl;
                        openFolder(foundFolders[choice - 1]);
                        
                        // Даем пользователю время увидеть папку
                        this_thread::sleep_for(chrono::seconds(3));
                        
                        // Очищаем экран только после открытия папки
                        #if defined(_WIN32) || defined(_WIN64)
                            system("cls");
                        #else
                            system("clear");
                        #endif
                    } else {
                        cout << "Некорректный выбор." << endl;
                        
                        // Даем пользователю время прочитать сообщение об ошибке
                        this_thread::sleep_for(chrono::seconds(2));
                        
                        // Очищаем экран
                        #if defined(_WIN32) || defined(_WIN64)
                            system("cls");
                        #else
                            system("clear");
                        #endif
                    }
                }
                break;
            }

            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
                
                // Задержка перед очисткой экрана
                this_thread::sleep_for(chrono::seconds(2));
                
                // Очищаем экран
                #if defined(_WIN32) || defined(_WIN64)
                    system("cls");
                #else
                    system("clear");
                #endif
        }
    }

    return 0;
}