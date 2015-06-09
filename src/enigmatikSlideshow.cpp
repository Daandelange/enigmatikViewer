//
//  enigmatikSlideshow.cpp
//  enigmatikViewer
//
//  Created by Daan de Lange on 13/4/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

// todo: make a file increment system to store images in the receival-order while storing their original names in an xml file.
// The original file name can then be shown for a few secs once you unlock a new slide.

// todo: slider rendering system

// todo: slider loading system

// todo: slider button-based transition system

#include "enigmatikConstants.h"
#include "enigmatikSlideshow.h"

//#include <algorithm>// needed for std::vector::find
//#include <map>// needed for std::vector::find

// MARK: Init

void enigmatikSlideshow::setup() {
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	fbo.begin();
	ofClear(0,0,0,1);
	ofClearAlpha();
	fbo.end();
	
	// todo: load currentSlideNum from xml settings
	currentSlideNum = 0;
	nextSlideNum = currentSlideNum + slideDirection;
	imagesFolder = "slideshowImages";
	showControls = true;
	slideDirection = 1; // 1 or -1
	
	// shader stuff
#if defined(TARGET_OPENGLES) || defined(BLA)
	ofLogVerbose("enigmatikSlideshow", "setup(): Loading GLES2 shader.");
	sGlitch2.load("shaders/shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
		ofLogVerbose("enigmatikSlideshow", "setup(): Loading GL3 shader.");
		sGlitch2.load("shaders/shadersGL3/shader");
	}else{
		ofLogVerbose("enigmatikSlideshow", "setup(): Loading GL2 shader.");
		sGlitch2.load("shaders/shadersGL2/shader");
	}
#endif
	//ofLogVerbose("enigmatikSlideshow", "setup(): Loading ???? shader.");
	//sGlitch2.load("shaders_GLES/shader");
	
	// setup GUI
	gui.setup("Slide Show Control", "slideShowSettings.xml");
	gui.add( prevButton.setup("<-- PREV SLIDE") );
	gui.add( currentSlideNum.setup("Current slide", 0, 20, 0) );
	//currentSlideNum.unregisterMouseEvents(); not working
	gui.add( nextButton.setup("--> NEXT SLIDE") );
	
	// load last used settings from save file
	//gui.loadFromFile("slideShowSettings.xml");
	loadSlideNum();
	
	// load images folder
	reloadFolderContent();
	
	// setup & cache current & next Slides
    currentSlide = *new ofImage();
    nextSlide = *new ofImage();
	cacheCurrentSlides();
	
	// init effects and button states
	resetEffects();
	
	// grid for easy shader communication
	slideGrid.set( ofGetWidth(), ofGetHeight() );
	slideGrid.setPosition( 0, 0, 0);//ofGetWidth()/2, ofGetHeight()/2, 0 );
	// 10 pixels per grid square
	slideGrid.setResolution( ceil(ofGetWidth()/10), ceil(ofGetHeight()/10) );
	slideGrid.setMode(OF_PRIMITIVE_TRIANGLES);
	slideGrid.mapTexCoordsFromTexture(currentSlide.getTextureReference());
	
	// bind slide-change event to XML writedown
	currentSlideNum.addListener(this, &enigmatikSlideshow::rememberSlideNum);
	prevButton.addListener(this, &enigmatikSlideshow::loadPrevSlide );
	nextButton.addListener(this, &enigmatikSlideshow::loadNextSlide );
	
#ifdef USE_THREADED_IMAGE_LOADER
	// image loaded event
	ofAddListener(imgLoader.imageLoaded,this, &enigmatikSlideshow::newImageLoaded);
#endif
	
	// setup buttons
	buttons.setName("Button States");
	
	// name virtual buttons/controls/parameters
	param2.setName("param2 (pot.rot)");
	param3.setName("param3 (rot.enc)");
	//param4.setName("param4 (3.pos)");
	param6.setName("param6 (pot.slide)");
	
#ifdef USE_RPI_GPIO
	// todo: link virtual buttons to physical buttons
	//param3.linkWithGPIO(1,16);
#endif
	
#ifdef USE_KEYBOARD_SHORTCUTS
	#pragma message("USE_KEYBOARD_SHORTCUTS is defined!")
	// bind keyboard shortcuts to control the effects
	enigmaPressedKeys.clear();
	ofAddListener(ofEvents().keyPressed, this, &enigmatikSlideshow::enigmaKeyPressed);
	ofAddListener(ofEvents().keyReleased, this, &enigmatikSlideshow::enigmaKeyReleased);
#endif
	
	// add them to a gui group
	buttons.add( param1 );
	buttons.add( param2 );
	buttons.add( param3 );
	buttons.add( param4 );
	buttons.add( param5 );
	buttons.add( param6 );
	//
	gui.add(buttons);
	
	// bind button parameters to glitches
	param2.addListener(this, &enigmatikSlideshow::setParam2Solved);
	param3.addListener(this, &enigmatikSlideshow::setParam3Solved);
	param6.addListener(this, &enigmatikSlideshow::setParam6Solved);
	
	// bind events for quitting
	ofAddListener(ofEvents().update, this, &enigmatikSlideshow::_update);
	ofAddListener(ofEvents().draw, this, &enigmatikSlideshow::_draw);
	ofAddListener(ofEvents().exit, this, &enigmatikSlideshow::_exit);
	
	// render output image
	reRenderOutput=true;
	reDrawOutput = true;
	
	// for Rpi this (may?) needs to be defined here
	ofSetBackgroundAuto(false);
}


