#include <cstdlib>
#include <iostream>
#include "helper.h"

using namespace std;

void restartComputer() {
    #if defined(_WIN32) || defined(_WIN64)
        system("shutdown /r /t 5");
    #else
        cout << "Нужны root-права для перезагрузки. Введите пароль при запросе." << endl;
        system("sudo shutdown -r +1");
    #endif
    
    showMessage("Команда выполнена: Перезагрузка компьютера...", 5);
}