#include <string>
#include <vector>

class Room {
    public:
        std::string Name;
        double Temperature;
        double Humidity;
        std::vector<int> LightsOn;
        std::vector<int> CurtainsON;
        std::vector<bool> SocketsOn;
        RoomController* Controller;
};

class RoomController {
    public:
        int DHT_PIN;
        std::vector<int> LIGHT_PINS;
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
};