// MARK: Loop
void enigmatikSlideshow::update() {
	
}

void enigmatikSlideshow::_update(ofEventArgs &e) {
	// no need to get button state, using interrupts :) (callbacks)
	
	// generate mixed image
	//if(true){ // todo: only regen if buttons' state changed
		//mixedSlide = currentSlide;// tmp
	//}
	
	// call the virtual (over-writeable) function
	update();
	
	// temp: reload shader on demand
	if( ofGetKeyPressed('r') ){
		ofLogNotice("enigmatikSlideshow::_update()") << "Reloading shaders...";
		sGlitch2.load("shaders/shadersES2/shader");
		//sGlitch2.load("shaders/shadersGL3/shader");
		
	}
	
	// handle pressed keys
#ifdef USE_KEYBOARD_SHORTCUTS
	if(enigmaPressedKeys.size()>0) for(enigmaKeyMap::iterator it=enigmaPressedKeys.begin(); it != enigmaPressedKeys.end(); /*nothing*/ ){
		
		// slideshow control
		if( it->first == OF_KEY_LEFT || it->first == OF_KEY_RIGHT ){
			(it->first == OF_KEY_LEFT)?loadPrevSlide():loadNextSlide();
			
			it->second.keyArg = 0;
			it->second.isPressed = false;
		}
		
		// gui hider
		else if(it->first == ']' ) {
			showControls = !showControls;
			it->second.keyArg = 0;
			it->second.isPressed = false;
		}
		
		else {
			// update value velocity
			if(it->second.isPressed == true){
				//cout << it->second.keyArg << endl;
				it->second.keyArg += 0.05f;
				if(it->second.keyArg > 1.f) it->second.keyArg=1.f; // max out value
			}
			else{
				it->second.keyArg *= 0.86f;
			}
			
			// synch with variables
			switch( it->first ){
				// PARAM 2
				case '1':
					param2 -= it->second.keyArg*6.f;
					if(param2 < param2.getMin() ) param2.set(param2.getMin());
					break;
				case '2':
					param2 += it->second.keyArg*6.f;
					if(param2 > param2.getMax() ) param2.set(param2.getMax());
					break;
				// PARAM 3
				case 'q':
					param3 -= it->second.keyArg*6.f;
					if(param3 < param3.getMin() ) param3.set(param3.getMin());
					break;
				case 'w':
					param3 += it->second.keyArg*6.f;
					if(param3 > param3.getMax() ) param3.set(param3.getMax());
					break;
				// PARAM 6
				case 'a':
					param6 -= it->second.keyArg*6.f;
					if(param6 < param2.getMin() ) param6.set(param6.getMin());
					break;
				case 's':
					param6 += it->second.keyArg*6.f;
					if(param6 > param6.getMax() ) param6.set(param6.getMax());
					break;
				default:
					
					// do nothing
					break;
			}
		}
			
		// erase & increment
		if( it->second.keyArg<0.05f ){
			auto item = enigmaPressedKeys.find(it->first);
			enigmaPressedKeys.erase( it++ );
		}
		else {
			it++;
		}
		
	}
#endif
	
	if( reRenderOutput == true ){
		ofLogVerbose("enigmatikSlideshow::_update") << "Rendering a new image";
		
		// update fbo image
		fbo.begin();
		ofPushMatrix();
		ofPushStyle();
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
		
		//currentSlide.bind();
		//nextSlide.bind();
		
		sGlitch2.begin();
		// upload variables to shader
		sGlitch2.setUniform1f("param2Solved", param2Solved);
		sGlitch2.setUniform1f("param3Solved", param3Solved);
		sGlitch2.setUniform1f("param6Solved", param6Solved);
		sGlitch2.setUniformTexture("tex0", currentSlide.getTextureReference(), 0);// currentSlide.getTextureReference().getTextureData().textureID );
		//sGlitch2.setUniformTexture("nextSlide1", nextSlide.getTextureReference(), 1);//nextSlide.getTextureReference().getTextureData().textureID );
		//ofTexture t;
		//t.allocate(glitchData2);
		//t.loadData(glitchData2);
		//t.allocate(glitchData2);
		//t.bind();
		
		// transmit data to shader
		//sGlitch2.setUniformTexture("glitchData2", t, 2);//t.getTextureData().textureID );
		
		sGlitch2.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
		sGlitch2.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
		
		// add shadertoy sandbox variables ( tmp?)
		sGlitch2.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(), 0);
		sGlitch2.setUniform4f("iMouse", ofGetMouseX(), ofGetMouseY(), 0, 0 );
		sGlitch2.setUniform1f("iGlobalTime", ofGetElapsedTimef());
		sGlitch2.setUniform2f("resolution", ofGetWidth(), ofGetHeight() );
		sGlitch2.setUniform2f("textureResolution", currentSlide.width, currentSlide.height );
		sGlitch2.setUniform2f("shapeCenterOffset", 0, 0 );// pShape->getCenterOffsetFromBoundingBox().x, pShape->getCenterOffsetFromBoundingBox().y);
		sGlitch2.setUniform1f("textureScale", 1);
		//sGlitch2.setUniform1i("tex", 0);
		
		//currentSlide.getTextureReference().bind();
		slideGrid.draw();
		//currentSlide.getTextureReference().unbind();
		
		//slideGrid.drawWireframe();
		sGlitch2.end();
		
		// tmp
		ofSetColor(255);
		//glitchData2.draw(-ofGetWidth()/2, -ofGetHeight()/2, ofGetWidth()/3, ofGetHeight()/3 );
		
		//t.unbind();
		//currentSlide.unbind();
		//nextSlide.unbind();
		
		ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
		ofPopStyle();
		ofPopMatrix();
		
		fbo.end();
		reRenderOutput = false;
		reDrawOutput = true;
	}
}


