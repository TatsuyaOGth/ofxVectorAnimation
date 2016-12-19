#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup(){
    
    mDrawerParams.setName("DRAWER_PARAMS");
    mDrawerParams.add(bFill.set("FILL", false));
    mDrawerParams.add(mStrokeWidth.set("STROKE_WIDTH", 1, 1, 255));
    mDrawerParams.add(mFillColor.set("FILL_COLOR", ofColor::black));
    mDrawerParams.add(mStrokeColor.set("STROKE_COLOR", ofColor::black));
    
    mAnimParams.setName("ANIMATION_PARAMS");
    mAnimParams.add(bPlay.set("PLAY", false));
    mAnimParams.add(mCurrentFrame.set("FRAME", 0, 0, 0));
    mAnimParams.add(mFps.set("FPS", 16, 1, 60));
    mAnimParams.add(mNumPrevFrameShow.set("PREVIOUS_FRAME", 1, 0, 6));
    mAnimParams.add(mOpacityPrevFrame.set("PREVFRAME_OPACITY", 0.2, 0, 1));
    
    mGui.setup();
    mGui.add(mDrawerParams);
    mGui.add(mAnimParams);
    mGui.setName("PARAMETERS");
    mGui.minimizeAll();
    
    bShowGui = true;
    
    bPlay.addListener(this, &ofApp::onChangedPlayButton);
    
    mMsgBox = make_shared<MessageBox>();
}

//--------------------------------------------------------------
void ofApp::update(){

    mAnime.setFilled(bFill);
    mAnime.setStrokeWidth(mStrokeWidth);
    mAnime.setFillColor(mFillColor);
    mAnime.setStrokeColor(mStrokeColor);
    
    if (mCurrentFrame.getMax() != mAnime.getNumFrames() - 1)
    {
        mCurrentFrame.setMax(mAnime.getNumFrames() - 1);
        mCurrentFrame = mAnime.getCurrentNumFrame();
    }
    
    if (mAnime.isPlaying())
        mCurrentFrame = mAnime.getCurrentNumFrame();
    else
        mAnime.changeFrame(mCurrentFrame);
    
    mAnime.setFrameRate(mFps);
    
    mAnime.update();
    
    mMsgBox->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    
    ofEnableAlphaBlending();
    
    if (mAnime.isPlaying() == false)
    {
        ofSetColor(255, (int)(mOpacityPrevFrame * 255));
        for (int i = 0; i < mNumPrevFrameShow; ++i)
        {
            mAnime.draw(mAnime.getCurrentNumFrame() - (i + 1));
        }
    }
    
    ofSetColor(255, 255);
    mAnime.draw();
    
    
    if (bShowGui)
    {
        mGui.draw();
        
        stringstream ss;
        ss << "FRAME: " << mAnime.getCurrentNumFrame() + 1 << "/" << mAnime.getNumFrames() << endl;
        ss << "FPS: " << mAnime.getFrameRate() << endl;
        ofSetColor(0, 255);
        ofDrawBitmapString(ss.str(), mGui.getPosition().x + 10, mGui.getPosition().y + mGui.getHeight() + 20);
    }
    
    mMsgBox->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key)
    {
        case 's':
            if (bOnCommandKey)
            {
                if (bOnShiftKey)
                {
                    if (mAnime.saveDialog())
                        mMsgBox->show("Saving file was succeed.");
                    else
                        mMsgBox->show("Saving file was failed.");
                }
                else {
                    if (mAnime.save())
                        mMsgBox->show("Saving file was succeed.");
                    else
                        mMsgBox->show("Saving file was failed.");

                }
            }
            break;
            
        case 'o':
            if (bOnCommandKey)
            {
                if (mAnime.loadDialog())
                    mMsgBox->show("Loading file was succeed.");
                else
                    mMsgBox->show("Loading file bas failed.");
            }
            break;
            
        case 'z':
            if (bOnCommandKey && bOnShiftKey)
                mAnime.redo();
            else if (bOnCommandKey)
                mAnime.undo();
            break;
            
        case 'y':
            if (bOnCommandKey) mAnime.redo();
            break;
            
        case 'c':
            if (bOnCommandKey) mAnime.clear();
            break;
            
        case OF_KEY_RETURN:
            if (mAnime.getCurrentNumFrame() == mAnime.getNumFrames() - 1)
                mAnime.addFrame();
            else
                mAnime.insertFrame(mAnime.getCurrentNumFrame());
            break;
            
        case OF_KEY_BACKSPACE:
        case OF_KEY_DEL:
            mAnime.removeFrame();
            break;
            
        case OF_KEY_LEFT:
            mCurrentFrame = mAnime.backFrame();
            break;
            
        case OF_KEY_RIGHT:
            mCurrentFrame = mAnime.nextFrame();
            break;
            
        case ' ':
            bPlay = !bPlay;
            break;
            
        case 'g':
            bShowGui = !bShowGui;
            break;
    }
    
    if (key == OF_KEY_COMMAND) bOnCommandKey = true;
    if (key == OF_KEY_SHIFT) bOnShiftKey = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == OF_KEY_COMMAND) bOnCommandKey = false;
    if (key == OF_KEY_SHIFT) bOnShiftKey = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    mAnime.addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    mAnime.addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    mAnime.addVertex(x, y);
    mAnime.establishVertices();
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){
    
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    mAnime.setDrawArea(0, 0, w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    string filename = dragInfo.files[0];
    if (mAnime.load(filename))
        mMsgBox->show("Loading file was succeed.");
    else
        mMsgBox->show("Loading file bas failed.");
}

void ofApp::onChangedPlayButton(bool &e)
{
    e ? mAnime.play() : mAnime.stop();
}






MessageBox::MessageBox(): mTick(FLT_MAX), mDuration(5), mOpacity(0) {}

void MessageBox::update()
{
    if (mTick < mDuration)
    {
        float p = (mTick / mDuration);
        if (p > 0.8)
            mOpacity = 1 - ((p - 0.8) / 0.2);
        else
            mOpacity = 1;
        mTick += ofGetLastFrameTime();
    }
    else
        mOpacity = 0;
}

void MessageBox::draw()
{
    if (mOpacity > 0)
    {
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofEnableAlphaBlending();
        ofColor bg(0, (int)(mOpacity * 255));
        ofColor fg(255, (int)(mOpacity * 255));
        
        
        int maxLineLength = 0;
        for(int j = 0; j < (int)mMessage.size(); j++) {
            if (mMessage[j] == '\t') {
                maxLineLength += 8 - (maxLineLength % 8);
            } else {
                maxLineLength++;
            }
        }
        
        int fontSize = 8;
        int width = maxLineLength * fontSize;

        ofDrawBitmapStringHighlight(mMessage, ofGetWidth()/2 - (width/2), ofGetHeight()/2, bg, fg);
        ofPopStyle();
    }
}

void MessageBox::show(string message)
{
    mMessage = message;
    mTick = 0;
}

