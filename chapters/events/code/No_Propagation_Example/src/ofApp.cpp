#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::draw(){

    cam.begin();
    // all that's in between cam.begin(); and cam.end(); is just to draw the magenta box with its vertices black
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::darkMagenta);
    ofDrawBox(0,0,0,100,100,100);
    ofSetColor(30);
    ofNoFill();
    ofDrawBox(0,0,0,100,100,100);
    ofPopStyle();
 
    cam.end();

    rect.draw();

    stringstream msg;
    msg << "Clic and drag over the red square to move it around." << endl;
    msg << "Clic and drag elsewhere to move the camera, hence rotate the magenta box" << endl << endl;

    msg << "Notice that when dragging the red square the camera also changes.\nThis behavior is the one we fix by controlling the event propagation"<<endl;


    ofDrawBitmapStringHighlight(msg.str(), 20,20);

}