void enigmatikSlideshow::draw() {
	// tmp : uncomment the comment below when RPI is ready not to erase FBO automatically and save enormous amounts of CPU power.
	if( /*reDrawOutput == true &&*/ fbo.isAllocated()){
		ofLogVerbose("enigmatikSlideshow::_draw") << "Drawing a new image";
		fbo.draw(0,0);
		reDrawOutput = false;
	}

	// show GUI elements
	if(showControls) gui.draw();
	
#ifdef USE_RPI_GPIO
	// saves battery
	//ofSleepMillis(50);
#endif
}

void enigmatikSlideshow::_draw(ofEventArgs &e){
	// here you can force actions (draw() is virtual)
	//ofSetColor(0,0,255,1);
	//ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
	//ofClear(0);
	
	//if(reRenderOutput==false) return;
	
	draw();

	return;
}

void enigmatikSlideshow::exit(){
#ifdef USE_THREADED_IMAGE_LOADER
	// stop images loader
	imgLoader.stopThread();
	
	// image loaded event
	ofRemoveListener(imgLoader.imageLoaded,this, &enigmatikSlideshow::newImageLoaded);
#endif
	
#ifdef USE_RPI_GPIO
	// todo: link virtual buttons to physical buttons
	//param3.unlinkWithGPIO(1,16);
	
#endif
	
#ifdef USE_KEYBOARD_SHORTCUTS
	ofRemoveListener(ofEvents().keyPressed, this, &enigmatikSlideshow::enigmaKeyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &enigmatikSlideshow::enigmaKeyReleased);
#endif
	

	
	// rm event listeners
	prevButton.removeListener(this, &enigmatikSlideshow::loadPrevSlide );
	nextButton.removeListener(this, &enigmatikSlideshow::loadNextSlide );
	
	// rm update, setup, exit
	ofRemoveListener(ofEvents().update, this, &enigmatikSlideshow::_update);
	ofRemoveListener(ofEvents().draw, this, &enigmatikSlideshow::_draw);
	ofRemoveListener(ofEvents().exit, this, &enigmatikSlideshow::_exit);
}
void enigmatikSlideshow::_exit(ofEventArgs &e){
	exit();
}

