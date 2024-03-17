# make clean ... delete all build files
# make ... create the application

CXXFLAGS = -Wall -O3 -std=c++0x -Wno-write-strings -Wno-narrowing -I /usr/include/freetype2 -I /usr/include/kmclib
LDFLAGS = -pthread -lpthread -lm -lkmclib -lfreetype -lrt -lmosquitto -ljansson
OBJ = pylonmonitor.o readbatt.o fifo.o mqttthread.o

default: $(OBJ)
	g++ $(CXXFLAGS) -o pylonmonitor $(OBJ) $(LDFLAGS)

clean:
	rm -rf *.o pylonmonitor
