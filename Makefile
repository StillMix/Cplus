CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

SOURCES = main.cpp helpers.cpp menu.cpp OpenBrowser.cpp TextEditor.cpp \
          RestartComputer.cpp Exit.cpp FolderSearch.cpp OpenFolder.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = computer_helper

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp helper.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean