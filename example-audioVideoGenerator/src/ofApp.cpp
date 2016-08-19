#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowShape(640, 480);
    
    //Set up video and audio templates
    //Record exactly 180 frames
    vid.setup("out/filename", 180);
    vid.useTimeFrom(&avSync);
    //"render mode" slows the framerate to prevent skipped frames in video recording
    vid.enableRenderMode(60);
    aud.setup(false);
    audioManager = aud.getAudioUnitManager();
    
    //Set up your initial variables how you like
    size = 100;
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
        avSync.logCommand("off");
        aud.sendMidi("C5 OFF", &chain);
    }else{
        synth.set(TALNoiseMaker_cutoff, ofRandom(0.3, 1));
        synth.set(TALNoiseMaker_osc1tune, ofRandom(1) < 0.5 ? 0.2 : 0.8);

        avSync.logCommand("on");
        aud.sendMidi("C5 ON", &chain);
    }
    noteOn = !noteOn;
}

void ofApp::update(){
    //As usual, update() fires on each new graphics frame
    position.x++;
    position.y++;

    for(auto const& command : avSync.getCommandsForCurrentFrame()){
        //Do your updates in response to logged commands which match
        //this graphics frame. When you are in "real-time" mode, this
        //will be instantaneous as usual. When you are in "slow render"
        //mode the graphics will appear to lag behind the sound, but
        //it will all match up in the rendered video file
        if(command == "on"){
            visible = true;
            position.x = 0;
            position.y = 0;
        }else if(command == "off"){
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