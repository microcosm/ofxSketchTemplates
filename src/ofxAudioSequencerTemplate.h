#pragma once

#include "ofMain.h"
#include "ofxAudioUnitManager.h"
#include "ofxAudioUnitParams.h"
#include "ofxTimeline.h"

class ofxAudioSequencerTemplate : public ofBaseApp{

public:
    void setup(float durationInSecs=10, float bpm=-1, int numChains=1);
    void draw(ofEventArgs& eventArgs);
    void exit(ofEventArgs& eventArgs);
    void keyPressed(ofKeyEventArgs& eventArgs);
    ofxAudioUnitManager* getAudioUnitManager();
    ofxTimeline* getTimeline();
    void midiEvent(ofxTLBangEventArgs &args);
    ofxMidiOut* toMidiOut(string midiTrackName);
protected:
    void setupTimeline(float duration, float bpm, int numChains);
    void executeMidiCommand(string command, ofxMidiOut *midi);
    int midiNote(string arg);
    void loadNotesMap();

    ofxAudioUnitManager audio;
    ofxTimeline timeline;

    string command;
    int octave, note, chainId;
    map<string, int> notes;
};