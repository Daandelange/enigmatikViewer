// uncomment to compile on RPI and activate
// also add -Wiringpi to compiler settings ( // todo: )
//#define USE_RPI_GPIO

#include "ofMain.h"
#include "enigmatikViewer.h"

#ifdef USE_RPI_GPIO
#include "ofAppEGLWindow.h"
#endif

//========================================================================
int main( ){
	ofPtr<ofBaseRenderer> renderer( new ofGLProgrammableRenderer() );
	renderer->setBackgroundAuto(false);
	renderer->disableAntiAliasing();
	ofSetCurrentRenderer(renderer);

#ifdef USE_RPI_GPIO
	
	
	ofAppEGLWindow::Settings settings;
	//settings.eglWindowOpacity = 127;
    settings.frameBufferAttributes[EGL_DEPTH_SIZE]   = 0; // 0 bits for depth
    settings.frameBufferAttributes[EGL_STENCIL_SIZE] = 0; // 0 bits for stencil
	ofAppEGLWindow window(settings);
	ofSetupOpenGL(&window, 800,600, OF_WINDOW);
#else
	
	//ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(800,600, OF_WINDOW);// <-------- setup the GL context
#endif

	ofRunApp(new enigmatikViewer());

}
