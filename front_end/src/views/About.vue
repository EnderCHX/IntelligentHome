<script setup>
const code1 = `#include <iostream>
#include <string>
#include <vector>

class Room
{
private:
	std::string Name;
	double Temperature;
	double Humidity;
	std::vector<int> LightsOn;
	std::vector<int> CurtainsON;
	std::vector<bool> SocketsOn;
public:
	Room(std::string name, 
		 double temperature, 
		 double humidity, 
		 std::vector<int> lightsOn,
		 std::vector<int> curtainsOn,
		 std::vector<bool> socketsOn) {
		Name = name;
		Temperature = temperature;
		Humidity = humidity;
		LightsOn = lightsOn;
		CurtainsON = curtainsOn;
		SocketsOn = socketsOn;
	}
	void SetTemperature(double temperature) {
		Temperature = temperature;
	}
	void SetHumidity(double humidity) {
		Humidity = humidity;
	}
	void SetLightsOn(std::vector<int> lightsOn) {
		LightsOn = lightsOn;
	}
	void SetCurtainsON(std::vector<int> curtainsOn) {
		CurtainsON = curtainsOn;
	}
	void SetSocketsOn(std::vector<bool> socketsOn) {
		SocketsOn = socketsOn;
	}

	std::string GetInfo() {
		std::string info = "{ \"name\": \""+ this->Name
		+"\", \"temperature\": " + std::to_string(this->Temperature)
		+", \"humidity\": " + std::to_string(this->Humidity)
		+", \"lights\": [";
		for (int l = 0; l < this->LightsOn.size(); ++l) {
			info += std::to_string(this->LightsOn[l]);
			if (l < this->LightsOn.size() - 1) {
				info += ", ";
			}
		}
		info += "], \"curtains\": [";
		for (int c = 0; c < this->CurtainsON.size(); ++c) {
			info += std::to_string(this->CurtainsON[c]);
			if (c < this->CurtainsON.size() - 1) {
				info += ", ";
				}
		}
		info += "], \"sockets\": [";
		for (int s = 0; s < this->SocketsOn.size(); ++s) {
			if( this->SocketsOn[s] ) {
				info += "true";
			} else {
				info += "false";
			}
			if (s < this->SocketsOn.size() - 1) {
				info += ", ";
			}
		}
		info += "] }";
		return info;
	}
};

std::string GetStat(std::vector<Room> rooms) {
	std::string stat = "{ \"stat\": [";
	for(int i=0;i<rooms.size();++i){
		stat += rooms[i].GetInfo();
		if(i<rooms.size()-1){
			stat += ", ";
		}
	}
	stat += "] }";
	return stat;
}

int main() {
	Room room("Living Room", 20, 50, {1, 2, 3, 4}, {3,4, 5, 6}, {false, true, false, true});
	Room room2("Kitchen", 15, 60, {7, 8, 9}, {10, 11, 12}, {false, true, false});
    Room room3("Bedroom", 25, 40, {13, 14, 15}, {16, 17, 18}, {true, false, true}); 
	std::vector<Room> rooms = {room, room2, room3};
	std::string stat = GetStat(rooms);
	std::cout << stat << std::endl;
	return 0;
}
`

const code2 = `package main

import "fmt"

type Room struct {
	Name        string
	Temperature float64
	Humidity    float64
	LightsOn    []int
	CurtainsON  []int
	SocketsOn   []bool
}

func (r *Room) SetTemperature(temperature float64) {
	r.Temperature = temperature
}

func (r *Room) SetHumidity(humidity float64) {
	r.Humidity = humidity
}

func (r *Room) SetLightsOn(lightsOn []int) {
	r.LightsOn = lightsOn
}

func (r *Room) SetCurtainsON(curtainsOn []int) {
	r.CurtainsON = curtainsOn
}

func (r *Room) SetSocketsOn(socketsOn []bool) {
	r.SocketsOn = socketsOn
}

func (r *Room) GetInfo() string {
	info := fmt.Sprintf("{ \"name\": \"%s\", \"temperature\": %.2f, \"humidity\": %.2f, \"lights\": [", r.Name, r.Temperature, r.Humidity)
	for i, l := range r.LightsOn {
		info += fmt.Sprintf("%d", l)
		if i < len(r.LightsOn)-1 {
			info += ", "
		}
	}
	info += "], \"curtains\": ["
	for i, c := range r.CurtainsON {
		info += fmt.Sprintf("%d", c)
		if i < len(r.CurtainsON)-1 {
			info += ", "
		}
	}
	info += "], \"sockets\": ["
	for i, s := range r.SocketsOn {
		if s {
			info += "true"
		} else {
			info += "false"
		}
		if i < len(r.SocketsOn)-1 {
			info += ", "
		}
	}
	info += "] }"
	return info
}

func GetStat(rooms []Room) string {
	stat := fmt.Sprintf("{ \"stat\": [")
	for i, r := range rooms {
		stat += r.GetInfo()
		if i < len(rooms)-1 {
			stat += ", "
		}
	}
	stat += "] }"
	return stat
}

func main() {
	room := Room{
		Name:        "Living Room",
		Temperature: 20,
		Humidity:    50,
		LightsOn:    []int{1, 2, 3, 4},
		CurtainsON:  []int{3, 4, 5, 6},
		SocketsOn:   []bool{false, true, false, true},
	}
	room2 := Room{
		Name:        "Kitchen",
		Temperature: 15,
		Humidity:    60,
		LightsOn:    []int{7, 8, 9},
		CurtainsON:  []int{10, 11, 12},
		SocketsOn:   []bool{false, true, false},
	}
	room3 := Room{
		Name:        "Bedroom",
		Temperature: 25,
		Humidity:    40,
		LightsOn:    []int{13, 14, 15},
		CurtainsON:  []int{16, 17, 18},
		SocketsOn:   []bool{true, false, true},
	}
	rooms := []Room{room, room2, room3}
	stat := GetStat(rooms)
	fmt.Println(stat)
}

`
</script>
<template>
    <n-flex>
        <n-card>
            <div style="overflow: auto">
                <n-code :code="code1" language="cpp" :show-line-numbers="true" />
            </div>
        </n-card>
        <n-card>
            <div style="overflow: auto">
                <n-code :code="code2" language="go" :show-line-numbers="true" />
            </div>
        </n-card>
    </n-flex>
</template>

<style scoped>
.n-card {
    width: 48vw;
    height: auto;
    padding: 2vw;
}
</style>