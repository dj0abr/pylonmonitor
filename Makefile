# make clean ... delete all build files
# make ... create the application

CXXFLAGS = -Wall -O3 -std=c++0x -Wno-write-strings -Wno-narrowing
LDFLAGS = -pthread -lpthread -lm -lmosquitto -ljansson
OBJ = pylonmonitor.o readbatt.o fifo.o mqttthread.o serial.o identifySerUSB.o serial_helper.o kmfifo.o helper.o

default: $(OBJ)
	g++ $(CXXFLAGS) -o pylonmonitor $(OBJ) $(LDFLAGS)

clean:
	rm -rf *.o pylonmonitor
