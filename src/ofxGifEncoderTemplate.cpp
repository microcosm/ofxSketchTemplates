#include "ofxGifEncoderTemplate.h"

void ofxGifEncoderTemplate::setup(string _filename, int _recordToFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    filename = _filename + ".gif";
    renderOnFrame = _recordToFrameCount - 1;
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

    fbo.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(255, 255, 255, 0);
    fbo.end();

    framerate = slowMode ? 5 : 10;
    ofSetFrameRate(framerate);
    gifEncoder.setup(width, height, frameDuration, colors);
}

void ofxGifEncoderTemplate::setupPaused(string _filename, int _recordFromFrameCount, int _recordToFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    setup(_filename, _recordToFrameCount, _width, _height, isolation);
    unpauseOnFrame = _recordFromFrameCount - 1;
    recordingMessage = "Recording from frame " + ofToString(_recordFromFrameCount) + " to frame " + ofToString(_recordToFrameCount);
    pause();
}

void ofxGifEncoderTemplate::setTextColor(ofColor _textColor) {
    textColor = _textColor;
}

void ofxGifEncoderTemplate::enableTextOverlay() {
    textOverlay = true;
}

void ofxGifEncoderTemplate::fadeInOut(int numFramesIn, int numFramesOut, ofColor color, int _numBlankFramesAfterFadeOut) {
    fadeAlphaIncrementIn = 255 / numFramesIn;
    fadeAlphaIncrementOut = 255 / numFramesOut;
    beginFadeOutOnFrame = renderOnFrame - numFramesOut;
    fadeColor = color;
    fade = true;
    numBlankFramesAfterFadeOut = _numBlankFramesAfterFadeOut;
    if(!paused) {
        fadeAlpha = 255;
    }
}

void ofxGifEncoderTemplate::begin(){
    fbo.begin();
    beginLayerIsolation();
}

void ofxGifEncoderTemplate::end(){
    endLayerIsolation();
    drawFadeIfNeeded();
    fbo.end();
}

void ofxGifEncoderTemplate::endAndCaptureFrame(){
    end();
    captureFrame();
}

void ofxGifEncoderTemplate::endCaptureDraw(){
    end();
    captureFrame();
    draw();
}

void ofxGifEncoderTemplate::draw(){
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

void ofxGifEncoderTemplate::pause(){
    paused = true;
    renderMessage = "[[Paused...]]";
}

void ofxGifEncoderTemplate::unpause(){
    paused = false;
    if(fade) {
        fadeAlpha = 255;
    }
    renderMessage = "[[RECORDING...]]";
}

void ofxGifEncoderTemplate::togglePause(){
    paused = !paused;
}

void ofxGifEncoderTemplate::enableSlowMode(){
    slowMode = true;
}

void ofxGifEncoderTemplate::captureFrame(){
    if(!renderingNow && !paused) {
        drawFboIntoGifEncoder();
        
        if(ofGetFrameNum() == renderOnFrame) {
            render();
        }
    }

    if(ofGetFrameNum() == unpauseOnFrame) {
        unpause();
    }
}

void ofxGifEncoderTemplate::rotateAroundCenter(float degrees){
    ofTranslate(halfWidth, halfHeight);
    ofRotateZ(degrees);
    ofTranslate(-halfWidth, -halfHeight);
}

void ofxGifEncoderTemplate::render() {
    drawBlankFrames();
    if(!renderingNow) {
        renderingNow = true;
        gifEncoder.save(filename);
        renderMessage = "[[Rendering...]]";
    }
}

void ofxGifEncoderTemplate::exit(){
    gifEncoder.exit();
}

ofVec2f ofxGifEncoderTemplate::size(){
    return ofVec2f(width, height);
}

void ofxGifEncoderTemplate::drawFboIntoGifEncoder() {
    fbo.readToPixels(pixels);
    gifEncoder.addFrame(pixels.getPixels(), width, height,
                        pixels.getBitsPerPixel(), frameDuration);
}

void ofxGifEncoderTemplate::drawFadeIfNeeded() {
    if(ofGetFrameNum() < beginFadeOutOnFrame && fadeAlpha > 0) {
        drawFade();
        fadeAlpha -= fadeAlphaIncrementIn;
    }
    if(ofGetFrameNum() >= beginFadeOutOnFrame && fadeAlpha < 255) {
        fadeAlpha += fadeAlphaIncrementOut;
        drawFade();
    }
}

void ofxGifEncoderTemplate::drawFade() {
    ofPushStyle();
    {
        ofSetColor(fadeColor, fadeAlpha);
        ofRect(0, 0, width, height);
    }
    ofPopStyle();
}

void ofxGifEncoderTemplate::drawBlankFrames() {
    fbo.begin();
    ofClear(fadeColor);
    fbo.end();
    for(int i = 0; i < numBlankFramesAfterFadeOut; i++) {
        drawFboIntoGifEncoder();
    }
}

void ofxGifEncoderTemplate::beginLayerIsolation() {
    if(drawingIsolation == ISOLATE_DRAWING) {
        ofPushMatrix();
        ofPushStyle();
    }
}

void ofxGifEncoderTemplate::endLayerIsolation() {
    if(drawingIsolation == ISOLATE_DRAWING) {
        ofPopStyle();
        ofPopMatrix();
    }
}