// uncomment to compile on RPI and activate
// also add -Wiringpi to compiler settings ( // todo: )
//#define USE_RPI_GPIO

#include "ofMain.h"
#include "enigmatikViewer.h"
#include "ofAppEGLWindow.h"

//========================================================================
int main( ){
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	//ofSetupOpenGL(800,600,OF_WINDOW);			// <-------- setup the GL context

	ofAppEGLWindow::Settings settings;
	//settings.eglWindowOpacity = 127;
    settings.frameBufferAttributes[EGL_DEPTH_SIZE]   = 0; // 0 bits for depth
    settings.frameBufferAttributes[EGL_STENCIL_SIZE] = 0; // 0 bits for stencil
	ofAppEGLWindow window(settings);

	ofSetupOpenGL(&window, 800,600, OF_WINDOW);// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new enigmatikViewer());

}
