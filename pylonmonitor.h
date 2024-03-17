#ifndef _PYLONMONITOR_H_
#define _PYLONMONITOR_H_

#include <string>

void exit_program();
char *ownIP();   // this is part of kmclib, but not declared in kmclib.h, so we do it here
void copyFile();

extern std::string myLocalIP;
extern int mqttfifo;

#endif // _PYLONMONITOR_H_
