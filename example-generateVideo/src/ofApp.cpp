#include "ofApp.h"

//About ofxVideoSketchTemplate
//===========================
//The video sketch template simplifies drawing whatever you like into
//a video file. It wraps ofxVideoRecorder making a lightweight API
//for you to quickly render video and audio. It is set up with a
//bunch of opinionated defaults which you can override if you like.

//If you want to try this out, run the app and wait until it says
//'recording'. Go look in the bin/data folder for the generated
//video.

void ofApp::setup(){
    //When you set the window shape this will become the video render size
    ofSetWindowShape(640, 480);

    //Record exactly 180 frames
    vid.setup("out/filename", 180);
    vid.enableSlowMode();

    //Take a look at the source code for vid.setup() and vid.setupPaused()
    //Here you can record to/from specified frames, set a custom video
    //width/height, and more.
}

void ofApp::update(){

}

void ofApp::draw(){
    //Everything you draw inside the begin/end will be rendered to the video
    vid.begin();
    {
        //Clear the buffer with black
        ofBackground(ofColor::black);

        //Draw a white rect
        float dist = ofGetFrameNum() * 5;
        float size = 100;
        ofDrawRectangle(dist, dist, size, size);
    }
    vid.endCaptureDraw();
    //This is shorthand for:
    //vid.end() - stop drawing to the buffer
    //vid.captureFrame() - save the contents of the buffer as a video frame
    //vid.draw() - draw the contents of the buffer to the screen so you
    //             can see what you are recording in as it runs
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