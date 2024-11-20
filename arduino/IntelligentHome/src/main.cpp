#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "readcommand.hpp"
#include "homecontrol.hpp"
#include <map>

Room room1(
  String("BedRoom"),
  0.0,
  0.0,
  std::vector<int> (1, 0),
  std::vector<int> (1, 0),
  std::vector<bool> (3, false),
  std::vector<bool> (3, false)
);

Room room2(
  String("LivingRoom"),
  0.0,
  0.0,
  std::vector<int> (1, 0),
  std::vector<int> (1, 0),
  std::vector<bool> (3, false),
  std::vector<bool> (3, false)
);

RoomController roomController1(
  room1,
  8,
  std::vector<int> {3},
  std::vector<int> {5},
  std::vector<int> {1,4,7},
  std::vector<String> {String("000"),String("001")},
  std::vector<int> {1,4,7},
  std::vector<String> {String("010"),String("011")}
);

RoomController roomController2(
  room2,
  10,
  std::vector<int> {6},
  std::vector<int> {9, 11},
  std::vector<int> {1,4,7},
  std::vector<String> {String("100"),String("101")},
  std::vector<int> {1,4,7},
  std::vector<String> {String("110"),String("111")}
);

RoomController controllers[2] = {roomController1, roomController2};

TaskHandle_t StartTask_Handler;

void start_task(void* pvParameters);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting IntelligentHome");

  xTaskCreate(
    start_task,
    "start_task",
    128,
    NULL,
    1,
    &StartTask_Handler);
  vTaskStartScheduler(); 
}

void loop() {
}

void start_task(void* pvParameters) {
  for (;;) {
    Serial.println("Starting read command");
    JsonEncoder encoder(room1);
    String json = encoder.Encode();
    Serial.println(json);
    delay(1000);
  }
}