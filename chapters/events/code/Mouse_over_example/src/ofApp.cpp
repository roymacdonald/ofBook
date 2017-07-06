#include "ofApp.h"

void ofApp::setup(){
    ofAddListener(eventsListener.intEvent, this, &ofApp::intEventReceived);
}
void ofApp::update(){}

void ofApp::draw(){
    eventsListener.draw();
}
void ofApp::exit(){
    ofRemoveListener(eventsListener.intEvent, this, &ofApp::intEventReceived);
}

void ofApp::intEventReceived(int & i){
    //Let's set the background color whenever we get this event
    ofSetBackgroundColor(ofColor((int)floor(ofRandom(255)),(int)floor(ofRandom(255)),(int)floor(ofRandom(255))));
}
