#include <string>
using string = std::string;

void send_to_mqtt(const string& topic, const string& payload);
bool read_from_readbatt(string& topic, string& payload);