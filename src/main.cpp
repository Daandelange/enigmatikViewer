// uncomment to compile on RPI and activate
// also add -Wiringpi to compiler settings ( // todo: )
//#define USE_RPI_GPIO

#include "ofMain.h"
#include "enigmatikViewer.h"

#ifdef USE_RPI_GPIO
#ifdef TARGET_OPENGLES
#include "ofGLProgrammableRenderer.h"
#endif
#include "ofAppEGLWindow.h"
#else
//#include "ofAppGLFWWindow.h"
#include "ofGLProgrammableRenderer.h"
#endif


//========================================================================
int main( ){

	ofSetLogLevel(OF_LOG_VERBOSE);
	
#ifdef USE_RPI_GPIO
	ofLogVerbose("enigmatikViewer") << "Running in rpi mode using OPENGL " << glGetString(GL_VERSION) << endl;
	// useful for later? http://ofxfenster.undef.ch/doc/structofAppEGLWindow_1_1Settings.html
	
	//ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLProgrammableRenderer()));
	//ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	
	//ofAppEGLWindow::Settings settings;
	//settings.eglWindowOpacity = 127;
	//settings.frameBufferAttributes[EGL_DEPTH_SIZE]   = 0; // 0 bits for depth
	//settings.frameBufferAttributes[EGL_STENCIL_SIZE] = 0; // 0 bits for stencil
	//ofAppEGLWindow window(settings);
	//ofSetupOpenGL( 800,600, OF_WINDOW);
	// car rearview screen resolution: 320*240
	
	ofGLESWindowSettings settings;
	settings.width = 800;
	settings.height = 600;
	settings.setGLESVersion(2);
	shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
	
	// configure renderer
	//shared_ptr<ofBaseRenderer> renderer = new ofGLProgrammableRenderer(window.get());
	//renderer->setBackgroundAuto(false);
	//renderer->enableAntiAliasing();
	//ofSetCurrentRenderer(renderer);
	
#else
	
	//ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	//ofAppGLFWWindow window;
	//ofSetupOpenGL(&window, 800,600, OF_WINDOW);// <-------- setup the GL context
	
	//ofPtr<ofBaseRenderer> renderer( new ofGLProgrammableRenderer(OF_WINDOW) );
	//renderer->setBackgroundAuto(false);
	//renderer->disableAntiAliasing();
	//ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	
	ofGLESWindowSettings settings;
	settings.width = 800;
	settings.height = 600;
	settings.setGLESVersion(2);
	shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
	
	// configure renderer
	//ofPtr<ofBaseRenderer> renderer( new ofGLProgrammableRenderer(window.get() ) );
	//renderer->setBackgroundAuto(false);
	//renderer->enableAntiAliasing();
	//ofSetCurrentRenderer(renderer);
	
	ofLogVerbose("enigmatikViewer") << "Running in desktop mode using OPENGL " << glGetString(GL_VERSION) << endl;
#endif
	
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	//ofSetBackgroundColor(0);//ofColor(255,255,255,255));
	ofSetBackgroundAuto(false); // tmp
	
	shared_ptr<enigmatikViewer> mainApp(new enigmatikViewer);
	ofRunApp(window, mainApp);
	ofRunMainLoop();
}
