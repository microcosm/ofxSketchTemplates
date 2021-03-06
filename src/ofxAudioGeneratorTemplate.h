#pragma once

#include "ofMain.h"
#include "ofxAudioUnitManager.h"
#include "ofxAudioUnitParams.h"

class ofxAudioGeneratorTemplate : public ofBaseApp{

public:
    void setup(bool toggleDebugUI=true);
    ofxAudioUnitManager* getAudioUnitManager();
    void sendMidi(string command, ofxAudioUnitChain* chain);
protected:
    int midiNote(string arg);
    void loadNotesMap();

    ofxAudioUnitManager audio;
    int octave, note;
    map<string, int> notes;
};