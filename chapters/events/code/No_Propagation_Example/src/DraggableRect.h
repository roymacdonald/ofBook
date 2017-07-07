#pragma once
#include "ofMain.h"

//This example is not implementing the events propagation. It is here to depict an undesired behavior.

class DraggableRect{
public:

    DraggableRect(){
        rect.set(100,100, 130,100);
        //I added all the mouse events call back just to show how handy the following function is.
         ofRegisterMouseEvents(this);
    }
    ~DraggableRect(){
        ofUnregisterMouseEvents(this);
    }
    void mouseDragged( ofMouseEventArgs & mouse ){
        if(bDragging){
            rect.x = mouse.x - offset.x;
            rect.y = mouse.y - offset.y;
        }
    }
    void mousePressed( ofMouseEventArgs & mouse ){
        if(rect.inside(mouse)){//notice that you can pass the mouse argument directly to the ofRectangle's inside method.
            bDragging = true;
            offset = mouse - rect.getPosition();
        }
    }
    void mouseReleased(ofMouseEventArgs & mouse){
        bDragging  = false;
    }
    //Even when the following mouse callbacks are not being used we need to have these declared
    //because otherwise the function ofRegisterMouseEvents will not work.
    void mouseMoved( ofMouseEventArgs & mouse ){}
    void mouseScrolled( ofMouseEventArgs & mouse ){}
    void mouseEntered( ofMouseEventArgs & mouse ){}
    void mouseExited( ofMouseEventArgs & mouse ){}

    void draw(){
        ofPushStyle();
        ofSetColor(ofColor::red);
        ofDrawRectangle(rect);
        ofPopStyle();
    }

private:

    ofVec2f offset;// this is to avoid having a jumpy rect when we start dragging.

    bool bDragging = false;
    ofRectangle rect;

};
