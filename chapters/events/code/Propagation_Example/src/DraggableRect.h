
#pragma once
#include "ofMain.h"

class DraggableRect{
public:

    DraggableRect(){
        rect.set(100,100, 130,100);
        //I added all the mouse events call back just to show how handy the following function is.

        //The ofEasyCam registers it's mouse events with OF_EVENT_ORDER_AFTER_APP, so in order
        //to be able to efectively stop propagation we need to make this class to register
        //with a lower priority value; it will get called earlier, so if propagation stops the
        //ofEasyCam will not get the notified mouse events.
        ofRegisterMouseEvents(this,OF_EVENT_ORDER_AFTER_APP -1);
    }
    ~DraggableRect(){
        ofUnregisterMouseEvents(this,OF_EVENT_ORDER_AFTER_APP -1);
    }

    bool mouseDragged( ofMouseEventArgs & mouse ){
        if(bDragging){
            rect.x = mouse.x - offset.x;
            rect.y = mouse.y - offset.y;
        }
        return bDragging;
    }

    bool mousePressed( ofMouseEventArgs & mouse ){
        if(rect.inside(mouse)){//notice that you can pass the mouse argument directly to the ofRectangle's inside method.
            bDragging = true;
            offset = mouse - rect.getPosition();
            return true;
        }
        return bDragging;
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