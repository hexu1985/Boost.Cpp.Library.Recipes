CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -DBOOST_STACKTRACE_LINK -I${HOME}/local/boost/include 
LDFLAGS = -L${HOME}/local/boost/lib
LDLIBS = -lboost_stacktrace_backtrace -ldl -lbacktrace -ldl

SOURCES = $(shell ls *.cpp)
PROGRAMS = $(patsubst %.cpp,%,$(SOURCES))

.PHONY: all 
all: $(PROGRAMS)

$(PROGRAMS): %: %.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(PROGRAMS)

