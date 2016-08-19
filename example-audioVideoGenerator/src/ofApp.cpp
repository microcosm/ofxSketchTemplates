#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowShape(640, 480);
    
    //Set up video and audio templates
    //Record exactly 180 frames
    vid.setup("out/filename", 180);
    vid.isolateDrawingStyle();
    vid.useTimeFrom(&avSync);
    //"render mode" slows the framerate to prevent skipped frames in video recording
    vid.enableRenderMode(60);
    aud.setup(false);
    audioManager = aud.getAudioUnitManager();
    
    //Set up your initial variables how you like
    visible = true;
    noteOn = false;
    
    //Set up a new AudioUnit to play with
    synth.setup("Synth 1", 'aumu', 'ncut', 'TOGU');
    audioManager->createChain(&chain).link(&synth).toMixer();
    synth.set(TALNoiseMaker_chorus1enable, 1);
    synth.set(TALNoiseMaker_chorus2enable, 1);
    
    //Set up the beat() method to regularly fire
    ofAddListener(audioManager->bpm.beatEvent, this, &ofApp::beat);
    audioManager->bpm.start();
}

void ofApp::beat(void){
    //The beat() method fires on each beat of the BPM
    //Make sure sync object knows when beat() first fires
    avSync.setupOnce();
    
    //Do whatever you want with sound here, and log commands which
    //will be synced below (so A/V matches up regardless of whether
    //you are in "real-time" or "slow render" mode)
    if(noteOn){
        aud.sendMidi("C5 OFF", &chain);
        avSync.logCommand("off");
    }else{
        cutoff = ofRandom(0.3, 1);
        tune = ofRandom(1) < 0.5 ? 0.2 : 0.8;
        synth.set(TALNoiseMaker_cutoff, cutoff);
        synth.set(TALNoiseMaker_osc1tune, tune);
        aud.sendMidi("C5 ON", &chain);

        avSync.logCommand("on", cutoff, tune);
    }
    noteOn = !noteOn;
}

void ofApp::update(){
    //As usual, update() fires on each new graphics frame
    position.x += multiplier * 18;
    position.y += multiplier * 12;

    for(auto& command : avSync.getCommandsForCurrentFrame()){
        //Do your updates in response to logged commands which match
        //this graphics frame. When you are in "real-time" mode, this
        //will be instantaneous as usual. When you are in "slow render"
        //mode the graphics will appear to lag behind the sound, but
        //it will all match up in the rendered video file
        if(command.is("on")){
            visible = true;
            multiplier = command.args[0];
            color = command.args[1] < 0.5 ? ofColor(255, 0, 0, 200) : ofColor(0, 255, 0, 200);
            position.x = 0;
            position.y = 0;
            size = 200 * multiplier;
        }else if(command.is("off")){
            visible = false;
        }
    }
}

void ofApp::draw(){
    //As usual, draw() fires on each new graphics frame (after update())
    //begin() opens a buffer for us to draw in
    vid.begin();
    {
        //Draw based on updated variable state, as usual
        ofBackground(ofColor::black);
        if(visible){
            ofSetColor(color);
            ofDrawRectangle(position, size, size);
        }
    }
    vid.endCaptureDraw();
    //endCaptureDraw() ends the buffer drawing, captures to the video
    //file on disk, and draws the buffer to the screen (so we can see
    //what we are rendering)
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