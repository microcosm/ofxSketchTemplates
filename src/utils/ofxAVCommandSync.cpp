#include "ofxAVCommandSync.h"

void ofxAVCommandSync::begin(){
    if(!begun){
        beginDelay = ofGetElapsedTimeMillis();
        begun = true;
    }
}

void ofxAVCommandSync::logCommand(string command) {
    int targetFrame = getTargetFrameForCurrentTime();
    commands[targetFrame] = command;
}

vector<string> ofxAVCommandSync::getCommandsForCurrentFrame() {
    vector<string> commandsForCurrentFrame;
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

int ofxAVCommandSync::getTargetFrameForCurrentTime() {
    //Equivalent to: round((getElapsedTime() / 1000) * 60)
    return round(getElapsedTime() * 0.06);
}

uint64_t ofxAVCommandSync::getElapsedTime(){
    return ofGetElapsedTimeMillis() - beginDelay;
}