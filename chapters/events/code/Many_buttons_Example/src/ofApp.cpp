#include "ofApp.h"

void ofApp::setup(){
    //Make 100 buttons
    buttons.resize(100);
    //add the listeners for each button
    for(int i = 0; i < buttons.size(); i++){
        ofAddListener(buttons[i].colorEvent, this, &ofApp::colorEventReceived);
    }
}
//--------------------------------------------------------------
void ofApp::update(){}
//--------------------------------------------------------------
void ofApp::exit(){
    for(int i = 0; i < buttons.size(); i++){
        ofRemoveListener(buttons[i].colorEvent, this, &ofApp::colorEventReceived);
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
//There's no need to draw the buttons. These draw by themselves. :)
}
//--------------------------------------------------------------
void ofApp::colorEventReceived(ofColor & color){
    //Let's set the background color to the one sent in the received event
    ofSetBackgroundColor(color);
}
