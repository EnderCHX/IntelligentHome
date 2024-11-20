#include "readcommand.hpp"


std::vector<String> readCommand(String commandstr) {

    String command;
    std::vector<String> commands;

    bool start = false; //开始写入命令
    unsigned int index = 0;

    for (index = 0; index < commandstr.length(); ++index) {

        if (index + 1 < commandstr.length() &&commandstr[index] == '#' && commandstr[index+1] == '{') {
            index+=2;
            start = true;
        }

        if (index + 1 < commandstr.length() &&commandstr[index] == '}' && commandstr[index+1] == '#') {
            start = false;
            commands.push_back(command);
            command = "";
            index++;
        }

        if (start) {
            command += commandstr[index];
        }
    }
    return commands;
}


void decodeCommand(std::vector<String> commands) {
    for (String command : commands) {

        std::vector<String> parts;
        String temp = "";
        for (unsigned int i = 0; i < command.length(); ++i) {
            if (command[i] == ' ') {
                if (temp.length() != 0) {
                    parts.push_back(temp);  
                } 
                temp = "";
            } else {
                temp += command[i];
            }
        }
        if (temp.length() > 0) {
            parts.push_back(temp);
        }

        if (parts.size() < 3) {
            Serial.println("Invalid command format.");
            continue;
        }

        // 提取命令名称、类型和参数
        String action = parts[0];  // 动作名称，例如 `set`
        String name = parts[1];  // 名称部分，例如 `Living Room`
        String type = parts[2];  // 类型，例如 `lights`

        std::vector<int> params;

        for (unsigned int i = 3; i < parts.size(); ++i) {
            params.push_back(parts[i].toInt());
        }

        Serial.println("parts");
        for (auto part : parts) {
            Serial.println(part);
        }

        Serial.print("params: [");
        for (int param : params) {
            Serial.print(param);
            Serial.print(", ");
        }
        Serial.print("] ");
        Serial.println(params.size());
    }
}