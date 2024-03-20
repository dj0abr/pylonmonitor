/*
* Pylontech Battery Monitor for Raspberry PI
* ==========================================
* Author: DJ0ABR
*
*   (c) DJ0ABR
*   www.dj0abr.de

pylonmain.cpp
=============
everything starts here in main()

Hardware:
=========
Raspberry PI Zero W,
this RPI board has the lowest power consumption (about 50mA at 5V)
and it's single core is fast enough for this application.
All other Raspberry PIs and other SBCs can also be used for this program.

Libraries used:
===============
libjansson
libmosquitto
libboost-dev
or simply run "prepare" to install all required libraries

Raspi supporting WLAN (RPI3, 4 and Zero-W) need remapping of serial port
========================================================================
1. Switch-off serial login shell (important !!!, double check)
    sudo raspi-config
    InterfaceOptions - Serial Port:
    login shell: NO
    hardware enabled: YES
2. map primary serial port to connector pins
    sudo nano /boot/config.txt and add at the end:
    # Switch serial ports
    dtoverlay=miniuart-bt
3. Reboot and check if ok:
    ls -l /dev/serial*
    serial0 must map to ttyAMA0
4. to use ttyAMA0 the program must be running as root   

Description of the various threads
==================================

main thread ... initialization, loop for diagnostics only
pylon thread .. reads the pylontech battery (or multiple 
                batteries, autodetection) and sends the 
                result to the mqtt thread as well as to the webserver
mqtt thread ... sends the battery values to an MQTT broker
web thread .... prepares the battery values for the webserver
                handles configuration via config webpage

*/

#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include "pylonmonitor.h"
#include "readbatt.h"
#include "mqttthread.h"
#include "helper.h"
#include "config.h"

std::string myLocalIP;

int keeprunning = 1;

// FIFO used to send messages from readbatt to mqtt threads
#define MQTTFIFO_MAXNUM     20
#define MQTTFIFO_MAXLEN     1000

int main(int argc, char *argv[])
{
    // Welcom message
    printf("Pylontech Battery Monitor ...\npress Ctrl-C to exit\n");

    // check if pylonmon is already running
    if(isRunning("pylonmain") == 1) {
        printf("pylonmain is already running. Do not start twice. Exiting\n");
        exit(0);
    }

    // catch Ctrl-C and call exit routine
    install_signal_handler(exit_program); // mainly used to catch Ctrl-C

    // copy index.html file
    copyFile();

    // create default config, if it does not already exist
    saveDefaultConfigToJson();

    // read the local IP address which is sent with an MQTT status message
    myLocalIP = std::string(ownIP());

    // start the MQTT handler thread
    MQTTTHREAD mqttthread;

    // start the battery monitor thread
    READBATT readbatt;

    time_t lastMQTTConnectionTime = 0;
    while(keeprunning)
    {
        // connect or reconnect MQTT (not faster than every 2 seconds)
        if(mqttthread.isConnected() == false || mqtt_changed) {
            time_t now = time(NULL);
            if (now - lastMQTTConnectionTime >= 2) {
                mqttthread.init();
                lastMQTTConnectionTime = now;
                mqtt_changed = false;
            }
        }

        mqttthread.publish();   // publish new messages
        usleep(10000);
    }
    
    exit_program();
    return 0;
}

void exit_program()
{
    printf("close program\n");
    // do cleanup
    keeprunning = 0;
    sleep(1);   // give threads a chance to exit
    printf("exit program\n");
    exit(0);
}

void copyFile() 
{
    const std::string sourcePath = "./html/index.html";
    const std::string destinationPath = "/var/www/html/index.html";

    std::ifstream src(sourcePath, std::ios::binary);
    std::ofstream dst(destinationPath, std::ios::binary);

    // Check if the source file and destination file stream objects are good to go
    if (!src.good() || !dst.good()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    dst << src.rdbuf();

    src.close();
    dst.close();
}

int unusedfunction()
{
    int a=1;
    return a;
}