// - --------

bool enigmatikSlideshow::setFolder(string _path){
	ofDirectory _dir(_path);
	
	if( !_dir.isDirectory() ){
		ofLogNotice("enigmatikSlideshow::setFolder «"+ _path +"» is not a valid folder.");
		return false;
	}
	// todo > canWrite isn't yet defined in OF... :(
	// See: https://github.com/openframeworks/openFrameworks/pull/2934
	/*else if( !_dir.canWrite() ){
		ofLogNotice("enigmatikSlideshow::setFolder() --> " + _path + " is a folder but is not writeable. You will not be able to add images.");
	}// */
	
	// 
	reloadFolderContent();
	
	return true;
}

// adds an image to the slides folder
// not a temporary insertion
bool enigmatikSlideshow::addImage(string _img){
	ofImage _tmp;
	if( !_tmp.loadImage(_img) ){
		ofLogNotice("enigmatikSlideshow::addImage() --> " + _img + " could not be loaded. Please check your image path.");
		return false;
	}
	
	return addImage(&_tmp, "");
}

// saves file relative
// to improve: secure it so you can't provide a path.
bool enigmatikSlideshow::addImage(ofImage* _img, string _fileName){
	_img->saveImage( _fileName );
	return true; // saveImage() has no return value...
}

void enigmatikSlideshow::reloadFolderContent(){
	ofDirectory dir(imagesFolder);
	
	dir.allowExt("png");
	dir.allowExt("gif"); // can we also hangle animated gifs ?
	dir.allowExt("jpg");
	dir.allowExt("jpeg");
	dir.allowExt("tiff");
	// add other formats ? tocheck which ones are compatible with the image decrypter
	
	dir.sort();
	dir.listDir(); // loads file repository into cache
	
	numSlides = dir.numFiles();
	
	if(numSlides==0) ofLogNotice("enigmatikSlideshow::reloadFolderContent - there are no slides!");
	
	/*/ remember each image
	ofAbstractParameter* bla = &currentSlideNum.getParameter();
	//currentSlideNum.value.getMax();
	
	//int * crap;// = bla;
	// *crap = &bla;
	//ofParameter<int> param = (ofParameter<int>) *bla;
	
	//bla.setMax(1);
	//ofParameter<int> bli = bla.cast< ofParameter<int> >();
	//ofParameter<Type> bli = bla.cast< ofParameter<Type> >();
	//currentSlideNum.ofParameter::setMax(numSlides);
	//fpsSlider.getMax().addListener(this, &testApp::maxFpsChange);// */
	
	
	// resize/adapt slider range
	//currentSlideNum.setMax(numSlides); // right way; currently unavailable
	// url: http://forum.openframeworks.cc/t/dynamic-range-in-ofxslider-ofxgui/15424
	// might help: http://forum.openframeworks.cc/t/controlling-a-parameters-from-ofxgui-and-some-kb-shortcut/15171/4
	//currentSlideNum.setup(currentSlideNum.getName(), (int)currentSlideNum, 0, numSlides-1); // alternative --> fucks up GUI drawing
	
    //ofParameter<int> tmp;
	//tmp = currentSlideNum;
	//tmp.setMax( numSlides-1 );
	
	imageFiles.resize( numSlides );
	for(int i = 0; i < dir.numFiles(); i++){
		//ofLogNotice(dir.getPath(i));
		imageFiles[i] = dir.getName(i);
	}
	
}

