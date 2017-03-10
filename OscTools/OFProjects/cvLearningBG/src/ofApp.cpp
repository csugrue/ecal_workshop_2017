#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    //------- osc
    oscDestination = OSC_DESTINATION_DEFAULT;
    oscAddress = OSC_ADDRESS_ROOT_DEFAULT;
    oscPort = OSC_PORT_DEFAULT;
    
    int deviceId = 0;
    
    ofXml xml;
    if(xml.load("settings_cv.xml")){
        oscDestination = xml.getValue("ip");
        oscPort = ofToInt(xml.getValue("port"));
        oscAddress = xml.getValue("address");
        deviceId = ofToInt(xml.getValue("deviceId"));
    }
    
    mov.load("fingers.mov");
    bUseMovie = false;
    
    
    cam.setDeviceID(deviceId);
    cam.setup(320, 240);
    
    gui.setup();
    gui.add(resetBackground.set("Reset Background", false));
    gui.add(learningTime.set("Learning Time", 30, 0, 30));
    gui.add(thresholdValue.set("Threshold Value", 10, 0, 255));
    gui.add(minSize.set("Min Area",1,0,200));
    gui.add(maxSize.set("Max Area",100,0,200));
    gui.setPosition(10, 250);
    
    
    
    osc.setup(oscDestination, oscPort);
    
    ofBackground(80,80,80);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    bool bNewFrame = false;
    
    if(resetBackground) {
        background.reset();
        resetBackground = false;
    }
    
    if(bUseMovie){
        mov.update();
        bNewFrame = mov.isFrameNew();
        
    }else{
        cam.update();
        bNewFrame = cam.isFrameNew();
        
    }
    
    if(bNewFrame){
        background.setLearningTime(learningTime);
        background.setThresholdValue(thresholdValue);
        if(bUseMovie)background.update(mov, thresholded);
        else background.update(cam, thresholded);
        thresholded.update();
        contourFinder.findContours(thresholded);
        
    }
    
    contourFinder.setThreshold(thresholdValue);
    contourFinder.setMinAreaRadius(minSize);
    contourFinder.setMaxAreaRadius(maxSize);
    
    if(bNewFrame){
    
        ofxOscMessage msg;
        msg.setAddress(oscAddress);
        int w = cam.getWidth();
        if(bUseMovie) w = mov.getWidth();
        
        // add contours
        RectTracker& tracker = contourFinder.getTracker();
        for(int i = 0; i < contourFinder.size(); i++) {
            int label = contourFinder.getLabel(i);
            msg.addIntArg(i);
            msg.addIntArg(label);
            msg.addIntArg(tracker.getAge(label));
            msg.addFloatArg(contourFinder.getCentroid(i).x/w);
            msg.addFloatArg(contourFinder.getCentroid(i).y/w);
            msg.addFloatArg(contourFinder.getBoundingRect(i).x/w);
            msg.addFloatArg(contourFinder.getBoundingRect(i).y/w);
            msg.addFloatArg(contourFinder.getBoundingRect(i).width/w);
            msg.addFloatArg(contourFinder.getBoundingRect(i).height/w);
        }
        
        // send
        osc.sendMessage(msg);
    }

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(bUseMovie)mov.draw(0,0);
    else cam.draw(0,0);
    
    contourFinder.draw();
    RectTracker& tracker = contourFinder.getTracker();
    
    ofSetColor(255, 255, 255);
    for(int i = 0; i < contourFinder.size(); i++) {
        int label = contourFinder.getLabel(i);
        ofDrawCircle(contourFinder.getCentroid(i).x, contourFinder.getCentroid(i).y,4);
        ofPushMatrix();
        ofTranslate(contourFinder.getCentroid(i).x, contourFinder.getCentroid(i).y);
        string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
        ofDrawBitmapString(msg, 0, 0);
        ofPopMatrix();
        
        
    }
    ofSetColor(255, 255, 255);
    // show osc info
    string oscMessageString = "SPACE to capture background\nSending OSC to "+oscDestination+", port "+ofToString(oscPort)+": address "+oscAddress;
    ofDrawBitmapString(oscMessageString, 15, ofGetHeight() - 30);
    
    
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'm'){
        bUseMovie = !bUseMovie;
        if(bUseMovie) mov.play();
        
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
