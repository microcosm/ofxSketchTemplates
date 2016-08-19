#include "ofxAudioGeneratorTemplate.h"

void ofxAudioGeneratorTemplate::setup(bool toggleDebugUI) {
    audio.setup();
    if(toggleDebugUI){
        audio.toggleDebugUI();
    }
    loadNotesMap();
}

ofxAudioUnitManager* ofxAudioGeneratorTemplate::getAudioUnitManager() {
    return &audio;
}

/* Format can be either: "60 ON", "60 OFF"  etc, or
   "C#5 ON", "C#5 OFF" etc */
void ofxAudioGeneratorTemplate::sendMidi(string command, ofxAudioUnitChain* chain) {
    vector<string> args = ofSplitString(command, " ");
    if(args.size() == 2) {
        int parsedNote = midiNote(args.at(0));
        if(args.at(1) == "ON") {
            cout << "Sending MIDI ON: " << parsedNote << endl;
            chain->midi.sendNoteOn(1, parsedNote);
        } else if(args.at(1) == "OFF") {
            cout << "Sending MIDI OFF: " << parsedNote << endl;
            chain->midi.sendNoteOff(1, parsedNote);
        }
    }
}

int ofxAudioGeneratorTemplate::midiNote(string arg) {
    if(arg.length() > 1 && ofToInt(arg) == 0) {
        octave = ofToInt(arg.substr(arg.length() - 1, arg.length()));
        note = notes[arg.substr(0, arg.length() - 1)];
        return note + 12 * octave;
    }
    return ofToInt(arg);
}

void ofxAudioGeneratorTemplate::loadNotesMap() {
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