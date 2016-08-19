#include "ofApp.h"

void ofApp::setup(){
    //---Video setup---//
    ofSetWindowShape(640, 480);
    
    //Record exactly 180 frames
    vid.setup("out/filename", 180);
    //vid.enableRenderMode(); //Does it slowly
    
    //Prep the variables
    position.x = 0;
    position.y = 0;
    size = 100;
    visible = true;

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
}

void ofApp::update(){
    processFrameCommands();
}

void ofApp::play(void){
    commandSync.begin();
    if(noteOn){
        commandSync.logCommand("off");
        aud.sendMidi("C5 OFF", &chain);
    }else{
        synth.set(TALNoiseMaker_cutoff, ofRandom(0.3, 1));
        synth.set(TALNoiseMaker_osc1tune, ofRandom(1) < 0.5 ? 0.2 : 0.8);

        commandSync.logCommand("on");
        aud.sendMidi("C5 ON", &chain);
    }
    noteOn = !noteOn;
}

void ofApp::draw(){
    vid.begin();
    {
        ofBackground(ofColor::black);
        if(visible){
            position.x++;
            position.y++;
            ofDrawRectangle(position, size, size);
        }
    }
    vid.endCaptureDraw();
}

void ofApp::processFrameCommands(){
    frameCommands = commandSync.getCommandsForCurrentFrame();
    for(auto const& command : frameCommands){
        if(command == "on"){
            visible = true;
            position.x = 0;
            position.y = 0;
        }
        if(command == "off"){
            visible = false;
        }
    }
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