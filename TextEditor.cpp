#include <cstdlib>
#include "helper.h"

void openTextEditor() {
    #if defined(_WIN32) || defined(_WIN64)
        system("notepad");
    #else
        system("gedit");
    #endif
    
    showMessage("Команда выполнена: Текстовый редактор открыт!", 2);
}