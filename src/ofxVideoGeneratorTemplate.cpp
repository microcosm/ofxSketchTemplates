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
    finishAudioAtTime = calculateAudioFinishTime(_recordToFrameCount);
    elapsedTime = 0;
    hasBegun = false;

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

    setupSound();
    videoRecorder.setup(filename, width, height, 60, sampleRate, inChannels);
    videoRecorder.setAutoSync(false);
    videoRecorder.setVideoCodec("mpeg4");
    videoRecorder.setVideoBitrate("8000k");
    videoRecorder.setAudioCodec("mp3");
    videoRecorder.setAudioBitrate("320k");
    videoRecorder.start();

    fbo.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(255, 255, 255, 0);
    fbo.end();

    ofAddListener(ofEvents().update, this, &ofxVideoGeneratorTemplate::update);
}

void ofxVideoGeneratorTemplate::setupPaused(string _filename, int _recordFromFrameCount, int _recordToFrameCount, int _width, int _height, ofxGifEncoderIsolation isolation) {
    setup(_filename, _recordToFrameCount, _width, _height, isolation);
    unpauseOnFrame = _recordFromFrameCount - 1;
    recordingMessage = "Recording from frame " + ofToString(_recordFromFrameCount) + " to frame " + ofToString(_recordToFrameCount);
    pause();
}

void ofxVideoGeneratorTemplate::update(ofEventArgs& args){
    if(avSync->hasBegun()){
        hasBegun = true;
        elapsedTime = avSync->getElapsedTime();
    }
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
    ofDrawRectangle(10, height - 124, 280, 114);
    ofSetColor(textColor);
    ofDrawBitmapString(
       recordingMessage +
       "\n\n" + message + "\n\n" +
       "Frame count: " + ofToString(ofGetFrameNum() + 1) + " / " +
       ofToString(ofGetFrameRate()) + "fps..." + "\n\n" +
       "Press 'v' to toggle audio UI",
       20, height - 104);
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

void ofxVideoGeneratorTemplate::enableRenderMode(int frameRate){
    ofSetFrameRate(frameRate);
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

void ofxVideoGeneratorTemplate::audioIn(float *input, int bufferSize, int nChannels){
    if(hasBegun && finishAudioAtTime > elapsedTime){
        if(renderMode && !finishingNow && !paused){
            videoRecorder.addAudioSamples(input, bufferSize, nChannels);
        }
    }
}

void ofxVideoGeneratorTemplate::useTimeFrom(ofxAVSync* _avSync){
    avSync = _avSync;
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
        if(videoRecorder.hasAudioError()) {
            ofLogWarning("Recorder failed to write some audio samples");
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

void ofxVideoGeneratorTemplate::setupSound(){
    soundStream.setDeviceID(getSoundflowerDeviceId());
    outChannels = 0;
    inChannels = 2;
    sampleRate = 44100; //Make sure SoundFlower is
    bufferSize = 256;   //configured using these values
    numBuffers = 4;
    soundStream.setup(this, outChannels, inChannels, sampleRate, bufferSize, numBuffers);
}

int ofxVideoGeneratorTemplate::getSoundflowerDeviceId(){
    vector<ofSoundDevice> devices = ofSoundStreamListDevices();
    vector<ofSoundDevice>::iterator it = find_if(devices.begin(), devices.end(),
        [](const ofSoundDevice &d) -> bool {
            return d.name.find("Soundflower (2ch)") != string::npos;
        }
    );
    return it->deviceID;
}

uint64_t ofxVideoGeneratorTemplate::calculateAudioFinishTime(int frameCount){
    return (frameCount / 60) * 1000;
}