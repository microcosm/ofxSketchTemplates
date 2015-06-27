#include "ofxTemplate.h"

void ofxTemplate::setup(string _filename, int _renderOnFrame, int _width, int _height) {
    filename = _filename + ".gif";
    renderOnFrame = _renderOnFrame;
    width = _width;
    height = _height;
    frameDuration = 0.2;
    colors = 256;
    slowMode = false;
    paused = false;
    renderingNow = false;
    renderMessage = "";

    halfWidth = width * 0.5;
    halfHeight = height * 0.5;
    doubleWidth = width * 2;
    doubleHeight = height * 2;
    tripleWidth = width * 3;
    tripleHeight = height * 3;

    fbo.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(255, 255, 255, 0);
    fbo.end();

    framerate = slowMode ? 5 : 10;
    ofSetFrameRate(framerate);
    gifEncoder.setup(width, height, frameDuration, colors);
}

void ofxTemplate::begin(){
    fbo.begin();
}

void ofxTemplate::end(){
    fbo.end();
}

void ofxTemplate::endAndCaptureFrame(){
    end();
    captureFrame();
}

void ofxTemplate::draw(){
    ofSetColor(ofColor::white);
    fbo.draw(0, 0);
    ofDrawBitmapString(
       "Recording to frame #" +
       ofToString(renderOnFrame) +
       " at " +
       ofToString(ofGetFrameRate()) +
       "fps...\nCurrent frame: " +
       ofToString(ofGetFrameNum()) +
       "\n" + renderMessage,
       20, height - 50);
}

void ofxTemplate::pause(){
    paused = true;
}

void ofxTemplate::unpause(){
    paused = false;
}

void ofxTemplate::togglePause(){
    paused = !paused;
}

void ofxTemplate::enableSlowMode(){
    slowMode = true;
}

void ofxTemplate::captureFrame(){
    if(!renderingNow && !paused) {
        fbo.readToPixels(pixels);
        gifEncoder.addFrame(pixels.getPixels(), width, height,
                            pixels.getBitsPerPixel(), frameDuration);
        
        if(ofGetFrameNum() == renderOnFrame) {
            renderGif();
        }
    }
}

void ofxTemplate::renderGif() {
    if(!renderingNow) {
        renderingNow = true;
        gifEncoder.save(filename);
        renderMessage = "Now rendering";
    }
}

void ofxTemplate::exit(){
    gifEncoder.exit();
}