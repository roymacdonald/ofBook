#include "ofApp.h"

void ofApp::setup(){}
    //--------------------------------------------------------------
void ofApp::update(){}
    //--------------------------------------------------------------
void ofApp::draw(){
    button.draw();

    stringstream msg;
    msg << "Press the spacebar to enable/disable (toggle) the button" << endl;
    msg << "Clic inside the colored rectangle (the button)." << endl;
    msg << "If it is listening to the mouse it will change its color to a random one" << endl;
    msg << endl << "Button is ";
    if(!button.isListeningMouseEvent()){
        msg << "NOT ";
    }
    msg << "listening to mouse.";

    ofDrawBitmapStringHighlight(msg.str(), 100,100);
}
    //--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' ') {
        button.toggleMouse();
    }
}
