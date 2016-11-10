#pragma once

#include "ofMain.h"

class ofxVectorAnimation
{
    class Shape : public ofPath
    {
    public:
        Shape();
        Shape(const Shape& o);
        //TODO: add draw type
    };
    
    class Frame
    {
    public:
        Frame();
        Frame(const Frame& o);
        vector<Shape> shapes;
    };
    
    // frame
    vector<shared_ptr<Frame> > mFrames;
    Shape mTmpShape;
    int mCurrentNumFrame;
    ofRectangle mDrawArea;
    
    // play transport
    bool bPlaying;
    bool bLoop;
    float mTick;
    float mFrameRate;
    
    ofFbo mDrawBuffer;
    
    
public:
    ofxVectorAnimation();
    virtual ~ofxVectorAnimation();
    
    void update();
    void draw();
    void draw(int numFrame);
    inline void drawPrevFrame() { draw(mCurrentNumFrame - 1); }
    inline void drawNextFrame() { draw(mCurrentNumFrame + 1); }
    
    void addVertex(int x, int y);
    void addVertex(const ofPoint& point);
    void establishVertices();

    int addFrame();
    int insertFrame(int frame);
    void removeFrame();
    void clear();
    
    void nextFrame(bool loop = false);
    void backFrame();
    void changeFrame(int frame);
    
    void play();
    void stop();
    void togglePlay();
    void setLoop(bool loopEnable);
    
    bool save(const string& filename);
    bool load(const string& filename);
    bool save();
    bool load();
    
    inline int  getCurrentNumFrame() const { return mCurrentNumFrame; }
    inline int  getNumFrames() const { return mFrames.size(); }
    
    inline void setFrameRate(int framerate) { mFrameRate = MAX(1, framerate); }
    inline int  getFrameRate() const { return mFrameRate; }
    
    inline void setDrawArea(ofRectangle rect) { mDrawArea = rect; allocateDrawBuffer(); }
    inline void setDrawArea(int x, int y, int w, int h) { mDrawArea.set(x, y, w, h); allocateDrawBuffer(); }
    inline const ofRectangle getDrawArea() const { return mDrawArea; }
    
protected:
    void allocateDrawBuffer();
    
};
