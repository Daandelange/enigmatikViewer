#pragma once

#include "enigmatikConstants.h"
#include "ofMain.h"
#include "ofEvents.h"
#include "enigmatikSlideshow.h"
#include "ofxMouseAutoHider.h"

class enigmatikViewer : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
private:
	
	// slideshow part
	enigmatikSlideshow slider;
		
};
