#include "homecontrol.hpp"

Room::Room(
    String name, 
    double temperature, 
    double humidity, 
    std::vector<int> lights_on, 
    std::vector<int> curtains_on, 
    std::vector<bool> sockets_on,
    std::vector<bool> fans_on
        ){
        Name = name;
        Temperature = temperature;
        Humidity = humidity;            
        LightsOn = lights_on;
        CurtainsON = curtains_on;
        SocketsOn = sockets_on;
        FansOn = fans_on;
}

Room::Room() {
    Name = "RoomName";
    Temperature = 0.0;
    Humidity = 0.0;
    LightsOn = std::vector<int>();
    CurtainsON = std::vector<int>();
    SocketsOn = std::vector<bool>();
}

RoomController::RoomController(Room room,
    int dht_pin,
    std::vector<int> light_pins,
    std::vector<int> curtain_pins,
    std::vector<int> socket_pins,
    std::vector<String> socket_addrs,
    std::vector<int> fan_pins,
    std::vector<String> fan_addrs) {
        ROOM = room;
        DHT_PIN = dht_pin;
        LIGHT_PINS = light_pins;
        CURTAIN_PINS = curtain_pins;
        SOCKET_PINS = socket_pins;
        SOCKET_ADDRS = socket_addrs;
        FAN_PINS = fan_pins;
        FAN_ADDRS = fan_addrs;
}

RoomController::RoomController() {
    DHT_PIN = -1;
    LIGHT_PINS = std::vector<int>();
    CURTAIN_PINS = std::vector<int>();
    SOCKET_PINS = std::vector<int>();
    SOCKET_ADDRS = std::vector<String>();
    FAN_PINS = std::vector<int>();
    FAN_ADDRS = std::vector<String>();
}

RoomController::RoomController(Room room) {
    ROOM = room;
    DHT_PIN = -1;
    LIGHT_PINS = std::vector<int>();
    CURTAIN_PINS = std::vector<int>();
    SOCKET_PINS = std::vector<int>();
    SOCKET_ADDRS = std::vector<String>();
    FAN_PINS = std::vector<int>();
    FAN_ADDRS = std::vector<String>();
}

JsonEncoder::JsonEncoder(Room room) { ROOM = room; };

String JsonEncoder::Encode() {
    String json = "#{{";
    json += "\"name\":\"" + ROOM.Name + "\",";
    json += "\"temperature\":" + String(ROOM.Temperature) + ",";
    json += "\"humidity\":" + String(ROOM.Humidity) + ",";
    json += "\"lights\":[";
    for (unsigned int i = 0; i < ROOM.LightsOn.size(); i++) {
        json += String(ROOM.LightsOn[i]);
        if (i != ROOM.LightsOn.size() - 1) {
            json += ",";
        }
    }
    json += "],";
    json += "\"curtains\":[";
    for (unsigned int i = 0; i < ROOM.CurtainsON.size(); i++) {
        json += String(ROOM.CurtainsON[i]);
        if (i != ROOM.CurtainsON.size() - 1) {
            json += ",";
        }
    }
    json += "],";
    json += "\"sockets\":[";
    for (unsigned int i = 0; i < ROOM.SocketsOn.size(); i++) {
        json += String(ROOM.SocketsOn[i]);
        if (i != ROOM.SocketsOn.size() - 1) {
            json += ",";
        }
    }
    json += "],";
    json += "\"fans\":[";
    for (unsigned int i = 0; i < ROOM.FansOn.size(); i++) {
        json += String(ROOM.FansOn[i]);
        if (i != ROOM.FansOn.size() - 1) {
            json += ",";
        }
    }
    json += "]";
    json += "}}#";
    return json;
}