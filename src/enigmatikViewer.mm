#include "enigmatikViewer.h"

//--------------------------------------------------------------
void enigmatikViewer::setup(){
	
	// some settings for optimisation
	ofSetBackgroundAuto(false); // allows to keep same image when it doesn't change
	ofSetVerticalSync(true); // cleaner output
	ofDisableAntiAliasing();
	ofDisableDepthTest();
	ofxMouseAutoHider::enable();
	
	// initialise slideshow
	slider.setup();
	slider.setFolder("slideshowImages");
	slider.enableControls();
	
	// add event controls
	
	
}

//--------------------------------------------------------------
void enigmatikViewer::update(){

	// get/update button states
	
	
}

//--------------------------------------------------------------
void enigmatikViewer::draw(){
	ofSetColor(0,128,255,0.3);
	ofFill();
	ofDrawRectangle(0,0, ofGetWidth(), ofGetHeight());
}

void enigmatikViewer::exit(){
	
}

//--------------------------------------------------------------
void enigmatikViewer::keyPressed(int key){

}

//--------------------------------------------------------------
void enigmatikViewer::keyReleased(int key){

}

//--------------------------------------------------------------
void enigmatikViewer::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void enigmatikViewer::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void enigmatikViewer::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void enigmatikViewer::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void enigmatikViewer::windowResized(int w, int h){

}

//--------------------------------------------------------------
void enigmatikViewer::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void enigmatikViewer::dragEvent(ofDragInfo dragInfo){ 

}

// CUSTOM FUNCTIONS

//



