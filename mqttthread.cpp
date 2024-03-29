/*
this thread handles mqtt messages
*/

#include <thread>
#include <unistd.h>
#include "mqttthread.h"
#include "fifo.h"
#include "config.h"

bool MQTTTHREAD::connected = false;

MQTTTHREAD::MQTTTHREAD()
{
}

bool MQTTTHREAD::init()
{
    cleanup();

    getMQTTtopic();
    
    mosquitto_lib_init();
    mqttclient = mosquitto_new("Pylonmonitor", true, NULL);
    if (!mqttclient)
    {
        printf("Failed to create MQTT client\n");
        mqttclient = nullptr;
        return false;
    }

    if(brokerusername.length() > 0) {
        printf("set username and password\n");
        // Set username and password for authentication
        int set_credentials = mosquitto_username_pw_set(mqttclient, brokerusername.c_str(), brokerpassword.c_str());
        if (set_credentials != MOSQ_ERR_SUCCESS)
        {
            printf("Failed to set MQTT username and password\n");
            mosquitto_destroy(mqttclient);
            mqttclient = nullptr;
            return false;
        }
    }

    // set callbacks
    mosquitto_connect_callback_set(mqttclient, MQTTTHREAD::on_connect);
    mosquitto_message_callback_set(mqttclient, MQTTTHREAD::on_message);
    mosquitto_disconnect_callback_set(mqttclient, MQTTTHREAD::on_disconnect);

    // connect to the MQTT broker
    string BROKER_IP = "1.2.3.4";
    if(mqttBrokerIP.length() >= 7) {
        BROKER_IP = mqttBrokerIP;
    }

    printf("connecting to Broker: %s:%d\n",BROKER_IP.c_str(), BROKER_PORT);
    int rc = mosquitto_connect(mqttclient, BROKER_IP.c_str(), BROKER_PORT, 20);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        printf("Failed to connect to MQTT Broker: %s:%d, return code %d\n", BROKER_IP.c_str(), BROKER_PORT,rc);
        mosquitto_destroy(mqttclient);
        mqttclient = nullptr;
        return false;
    }
    printf("connected to Broker: %s:%d\n",BROKER_IP.c_str(), BROKER_PORT);

    // start the network loop thread
    rc = mosquitto_loop_start(mqttclient);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        printf("Failed to start MQTT network loop thread, return code %d\n", rc);
        mosquitto_disconnect(mqttclient);
        mosquitto_destroy(mqttclient);
        mqttclient = nullptr;
        return false;
    }
    return true;
}

MQTTTHREAD::~MQTTTHREAD() 
{
    cleanup();
}

void MQTTTHREAD::cleanup() 
{
    if (mqttclient != nullptr) {
        printf("MQTT cleanup\n");
        mosquitto_disconnect(mqttclient);
        mosquitto_loop_stop(mqttclient, true);
        mosquitto_destroy(mqttclient);
        mosquitto_lib_cleanup();
        mqttclient = nullptr; // Set to nullptr after cleanup
    }
}

bool MQTTTHREAD::isConnected()
{
    return connected;
}

void MQTTTHREAD::on_connect(struct mosquitto *mosq, void *userdata, int result)
{
    if (result == 0) {
        printf("connected to MQTT Broker\n");
        connected = true;

/*        printf("subscribe to topics\n");
        int ret = mosquitto_subscribe(mqttclient, NULL, topic.c_str(), QOS);
        if(ret)
        {
            printf("ERROR subscribing to topic: %s errocode:%d\n",topic.c_str(),ret);
        }*/
    }
    else
    {
        printf("Failed to connect to MQTT Broker, return code %d\n", result);
    }
}

// receive a message from the MQTT broker
void MQTTTHREAD::on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    // If message is null, return
    if(message == nullptr) return;
    if(message->topic == nullptr) return;

    string topic = string(message->topic);
    if(message->payload != nullptr && message->payloadlen > 0) {
        string payload = string(static_cast<const char*>(message->payload), message->payloadlen);
    }
    
    //do something with topic and payload
}

void MQTTTHREAD::on_disconnect(struct mosquitto *mosq, void *userdata, int rc)
{
    printf("\nDisconnected from MQTT Broker. Reconnect ...\n");
    connected = false;
}

void MQTTTHREAD::publish()
{
    string topic;
    string payload;

    bool dataavail = read_from_readbatt(topic, payload);
    if(dataavail) {
        //printf("MQTT publish <%s>,<%s>\n",topic.c_str(),payload.c_str());
        int ret = 0;
        if(payload.length() == 0)
            ret = mosquitto_publish(mqttclient, NULL, topic.c_str(), 0, NULL, 0, false);
        else
            ret = mosquitto_publish(mqttclient, NULL, topic.c_str(), payload.length(), payload.c_str(), 0, false);
        if (ret != MOSQ_ERR_SUCCESS)
        {
            //printf("Failed to publish message <%s>. Return code: %d\n", topic.c_str(), ret);
            connected = false;
            return;
        }
    }
}

