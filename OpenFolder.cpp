#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "helper.h"

using namespace std;

void openFolderMenu() {
    string folderName;
    cout << "Введите название папки для поиска: ";
    cin.ignore();
    getline(cin, folderName);
    
    // Если ввод пустой, сообщаем об ошибке
    if (folderName.empty()) {
        showMessage("Ошибка: имя папки не может быть пустым.", 0);
        return;
    }
    
    // Показываем пользователю, что идет поиск
    cout << "Выполняется поиск папок \"" << folderName << "\"..." << endl;
    cout << "Пожалуйста, подождите..." << endl;
    
    vector<string> foundFolders = findFolders(folderName);

    if (foundFolders.empty()) {
        showMessage("Папка с таким названием не найдена.", 0);
    } else if (foundFolders.size() == 1) {
        cout << "Единственная найденная папка: " << foundFolders[0] << endl;
        cout << "Открываем папку..." << endl;
        
        // Открываем папку
        openFolder(foundFolders[0]);
        
        // Даем пользователю время увидеть папку
        this_thread::sleep_for(chrono::seconds(3));
        clearScreen();
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
            clearScreen();
        } else {
            showMessage("Некорректный выбор.", 0);
        }
    }
}