//
//  enigmaSlideshow.h
//  enigmatikViewer
//
//  Created by Daan de Lange on 13/4/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofParameter.h"
#include "ofxGui.h"
#include "ofxThreadedImageLoader.h"
//#include "ofxGuiDynamicIntSlider.h"

// comment to disable
//#define USE_THREADED_IMAGE_LOADER

class enigmatikSlideshow {

public:

	void setup();
	void _update(ofEventArgs &e);
	void update();
	void _draw(ofEventArgs &e);
	void draw();
	//void draw(int x, int y)
	void _exit(ofEventArgs &e);
	void exit();
	
	bool setFolder( string _path );
	bool addImage(string _img);
	bool addImage(ofImage* _img, string _fileName);
	void reloadFolderContent();
	
	//int currentSlideNum, nextSlideNum;
	
	void loadNextSlide();
	void loadPrevSlide();
	void enableControls();
	
private:
	void newImageLoaded(string &_img);
	void resizeImageToScreen(ofImage &_img);
	bool cacheCurrentSlides();
	int getRealSlideNum(int _num);
	int slideDirection = 1;
	vector<string> imageFiles;
	ofImage currentSlide, nextSlide; // static for multithreading ?
	string imagesFolder;
	int numSlides;
	bool showControls = false;
	
	ofxPanel gui;
	ofxButton nextButton, prevButton;
	ofxIntSlider currentSlideNum, nextSlideNum;
#ifdef USE_THREADED_IMAGE_LOADER
	ofxThreadedImageLoader imgLoader;
#endif

};
