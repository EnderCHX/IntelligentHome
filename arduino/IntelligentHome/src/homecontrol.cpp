#include "homecontrol.hpp"

Room::Room(
    int id,
    float temperature,
    float humidity,
    int lightcount,
    int lightson[],
    int curtaincount,
    int curtainson[],
    int socketcount,
    bool socketson[]
){
    RoomID = id;
    Temperature = temperature;
    Humidity = humidity;
    LightCount = lightcount;
    CurtainCount = curtaincount;
    SocketCount = socketcount;
    LDRValue = 0;

    for(int i = 0; i < lightcount; i++) {
        LightsOn[i] = lightson[i];
    }
    for(int i = 0; i < curtaincount; i++) {
        CurtainsON[i] = curtainson[i];
    }
    for(int i = 0; i < socketcount; i++) {
        SocketsOn[i] = socketson[i];
    }
}

Room::Room() {
    RoomID = millis();
    Temperature = 0;
    Humidity = 0;
    LightCount = 0;
    CurtainCount = 0;
    SocketCount = 0;
    LDRValue = 0;

}

RoomController::RoomController(
    Room *room,
    int dht_pin,
    int ldr_pin,
    int light_pins[],
    int curtain_pins[],
    Servo servo[],
    int socket_pins[3],
    byte *socket_state,
    byte socket_addrs[]
) {
    ROOM = room;
    DHT_PIN = dht_pin;
    LDR_PIN = ldr_pin;
    SOCKET_PINS[0] = socket_pins[0];
    SOCKET_PINS[1] = socket_pins[1];
    SOCKET_PINS[2] = socket_pins[2];
    SOCKET_STATE = socket_state;
    Dht = SimpleDHT11(DHT_PIN);
    
    for(int i = 0; i < room->LightCount; i++) {
        LIGHT_PINS[i] = light_pins[i];
        IF_LIGHT_AUTO[i] = false;
    }
    for(int i = 0; i < room->CurtainCount; i++) {
        Servos[i] = servo[i];
        CURTAIN_PINS[i] = curtain_pins[i];
        IF_CURTAIN_AUTO[i] = false;
    }
    for(int i = 0; i < room->SocketCount; i++) {
        SOCKET_ADDRS[i] = socket_addrs[i];
    }
}

void RoomController::Init() {
    for (int i = 0; i < ROOM->LightCount; i++) {
        pinMode(LIGHT_PINS[i], OUTPUT);
    }

    for (int i = 0; i < ROOM->CurtainCount; i++) {
        Servos[i].attach(CURTAIN_PINS[i]);
        Servos[i].write(0);
    }
}

void RoomController::UpdateTemperatureAndHumidityAndLDRValue() {
    // byte data[40];
    // Dht.read2(&ROOM->Temperature, &ROOM->Humidity, data);
    ROOM->LDRValue = analogRead(LDR_PIN);
}

int RoomController::GetDHTPin() {
    return DHT_PIN;
}

int * RoomController::GetCurtainPins() {
    return CURTAIN_PINS;
}

void RoomController::SetLights(int lights[MAX_LIGHTS]){
    for(int i = 0; i < ROOM->LightCount; i++) {
        ROOM->LightsOn[i] = lights[i];
        if (ROOM->LightsOn[i] <= 100) {
            IF_LIGHT_AUTO[i] = false;
            analogWrite(LIGHT_PINS[i], (ROOM->LightsOn[i]) * 255 / 100);
        } else {
            IF_LIGHT_AUTO[i] = true;
        }

    }
}

void RoomController::SetCurtains(int curtains[MAX_CURTAINS]){
    for(int i = 0; i < ROOM->CurtainCount; i++) {
        ROOM->CurtainsON[i] = curtains[i];
        if (ROOM->CurtainsON[i] <= 100) {
            IF_CURTAIN_AUTO[i] = false;
            Servos[i].write(int(180 * ROOM->CurtainsON[i] / 100));
        } else {
            IF_CURTAIN_AUTO[i] = true;
        }
    }
}

void RoomController::AutoLight(int min, int max) {
    for (int i = 0; i < ROOM->LightCount; i++) {
        if (!IF_LIGHT_AUTO[i]) {
            continue;
        }
        if (ROOM->LDRValue > max) {
            analogWrite(LIGHT_PINS[i], 0);
        }
        if (ROOM->LDRValue < min) {
            analogWrite(LIGHT_PINS[i], 255);
        }
        if (ROOM->LDRValue >= min && ROOM->LDRValue <= max) {
            analogWrite(LIGHT_PINS[i], int(100 - (ROOM->LDRValue - min) / ((max - min) / 100)) );
        }
    }
}


void RoomController::AutoCurtain(int min, int max) {
    for (int i = 0; i < ROOM->CurtainCount; i++) {
        if (!IF_CURTAIN_AUTO[i]) {
            continue;
        }
        if (ROOM->LDRValue > max) {
            Servos[i].write(180);
        }
        if (ROOM->LDRValue < min) {
            Servos[i].write(0);
        }
        if (ROOM->LDRValue >= min && ROOM->LDRValue <= max) {
            Servos[i].write(int(180 * (ROOM->LDRValue - min) / ((max - min) / 100)));
        }
    }
}


void RoomController::SetSockets(bool sockets[MAX_SOCKETS]){
    for(int i = 0; i < ROOM->SocketCount; i++) {
        ROOM->SocketsOn[i] = sockets[i];
        byte addr = SOCKET_ADDRS[i];
        if (ROOM->SocketsOn[i]) {
            *SOCKET_STATE |= (1 << addr);
        } else {
            *SOCKET_STATE &= ~(1 << addr);
        }
    }
    digitalWrite(SOCKET_PINS[2], LOW);
    shiftOut(SOCKET_PINS[0], SOCKET_PINS[1], MSBFIRST, *SOCKET_STATE);
    digitalWrite(SOCKET_PINS[2], HIGH);
}
