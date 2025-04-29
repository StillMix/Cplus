#include <cstdlib>
#include "helper.h"

void openBrowser() {
    #if defined(_WIN32) || defined(_WIN64)
        system("start https://ya.ru");
    #else
        system("xdg-open https://ya.ru");
    #endif
    
    showMessage("Команда выполнена: Браузер открыт!", 2);
}