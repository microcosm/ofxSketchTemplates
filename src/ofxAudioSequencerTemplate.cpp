#include "ofxAudioSequencerTemplate.h"

void ofxAudioSequencerTemplate::setup(float durationInSecs, float bpm, int numChains) {
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    ofSetWindowPosition(0, 0);
    audio.setup();
    audio.onlyFocusOnCommand();
    audio.toggleDebugUI();
    setupTimeline(durationInSecs, bpm, numChains);
    loadNotesMap();
    ofAddListener(ofEvents().draw, this, &ofxAudioSequencerTemplate::draw);
    ofAddListener(ofEvents().exit, this, &ofxAudioSequencerTemplate::exit);
    ofAddListener(ofEvents().keyPressed, this, &ofxAudioSequencerTemplate::keyPressed);
}

void ofxAudioSequencerTemplate::draw(ofEventArgs& eventArgs) {
    audio.draw();
    timeline.draw();
}

void ofxAudioSequencerTemplate::exit(ofEventArgs& eventArgs) {
    audio.exit();
}

void ofxAudioSequencerTemplate::keyPressed(ofKeyEventArgs& eventArgs){
    audio.keyPressed(eventArgs.key);
}

ofxAudioUnitManager* ofxAudioSequencerTemplate::getAudioUnitManager() {
    return &audio;
}

ofxTimeline* ofxAudioSequencerTemplate::getTimeline() {
    return &timeline;
}

void ofxAudioSequencerTemplate::midiEvent(ofxTLBangEventArgs &args) {
    //cout << endl << ofGetTimestampString() << endl;
    //cout << "Flagged: " << command << endl;
    executeMidiCommand(args.flag, toMidiOut(args.track->getName()));
}

ofxMidiOut* ofxAudioSequencerTemplate::toMidiOut(string midiTrackName) {
    chainId = ofToInt(ofSplitString(midiTrackName, " ")[1]) - 1;
    return &audio.getChain(chainId)->midi;
}

void ofxAudioSequencerTemplate::setupTimeline(float duration, float bpm, int numChains) {
    timeline.setup();
    timeline.setLoopType(OF_LOOP_NORMAL);
    timeline.setDurationInSeconds(duration);
    timeline.setOffset(ofVec2f(10, 480));
    timeline.setWidth(ofGetWidth() - 370);
    if(bpm > -1) {
        timeline.setShowBPMGrid(true);
        timeline.enableSnapToBPM(true);
        timeline.setBPM(bpm);
    }
    for(int i = 0; i < numChains; i++) {
        timeline.addFlags("chain " + ofToString(i + 1) + " midi");
    }
    ofAddListener(timeline.events().bangFired, this, &ofxAudioSequencerTemplate::midiEvent);
}

/* Format can be either: "60 ON", "60 OFF"  etc, or
   "C#5 ON", "C#5 OFF" etc */
void ofxAudioSequencerTemplate::executeMidiCommand(string command, ofxMidiOut *midi) {
    vector<string> args = ofSplitString(command, " ");
    if(args.size() == 2) {
        int parsedNote = midiNote(args.at(0));
        if(args.at(1) == "ON") {
            cout << "Sending MIDI ON: " << parsedNote << endl;
            midi->sendNoteOn(1, parsedNote);
        } else if(args.at(1) == "OFF") {
            cout << "Sending MIDI OFF: " << parsedNote << endl;
            midi->sendNoteOff(1, parsedNote);
        }
    }
}

int ofxAudioSequencerTemplate::midiNote(string arg) {
    if(arg.length() > 1 && ofToInt(arg) == 0) {
        octave = ofToInt(arg.substr(arg.length() - 1, arg.length()));
        note = notes[arg.substr(0, arg.length() - 1)];
        return note + 12 * octave;
    }
    return ofToInt(arg);
}

void ofxAudioSequencerTemplate::loadNotesMap() {
    notes["C"] = 0;
    notes["C#"] = 1;
    notes["D"] = 2;
    notes["D#"] = 3;
    notes["E"] = 4;
    notes["F"] = 5;
    notes["F#"] = 6;
    notes["G"] = 7;
    notes["G#"] = 8;
    notes["A"] = 9;
    notes["A#"] = 10;
    notes["B"] = 11;
}