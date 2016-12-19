#pragma once

#include "ofMain.h"
#include "ofxVectorAnimation.h"
#include "ofxGui.h"

class MessageBox;

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void mouseScrolled(int x, int y, float scrollX, float scrollY );
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void onChangedPlayButton(bool& e);
    
    
    ofxVectorAnimation mAnime;
    
    ofxPanel mGui;
    
    ofParameterGroup mDrawerParams;
    ofParameter<bool> bFill;
    ofParameter<int> mStrokeWidth;
    ofParameter<ofColor> mFillColor;
    ofParameter<ofColor> mStrokeColor;
    
    ofParameterGroup mAnimParams;
    ofParameter<bool> bPlay;
    ofParameter<int> mCurrentFrame;
    ofParameter<int> mFps;
    ofParameter<int> mNumPrevFrameShow;
    ofParameter<float> mOpacityPrevFrame;
    
    bool bOnCommandKey;
    bool bOnShiftKey;
    bool bShowGui;
    
    shared_ptr<MessageBox> mMsgBox;
};




class MessageBox
{
    float mTick;
    float mDuration;
    float mOpacity;
    string mMessage;
    
public:
    MessageBox();
    
    void update();
    void draw();
    void show(string message);
};
