#pragma once

#include "ofMain.h"

class ofxAVCommand : public ofBaseApp{

public:
    void setup(string _name);
    bool is(string _name);
    void add(float value);
    vector<float> args;
protected:
    string name;
};