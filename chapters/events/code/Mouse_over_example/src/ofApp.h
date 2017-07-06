#pragma once

#include "ofMain.h"
#include "MouseOverListener.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void exit(); 

        void intEventReceived(int & i);

        MouseOverListener eventsListener;
};

