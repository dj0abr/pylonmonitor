/*
this thread handles mqtt messages
*/

#include <thread>
#include "mqttthread.h"
#include "fifo.h"
#include "kmclib.h"

MQTTTHREAD::MQTTTHREAD()
{
    while(!init()) {
        sleep(2);
        printf("retrying to connect to MQTT broker\n");
    }
}

bool MQTTTHREAD::init()
{
    mosquitto_lib_init();
    mqttclient = mosquitto_new("Pylonmonitor", true, NULL);
    if (!mqttclient)
    {
        printf("Failed to create MQTT client\n");
        return false;
    }

    // set callbacks
    mosquitto_connect_callback_set(mqttclient, MQTTTHREAD::on_connect);
    mosquitto_message_callback_set(mqttclient, MQTTTHREAD::on_message);
    mosquitto_disconnect_callback_set(mqttclient, MQTTTHREAD::on_disconnect);

    // connect to the MQTT broker
    printf("connecting to Broker: %s:%d\n",BROKER_IP.c_str(), BROKER_PORT);
    int rc = mosquitto_connect(mqttclient, BROKER_IP.c_str(), BROKER_PORT, 20);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        printf("Failed to connect to MQTT Broker: %s:%d, return code %d\n", BROKER_IP.c_str(), BROKER_PORT,rc);
        mosquitto_destroy(mqttclient);
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
        return false;
    }
    return true;
}

MQTTTHREAD::~MQTTTHREAD() 
{
    mosquitto_disconnect(mqttclient);
    mosquitto_loop_stop(mqttclient, true);
    mosquitto_destroy(mqttclient);
    mosquitto_lib_cleanup();
}

void MQTTTHREAD::on_connect(struct mosquitto *mosq, void *userdata, int result)
{
    if (result == 0) {
        printf("connected to MQTT Broker\n");

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

    sleep(1);   // do not overwhelm the broker in case of errors

    while(true) {
        int reconnect_result = mosquitto_reconnect_async(mosq);
        if(reconnect_result == MOSQ_ERR_SUCCESS)
            break;

        printf("Reconnection attempt failed with code %d\n", reconnect_result);
        sleep(2);
    }
}

void MQTTTHREAD::publish()
{
    string topic;
    string payload;

    bool dataavail = read_from_readbatt(topic, payload);
    if(dataavail) {
        printf("MQTT publish <%s>,<%s>\n",topic.c_str(),payload.c_str());
        int ret = 0;
        if(payload.length() == 0)
            ret = mosquitto_publish(mqttclient, NULL, topic.c_str(), 0, NULL, 0, false);
        else
            ret = mosquitto_publish(mqttclient, NULL, topic.c_str(), payload.length(), payload.c_str(), 0, false);
        if (ret != MOSQ_ERR_SUCCESS)
        {
            printf("Failed to publish message <%s>. Return code: %d\n", topic.c_str(), ret);
        }
    }
}

