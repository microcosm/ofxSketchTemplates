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

void ofxAVSync::logCommand(string command) {
    int targetFrame = getTargetFrameForCurrentTime();
    commands[targetFrame] = command;
}

vector<string> ofxAVSync::getCommandsForCurrentFrame() {
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