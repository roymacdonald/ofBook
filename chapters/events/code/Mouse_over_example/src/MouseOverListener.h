#pragma once
#include "ofMain.h"

class MouseOverListener{
public:
    MouseOverListener(){
        //let's start listening to the mouseMoved ofEvent when this object gets created.
        ofAddListener(ofEvents().mouseMoved, this, &MouseOverListener::mouseMoved);
        //let's set the rectangle we are going to draw.
        rect.set(200,300, 100,100);

        //let's set rectangle's color to red.
        color = ofColor::red;

    }
    ~MouseOverListener(){
        //this is the destructor. We need to rem
        ofRemoveListener(ofEvents().mouseMoved, this, &MouseOverListener::mouseMoved);
    }
    void mouseMoved(ofMouseEventArgs& args){
        if(rect.inside(args.x, args.y)){
            //Instead of changing the rectangles color let's notify a message.
            int i = 0; // this could be any integer number.
            ofNotifyEvent(intEvent, i);
        }
    }

    void draw(){
        ofPushStyle();
        ofSetColor(color);
        ofDrawRectangle(rect);
        ofPopStyle();
    }

    ofEvent<int> intEvent;

protected:
    ofRectangle rect;
    ofColor color;

};
