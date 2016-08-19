#pragma once

#include "ofMain.h"
#include "ofxAudioGeneratorTemplate.h"
#include "ofxVideoGeneratorTemplate.h"
#include "ofxAVSync.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void beat();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxAudioGeneratorTemplate aud;
    ofxVideoGeneratorTemplate vid;
    ofxAVSync avSync;

    ofxAudioUnitManager* audioManager;
    ofxAudioUnitChain chain;
    ofxManagedAudioUnit synth;
    bool noteOn;

    ofVec2f position;
    ofColor color;
    float size, cutoff, tune, multiplier;
    bool visible;
};
