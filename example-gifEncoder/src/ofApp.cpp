#include "ofApp.h"

//About ofxGifEncoderTemplate
//===========================
//The gif encoder template simplifies drawing whatever you like into
//an animated gif. It wraps ofxGifEncoder making a lightweight API
//for you to do things like fade in / out. It is set up with a
//bunch of opinionated defaults which you can override if you like.

//If you want to try this out, run the app and wait until it says
//'recording'. Go look in the bin/data folder for the generated
//gif.

void ofApp::setup(){
    //When you set the window shape this will become the gif render size
    ofSetWindowShape(500, 500);

    //Record exactly 12 frames
    gif.setup("out/filename", 12);

    //Take a look at the source code for gif.setup() and gif.setupPaused()
    //Here you can record to/from specified frames, set a custom gif
    //width/height, and more. Also check out gif.fadeInOut() to do nice
    //fades to prevent jumps on the gif's repeat.
}

void ofApp::update(){

}

void ofApp::draw(){
    //Everything you draw inside the begin/end will be rendered to the gif
    gif.begin();
    {
        //Clear the buffer with black
        ofBackground(ofColor::black);

        //Draw a white rect
        float dist = ofGetFrameNum() * 5;
        float size = 100;
        ofRect(dist, dist, size, size);
    }
    gif.endCaptureDraw();
    //This is shorthand for:
    //gif.end() - stop drawing to the buffer
    //gif.captureFrame() - save the contents of the buffer as a gif frame
    //gif.draw() - draw the contents of the buffer to the screen so you
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