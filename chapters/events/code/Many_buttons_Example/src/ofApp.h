#pragma once
#include "ofMain.h"
#include "RandomColorButton.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void exit();

        // This is the callback method. Notice that it's argument is an ofColor which matches the RandomColorButton ofEvent.
        void colorEventReceived(ofColor & color);

        //This will be the collection of buttons
        vector<RandomColorButton> buttons;
};
