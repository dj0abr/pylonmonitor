#ifndef _READBATT_H_
#define _READBATT_H_

#include <string>

using string = std::string;

#define MAXRXBUFLEN 2100
#define MAXBATTNUMBER   (64+1)  // +1 because the first batt has No 1 (not 0)
#define CELLNUMBER      15      // number of cells per battery

typedef struct {
    int cellNumber;
    double voltage;
    double current;
    double temperature;
    char state[20];
    char voltState[20];
    char currState[20];
    char tempState[20];
    int soc;
    double coulomb;
    char balance;
} BatteryCell;

class READBATT
{
public:
    READBATT();

    string mqttTopic = "kmpub/Solaranlage/Pylontech Akku Monitor";

private:
    void serial_printf(const char *format, ...);
    void batteryhandler_thread();
    void loop_pylon();
    string shrink(char *text);
    void processBatData();
    void publishBattdata();
    void sendJson(int mode, char *name);
    void displayCells();
    string convertPylonDataToJson();

    enum PYLONSTATE {
        PYLON_SEARCH,   // check if a pylontech battery is available
        PYLON_ACK,      // read the response to the search message
        PYLON_REQUEST,  // request data from the next battery number (beginning at 1)
        PYLON_READ,     // read data from the battery
    };

    int pylonState = PYLON_SEARCH;      // state machine's actual state
    int battnumber = 0;                 // number of installed batteries (automatic detection)
    int battnum = 1;                    // Pylontech battery currently in process (first battery = 1)
    char pylon_rxbuf[MAXRXBUFLEN];      // read buffer for information received from the battery
    int rxidx = 0;                      // index pointer for pylon_rxbuf
    BatteryCell cells[MAXBATTNUMBER][CELLNUMBER]; // array to hold all battery data
};

#endif // _READBATT_H_