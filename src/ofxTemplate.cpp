#include "ofxTemplate.h"

void ofxTemplate::setup(string _filename, int _renderOnFrame, int _width, int _height, bool _paused, bool _slowMode) {
    slowMode = _slowMode;
    paused = _paused;
    width = _width;
    height = _height;
    frameDuration = 0.2;
    colors = 256;
    renderOnFrame = _renderOnFrame;

    filename = _filename + ".gif";
    framerate = slowMode ? 5 : 10;

    halfWidth = width * 0.5;
    halfHeight = height * 0.5;
    doubleWidth = width * 2;
    doubleHeight = height * 2;
    tripleWidth = width * 3;
    tripleHeight = height * 3;

    ofSetFrameRate(framerate);
    gifEncoder.setup(width, height, frameDuration, colors);
    renderingNow = false;
    renderMessage = "";
    
    fbo.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(255, 255, 255, 0);
    fbo.end();
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