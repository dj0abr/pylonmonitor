/*
static FIFOs accessible from all processes

uses: libboost-dev library
*/

#include <boost/lockfree/spsc_queue.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include "fifo.h"

using string = std::string;

struct Message {
    string topic;
    string payload;
    Message(const string& t1, const string& t2) : topic(t1), payload(t2) {}
};

constexpr size_t queue_size = 10000;
boost::lockfree::spsc_queue<Message, boost::lockfree::capacity<queue_size>> mqttqueue;

void send_to_mqtt(const string& topic, const string& payload)
{
    mqttqueue.push(Message(topic,payload));
}

bool read_from_readbatt(string& topic, string& payload)
{
    Message message("", "");

    bool ret = mqttqueue.pop(message);
    if(ret) {
        topic = message.topic;
        payload = message.payload;
    }

    return ret;
}