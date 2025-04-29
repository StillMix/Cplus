#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <vector>

// Вспомогательные функции
bool isCommandAvailable(const std::string& command);
bool isDirectory(const std::string& path);
void installPlocate();

// Основные функции меню
void openBrowser();
void openTextEditor();
void restartComputer();
void exitProgram();
void openFolderMenu();

// Функции для поиска папок
std::vector<std::string> findFolders(const std::string& folderName);
void openFolder(const std::string& folderPath);

// Интерфейс пользователя
void showMenu();
void clearScreen();
void showMessage(const std::string& message, int delaySeconds = 2);

#endif