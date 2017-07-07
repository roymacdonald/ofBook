#pragma once
#include "ofMain.h"
#include "DraggableRect.h"

class ofApp : public ofBaseApp{

    public:
        void draw();

        DraggableRect rect;

        ofEasyCam cam;
};
