#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "homecontrol.hpp"
#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>

Room *room1 = new Room(String("BedRoom"), float(21.0), float(16.0), 2, new int[2] {0, 0}, 2, new int[2] {0, 0}, 8, new bool[8] {false, false, false, false, false, false, false, false});

int *socket_pins = new int[3] { 7, 4, 12 }; //数据、移位、锁存
byte socket_state = 0b00000000;
unsigned long last_time = millis();

RoomController roomController = RoomController(room1, 2, new int[2] {3, 5}, new int[2] {6, 9}, socket_pins, &socket_state, new byte[8] {0, 1, 2, 3, 4, 5, 6, 7});


TaskHandle_t SendStatus_Handler;
TaskHandle_t GetTemperature_Handler;

void send_status(void *pvParameters);
void get_temperature(void *pvParameters);


void setup()
{
    Serial.begin(9600);
    Serial.println("Starting IntelligentHome");


    //start 初始化插座状态
    pinMode(socket_pins[0], OUTPUT); //数据
    pinMode(socket_pins[1], OUTPUT); //移位
    pinMode(socket_pins[2], OUTPUT); //锁存

    digitalWrite(socket_pins[2], LOW);
    shiftOut(socket_pins[0], socket_pins[1], MSBFIRST, socket_state);
    digitalWrite(socket_pins[2], HIGH);
    //end 初始化插座状态

    

    xTaskCreate(
            send_status,
            "send_status",
            192,
            NULL,
            1,
            &SendStatus_Handler);

    xTaskCreate(
            get_temperature,
            "get_temperature",
            128,
            NULL,
            0,
            &GetTemperature_Handler);

    vTaskStartScheduler();
}

void loop()
{
}


void send_status(void *pvParameters)
{
    Serial.println("Starting send status");
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(2000);
    for(;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        

        if (Serial.available()) {
            String commandstr = Serial.readStringUntil('\n');
            Serial.println(commandstr);
            String parts[3];

            int count = 0;

            for (unsigned int i = 0; i < commandstr.length(); i++) {
                if (commandstr[i] == '_') {
                    count++;
                    continue;
                }
                Serial.print(commandstr[i]);
                parts[count] += commandstr.charAt(i);
            }
            
            int lights[MAX_LIGHTS] = {0};
            int curtains[MAX_CURTAINS] = {0};
            bool sockets[MAX_SOCKETS] = {0};

            String temp = "";
            count = 0;
            for (unsigned i = 0; i < parts[0].length(); ++i) {
                if (parts[0][i] == ',') {
                    lights[count] = temp.toInt();
                    count++;
                    temp = "";
                    continue;
                }
                temp += parts[0].charAt(i);
            }
            if (temp.length() > 0) {
                lights[count] = temp.toInt();
            }

            count = 0;
            temp = "";
            for (unsigned int i = 0; i < parts[1].length(); ++i) {
                if (parts[1][i] == ',') {
                    curtains[count] = temp.toInt();
                    count++;
                    temp = "";
                    continue;
                }
                temp += parts[1].charAt(i);
            }
            if (temp.length() > 0) {
                curtains[count] = temp.toInt();
            }

            count = 0;
            temp = "";
            for (unsigned int i = 0; i < parts[2].length(); ++i) {
                if (parts[2][i] == ',') {
                    sockets[count] = temp.toInt();
                    count++;
                    temp = "";
                    continue;
                }
                temp += parts[2].charAt(i);
            }
            if (temp.length() > 0) {
                sockets[count] = temp.toInt();
            }

            roomController.SetLights(lights);
            roomController.SetCurtains(curtains);
            roomController.SetSockets(sockets);
        }

        Serial.print("#{");
        Serial.print(roomController.ROOM->Name);
        Serial.print("_");
        Serial.print(roomController.ROOM->Temperature);
        Serial.print("_");
        Serial.print(roomController.ROOM->Humidity);
        Serial.print("_");
        for (int i = 0; i < roomController.ROOM->LightCount; ++i) {
            Serial.print(roomController.ROOM->LightsOn[i]);
            if (i < roomController.ROOM->LightCount - 1) {
                Serial.print(",");
            }
        }
        Serial.print("_");
        for (int i = 0; i < roomController.ROOM->CurtainCount; ++i) {
            Serial.print(roomController.ROOM->CurtainsON[i]);
            if (i < roomController.ROOM->CurtainCount - 1) {
                Serial.print(",");
            }
        }
        Serial.print("_");
        for (int i = 0; i < roomController.ROOM->SocketCount; ++i) {
            Serial.print(roomController.ROOM->SocketsOn[i]);
            if (i < roomController.ROOM->SocketCount - 1) {
                Serial.print(",");
            }
        }
        Serial.print("}#");
        Serial.println();
    }
}

void get_temperature( void *pvParameters ) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1000);
    roomController.DHTBegin();
    for (;;) {
        vTaskDelay(roomController.GetDht()->);

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}