// uncomment to compile on RPI and activate
// also add -Wiringpi to compiler settings ( // todo: )
//#define USE_RPI_GPIO

#include "ofMain.h"
#include "enigmatikViewer.h"

#ifdef USE_RPI_GPIO
#include "ofAppEGLWindow.h"
#else
#include "ofAppGLFWWindow.h"
#endif

// to use GL ES
#include "ofGLProgrammableRenderer.h"



//========================================================================
int main( ){
	

#ifdef USE_RPI_GPIO
	ofPtr<ofBaseRenderer> renderer( new ofGLProgrammableRenderer() );
	renderer->setBackgroundAuto(false);
	renderer->disableAntiAliasing();
	ofSetCurrentRenderer(renderer);
	//ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	
	ofAppEGLWindow::Settings settings;
	//settings.eglWindowOpacity = 127;
    settings.frameBufferAttributes[EGL_DEPTH_SIZE]   = 0; // 0 bits for depth
    settings.frameBufferAttributes[EGL_STENCIL_SIZE] = 0; // 0 bits for stencil
	ofAppEGLWindow window(settings);
	ofSetupOpenGL(&window, 800,600, OF_WINDOW);
	// car rearview screen resolution: 320*240
#else
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(800,600, OF_WINDOW);// <-------- setup the GL context
#endif

	ofRunApp(new enigmatikViewer());

}
