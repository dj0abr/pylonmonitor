#include <string>

using std::string;

bool saveDefaultConfigToJson();
bool readConfigFromJson();
string getMQTTtopic();

extern string ssid;
extern string password;
extern string mqttBrokerIP;
extern string brokerusername;
extern string brokerpassword;
extern string publishTopic;
extern string responseTopic;
extern string locationTopic;
extern string deviceTopic;
extern bool mqtt_changed;
