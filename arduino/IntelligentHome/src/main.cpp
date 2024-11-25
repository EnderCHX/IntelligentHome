#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "homecontrol.hpp"
#include <SimpleDHT.h>

Room *room1 = new Room(1, float(21.0), float(16.0), 2, new int[2] {0, 0}, 2, new int[2] {0, 0}, 8, new bool[8] {false, false, false, false, false, false, false, false});

int *socket_pins = new int[3] { 8, 4, 12 }; //数据、移位、锁存
byte socket_state = 0b00000000;

RoomController roomController = RoomController(room1, 14, 15, new int[2] {3, 5}, new int[2] {6, 9}, socket_pins, &socket_state, new byte[8] {0, 1, 2, 3, 4, 5, 6, 7});

TaskHandle_t ReceiveCommandAndSendStatus_Handler;
TaskHandle_t AutoLightCurtain_Handler;



void receive_command_and_send_status(void *pvParameters);
void auto_light_curtain(void *pvParameters);


void parseValues(const char* input, int* output, int maxSize);
void parseValues(const char* input, bool* output, int maxSize);


void setup()
{
    Serial.begin(115200);

    //start 初始化插座状态
    pinMode(socket_pins[0], OUTPUT); //数据
    pinMode(socket_pins[1], OUTPUT); //移位
    pinMode(socket_pins[2], OUTPUT); //锁存

    digitalWrite(socket_pins[2], LOW);
    shiftOut(socket_pins[0], socket_pins[1], MSBFIRST, socket_state);
    digitalWrite(socket_pins[2], HIGH);
    //end 初始化插座状态

    xTaskCreate(
            receive_command_and_send_status,
            "receive_command",
            288,
            NULL,
            1,
            &ReceiveCommandAndSendStatus_Handler);
    
    xTaskCreate(
            auto_light_curtain,
            "auto_light_curtain",
            128,
            NULL,
            2,
            &AutoLightCurtain_Handler);
    

    vTaskStartScheduler();
}

void loop()
{
}

void receive_command_and_send_status(void* pvParameters) {

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1000);

    unsigned long lastTime = millis();
    const int BUFFER_SIZE = 128;
    char buffer[BUFFER_SIZE];  // 用于存储串口数据

    for (;;) {
        // 每隔 1000 ms 执行一次
        if (millis() - lastTime < 1000) {            
            continue;
        }
        lastTime = millis();
        

        // 检查是否有串口数据
        if (Serial.available()) {
            // 读取串口数据
            size_t len = Serial.readBytesUntil('\n', buffer, BUFFER_SIZE - 1);
            buffer[len] = '\0';  // 添加字符串结束符

            // 分割主命令字符串
            char* parts[3];
            int partCount = 0;
            char* token = strtok(buffer, "_");
            while (token != NULL && partCount < 3) {
                parts[partCount++] = token;
                token = strtok(NULL, "_");
            }

            // 初始化设备数组
            int lights[MAX_LIGHTS] = {0};
            int curtains[MAX_CURTAINS] = {0};
            bool sockets[MAX_SOCKETS] = {0};

            // 解析各部分数据
            if (partCount > 0) parseValues(parts[0], lights, MAX_LIGHTS);
            if (partCount > 1) parseValues(parts[1], curtains, MAX_CURTAINS);
            if (partCount > 2) parseValues(parts[2], sockets, MAX_SOCKETS);

            // 调用控制器方法
            roomController.SetLights(lights);
            roomController.SetCurtains(curtains);
            roomController.SetSockets(sockets);
        }

        roomController.UpdateTemperatureAndHumidityAndLDRValue();

        Serial.print('#');
        Serial.print('{');
        Serial.print(roomController.ROOM->RoomID);
        Serial.print('_');
        Serial.print(roomController.ROOM->Temperature);
        Serial.print('_');
        Serial.print(roomController.ROOM->Humidity);
        Serial.print('_');
        for (int i = 0; i < roomController.ROOM->LightCount; ++i) {
            Serial.print(roomController.ROOM->LightsOn[i]);
            if (i < roomController.ROOM->LightCount - 1) {
                Serial.print(',');
            }
        }
        Serial.print('_');
        for (int i = 0; i < roomController.ROOM->CurtainCount; ++i) {
            Serial.print(roomController.ROOM->CurtainsON[i]);
            if (i < roomController.ROOM->CurtainCount - 1) {
                Serial.print(',');
            }
        }
        Serial.print('_');
        for (int i = 0; i < roomController.ROOM->SocketCount; ++i) {
            Serial.print(roomController.ROOM->SocketsOn[i]);
            if (i < roomController.ROOM->SocketCount - 1) {
                Serial.print(',');
            }
        }
        Serial.print('}');
        Serial.print('#');
        Serial.println();

        vTaskDelayUntil(&xLastWakeTime, xFrequency);

    }
}

void auto_light_curtain(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(3000);

    for (;;) {
        roomController.AutoLight(200, 1000);
        roomController.AutoCurtain(200, 1000);

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


void parseValues(const char* input, int* output, int maxSize) {
    // 分割字符串并将值存入数组
    char* token = strtok((char*)input, ",");
    int count = 0;

    while (token != NULL && count < maxSize) {
        output[count++] = atoi(token);  // 转换为整数
        token = strtok(NULL, ",");
    }

    // 如果未完全填充数组，补零
    while (count < maxSize) {
        output[count++] = 0;
    }
}

void parseValues(const char* input, bool* output, int maxSize) {
    // 分割字符串并将值存入数组
    char* token = strtok((char*)input, ",");
    int count = 0;

    while (token != NULL && count < maxSize) {
        output[count++] = atoi(token);  // 转换为整数
        token = strtok(NULL, ",");
    }

    // 如果未完全填充数组，补零
    while (count < maxSize) {
        output[count++] = 0;
    }
}