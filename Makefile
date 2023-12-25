.PHONY: test doxy clean
CXX = g++
CXXFLAGS = -Wall -lboost_program_options -lcrypto++ -lUnitTest++ -lboost_filesystem
TARGET = Utest
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = $(wildcard *.h)
test: $(TARGET) b.txt ll.txt
doxy:
	doxygen
b.txt:
	@echo "login:password" >b.txt
ll.txt:
	touch ll.txt
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -o $(TARGET) 

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)