#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

enum ofxGifEncoderIsolation {
    DO_NOT_ISOLATE_DRAWING,
    ISOLATE_DRAWING
};

class ofxGifEncoderTemplate : public ofBaseApp{

public:
    void setup(string _filename, int _renderFromFrameCount=-1, int _width=ofGetWidth(), int _height=ofGetHeight(), ofxGifEncoderIsolation isolation=DO_NOT_ISOLATE_DRAWING);
    void setupPaused(string _filename, int _unpauseFromFrameCount=-1, int _renderFromFrameCount=-1, int _width=ofGetWidth(), int _height=ofGetHeight(), ofxGifEncoderIsolation isolation=DO_NOT_ISOLATE_DRAWING);
    void setTextColor(ofColor _textColor);
    void fadeInOut(int numFramesIn, int numFramesOut, ofColor color=ofColor::black, int _numBlankFramesAfterFadeOut=0);
    void begin();
    void end();
    void endAndCaptureFrame();
    void endCaptureDraw();
    void draw();
    void pause();
    void unpause();
    void togglePause();
    void enableSlowMode();
    void captureFrame();
    void rotateAroundCenter(float degrees);
    void render();
    void exit();

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
    int colors, frameDuration;
    int framerate, renderOnFrame, unpauseOnFrame;
    string filename, renderMessage, recordingMessage;
    bool slowMode, renderingNow, paused;
    ofxGifEncoderIsolation drawingIsolation;
    
    ofxGifEncoder gifEncoder;
    ofFbo fbo;
    ofPixels pixels;
    ofColor fadeColor, textColor;
    float fadeAlpha, fadeAlphaIncrementIn, fadeAlphaIncrementOut;
    bool fade;
    int beginFadeOutOnFrame, numBlankFramesAfterFadeOut;
};