#ifndef _PYLONMONITOR_H_
#define _PYLONMONITOR_H_

#include <string>

void exit_program();
char *ownIP();
void copyFile();

extern std::string myLocalIP;
extern int mqttfifo;
extern int keeprunning;

#endif // _PYLONMONITOR_H_
