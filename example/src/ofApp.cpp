#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

}

//--------------------------------------------------------------
void ofApp::update(){
    mAnime.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    
    ofEnableAlphaBlending();
    ofSetColor(255, 90);
    mAnime.drawPrevFrame();
    
    ofSetColor(255, 255);
    mAnime.draw();
    
    
    stringstream ss;
    ss << "FRAME: " << mAnime.getCurrentNumFrame() + 1 << "/" << mAnime.getNumFrames() << endl;
    ss << "FPS: " << mAnime.getFrameRate() << endl;
    ofSetColor(0, 255);
    ofDrawBitmapString(ss.str(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key)
    {
        case 's':
            mAnime.save("drawpath.xml");
            break;
            
        case 'l':
            mAnime.load("drawpath.xml");
            break;
            
        case 'c':
            mAnime.clear();
            break;
            
        case OF_KEY_RETURN:
            mAnime.addFrame();
            break;
            
        case OF_KEY_LEFT:
            mAnime.backFrame();
            break;
            
        case OF_KEY_RIGHT:
            mAnime.nextFrame();
            break;
            
        case ' ':
            mAnime.togglePlay();
            break;
            
        case OF_KEY_UP:
            mAnime.setFrameRate(mAnime.getFrameRate() + 1);
            break;
            
        case OF_KEY_DOWN:
            mAnime.setFrameRate(mAnime.getFrameRate() - 1);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::windowResized(int w, int h){
    mAnime.setDrawArea(0, 0, w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
