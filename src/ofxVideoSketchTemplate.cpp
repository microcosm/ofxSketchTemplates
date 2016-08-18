#include "ofxVideoSketchTemplate.h"

void ofxVideoTemplate::setup(int _renderFromFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    setup("out/" + ofGetTimestampString("%Y-%m-%d"), _renderFromFrameCount, _width, _height, isolation);
}

void ofxVideoTemplate::setupPaused(int _unpauseFromFrameCount, int _renderFromFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    setupPaused("out/" + ofGetTimestampString("%Y-%m-%d"), _unpauseFromFrameCount, _renderFromFrameCount, _width, _height, isolation);
}

void ofxVideoTemplate::setup(string _filename, int _recordToFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    filename = _filename + ".mov";
    finishOnFrame = _recordToFrameCount - 1;
    fadeAlpha = 0;
    textColor = ofColor::white;
    textOverlay = false;
    fade = false;
    width = _width;
    height = _height;
    frameDuration = 0.2;
    colors = 256;
    slowMode = false;
    paused = false;
    renderingNow = false;
    renderMessage = "[[Recording...]]";
    recordingMessage = "Recording to frame " + ofToString(_recordToFrameCount) + " frames";
    unpauseOnFrame = -1;
    drawingIsolation = isolation;

    halfWidth = width * 0.5;
    halfHeight = height * 0.5;
    doubleWidth = width * 2;
    doubleHeight = height * 2;
    tripleWidth = width * 3;
    tripleHeight = height * 3;

    framerate = slowMode ? 5 : 10;
    ofSetFrameRate(framerate);

    videoRecorder.setup(filename, width, height, 60);
    videoRecorder.setVideoCodec("mpeg4");
    videoRecorder.setVideoBitrate("800k");
    videoRecorder.start();

    fbo.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(255, 255, 255, 0);
    fbo.end();
}

void ofxVideoTemplate::setupPaused(string _filename, int _recordFromFrameCount, int _recordToFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    setup(_filename, _recordToFrameCount, _width, _height, isolation);
    unpauseOnFrame = _recordFromFrameCount - 1;
    recordingMessage = "Recording from frame " + ofToString(_recordFromFrameCount) + " to frame " + ofToString(_recordToFrameCount);
    pause();
}

void ofxVideoTemplate::setTextColor(ofColor _textColor) {
    textColor = _textColor;
}

void ofxVideoTemplate::enableTextOverlay() {
    textOverlay = true;
}

void ofxVideoTemplate::fadeInOut(int numFramesIn, int numFramesOut, ofColor color, int _numBlankFramesAfterFadeOut) {
    fadeAlphaIncrementIn = 255 / numFramesIn;
    fadeAlphaIncrementOut = 255 / numFramesOut;
    beginFadeOutOnFrame = finishOnFrame - numFramesOut;
    fadeColor = color;
    fade = true;
    numBlankFramesAfterFadeOut = _numBlankFramesAfterFadeOut;
    if(!paused) {
        fadeAlpha = 255;
    }
}

void ofxVideoTemplate::begin(){
    fbo.begin();
    beginLayerIsolation();
}

void ofxVideoTemplate::end(){
    endLayerIsolation();
    drawFadeIfNeeded();
    fbo.end();
}

void ofxVideoTemplate::endAndCaptureFrame(){
    end();
    captureFrame();
}

void ofxVideoTemplate::endCaptureDraw(){
    end();
    captureFrame();
    draw();
}

void ofxVideoTemplate::draw(){
    ofPushStyle();
    ofSetColor(ofColor::white);
    fbo.draw(0, 0);
    ofSetColor(ofColor(ofColor::gray, 128));
    ofRect(10, height - 120, 280, 90);
    ofSetColor(textColor);
    ofDrawBitmapString(
       recordingMessage +
       "\n\n" + renderMessage + "\n\n" +
       "Frame count: " + ofToString(ofGetFrameNum() + 1) + " / " +
       ofToString(ofGetFrameRate()) + "fps...",
       20, height - 100);
    ofPopStyle();
}

void ofxVideoTemplate::pause(){
    paused = true;
    videoRecorder.setPaused(paused);
    renderMessage = "[[Paused...]]";
}

void ofxVideoTemplate::unpause(){
    paused = false;
    videoRecorder.setPaused(paused);
    if(fade) {
        fadeAlpha = 255;
    }
    renderMessage = "[[RECORDING...]]";
}

void ofxVideoTemplate::togglePause(){
    paused = !paused;
}

void ofxVideoTemplate::enableSlowMode(){
    slowMode = true;
}

void ofxVideoTemplate::captureFrame(){
    if(!renderingNow && !paused) {
        drawFboIntoGifEncoder();
        
        if(ofGetFrameNum() == finishOnFrame) {
            finish();
        }
    }

    if(ofGetFrameNum() == unpauseOnFrame) {
        unpause();
    }
}

void ofxVideoTemplate::rotateAroundCenter(float degrees){
    ofTranslate(halfWidth, halfHeight);
    ofRotateZ(degrees);
    ofTranslate(-halfWidth, -halfHeight);
}

void ofxVideoTemplate::finish() {
    drawBlankFrames();
    videoRecorder.close();
    if(!renderingNow) {
        renderingNow = true;
        renderMessage = "[[Rendering...]]";
    }
}

void ofxVideoTemplate::exit(){
    videoRecorder.close();
}

ofVec2f ofxVideoTemplate::size(){
    return ofVec2f(width, height);
}

void ofxVideoTemplate::drawFboIntoGifEncoder() {
    fbo.readToPixels(pixels);
    if(!videoRecorder.addFrame(pixels)) {
        ofLogWarning("Frame could not be drawn to recorder");
    }
    if(videoRecorder.hasVideoError()) {
        ofLogWarning("Recorder failed to write frame");
    }
}

void ofxVideoTemplate::drawFadeIfNeeded() {
    if(ofGetFrameNum() < beginFadeOutOnFrame && fadeAlpha > 0) {
        drawFade();
        fadeAlpha -= fadeAlphaIncrementIn;
    }
    if(ofGetFrameNum() >= beginFadeOutOnFrame && fadeAlpha < 255) {
        fadeAlpha += fadeAlphaIncrementOut;
        drawFade();
    }
}

void ofxVideoTemplate::drawFade() {
    ofPushStyle();
    {
        ofSetColor(fadeColor, fadeAlpha);
        ofRect(0, 0, width, height);
    }
    ofPopStyle();
}

void ofxVideoTemplate::drawBlankFrames() {
    fbo.begin();
    ofClear(fadeColor);
    fbo.end();
    for(int i = 0; i < numBlankFramesAfterFadeOut; i++) {
        drawFboIntoGifEncoder();
    }
}

void ofxVideoTemplate::beginLayerIsolation() {
    if(drawingIsolation == ISOLATE_DRAWING) {
        ofPushMatrix();
        ofPushStyle();
    }
}

void ofxVideoTemplate::endLayerIsolation() {
    if(drawingIsolation == ISOLATE_DRAWING) {
        ofPopStyle();
        ofPopMatrix();
    }
}