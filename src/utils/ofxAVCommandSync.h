#pragma once

#include "ofMain.h"

class ofxAVCommandSync : public ofBaseApp{

public:
    void begin();
    void logCommand(string command);
    vector<string> getCommandsForCurrentFrame();
protected:
    int getTargetFrameForCurrentTime();
    uint64_t getElapsedTime();
    map<int, string> commands;
    uint64_t beginDelay;
    bool begun = false;
};