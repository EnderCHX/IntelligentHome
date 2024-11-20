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
        RoomController* Controller;
        Room(String name, double temperature, double humidity, RoomController* controller) {
            Name = name;
            Temperature = temperature;
            Humidity = humidity;
            LightsOn = std::vector<int>();
            CurtainsON = std::vector<int>();
            SocketsOn = std::vector<bool>();
            Controller = controller;
        }
};

class RoomController {
    public:
        int DHT_PIN;
        int LIGHT_PIN[3];
        std::vector<int> LIGHT_ADDRS;
        std::vector<int> CURTAIN_PINS;
        std::vector<int> FAN_PINS;

        RoomController(int dht_pin, 
                        std::vector<int> light_pins,
                        std::vector<int> curtain_pins, 
                        std::vector<int> fan_pins
        ) {
            DHT_PIN = dht_pin;
            LIGHT_PINS = light_pins;
            CURTAIN_PINS = curtain_pins;
            FAN_PINS = fan_pins;
        }

        void switchLight() {
            
        }
};