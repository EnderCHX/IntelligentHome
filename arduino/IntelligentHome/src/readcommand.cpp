#include <Arduino.h>
#include <Regexp.h>

String * readCommand(String command) {
    char *pattern = "#\\{(.*?)\\}#";
    MatchState ms;
    char buffer[command.length() + 1];
    command.toCharArray(buffer, sizeof(buffer));
    ms.Target(buffer);

    unsigned int offset = 0;
    char result[command.length()/3];
    String commands[3];

    while (ms.Match(pattern, offset) == REGEXP_MATCHED) {
        ms.GetCapture(result, 0);
        commands[0] = result;
        offset = ms.MatchStart + ms.MatchLength; // 更新偏移量
    }
    return commands;
}

/*
set room1 lights 100 10 20
set room1 curtain 50 10 20
set room1 sockets false true false
*/
void decodeCommand(String commands[]) {
    for (String c : commands) {
        
    }
}