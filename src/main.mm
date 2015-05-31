#include "ofMain.h"
#include "enigmatikViewer.h"

//#include "ofAppGLFWWindow.h"
//#import <OpenGL/OpenGL.h>
//#import <GLUT/GLUT.h>
//#include "ofGLProgrammableRenderer.h"
#include "ofAppiOSWindow.h"

//========================================================================
int main( ){

	ofSetLogLevel(OF_LOG_VERBOSE);
	
	//  here are the most commonly used iOS window settings.
	//------------------------------------------------------
	ofiOSWindowSettings settings;
	settings.enableRetina = false; // enables retina resolution if the device supports it.
	settings.enableDepth = false; // enables depth buffer for 3d drawing.
	settings.enableAntiAliasing = false; // enables anti-aliasing which smooths out graphics on the screen.
	settings.numOfAntiAliasingSamples = 0; // number of samples used for anti-aliasing.
	settings.enableHardwareOrientation = false; // enables native view orientation.
	settings.enableHardwareOrientationAnimation = false; // enables native orientation changes to be animated.
	settings.glesVersion = OFXIOS_RENDERER_ES2; // type of renderer to use, ES1, ES2, etc.
	
	shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
	
	ofSetVerticalSync(false);
	ofEnableAlphaBlending(); // otherwise alpha channel is ignored by GL
	ofSetBackgroundColor(0,255,255,.5f);//ofColor(255,255,255,255));
	ofSetBackgroundAuto(false); // tmp
	
	shared_ptr<enigmatikViewer> mainApp(new enigmatikViewer);
	ofRunApp(window, mainApp);
	return ofRunMainLoop();
}

