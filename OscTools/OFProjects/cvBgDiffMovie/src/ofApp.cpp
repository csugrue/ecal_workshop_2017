#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    mov.load("fingers.mov");
    bUseMovie = false;
    
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
    
    bool bNewFrame = false;
    float w = 1;
    
    if(bUseMovie){
        mov.update();
        bNewFrame = mov.isFrameNew();
        w = mov.getWidth();
        if(bNewFrame){
            absdiff(mov, bg, diff);
        }
    }else{
        cam.update();
        w = cam.getWidth();
        bNewFrame = cam.isFrameNew();
        if(bNewFrame){
            absdiff(cam, bg, diff);
        }
    }
    
    
    
    if(bNewFrame){
        
        diff.update();
        contourFinder.findContours(diff);
        
        ofxOscMessage msg;
        msg.setAddress(oscAddress);
        
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
    
    if(bUseMovie)mov.draw(0,0);
    else cam.draw(0,0);
    
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
    ofSetColor(255, 255, 255);
    
    if(bUseMovie)bg.draw(mov.getWidth(),0);
    else bg.draw(cam.getWidth(),0);
    
    // show osc info
    string oscMessageString = "SPACE to capture background\nSending OSC to "+oscDestination+", port "+ofToString(oscPort)+": address "+oscAddress;
    ofDrawBitmapString(oscMessageString, 15, ofGetHeight() - 30);

    
     gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == ' '){
        if(bUseMovie) copy(mov, bg);
        else copy(cam, bg);
        bg.update();
    }else if(key == 'm'){
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
