#include <Arduino.h>
#include "homecontrol.hpp"

Room rooms[2];

void setRooms() {
  rooms[0].Name = "Living Room";
  rooms[0].Controller->switchLight();
}

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}