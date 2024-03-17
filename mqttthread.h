#ifndef _MQTTTHREAD_H_
#define _MQTTTHREAD_H_

#include <mosquitto.h>
#include <string>

using string = std::string;

class MQTTTHREAD {
public:
    MQTTTHREAD();
    ~MQTTTHREAD();
    bool init();
    void publish();

    string BROKER_IP = "192.168.10.89";

private:
    static void on_connect(struct mosquitto *mosq, void *userdata, int result);
    static void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
    static void on_disconnect(struct mosquitto *mosq, void *userdata, int rc);
    struct mosquitto *mqttclient;
    int BROKER_PORT = 1883;
};

#endif // _MQTTTHREAD_H_