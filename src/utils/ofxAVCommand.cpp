#include "ofxAVCommand.h"

void ofxAVCommand::setup(string _name){
    name = _name;
    args.clear();
}

bool ofxAVCommand::is(string _name){
    return name == _name;
}

void ofxAVCommand::add(float value){
    args.push_back(value);
}