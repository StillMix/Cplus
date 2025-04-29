CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Директории
OBJ_DIR = bin/data
BIN_DIR = bin

# Создаем список исходных файлов, заменяя "OpenBrowser.cpp" на "openBrowser.cpp"
SOURCES = main.cpp helpers.cpp menu.cpp openBrowser.cpp TextEditor.cpp \
          RestartComputer.cpp Exit.cpp FolderSearch.cpp OpenFolder.cpp

# Преобразуем исходники в объектные файлы в директории OBJ_DIR
OBJECTS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(BIN_DIR)/computer_helper

all: create_dirs $(EXECUTABLE)

# Создание нужных директорий
create_dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Компиляция cpp файлов в объектные файлы
$(OBJ_DIR)/%.o: %.cpp helper.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean create_dirs