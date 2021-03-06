#include "enigmatikViewer.h"

//--------------------------------------------------------------
void enigmatikViewer::setup(){
	
	// some settings for optimisation
	//ofSetBackgroundColor(0,255,255,.5f);//ofColor(255,255,255,255));
	ofSetBackgroundAuto(false); // allows to keep same image when it doesn't change
	//ofSetVerticalSync(true); // cleaner output
	//ofDisableAntiAliasing();
	//ofDisableDepthTest();
	
	//ofEnableAlphaBlending(); // otherwise alpha channel is ignored by GL
	//ofEnableArbTex();
	
	// initialise slideshow
	slider.setup();
	slider.setFolder("slideshowImages");
	slider.enableControls();
	
	// add event controls
	
	
	ofxMouseAutoHider::enable();
}

//--------------------------------------------------------------
void enigmatikViewer::update(){

	// get/update button states
	
	
}

//--------------------------------------------------------------
void enigmatikViewer::draw(){
	
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