void enigmatikSlideshow::loadNextSlide(){
	currentSlideNum = (int) getRealSlideNum((currentSlideNum+1)*slideDirection);
	nextSlideNum = getRealSlideNum((currentSlideNum+1)*slideDirection);
	
	if(currentSlideNum >= 0) cacheCurrentSlides();
	
	resetEffects();
	
	reRenderOutput = true;
}

void enigmatikSlideshow::loadPrevSlide(){
	currentSlideNum = getRealSlideNum( currentSlideNum-1*slideDirection);
	nextSlideNum = getRealSlideNum(currentSlideNum+1*slideDirection);
	
	if(currentSlideNum >= 0) cacheCurrentSlides();
	
	resetEffects();
	
	reRenderOutput = true;
}

void enigmatikSlideshow::enableControls(){
	showControls = true;
}

bool enigmatikSlideshow::rememberSlideNum(int & value){
	return false;
	// this ofXML is fucked up!
	int slidenum = (int) currentSlideNum;
	string path = "enigmatik/currentSlideNum";
	ofXml buffer(path);
	//buffer.load(path);
	//if(!buffer.exists(path)) buffer.addValue<int>(path, slidenum, 1);
	//else buffer.setValue<int>(path, slidenum );
	//buffer.setAttribute(path, ofToString(slidenum));
	//buffer.setAttribute(path, "lol");
	//buffer.setTo(path);
	buffer.setValue(path, "lol");//ofToString(slidenum) );
	
	return buffer.save("enigmatikCurrentSlideNum.xml");
}

bool enigmatikSlideshow::loadSlideNum(){
	ofXml buffer;
	
	if(!buffer.load("enigmatikCurrentSlideNum.xml")) currentSlideNum=0;
	else currentSlideNum = buffer.getIntValue("enigmatik/currentSlideNum");
	
	nextSlideNum = currentSlideNum + slideDirection;
	
	return true;
}

