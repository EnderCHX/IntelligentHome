#ifndef READCOMMAND_H
#define READCOMMAND_H

#include <Arduino.h>
#include <vector>

// 函数声明
std::vector<String> readCommand(String commandstr);
void decodeCommand(std::vector<String> commands);

#endif
