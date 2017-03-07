#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#define OSC_DESTINATION_DEFAULT "localhost"
#define OSC_ADDRESS_ROOT_DEFAULT "/cv/blobs"
#define OSC_PORT_DEFAULT 6448

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofVideoGrabber cam;
        ofImage diff;
        ofImage bg;
        ofxCv::ContourFinder contourFinder;
    
        ofxPanel gui;
        ofParameter<float> thresholdValue;
        ofParameter<int> minSize;
        ofParameter<int> maxSize;
    
    ofxOscSender osc;
    int oscPort;
    string oscDestination;
    string oscAddress;
};
