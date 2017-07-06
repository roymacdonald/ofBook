#pragma once

#include "ofMain.h"


class ToggleableButton{
public:
    ToggleableButton(){
        enableMouse();
        rect.set(ofGetWidth()*0.25,ofGetHeight()*0.25,ofGetWidth()*0.5,ofGetHeight()*0.5);
    }
    ~ToggleableButton(){
        //this is the destructor.
        disableMouse();
    }

    void enableMouse(){
        if(!bListeningMouseEvent){
            //So only when it is not listening we will add a listener
            bListeningMouseEvent = true;
            ofAddListener(ofEvents().mouseReleased, this, &ToggleableButton::mouseReleased);
        }
    }
    void disableMouse(){
        if(bListeningMouseEvent){
            bListeningMouseEvent = false;
            ofRemoveListener(ofEvents().mouseReleased, this, &ToggleableButton::mouseReleased);
        }
    }

    //This is just a helper method.
    void toggleMouse(){
        if(bListeningMouseEvent){
            disableMouse();
        }else{
            enableMouse();
        }
    }
    void mouseReleased(ofMouseEventArgs& args){
        if(rect.inside(args.x, args.y)){
            //just change the color to a random one
            color = ofColor((int)floor(ofRandom(255)),(int)floor(ofRandom(255)),(int)floor(ofRandom(255)));
        }
    }

    void draw(){
        ofPushStyle();
        ofSetColor(color);
        ofDrawRectangle(rect);
        ofPopStyle();
    }


    bool isListeningMouseEvent(){
        return bListeningMouseEvent;
    }

protected:

    //We will use this bool to check if the ofEvents are registered or not.
    //It is protected for safety, so it cannot be modified from the outside.
    bool bListeningMouseEvent = false;

    ofRectangle rect;
    ofColor color;

};
