#include "ofxAVSync.h"

void ofxAVSync::setupOnce(){
    if(!begun){
        begin();
    }
}

void ofxAVSync::begin(){
    beginDelay = ofGetElapsedTimeMillis();
    begun = true;
}

void ofxAVSync::logCommand(string commandName) {
    command.setup(commandName);
    commands[getTargetFrameForCurrentTime()] = command;
}

void ofxAVSync::logCommand(string commandName, float val1) {
    command.setup(commandName);
    command.add(val1);
    commands[getTargetFrameForCurrentTime()] = command;
}

void ofxAVSync::logCommand(string commandName, float val1, float val2) {
    command.setup(commandName);
    command.add(val1);
    command.add(val2);
    commands[getTargetFrameForCurrentTime()] = command;
}

void ofxAVSync::logCommand(string commandName, float val1, float val2, float val3) {
    command.setup(commandName);
    command.add(val1);
    command.add(val2);
    command.add(val3);
    commands[getTargetFrameForCurrentTime()] = command;
}

void ofxAVSync::logCommand(string commandName, float val1, float val2, float val3, float val4) {
    command.setup(commandName);
    command.add(val1);
    command.add(val2);
    command.add(val3);
    command.add(val4);
    commands[getTargetFrameForCurrentTime()] = command;
}

void ofxAVSync::logCommand(string commandName, float val1, float val2, float val3, float val4, float val5) {
    command.setup(commandName);
    command.add(val1);
    command.add(val2);
    command.add(val3);
    command.add(val4);
    command.add(val5);
    commands[getTargetFrameForCurrentTime()] = command;
}

void ofxAVSync::logCommand(string commandName, float val1, float val2, float val3, float val4, float val5, float val6) {
    command.setup(commandName);
    command.add(val1);
    command.add(val2);
    command.add(val3);
    command.add(val4);
    command.add(val5);
    command.add(val6);
    commands[getTargetFrameForCurrentTime()] = command;
}

vector<ofxAVCommand> ofxAVSync::getCommandsForCurrentFrame() {
    vector<ofxAVCommand> commandsForCurrentFrame;
    for(auto it = commands.cbegin(); it != commands.cend(); /* it++ */) {
        if(ofGetFrameNum() >= it->first) {
            commandsForCurrentFrame.push_back(it->second);
            commands.erase(it++);    // or "it = m.erase(it)" since C++11
        } else {
            ++it;
        }
    }
    return commandsForCurrentFrame;
}

uint64_t ofxAVSync::getElapsedTime(){
    return ofGetElapsedTimeMillis() - beginDelay;
}

bool ofxAVSync::hasBegun(){
    return begun;
}

int ofxAVSync::getTargetFrameForCurrentTime() {
    //Equivalent to: round((getElapsedTime() / 1000) * 60)
    return round(getElapsedTime() * 0.06);
}