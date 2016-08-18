#include "ofApp.h"

void ofApp::setup(){
    //---Audio setup---//
    aud.setup();

    //Set up a chain and synth
    manager = aud.getAudioUnitManager();
    synth.setup("Synth 1", 'aumu', 'ncut', 'TOGU');
    manager->createChain(&chain)
      .link(&synth)
      .toMixer();

    //Use ofxBpm to regularly schedule MIDI events
    ofAddListener(manager->bpm.beatEvent, this, &ofApp::play);
    manager->bpm.start();
    noteOn = false;

    //Fatten up the sound :)
    synth.set(TALNoiseMaker_chorus1enable, 1);
    synth.set(TALNoiseMaker_chorus2enable, 1);

    //---Video setup---//
    ofSetWindowShape(640, 480);
    
    //Record exactly 180 frames
    vid.setup("out/filename", 180);
    //vid.enableSlowMode();
}

void ofApp::update(){
    
}

void ofApp::play(void){
    if(noteOn){
        aud.sendMidi("C5 OFF", &chain);
    }else{
        synth.set(TALNoiseMaker_cutoff, ofRandom(0.3, 1));
        synth.set(TALNoiseMaker_osc1tune, ofRandom(1) < 0.5 ? 0.2 : 0.8);
        aud.sendMidi("C5 ON", &chain);
    }
    noteOn = !noteOn;
}

void ofApp::draw(){
    vid.begin();
    {
        ofBackground(ofColor::black);
        float dist = ofGetFrameNum() * 2;
        float size = 100;
        ofDrawRectangle(dist, dist, size, size);
    }
    vid.endCaptureDraw();
}

void ofApp::keyPressed(int key){
    
}

void ofApp::keyReleased(int key){
    
}

void ofApp::mouseMoved(int x, int y){
    
}

void ofApp::mouseDragged(int x, int y, int button){
    
}

void ofApp::mousePressed(int x, int y, int button){
    
}

void ofApp::mouseReleased(int x, int y, int button){
    
}

void ofApp::windowResized(int w, int h){
    
}

void ofApp::gotMessage(ofMessage msg){
    
}

void ofApp::dragEvent(ofDragInfo dragInfo){
    
}