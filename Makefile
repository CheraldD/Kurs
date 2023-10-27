.PHONY: all clean
# Имя компилятора
CXX = g++

# Флаги компилятора
CXXFLAGS = -Wall -lboost_program_options

# Имя исполняемого файла
TARGET = server

# Исходные файлы
SOURCES = main.cpp base.cpp communicator.cpp ui.cpp logger.cpp

# Объектные файлы
OBJECTS = $(SOURCES:.cpp=.o)

# Зависимости на заголовочные файлы
DEPS = base.h ui.h communicator.h logger.h

# Цель по умолчанию
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(TARGET) 

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)