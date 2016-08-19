#pragma once

#include "ofMain.h"
#include "ofxAVCommand.h"

class ofxAVSync : public ofBaseApp{

public:
    void setupOnce();
    void begin();
    void logCommand(string commandName);
    void logCommand(string commandName, float val1);
    void logCommand(string commandName, float val1, float val2);
    void logCommand(string commandName, float val1, float val2, float val3);
    void logCommand(string commandName, float val1, float val2, float val3, float val4);
    void logCommand(string commandName, float val1, float val2, float val3, float val4, float val5);
    void logCommand(string commandName, float val1, float val2, float val3, float val4, float val5, float val6);
    vector<ofxAVCommand> getCommandsForCurrentFrame();
    uint64_t getElapsedTime();
    bool hasBegun();
protected:
    int getTargetFrameForCurrentTime();
    ofxAVCommand command;
    map<int, ofxAVCommand> commands;
    uint64_t beginDelay;
    bool begun = false;
};