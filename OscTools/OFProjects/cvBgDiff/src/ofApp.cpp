#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setup(320, 240);
    
    // imitate() will set up previous and diff
    // so they have the same size and type as cam
    imitate(bg, cam);
    imitate(diff, cam);
    
    contourFinder.setMinAreaRadius(1);
    contourFinder.setMaxAreaRadius(100);
    
    
    gui.setup();
    gui.add(thresholdValue.set("Threshold Value", 10, 0, 255));
    gui.add(minSize.set("Min Area",1,0,200));
    gui.add(maxSize.set("Max Area",100,0,200));
    gui.setPosition(10, 250);
    contourFinder.setThreshold(15);
   
    //------- osc
    oscDestination = OSC_DESTINATION_DEFAULT;
    oscAddress = OSC_ADDRESS_ROOT_DEFAULT;
    oscPort = OSC_PORT_DEFAULT;
    
    // load settings from file
    
    ofXml xml;
    if(xml.load("settings_cv.xml")){
        oscDestination = xml.getValue("ip");
        oscPort = ofToInt(xml.getValue("port"));
        oscAddress = xml.getValue("address");
    }
    
    osc.setup(oscDestination, oscPort);
    
    ofBackground(80,80,80);
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    
    if(cam.isFrameNew()){
        absdiff(cam, bg, diff);
        diff.update();
        contourFinder.findContours(diff);
        
        ofxOscMessage msg;
        msg.setAddress(oscAddress);
        
        float w = cam.getWidth();
        
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
    contourFinder.setThreshold(thresholdValue);
    contourFinder.setMinAreaRadius(minSize);
    contourFinder.setMaxAreaRadius(maxSize);
    
   
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    cam.draw(0,0);
    contourFinder.draw();
    RectTracker& tracker = contourFinder.getTracker();
    
    ofSetColor(255, 255, 255);
    for(int i = 0; i < contourFinder.size(); i++) {
        int label = contourFinder.getLabel(i);
        //float smoothX = tracker.getSmoothed(label).x + tracker.getSmoothed(label).width*.5;
        //float smoothY = tracker.getSmoothed(label).y + tracker.getSmoothed(label).height*.5;
        //ofDrawCircle(smoothX,smoothY,4);
        ofDrawCircle(contourFinder.getCentroid(i).x, contourFinder.getCentroid(i).y,4);
        ofPushMatrix();
        ofTranslate(contourFinder.getCentroid(i).x, contourFinder.getCentroid(i).y);
        string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
        ofDrawBitmapString(msg, 0, 0);
        ofPopMatrix();
        
        
    }
    ofSetColor(255, 255, 255);    bg.draw(cam.getWidth(),0);
    
    // show osc info
    string oscMessageString = "SPACE to capture background\nSending OSC to "+oscDestination+", port "+ofToString(oscPort)+": address "+oscAddress;
    ofDrawBitmapString(oscMessageString, 15, ofGetHeight() - 30);

    
     gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    copy(cam, bg);
    bg.update();

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
