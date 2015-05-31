// uncomment to compile on RPI and activate
// also add -Wiringpi to compiler settings ( // todo: )
#define USE_RPI_GPIO
//#define USE_PROGRAMMABLE_GL

#include "ofMain.h"
#include "enigmatikViewer.h"

#ifdef USE_RPI_GPIO
#ifdef TARGET_OPENGLES
#include "ofGLProgrammableRenderer.h"
#endif
#include "ofAppEGLWindow.h"
#else
//#include "ofAppGLFWWindow.h"
//#import <OpenGL/OpenGL.h>
//#import <GLUT/GLUT.h>
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
	
	ofGLESWindowSettings windowSettings;
	windowSettings.width = 800;
	windowSettings.height = 600;
	windowSettings.setGLESVersion(2);
	shared_ptr<ofAppBaseWindow> window = ofCreateWindow(windowSettings);
	
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
	
	#ifdef USE_PROGRAMMABLE_GL
		// we are using the programmable gl renderer.
	
		ofGLESWindowSettings windowSettings;
		//ofGLWindowSettings windowSettings;
		//ofGLFWWindowSettings windowSettings;
		windowSettings.width = 800;
		windowSettings.height = 600;
	
		windowSettings.setGLESVersion(2);
		//windowSettings.setGLVersion(3, 2);
		shared_ptr<ofAppBaseWindow> window = ofCreateWindow(windowSettings);
	
		// configure renderer
		ofGLProgrammableRenderer * renderer = new ofGLProgrammableRenderer( window.get() );
		renderer->setBackgroundAuto(false);
		renderer->enableAntiAliasing();
		ofSetCurrentRenderer(shared_ptr<ofBaseRenderer>((ofBaseRenderer*) renderer));
	
		ofLogVerbose("enigmatikViewer") << "Running in desktop mode using PROGRAMMABLE_GL " << glGetString(GL_VERSION) << endl;
	#else
	
		//ofGLESWindowSettings windowSettings;
		ofGLWindowSettings windowSettings;
		//ofGLFWWindowSettings windowSettings;
		windowSettings.width = 800;
		windowSettings.height = 600;
	
		windowSettings.setGLVersion(3, 2);
		shared_ptr<ofAppBaseWindow> window = ofCreateWindow(windowSettings);
		//ofDisableArbTex();
	#endif
	
#endif
	
	ofSetVerticalSync(false);
	ofEnableAlphaBlending(); // otherwise alpha channel is ignored by GL
	ofSetBackgroundColor(0,255,255,.5f);//ofColor(255,255,255,255));
	ofSetBackgroundAuto(false); // tmp
	
	shared_ptr<enigmatikViewer> mainApp(new enigmatikViewer);
	ofRunApp(window, mainApp);
	ofRunMainLoop();
}
