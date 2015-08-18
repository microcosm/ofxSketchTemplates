#pragma once

#include "ofMain.h"
#include "ofxAudioUnitManager.h"
#include "ofxTimeline.h"

class ofxAudioSequencerTemplate : public ofBaseApp{

public:
    void setup(float durationInSecs=10, float bpm=-1);
    void draw(ofEventArgs& eventArgs);
    void exit(ofEventArgs& eventArgs);
    void keyPressed(ofKeyEventArgs& eventArgs);
    ofxAudioUnitManager* getAudioUnitManager();
    ofxTimeline* getTimeline();
    void midiEvent(ofxTLSwitchEventArgs &args);
    
protected:
    void setupTimeline(float duration, float bpm);
    void executeMidiCommand(string command, ofxMidiOut *midi);
    void executeMidiCommand(string command);
    int midiNote(string arg);
    void loadNotesMap();

    ofxAudioUnitManager audio;
    ofxTimeline timeline;
    vector<ofxAudioUnitChain*> chains;

    int octave, note;
    map<string, int> notes;
};