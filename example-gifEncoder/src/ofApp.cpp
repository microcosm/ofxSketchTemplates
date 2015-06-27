#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowShape(500, 500);
    gifEncoder.setup(ofGetTimestampString("%Y-%m-%d") + ".gif");
}

void ofApp::update(){

}

void ofApp::draw(){
    gifEncoder.begin();
    {
        ofBackground(ofColor::black);
        ofRect(ofGetFrameNum() * 5, ofGetFrameNum() * 5, gifEncoder.width * 0.2, gifEncoder.height * 0.2);
    }
    gifEncoder.endAndCaptureFrame();
    gifEncoder.draw();
}

void ofApp::keyPressed(int key){

}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y ){

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