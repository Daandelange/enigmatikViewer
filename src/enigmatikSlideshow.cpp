//
//  enigmatikSlideshow.cpp
//  enigmatikViewer
//
//  Created by Daan de Lange on 13/4/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

// todo: make a file increment system to store images in the receival-order while storing their original names in an xml file.
// The original file name can then be shown for a few secs once you unlock a new slide.

// todo: slidercontrol panel

// todo: slider rendering system

// todo: slider loading system

// todo: slider button-based transition system

#include "enigmatikSlideshow.h"

// MARK: Init

void enigmatikSlideshow::setup() {
	
	
	// todo: load currentSlideNum from xml settings
	currentSlideNum=0;
	nextSlideNum=-1; // -1 = none;
	imagesFolder="slideshowImages";
	
	// setup GUI
	gui.setup("Slide Show Control", "slideShowSettings.xml");
	gui.add( prevButton.setup("<-- PREV SLIDE") );
	gui.add( currentSlideNum.setup("Current slide", 0, 0, 0) );
	gui.add( nextButton.setup("--> NEXT SLIDE") );
	
	prevButton.addListener(this, &enigmatikSlideshow::loadPrevSlide );
	nextButton.addListener(this, &enigmatikSlideshow::loadNextSlide );
	
	// load last used settings
	gui.loadFromFile("slideShowSettings.xml");
	
	// load images folder
	reloadFolderContent();
	
	// cache current & next Slides
	cacheCurrentSlides();
	
	ofAddListener(ofEvents().update, this, &enigmatikSlideshow::_update);
	ofAddListener(ofEvents().draw, this, &enigmatikSlideshow::_draw);
	ofAddListener(ofEvents().exit, this, &enigmatikSlideshow::_exit);
	
#ifdef USE_THREADED_IMAGE_LOADER
	// image loaded event
	ofAddListener(imgLoader.imageLoaded,this, &enigmatikSlideshow::newImageLoaded);
#endif
}


// MARK: Loop

void enigmatikSlideshow::update() {
	
}
void enigmatikSlideshow::_update(ofEventArgs &e) {
	update();
}


void enigmatikSlideshow::draw() {
	
	// show slide
	currentSlide.draw(0,0);
	
	// show GUI elements
	if(showControls){
		gui.draw();
	}
	
}
void enigmatikSlideshow::_draw(ofEventArgs &e){
	draw();
}

void enigmatikSlideshow::exit(){
#ifdef USE_THREADED_IMAGE_LOADER
	// stop images loader
	imgLoader.stopThread();
	
	// image loaded event
	ofRemoveListener(imgLoader.imageLoaded,this, &enigmatikSlideshow::newImageLoaded);
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
	// todo this shit
	//currentSlideNum.setMax(numSlides); // right way; currently unavailable
	// url: http://forum.openframeworks.cc/t/dynamic-range-in-ofxslider-ofxgui/15424
	currentSlideNum.setup(currentSlideNum.getName(), (int)currentSlideNum, 0, numSlides-1); // fucks up GUI
	
	imageFiles.resize( numSlides );
	for(int i = 0; i < dir.numFiles(); i++){
		//ofLogNotice(dir.getPath(i));
		imageFiles[i] = dir.getName(i);
	}
	
}

void enigmatikSlideshow::loadNextSlide(){
	currentSlideNum = getRealSlideNum(currentSlideNum+1*slideDirection);
	nextSlideNum = getRealSlideNum(currentSlideNum+1*currentSlideNum);
	
	cacheCurrentSlides();
}

void enigmatikSlideshow::loadPrevSlide(){
	currentSlideNum = getRealSlideNum( currentSlideNum-1*slideDirection);
	nextSlideNum = getRealSlideNum(currentSlideNum+1*currentSlideNum);
	
	cacheCurrentSlides();
}

void enigmatikSlideshow::enableControls(){
	showControls = true;
}

void enigmatikSlideshow::newImageLoaded(string &_img){
	// get corresponding image
	int imgIndex = 0;
	bool found = false;
	while( imgIndex < imageFiles.size() ){
		if(imagesFolder+"/"+imageFiles[imgIndex] == _img){
			found = true;
			break;
		}
		imgIndex++;
	}
	int bla=imageFiles.size();
	if(!found){
		ofLogVerbose("enigmatikSlideshow::newImageLoaded - Image was loaded, but already unloaded from stage.");
		return;
	}
	
	// resize image
	if(imgIndex == currentSlideNum) resizeImageToScreen(currentSlide);
	else if(imgIndex == nextSlideNum) resizeImageToScreen(nextSlide);
}

void enigmatikSlideshow::resizeImageToScreen(ofImage &_img){
	// todo: respect image proportions / don't stretch
	_img.resize(ofGetWidth(), ofGetHeight());
}

// loads current slides in ofImage variable
bool enigmatikSlideshow::cacheCurrentSlides(){
	if(imageFiles.size()<1) return false;
	
	if( currentSlideNum < 0 || currentSlideNum > imageFiles.size() ) return false;
	
	// update menu item indicator
	currentSlideNum.setName("Current slide: ("+ofToString(currentSlideNum+1)+"/" + ofToString(numSlides) + ")");
	
	bool ret = true;
	
#ifdef USE_THREADED_IMAGE_LOADER
	// cancel current requests
	//imgLoader = ofxThreadedImageLoader(); // clears loading list (no function available for this)
	imgLoader.clearQueue();
	
	imgLoader.loadFromDisk( currentSlide, imagesFolder +"/"+ imageFiles[currentSlideNum]);
#else
	ret *= currentSlide.loadImage( imagesFolder +"/"+ imageFiles[currentSlideNum] );
	
	resizeImageToScreen(currentSlide);
#endif
	
	
	if( nextSlideNum < 0 || nextSlideNum >imageFiles.size() ) return false;

#ifdef USE_THREADED_IMAGE_LOADER
	imgLoader.loadFromDisk( nextSlide, imagesFolder +"/"+ imageFiles[nextSlideNum]);
#else
	ret *= nextSlide.loadImage( imagesFolder +"/"+ imageFiles[nextSlideNum] );
	resizeImageToScreen(nextSlide);
#endif
	
	
	
	return ret;
}

// return an int within slide numbers
// -1 when there re no slides
int enigmatikSlideshow::getRealSlideNum(int _num){
	if(_num<0) _num *= -1*(numSlides-1);
	
	if(numSlides==0) return -1;
	else return abs( _num % numSlides );
}

