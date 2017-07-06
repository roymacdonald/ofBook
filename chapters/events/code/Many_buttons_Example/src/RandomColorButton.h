#pragma once
#include "ofMain.h"

class RandomColorButton{
public:
    RandomColorButton(){
        //In this case we will be listening to mouseReleased instead of mouseMoved
        ofAddListener(ofEvents().mouseReleased, this, &RandomColorButton::mouseReleased);
        //Also we will listen to the draw ofEvent so this class draws by itself.
        ofAddListener(ofEvents().draw, this, &RandomColorButton::draw);

        //let's set the rectangle we are going to draw to a random size and position.
        //Instead of calling ofRandom directly inside rect.set() the following code avoids that
        //the rectangle falls out of the window.
        float w = ofRandom(30, 300);
        float h = ofRandom(30, 300);
        float x = ofRandom(ofGetWidth() - w);
        float y = ofRandom(ofGetHeight() - h);
        rect.set(x,y,w,h);

        //let's set rectangle's color to a random color
        color = ofColor((int)floor(ofRandom(255)),(int)floor(ofRandom(255)),(int)floor(ofRandom(255)));

    }
    ~RandomColorButton(){
        //this is the destructor. We need to rem
        ofRemoveListener(ofEvents().mouseMoved, this, &RandomColorButton::mouseReleased);
        ofRemoveListener(ofEvents().draw, this, &RandomColorButton::draw);
    }
    void mouseReleased(ofMouseEventArgs& args){
        if(rect.inside(args.x, args.y)){
            // now when you release the mouse inside this rectangle it will broadcast an event with its color
            // This way ofApp will be able to change its background to the color of the clicked rectangle.
            ofNotifyEvent(colorEvent, color);
        }
    }

    void draw(ofEventArgs&){
        ofPushStyle();
        ofSetColor(color);
        ofDrawRectangle(rect);
        ofPopStyle();
    }

    ofEvent<ofColor> colorEvent;

protected:
    ofRectangle rect;
    ofColor color;

};