// function that resets/randomizes 
void enigmatikSlideshow::resetEffects(){
	// randomize button targets
	param2.randomizeTarget();
	param3.randomizeTarget();
	param6.randomizeTarget();
	// etc.
	// etc.
	
	// randomize glitch zone 2
	{
		glitchData2.allocate( currentSlide.getWidth(), currentSlide.getHeight(), OF_IMAGE_COLOR_ALPHA );
		int numPixels = currentSlide.width * currentSlide.height;
		glitchZones2.resize(numPixels);
		/*for(int i=0; i<numPixels;i++){
		 // todo: make this "identifiable" zones (rects?) unstead of full random.
		 glitchZones2[i] = ( round(ofRandom(.0f,1.0f)) == true )?ofRandom(.0f,1.0f):0;
		 }*/
		// empty
		for(int i=0; i<numPixels;i++) glitchZones2[i] = 0;
		
		// generate zones
		int numZones = 160;
		for(int z=0; z<numZones;z++){
			int centerPixel = (int) ofRandom(0, numPixels);
			int centerX = centerPixel % currentSlide.width;
			int centerY = ceil(centerPixel / currentSlide.width);
			int gWidth = ofRandom(20,120);
			int gHeight = ofRandom(8,20);
			
			// restrain x to workspace
			int x=centerX-gWidth/2;
			if(x<0) x=0;
			
			for(; x<centerX+gWidth/2 && x<currentSlide.width; x++){
				int y=centerY-gHeight/2;
				if(y<0) y=0;
				
				for(; y<centerY+gHeight/2 && y<currentSlide.height; y++ ){
					glitchZones2[(x+y*currentSlide.width)] = ofRandom(.0f,1.0f);//( round(ofRandom(.0f,1.0f)) == true )?ofRandom(.0f,1.0f):0;
				}
				
			}
		}
	} // end glitch zone 2
	
	// ask for new frame
	reRenderOutput = true;
}

// control params with keyboard
#ifdef USE_KEYBOARD_SHORTCUTS
void enigmatikSlideshow::enigmaKeyPressed(ofKeyEventArgs &e){
	vector<int> availableKeys;
	availableKeys.resize(0);
	availableKeys.push_back('1');
	availableKeys.push_back('2');
	availableKeys.push_back('q');
	availableKeys.push_back('w');
	availableKeys.push_back('a');
	availableKeys.push_back('s');
	availableKeys.push_back(OF_KEY_LEFT);
	availableKeys.push_back(OF_KEY_RIGHT);
	availableKeys.push_back(']');
	
	// route key
	for (int i=0; i<availableKeys.size(); ++i){
		
		if( availableKeys[i] == e.key ){
			auto key = enigmaPressedKeys.find(e.key);
			if( key == enigmaPressedKeys.end() ){
				enigmaKeyState ks;
				ks.keyArg = 0.1f;
				ks.isPressed=true;
				enigmaPressedKeys[e.key] = ks;
			}
			else { // key already exists
				key->second.keyArg = key->second.keyArg + .1f;
				key->second.isPressed = true;
			}
			
			return;
		}
	}
}

void enigmatikSlideshow::enigmaKeyReleased(ofKeyEventArgs &e){
	
	// route key
	auto it = enigmaPressedKeys.find(e.key);
	if( it!=enigmaPressedKeys.end() ){
		enigmaPressedKeys[e.key].isPressed=false;
	}
}
#endif

// GLITCH EFFECTS

// GLITCH #2 : enigmatikPotentiometer
void enigmatikSlideshow::setParam2Solved( int& value ){
	// todo: void seems to be called twice for each change.
	// compare to prev value and set timer not to overload the cpu/gpu.
	
	//cout << "OK = " << value << endl;
	//int bla = buttons.cast<>(0);
	//enigmatikPotentiometer btn2 = buttons.ofParameterGroup::get<enigmatikPotentiometer>(1);
	param2Solved = 1 - param2.getDistFromTarget();
	if(param2Solved == lastParam2Solved) return;
	else lastParam2Solved = param2Solved;
	//cout << "param2Solved = " << param2Solved << endl;
	
	// triggers a new rendering
	reRenderOutput = true;
	
	//cout << "enigmatikSlideshow::setParam2Solved : update done" << endl;
	
}

void enigmatikSlideshow::setParam3Solved( int& value ){
	param3Solved = 1 - param3.getDistFromTarget();
	
	if(param3Solved == lastParam3Solved) return;
	else lastParam3Solved = param3Solved;
	
	//cout << "param3Solved = " << param3Solved << endl;
	
	// triggers a new rendering
	reRenderOutput = true;
	
}

