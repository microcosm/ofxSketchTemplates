#pragma once

#include "ofMain.h"

class ofxAVSync : public ofBaseApp{

public:
    void begin();
    void logCommand(string command);
    vector<string> getCommandsForCurrentFrame();
    uint64_t getElapsedTime();
    bool hasBegun();
protected:
    int getTargetFrameForCurrentTime();
    map<int, string> commands;
    uint64_t beginDelay;
    bool begun = false;
};