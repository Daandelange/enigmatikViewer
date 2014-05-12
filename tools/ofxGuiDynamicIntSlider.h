#pragma once

#include "ofxSlider.h"

template<typename Type>

class ofxGuiDynamicIntSlider : public ofxIntSlider {
public:
    ofxGuiDynamicIntSlider() : ofxIntSlider() {}
    virtual ~ofxGuiDynamicIntSlider() {}
	
    //ofxFpsSlider* setup(string _name, float _max, float width = defaultWidth, float height = defaultHeight);
	
    //void draw();
	template<class ListenerClass, typename ListenerMethod> void setMax(int _max);
	
};

typedef ofxFloatSlider<float> ofxDynamicIntSlider;
typedef ofxIntSlider<int> ofxDynamicIntSlider;