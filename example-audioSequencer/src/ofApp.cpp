#include "ofApp.h"

void ofApp::setup(){
    //10 seconds at 60bpm
    audioSequencer.setup(10, 60);

    //Set up audio unit manager with a synth and a chain
    audio = audioSequencer.getAudioUnitManager();
    synth.setup("Synth 1", 'aumu', 'ncut', 'TOGU');
    audio->createChain(&chain)
          .link(&synth)
          .toMixer();

    //Note that the MIDI event track is set up for you by default
    //You can add more tracks to that below
    timeline = audioSequencer.getTimeline();
    timeline->addCurves("filter cutoff");
}

void ofApp::update(){
    synth.set(TALNoiseMaker_cutoff, timeline->getValue("filter cutoff"));
}

void ofApp::draw(){
    
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