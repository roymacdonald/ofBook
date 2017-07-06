#pragma once
#include "ofMain.h"

//This class is so simple and short that there's no need to make a .cpp file for it.
class SimpleEventsListener{
public:
    SimpleEventsListener(){//this is the class constructor
        //let's start listening to the mouseMoved ofEvent when this object gets created.
        ofAddListener(ofEvents().mouseMoved, this, &SimpleEventsListener::mouseMoved);
        //let's set the rectangle we are going to draw.
        rect.set(200,300, 100,100);
    }
    ~SimpleEventsListener(){//this is the class destructor
        //this is the destructor. We need to rem
        ofRemoveListener(ofEvents().mouseMoved, this, &SimpleEventsListener::mouseMoved);
    }
    void mouseMoved(ofMouseEventArgs& args){
        if(rect.inside(args.x, args.y)){
            //just make a random color.
            color = ofColor((int)floor(ofRandom(255)),(int)floor(ofRandom(255)),(int)floor(ofRandom(255)));
        }
    }

    void draw(){
        ofPushStyle();
        ofSetColor(color);
        ofDrawRectangle(rect);
        ofPopStyle();
    }


protected:
    ofRectangle rect;
    ofColor color;

};
