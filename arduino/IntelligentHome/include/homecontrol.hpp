#ifndef HOMECONTROL_H
#define HOMECONTROL_H


#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <SimpleDHT.h>
#include <Servo.h>

#define MAX_LIGHTS 10       // 最大灯光数量
#define MAX_CURTAINS 10     // 最大窗帘数量
#define MAX_SOCKETS 10      // 最大插座数量

class Room {
    public:
        int RoomID;
        float Temperature;
        float Humidity;
        int LightCount;
        int LightsOn[MAX_LIGHTS];
        int CurtainCount;
        int CurtainsON[MAX_CURTAINS];
        int SocketCount;
        bool SocketsOn[MAX_SOCKETS];
        int LDRValue;

        Room(
            int id,
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
        SimpleDHT11 Dht;
        int LIGHT_PINS[MAX_LIGHTS];
        int CURTAIN_PINS[MAX_CURTAINS];
        Servo Servos[MAX_CURTAINS];
        int SOCKET_PINS[3];
        byte *SOCKET_STATE;
        byte SOCKET_ADDRS[MAX_SOCKETS];
        int LDR_PIN;
        bool IF_LIGHT_AUTO[MAX_LIGHTS];
        bool IF_CURTAIN_AUTO[MAX_CURTAINS];
    public:
        Room *ROOM;
        
        RoomController(
            Room *room,
            int dht_pin,
            int ldr_pin,
            int light_pins[],
            int curtain_pins[],
            Servo servos[],
            int socket_pins[2],
            byte *socket_state,
            byte socket_addrs[]
        );

        void Init();
        // void DHTBegin();

        void UpdateTemperatureAndHumidityAndLDRValue();

        void AutoLight(int min, int max);

        void AutoCurtain(int min, int max);

        int GetDHTPin();
        int *GetCurtainPins();
        

        void SetLights(int lights[MAX_LIGHTS]);
        void SetCurtains(int curtains[MAX_CURTAINS]);
        void SetSockets(bool sockets[MAX_SOCKETS]);
};


// void JsonPrint(RoomController *controllers, int count);
// void PrintStatus(RoomController* rc, int count);

#endif