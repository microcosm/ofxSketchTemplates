#include "ofxVideoGeneratorTemplate.h"

void ofxVideoGeneratorTemplate::setup(int _renderFromFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    setup("out/" + ofGetTimestampString("%Y-%m-%d"), _renderFromFrameCount, _width, _height, isolation);
}

void ofxVideoGeneratorTemplate::setupPaused(int _unpauseFromFrameCount, int _renderFromFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    setupPaused("out/" + ofGetTimestampString("%Y-%m-%d"), _unpauseFromFrameCount, _renderFromFrameCount, _width, _height, isolation);
}

void ofxVideoGeneratorTemplate::setup(string _filename, int _recordToFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    ofSetFrameRate(60);
    renderMode = false;

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
    paused = false;
    finishingNow = false;
    message = "[[Recording...]]";
    recordingMessage = "Recording to frame " + ofToString(_recordToFrameCount) + " frames";
    unpauseOnFrame = -1;
    drawingIsolation = isolation;

    halfWidth = width * 0.5;
    halfHeight = height * 0.5;
    doubleWidth = width * 2;
    doubleHeight = height * 2;
    tripleWidth = width * 3;
    tripleHeight = height * 3;

    videoRecorder.setup(filename, width, height, 60);
    videoRecorder.setVideoCodec("mpeg4");
    videoRecorder.setVideoBitrate("8000k");
    videoRecorder.start();

    fbo.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(255, 255, 255, 0);
    fbo.end();
}

void ofxVideoGeneratorTemplate::setupPaused(string _filename, int _recordFromFrameCount, int _recordToFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    setup(_filename, _recordToFrameCount, _width, _height, isolation);
    unpauseOnFrame = _recordFromFrameCount - 1;
    recordingMessage = "Recording from frame " + ofToString(_recordFromFrameCount) + " to frame " + ofToString(_recordToFrameCount);
    pause();
}

void ofxVideoGeneratorTemplate::setTextColor(ofColor _textColor) {
    textColor = _textColor;
}

void ofxVideoGeneratorTemplate::enableTextOverlay() {
    textOverlay = true;
}

void ofxVideoGeneratorTemplate::fadeInOut(int numFramesIn, int numFramesOut, ofColor color, int _numBlankFramesAfterFadeOut) {
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

void ofxVideoGeneratorTemplate::begin(){
    fbo.begin();
    beginLayerIsolation();
}

void ofxVideoGeneratorTemplate::end(){
    endLayerIsolation();
    drawFadeIfNeeded();
    fbo.end();
}

void ofxVideoGeneratorTemplate::endAndCaptureFrame(){
    end();
    captureFrame();
}

void ofxVideoGeneratorTemplate::endCaptureDraw(){
    end();
    captureFrame();
    draw();
}

void ofxVideoGeneratorTemplate::draw(){
    ofPushStyle();
    ofSetColor(ofColor::white);
    fbo.draw(0, 0);
    ofSetColor(ofColor(ofColor::gray, 128));
    ofDrawRectangle(10, height - 120, 280, 90);
    ofSetColor(textColor);
    ofDrawBitmapString(
       recordingMessage +
       "\n\n" + message + "\n\n" +
       "Frame count: " + ofToString(ofGetFrameNum() + 1) + " / " +
       ofToString(ofGetFrameRate()) + "fps...",
       20, height - 100);
    ofPopStyle();
}

void ofxVideoGeneratorTemplate::pause(){
    paused = true;
    if(renderMode){
        videoRecorder.setPaused(paused);
    }
    message = "[[Paused...]]";
}

void ofxVideoGeneratorTemplate::unpause(){
    paused = false;
    if(renderMode){
        videoRecorder.setPaused(paused);
    }
    if(fade) {
        fadeAlpha = 255;
    }
    message = "[[RECORDING...]]";
}

void ofxVideoGeneratorTemplate::togglePause(){
    paused = !paused;
}

void ofxVideoGeneratorTemplate::enableRenderMode(){
    ofSetFrameRate(10);
    renderMode = true;
}

void ofxVideoGeneratorTemplate::captureFrame(){
    if(renderMode){
        if(!finishingNow && !paused) {
            drawFboIntoGifEncoder();

            if(ofGetFrameNum() == finishOnFrame) {
                finish();
            }
        }

        if(ofGetFrameNum() == unpauseOnFrame) {
            unpause();
        }
    }
}

void ofxVideoGeneratorTemplate::rotateAroundCenter(float degrees){
    ofTranslate(halfWidth, halfHeight);
    ofRotateZ(degrees);
    ofTranslate(-halfWidth, -halfHeight);
}

void ofxVideoGeneratorTemplate::finish() {
    if(renderMode){
        drawBlankFrames();
        videoRecorder.close();
    }
    if(!finishingNow) {
        finishingNow = true;
        message = "[[Finishing...]]";
    }
}

void ofxVideoGeneratorTemplate::exit(){
    if(renderMode){
        videoRecorder.close();
    }
}

ofVec2f ofxVideoGeneratorTemplate::size(){
    return ofVec2f(width, height);
}

void ofxVideoGeneratorTemplate::drawFboIntoGifEncoder() {
    if(renderMode){
        fbo.readToPixels(pixels);
        if(!videoRecorder.addFrame(pixels)) {
            ofLogWarning("Frame could not be drawn to recorder");
        }
        if(videoRecorder.hasVideoError()) {
            ofLogWarning("Recorder failed to write frame");
        }
    }
}

void ofxVideoGeneratorTemplate::drawFadeIfNeeded() {
    if(renderMode){
        if(ofGetFrameNum() < beginFadeOutOnFrame && fadeAlpha > 0) {
            drawFade();
            fadeAlpha -= fadeAlphaIncrementIn;
        }
        if(ofGetFrameNum() >= beginFadeOutOnFrame && fadeAlpha < 255) {
            fadeAlpha += fadeAlphaIncrementOut;
            drawFade();
        }
    }
}

void ofxVideoGeneratorTemplate::drawFade() {
    ofPushStyle();
    {
        ofSetColor(fadeColor, fadeAlpha);
        ofDrawRectangle(0, 0, width, height);
    }
    ofPopStyle();
}

void ofxVideoGeneratorTemplate::drawBlankFrames() {
    fbo.begin();
    ofClear(fadeColor);
    fbo.end();
    for(int i = 0; i < numBlankFramesAfterFadeOut; i++) {
        drawFboIntoGifEncoder();
    }
}

void ofxVideoGeneratorTemplate::beginLayerIsolation() {
    if(drawingIsolation == ISOLATE_DRAWING) {
        ofPushMatrix();
        ofPushStyle();
    }
}

void ofxVideoGeneratorTemplate::endLayerIsolation() {
    if(drawingIsolation == ISOLATE_DRAWING) {
        ofPopStyle();
        ofPopMatrix();
    }
}