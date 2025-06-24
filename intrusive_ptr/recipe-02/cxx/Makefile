CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = 
LDLIBS = 
PROGRAM = intrusive_ptr_demo

SOURCES = $(shell ls *.cpp)
HEADERS = $(shell ls *.h)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

.PHONY: all 
all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(PROGRAM) $(OBJECTS) $(LDLIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(PROGRAM)

