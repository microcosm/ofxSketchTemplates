#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"
#include "ofxAVSync.h"

enum ofxGifEncoderIsolation {
    DO_NOT_ISOLATE_DRAWING,
    ISOLATE_DRAWING
};

class ofxVideoGeneratorTemplate : public ofBaseApp{

public:
    void setup(int _renderFromFrameCount, int _width=ofGetWidth(), int _height=ofGetHeight(), ofxGifEncoderIsolation isolation=DO_NOT_ISOLATE_DRAWING);
    void setupPaused(int _unpauseFromFrameCount=-1, int _renderFromFrameCount=-1, int _width=ofGetWidth(), int _height=ofGetHeight(), ofxGifEncoderIsolation isolation=DO_NOT_ISOLATE_DRAWING);
    void setup(string _filename, int _recordToFrameCount, int _width=ofGetWidth(), int _height=ofGetHeight(), ofxGifEncoderIsolation isolation=DO_NOT_ISOLATE_DRAWING);
    void setupPaused(string _filename, int _unpauseFromFrameCount=-1, int _renderFromFrameCount=-1, int _width=ofGetWidth(), int _height=ofGetHeight(), ofxGifEncoderIsolation isolation=DO_NOT_ISOLATE_DRAWING);
    void update(ofEventArgs& args);
    void setTextColor(ofColor _textColor);
    void enableTextOverlay();
    void fadeInOut(int numFramesIn, int numFramesOut, ofColor color=ofColor::black, int _numBlankFramesAfterFadeOut=0);
    void begin();
    void end();
    void endAndCaptureFrame();
    void endCaptureDraw();
    void draw();
    void pause();
    void unpause();
    void togglePause();
    void enableRenderMode();
    void captureFrame();
    void rotateAroundCenter(float degrees);
    void finish();
    void exit();
    ofVec2f size();
    void audioIn(float * input, int bufferSize, int nChannels);
    void useTimeFrom(ofxAVSync* _avSync);

    int width, height;
    int halfWidth, halfHeight;
    int doubleWidth, doubleHeight;
    int tripleWidth, tripleHeight;
protected:
    void drawFboIntoGifEncoder();
    void drawFadeIfNeeded();
    void drawFade();
    void drawBlankFrames();
    void beginLayerIsolation();
    void endLayerIsolation();
    void setupSound();
    int getSoundflowerDeviceId();
    uint64_t calculateAudioFinishTime(int frameCount);

    int colors, frameDuration;
    int framerate, finishOnFrame, unpauseOnFrame;
    string filename, message, recordingMessage;
    bool renderMode, finishingNow, paused;
    ofxGifEncoderIsolation drawingIsolation;

    ofSoundStream soundStream;
    ofxAVSync* avSync;
    int outChannels, inChannels, sampleRate, bufferSize, numBuffers;
    uint64_t elapsedTime, finishAudioAtTime;
    bool hasBegun;

    ofxVideoRecorder videoRecorder;
    ofFbo fbo;
    ofPixels pixels;
    ofColor fadeColor, textColor;
    float fadeAlpha, fadeAlphaIncrementIn, fadeAlphaIncrementOut;
    bool fade, textOverlay;
    int beginFadeOutOnFrame, numBlankFramesAfterFadeOut;
};