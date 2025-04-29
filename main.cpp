#include <iostream>
#include <string>
#include "helper.h"

using namespace std;

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
                openBrowser();
                break;

            case 2:
                openTextEditor();
                break;

            case 3:
                restartComputer();
                break;

            case 4:
                exitProgram();
                return 0;

            case 5:
                openFolderMenu();
                break;

            default:
                showMessage("Неверный выбор. Попробуйте снова.");
        }
    }

    return 0;
}