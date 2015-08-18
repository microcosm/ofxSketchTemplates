#include "ofApp.h"

void ofApp::setup(){
    //10 seconds at 60bpm
    audioSequencer.setup(10, 60);
    audio = audioSequencer.getAudioUnitManager();
    timeline = audioSequencer.getTimeline();

    //Set up any audio units and timeline interactions
    setupAudioUnits();
    setupTimeline();
}

void ofApp::setupAudioUnits() {
    noiseMaker.setup('aumu', 'ncut', 'TOGU', AU_TYPE_SYNTH, "TALNoiseMaker");
    audio->add(&chain, "tal-noisemaker", ofColor::blue);
    chain.link(&noiseMaker).toMixer();
    audio->loadPresets(&chain);
}

void ofApp::setupTimeline() {
    //Note that the MIDI event track is set up for you by default
    //You can add more tracks to that below
    timeline->addCurves("filter cutoff");
}

void ofApp::update(){
    noiseMaker.set(TALNoiseMaker_cutoff, timeline->getValue("filter cutoff"));
}

void ofApp::draw(){
    audioSequencer.draw();
}

void ofApp::keyPressed(int key){
    audioSequencer.keyPressed(key);
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
