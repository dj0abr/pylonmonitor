# make clean ... delete all build files
# make ... create the application

CXX = g++
CXXFLAGS = -Wall -O3 -std=c++0x -Wno-write-strings -Wno-narrowing
LDFLAGS = -pthread -lpthread -lm -lmosquitto -ljansson
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)  # Add dependency files

.PHONY: default clean

default: pylonmonitor

pylonmonitor: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

-include $(DEP)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

clean:
	rm -rf $(OBJ) $(DEP) pylonmonitor

