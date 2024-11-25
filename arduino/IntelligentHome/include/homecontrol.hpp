#ifndef HOMECONTROL_H
#define HOMECONTROL_H


#include <Arduino.h>
#include <DHT.h>


#define MAX_LIGHTS 10       // 最大灯光数量
#define MAX_CURTAINS 10     // 最大窗帘数量
#define MAX_SOCKETS 10      // 最大插座数量

class Room {
    public:
        String Name;
        float Temperature;
        float Humidity;
        int LightCount;
        int LightsOn[MAX_LIGHTS];
        int CurtainCount;
        int CurtainsON[MAX_CURTAINS];
        int SocketCount;
        bool SocketsOn[MAX_SOCKETS];

        Room(
            String name,
            float temperature,
            float humidity,
            int lightcount,
            int lightson[],
            int curtaincount,
            int curtainson[],
            int socketcount,
            bool socketson[]
        );
        Room();
};

class RoomController {
    private:
        int DHT_PIN;
        DHT *Dht;
        int LIGHT_PINS[MAX_LIGHTS];
        int CURTAIN_PINS[MAX_CURTAINS];
        int SOCKET_PINS[3];
        byte *SOCKET_STATE;
        byte SOCKET_ADDRS[MAX_SOCKETS];

    public:
        Room *ROOM;
        
        RoomController(
            Room *room,
            int dht_pin,
            int light_pins[],
            int curtain_pins[],
            int socket_pins[2],
            byte *socket_state,
            byte socket_addrs[]
        );

        void Init();
        void DHTBegin();

        float GetTemperature();
        float GetHumidity();

        bool SetDHTPin(int pin);
        bool SetLightPins(int* pins);
        bool SetCurtainPins(int* pins);
        bool SetSocketPins(int* pins);
        bool SetSocketAddrs(String* addrs);
        bool SetFanPins(int* pins);
        bool SetFanAddrs(String* addrs);

        int GetDHTPin();
        int* GetLightPins();
        int* GetCurtainPins();
        int* GetSocketPins();
        String* GetSocketAddrs();
        
        DHT* GetDht();

        void SetLights(int lights[MAX_LIGHTS]);
        void SetCurtains(int curtains[MAX_CURTAINS]);
        void SetSockets(bool sockets[MAX_SOCKETS]);
};


void JsonPrint(RoomController *controllers, int count);
void PrintStatus(RoomController* rc, int count);

#endif