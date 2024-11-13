#include <iostream>
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
		+"\", \"temperature\": \"" + std::to_string(this->Temperature)
		+"\", \"humidity\": \"" + std::to_string(this->Humidity)
		+"\", \"lights\": [";
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
			info += std::to_string(this->SocketsOn[s]);
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
	Room room("Living Room", 20, 50, {1, 2, 3}, {4, 5, 6}, {true, false, true});
	Room room2("Kitchen", 15, 60, {7, 8, 9}, {10, 11, 12}, {false, true, false});
    Room room3("Bedroom", 25, 40, {13, 14, 15}, {16, 17, 18}, {true, false, true}); 
	std::vector<Room> rooms = {room, room2, room3};
	std::string stat = GetStat(rooms);
	std::cout << stat << std::endl;
	return 0;
}
