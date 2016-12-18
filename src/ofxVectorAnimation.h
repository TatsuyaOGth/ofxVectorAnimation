#pragma once

#include "ofMain.h"

class ofxVectorAnimation
{
public:
    class Shape : public ofPath
    {
    public:
        Shape();
        Shape(const Shape& o);
    };
    
    class Frame
    {
    public:
        Frame();
        Frame(const Frame& o);
        vector<Shape> shapes;
    };
    
protected:
    
    // frame and shape
    vector<shared_ptr<Frame> > mFrames;
    Shape mTmpShape;
    ofPoint mLastDrawPoint;
    bool bHasDrewVertex;
    stack<Shape> mRedoShape;
    int mCurrentNumFrame;
    ofRectangle mDrawArea;
    
    // play transport
    bool bPlaying;
    bool bLoop;
    float mTick;
    float mFrameRate;
    
    bool bSmoothing;
    
    ofFbo mDrawBuffer;
    string mFilePath;
    
public:
    ofxVectorAnimation();
    virtual ~ofxVectorAnimation();
    
    void update();

    void draw();
    void draw(float x, float y, float w, float h);
    void draw(float x, float y);
    void draw(ofPoint& pos);
    void draw(ofRectangle& rect);
    
    void draw(int numFrame);
    void draw(int numFrame, float x, float y, float w, float h);
    void draw(int numFrame, float x, float y);
    void draw(int numFrame, ofPoint& pos);
    void draw(int numFrame, ofRectangle& rect);
    
    inline void drawPrevFrame() { draw(mCurrentNumFrame - 1); }
    inline void drawNextFrame() { draw(mCurrentNumFrame + 1); }
    inline void drawPrevFrame(float x, float y, float w, float h) { draw(mCurrentNumFrame - 1, x, y, h, w); }
    inline void drawNextFrame(float x, float y, float w, float h) { draw(mCurrentNumFrame + 1, x, y, h, w); }
    inline void drawPrevFrame(float x, float y) { draw(mCurrentNumFrame - 1, x, y); }
    inline void drawNextFrame(float x, float y) { draw(mCurrentNumFrame + 1, x, y); }
    inline void drawPrevFrame(ofPoint& pos) { draw(mCurrentNumFrame - 1, pos); }
    inline void drawNextFrame(ofPoint& pos) { draw(mCurrentNumFrame + 1, pos); }
    inline void drawPrevFrame(ofRectangle& rect) { draw(mCurrentNumFrame - 1, rect); }
    inline void drawNextFrame(ofRectangle& rect) { draw(mCurrentNumFrame + 1, rect); }
    
    inline void setAnchorPercent(float xPct, float yPct) { mDrawBuffer.setAnchorPercent(xPct, yPct); }
    inline void setAnchorPoint(float x, float y) { mDrawBuffer.setAnchorPoint(x, y); }
    inline void resetAnchor() { mDrawBuffer.resetAnchor(); }

    ofTexture getFrameTexture();
    ofTexture getFrameTexture(int numFrame);
    
    
    void addVertex(int x, int y);
    void addVertex(const ofPoint& point);
    void establishVertices(bool simplify = true, float tolerance = 0.3);
    void undo();
    void redo();

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
    bool saveDialog();
    bool loadDialog();
    
    const Frame& getFrame() const;
    const vector<Shape>& getShapes() const;
    vector<ofPath> getPaths() const;
    vector<vector<ofPoint> > getPoints() const;
    
    const Frame& getFrame(int numFrame) const;
    const vector<Shape>& getShapes(int numFrame) const;
    vector<ofPath> getPaths(int numFrame) const;
    vector<vector<ofPoint> > getPoints(int numFrame) const;
    
    inline int  getCurrentNumFrame() const { return mCurrentNumFrame; }
    inline int  getNumFrames() const { return mFrames.size(); }
    
    inline void setFrameRate(int framerate) { mFrameRate = MAX(1, framerate); }
    inline int  getFrameRate() const { return mFrameRate; }
    
    inline void setDrawArea(ofRectangle rect) { mDrawArea = rect; allocateDrawBuffer(); }
    inline void setDrawArea(int x, int y, int w, int h) { mDrawArea.set(x, y, w, h); allocateDrawBuffer(); }
    inline const ofRectangle getDrawArea() const { return mDrawArea; }
    inline float getWidth() const { return mDrawArea.getWidth(); }
    inline float getHeight() const { return mDrawArea.getHeight(); }
    
    inline bool isPlaying() const { return bPlaying; }
    
    inline const string& getFilePath() const { return mFilePath; }
    
    inline void setSmoothing(bool enable) { bSmoothing = enable; }
    inline bool isSmoothing() const { return bSmoothing; }
    
    inline bool hasDrewVertex() { return bHasDrewVertex; }
    
protected:
    void initVertecs();
    void allocateDrawBuffer();
    void renderFrame(int frameIndex);
};
