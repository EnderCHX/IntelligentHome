#ifndef HOMECONTROL_H
#define HOMECONTROL_H


#include <Arduino.h>
#include <vector>

class Room {
    public:
        String Name;
        double Temperature;
        double Humidity;
        std::vector<int> LightsOn;
        std::vector<int> CurtainsON;
        std::vector<bool> SocketsOn;
        std::vector<bool> FansOn;

        Room(
            String name, 
            double temperature, 
            double humidity, 
            std::vector<int> lights_on, 
            std::vector<int> curtains_on, 
            std::vector<bool> sockets_on,
            std::vector<bool> fans_on
        );

        Room();
};

class RoomController {
    private:
        int DHT_PIN;
        std::vector<int> LIGHT_PINS;
        std::vector<int> CURTAIN_PINS;
        std::vector<int> SOCKET_PINS;
        std::vector<String> SOCKET_ADDRS;
        std::vector<int> FAN_PINS;
        std::vector<String> FAN_ADDRS;

    public:
        Room ROOM;
        
        RoomController(
            Room room,
            int dht_pin,
            std::vector<int> light_pins,
            std::vector<int> curtain_pins,
            std::vector<int> socket_pins,
            std::vector<String> socket_addrs,
            std::vector<int> fan_pins,
            std::vector<String> fan_addrs
        );

        RoomController();
        RoomController(Room room);

        bool SetDHTPin(int pin);
        bool SetLightPins(std::vector<int> pins);
        bool SetCurtainPins(std::vector<int> pins);
        bool SetSocketPins(std::vector<int> pins);
        bool SetSocketAddrs(std::vector<String> addrs);
        bool SetFanPins(std::vector<int> pins);
        bool SetFanAddrs(std::vector<String> addrs);

        int GetDHTPin();
        std::vector<int> GetLightPins();
        std::vector<int> GetCurtainPins();
        std::vector<int> GetSocketPins();
        std::vector<String> GetSocketAddrs();
        std::vector<int> GetFanPins();
        std::vector<String> GetFanAddrs();

        bool ReadDHT();
        bool SetLights(std::vector<int> lights);
        bool SetCurtains(std::vector<int> curtains);
        bool SetSockets(std::vector<bool> sockets);
        bool SetFans(std::vector<int> fans);
};

class JsonEncoder {
    public:
        Room ROOM;

        JsonEncoder(Room room);

        String Encode();
};

#endif