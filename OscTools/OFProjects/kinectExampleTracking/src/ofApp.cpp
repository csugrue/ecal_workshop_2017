#include "ofApp.h"



using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup() {
	
    ofSetLogLevel(OF_LOG_VERBOSE);
	
	
    // --------- KINECT AND TRACKING SETUP
    // enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(false, false); // disable video image (faster fps)
	kinect.open();		// opens first available kinect
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
		
	// -----------
    contourFinder.setMinAreaRadius(1);
    contourFinder.setMaxAreaRadius(3000);
    contourFinder.setThreshold(100);
    //contourFinder.getTracker().setSmoothingRate(.8);

    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(15);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(32);
    
    
    //------- gui
    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    gui.setPosition(10, 500);
    gui.add(nearThreshold.set("near", 255,0,255));
    gui.add(farThreshold.set("far", 200,0,255));

	//------- osc
    oscDestination = OSC_DESTINATION_DEFAULT;
    oscAddress = OSC_ADDRESS_ROOT_DEFAULT;
    oscPort = OSC_PORT_DEFAULT;
    
    // load settings from file
    
    ofXml xml;
    if(xml.load("settings_kinect.xml")){
        oscDestination = xml.getValue("ip");
        oscPort = ofToInt(xml.getValue("port"));
        oscAddress = xml.getValue("address");
    }
    
    osc.setup(oscDestination, oscPort);

	
}

//--------------------------------------------------------------
void ofApp::update() {
	
	ofBackground(100, 100, 100);
	
	// --------- KINECT AND TRACKING
    kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels());
		
		grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		
        grayImage.flagImageChanged();
        
        grayImage.dilate();
        grayImage.blurGaussian();
        contourFinder.findContours(grayImage);
        
        ofxOscMessage msg;
        msg.setAddress(oscAddress);
        
        float w = kinect.width;
        
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
    // ---------
    
    
   // smoothPoints[i] = .*smoothPoints[i] + .01 * currentPoints[i];
    
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	ofSetColor(255, 255, 255);
    
    // depth image mini
    kinect.drawDepth(640,0,160,120);
    
    // drwa binary image
    grayImage.draw(0,0);
    
    // draw contours
    contourFinder.draw();
    
    // draw tracking info
    RectTracker& tracker = contourFinder.getTracker();

    ofSetColor(255, 0, 255);
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

    // show osc info
    string oscMessageString = "Sending OSC to "+oscDestination+", port "+ofToString(oscPort)+": address "+oscAddress;
    ofDrawBitmapString(oscMessageString, 15, ofGetHeight() - 10);

    
    // show gui
    gui.draw();
    
}



//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
	switch (key) {
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		
		
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}