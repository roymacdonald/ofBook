#pragma once
#include "ofMain.h"
#include "SimpleEventsListener.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        SimpleEventsListener eventsListener;
};