void enigmatikSlideshow::setParam6Solved( int& value ){
	param6Solved = 1 - param6.getDistFromTarget();
	
	if(param6Solved == lastParam6Solved) return;
	else lastParam6Solved = param6Solved;
	
	//cout << "param6Solved = " << param6Solved << endl;
	
	// triggers a new rendering
	reRenderOutput = true;
	
}


// PRIVATE FUNCTIONS

void enigmatikSlideshow::newImageLoaded(string &_img){
	// get corresponding image
	unsigned int imgIndex = 0;
	bool found = false;
	while( imgIndex < imageFiles.size() ){
		if(imagesFolder+"/"+imageFiles[imgIndex] == _img){
			found = true;
			break;
		}
		imgIndex++;
	}
	unsigned int bla=imageFiles.size();
	if(!found){
		ofLogVerbose("enigmatikSlideshow::newImageLoaded - Image was loaded, but already loaded on stage.");
		return;
	}
	
	// resize image
	if(imgIndex == currentSlideNum) resizeImageToScreen(currentSlide);
	else if(imgIndex == nextSlideNum) resizeImageToScreen(nextSlide);
}

void enigmatikSlideshow::resizeImageToScreen(ofImage &_img){
	// todo: respect image proportions / don't stretch
	//ofPixelsRef& pix = _img.getPixelsRef();
	//cout << "Colorspace before resize: " << _img.getPixelsRef().pixelFormat << endl;
	_img.resize(ofGetWidth(), ofGetHeight());
	//ofPixelsRef& pix = _img.getPixelsRef();
	
}

// loads current slides in ofImage variable
bool enigmatikSlideshow::cacheCurrentSlides(){
	if(imageFiles.size() < (unsigned int)1) return false;
	
	if( currentSlideNum < 0 || (unsigned int)currentSlideNum >= imageFiles.size() ) return false;
	
	// update menu item indicator
	currentSlideNum.setName("Current slide: "+ofToString(currentSlideNum+1)+" of " + ofToString(numSlides));
	gui.ofxPanel::setPosition(gui.ofxPanel::getPosition()); // dirty hack to force gui to re-render text (wich just got changed)
	
	bool ret = true;
	
#ifdef USE_THREADED_IMAGE_LOADER
	// cancel current requests
	//imgLoader = ofxThreadedImageLoader(); // clears loading list (no function available for this)
	imgLoader.clearQueue();
	
	imgLoader.loadFromDisk( currentSlide, imagesFolder +"/"+ imageFiles[currentSlideNum]);
#else
    // tmp
    string test = imagesFolder +"/"+ imageFiles[currentSlideNum];
    
    // dirty hack. images can (atm) only be in graaysale or rgb, not bgr
    // [or not]
    ret *= currentSlide.loadImage( test);//imagesFolder +"/"+ imageFiles[currentSlideNum] );
	
	if(ret) resizeImageToScreen(currentSlide);
#endif
	
	// load next image ?
	if( nextSlideNum < 0 || nextSlideNum >imageFiles.size() ) return false;

#ifdef USE_THREADED_IMAGE_LOADER
	imgLoader.loadFromDisk( nextSlide, imagesFolder +"/"+ imageFiles[nextSlideNum]);
#else
	ret *= nextSlide.loadImage( imagesFolder +"/"+ imageFiles[nextSlideNum] );
	if(ret) resizeImageToScreen(nextSlide);
#endif
	
	return ret;
}

// return an int within slide numbers
// -1 when there are no slides
int enigmatikSlideshow::getRealSlideNum(int _num){
	if(_num<0) _num *= -1*(numSlides-1);
	
	if(numSlides==0) return -1;
	else return abs( _num % numSlides );
}


// forward declaration
//class enigmatikSlideshow;
//ofEvent<ofKeyEventArgs> durationReceiver::durationTempoEvent;
