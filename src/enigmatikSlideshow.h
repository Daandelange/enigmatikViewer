//
//  enigmaSlideshow.h
//  enigmatikViewer
//
//  Created by Daan de Lange on 13/4/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

// uncomment to enable (untested)
//#define USE_THREADED_IMAGE_LOADER

#include "ofMain.h"
#include "ofEvents.h"
//#include "ofParameter.h"
#include "ofParameterGroup.h"
#include "ofxGui.h"
#include "ofxThreadedImageLoader.h"
//#include "ofxGuiDynamicIntSlider.h"
#include "enigmatikParam.h"

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
	
	bool rememberSlideNum(int & useless);
	bool loadSlideNum();
	
	// glitch params
	enigmatikButton param1;
	enigmatikPotentiometer param2;
	enigmatikRotaryEncoder param3;
	enigmatikThreePosSwitch param4;
	enigmatikPushButton param5;
	
	// glitch effects
	void glitchEffect2(int& value);
	float solved2;
	void resetEffects();
	
private:
	void newImageLoaded(string &_img);
	void resizeImageToScreen(ofImage &_img);
	bool cacheCurrentSlides();
	int getRealSlideNum(int _num);
	int slideDirection = 1;
	vector<string> imageFiles;
	ofImage currentSlide, nextSlide, mixedSlide; // static for multithreading ?
	//ofFbo mixedSlide;
	
	string imagesFolder;
	int numSlides;
	bool showControls = false;
	
	// glitch stuff
	ofShader sGlitch2;
	ofPlanePrimitive slideGrid;
	ofImage glitchData2; // data texture
	
	vector<float> glitchZones2;
	
	ofxPanel gui;
	ofxButton nextButton, prevButton;
	ofxIntSlider currentSlideNum, nextSlideNum;
#ifdef USE_THREADED_IMAGE_LOADER
	ofxThreadedImageLoader imgLoader;
#endif
	
	ofParameterGroup buttons;
	ofxPanel buttonGui;
	bool reRenderOutput;
};
