# Events
Written by Roy J. Macdonald

openFrameworks has a super powerful events subsystem, which drives a lot of its internal workings and which you can make use to make your code a lot more cleaner, eficient and powerful.

### what is an events
An event is like a broadcasted message; sent from somewhere and listened by anyone that wants to. Each time that an event is sent (notified), the events subsystem will check if there are any listeners (other parts of the code that are expecting for this message) and if so it will call the callback functions that were registered with each listener.


### The events subsystem
Imagine the following.
There's a big company with lots of different subdivisions, but a lot of the work that each subdivision has to do depends on another subdivision having finished its work.
Think of two subdivisions, `A` and `B` (in code each could be a different class). `A` depends on `B` having its work finished in order to make its own.

The company has an events management system (just like OF). When operations in `A` start each day, `A` will call the events management and tell "Hey, please let me know when `B` has finished its job. Call back to this subdivision and give us the information related to the job finished." 
So now, `A` can forget about whatever `B` is doing until it gets called back with the new information about the finished job.
When `B` finishes its job, it will call to events management and give it the relevant information about this just finished job. Then events management will call back to whom ever wanted to know that `B` finished its job. In this case it is just `A`, but it could be any number of subdivisions.
Once `A` has finished its labor day it will call to events management and say "I don't need to be notified about `B` finishing his job". This way it frees events management from spending time trying to reach an unoperative subdivision. 

This is more or less how the events work inside OF.

### What are events useful for

This has a lot of uses but the most important thing is that it makes the code more eficient, cleaner, flexible and way more powerful. 

### Understanding events

There are two important things that we need to be able to notify events; An ofEvent instance and a callback function or method.

The ofEvent instance will probably be part of the class that will notify (send) this event. I say probably because it is not mandatory but it tends to be the common practice.
If you make this object public it will be available to any other class but if it is private only other objects of the same class will be able to listen this event notifications.

The callback function or method can be anywhere in your code, even a different class. It is up to you where you put it, it depends on what you want to do.

As said, events are like messages, hence these will pass some kind of information. Because of this, this information needs to be of a previously specified type. This can be from a simple `int` to any kind of class. Because so, the event and the callback need to have the same kind of information.

For example, we can have a class called `B` that has an ofEvent that  will send an `int`. Something like:

	class B {
	public:
	    //whatever else stuff of this class.
	    void update(){
	    	//when the desired conditions for sending the event are met we call
			int i = ofGetFrameNum();//this is the "message" that the event will pass.
	    	ofNotifyEvent(intEvent, i);
	    }
	    ofEvent<int> intEvent;
	};
	
Notice that the event type goes in between `ofEvent<` and `>`. Lets call this the event's associated type.

The callback function, which will probably be somewhere else in your code, maybe a different class will look like:
For this example let's say that it belongs to a class called `A`. Just for the sake of simplicity, let's make `A` the owner of the `B` instance that we want to listen to.
	
	class A {
	public:
		A(){// this is the class constructor.
		
			//Here we will register our listener just for simplicity but you could 
			//do this whenever you need to.
			ofAddListener(myBInstance.intEvent, this, &B::myCallBackFunction);
		}
		~A(){// this is the class destructor.
		
			//We will unregister from the event when this object gets destroyed.			ofRemoveListener(myBInstance.intEvent, this, &B::myCallBackFunction);
		}

		void myCallBackFunction(int & i){// this is the callback method.
		
			// here you write what you want to do when this event is received. 			//Probably using the passed argument, setting up a flag or calling some 			//other functions or methods.
			cout << "new event : "<< i << endl;
		}
		
		B myBInstance;
		
		// anything else that this class need for working.
	};
	
	
Notice that the argument type of the callback function is the same as the events asociated type -the one in between the `<` and `>`. Also notice the `&` that follows the type in the callback. It is super important that you add it. Otherwise it will not work. This `&` means that the argument is passed as reference. [TO DO: link to the explanation of references, hopefully in the ofBook]

You can see that the arguments passed are the same in `ofAddListener` and `ofRemoveListener`, these are:

		ofAddListener(myBInstance.intEvent,// this is the instance of the event that we want to listen to.
					  this,// this is the pointer to the object that has the callback 
					  &B::myCallBackFunction// this is the pointer to the callback.
					  );

The first argument is quite straight forwards. You just pass the event that you want to listen to.
The second, `this` could be a bit confusing for newcomers. If you dont know what it means read here.[TODO: Put link to adecuate resource.]
The third has a bit more things. Firts thers a `&` which is used to dereference. Read about pointers and references if you dont know what it means. Here [TODO: Put link to adecuate resource.]

To remove a listener using `ofRemoveListener` you need to pass the exact same arguments you passed in `ofAddListener`.


### ofEvents()

The most simple way to use events is by listening to openFrameworks core events.
In this case we only have to worry about providing an adecuate callback function and registering to the desired event. OF will take care of the rest internally.

These are:

	ofEvent<ofEventArgs> 		setup;
	ofEvent<ofEventArgs> 		update;
	ofEvent<ofEventArgs> 		draw;
	ofEvent<ofEventArgs> 		exit;

	ofEvent<ofResizeEventArgs> 	windowResized;
	ofEvent<ofWindowPosEventArgs> 	windowMoved;

	ofEvent<ofKeyEventArgs> 	keyPressed;
	ofEvent<ofKeyEventArgs> 	keyReleased;

	ofEvent<ofMouseEventArgs> 	mouseMoved;
	ofEvent<ofMouseEventArgs> 	mouseDragged;
	ofEvent<ofMouseEventArgs> 	mousePressed;
	ofEvent<ofMouseEventArgs> 	mouseReleased;
	ofEvent<ofMouseEventArgs> 	mouseScrolled;
	ofEvent<ofMouseEventArgs> 	mouseEntered;
	ofEvent<ofMouseEventArgs> 	mouseExited;

	ofEvent<ofTouchEventArgs>	touchDown;
	ofEvent<ofTouchEventArgs>	touchUp;
	ofEvent<ofTouchEventArgs>	touchMoved;
	ofEvent<ofTouchEventArgs>	touchDoubleTap;
	ofEvent<ofTouchEventArgs>	touchCancelled;

	ofEvent<ofMessage>			messageEvent;
	ofEvent<ofDragInfo>			fileDragEvent;
	ofEvent<uint32_t>			charEvent;

In order to register to listen to any of these events we need to provide an adecuate callback; one that has the same argument type as the associated type of the event.	
You can notice that these events are named the same as the functions that are declared by default in your `ofApp`. Also you can notice that `ofApp` inherits from `ofBaseApp`, which is the base class that is registernig to these OF core events and calling the according function of `ofApp`. Go and open `ofBaseApp` and take a look at what it is going on there. 


Example.
Make a new empty project with the project generator.
put the following in ofApp.h


````
#pragma once

#include "ofMain.h"

class SimpleEventsListener{
public:
    SimpleEventsListener(){
        //lets start listening to the mouseMoved event when this object gets created.
        ofAddListener(ofEvents().mouseMoved, this, &SimpleEventsListener::mouseMoved);
        //lets set the rectangle we are going to draw.
        rect.set(200,300, 100,100);
    }
    ~SimpleEventsListener(){
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


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        SimpleEventsListener eventsListener;
};
		
````

and this in ofApp.cpp


````
#include "ofApp.h"

void ofApp::setup(){}
void ofApp::update(){}

void ofApp::draw(){
    eventsListener.draw();
}
void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}

````


Run it and move the mouse over the rectangle.

The rectangle changes its color to a random one when we move the mouse over it. Not very impresive but did you notice that theres nothing else but the `eventsListener.draw();` call in the cpp file. This makes `ofApp` not having to worry about whatever `eventsListener` has to do. This makes this class very independent from ofApp, meaning that you could be able to have another `SimpleEventsListener` object anywhere else and expect the same behavior. Try to expand this example and make a button class.


If you want to register to all the mouse events you'll need to declare the following callback functions.

	void mouseMoved(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void mouseScrolled(ofMouseEventArgs& args);
	void mouseEntered(ofMouseEventArgs& args);
	void mouseExited(ofMouseEventArgs& args);

Instead of having to start and stop listening to each individual event, openFrameworks comes with some handy helper functions for listening to mouse events, as well as keyboard and touch events.

So to start listening to the mouse events in a class, named for example `ListenerClass` you just need to use
	
	ofRegisterMouseEvents(this);

instead of

	ofAddListener(ofEvents().mouseDragged, this,&ListenerClass::mouseDragged);
	ofAddListener(ofEvents().mouseMoved,   this,&ListenerClass::mouseMoved);
	ofAddListener(ofEvents().mousePressed, this,&ListenerClass::mousePressed);
	ofAddListener(ofEvents().mouseReleased,this,&ListenerClass::mouseReleased);
	ofAddListener(ofEvents().mouseScrolled,this,&ListenerClass::mouseScrolled);
	ofAddListener(ofEvents().mouseEntered, this,&ListenerClass::mouseEntered);
	ofAddListener(ofEvents().mouseExited,  this,&ListenerClass::mouseExited);

and to stop listening

	ofUnregisterMouseEvents(this);

Super handy! :)

The same pattern applies for key and touch.


	ofRegisterKeyEvents(this);
	ofRegisterTouchEvents(this);

	ofUnregisterKeyEvents(this);
	ofUnregisterTouchEvents(this);


###Custom Events

As we have already seen how to use openFrameworks core events it is time to see how to use custom events.
Let's expand a litle bit the previous example.
Lets make the ofApp react against an event that the SimpleEventsListener object will send when the mouse is over its rectangle.

ofApp.h


````
#pragma once

#include "ofMain.h"

class SimpleEventsListener{
public:
    SimpleEventsListener(){
        //lets start listening to the mouseMoved event when this object gets created.
        ofAddListener(ofEvents().mouseMoved, this, &SimpleEventsListener::mouseMoved);
        //lets set the rectangle we are going to draw.
        rect.set(200,300, 100,100);
        
        //lets set rectangle's color to red.
        color = ofColor::red;
        
    }
    ~SimpleEventsListener(){
        //this is the destructor. We need to rem
        ofRemoveListener(ofEvents().mouseMoved, this, &SimpleEventsListener::mouseMoved);
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


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    	void intEventReceived(int & i);
    
        SimpleEventsListener eventsListener;
};
		
````

ofApp.cpp

````
#include "ofApp.h"

void ofApp::setup(){
	ofAddListener(eventsListener.intEvent, this, &ofApp::intEventReceived);
}
void ofApp::update(){}

void ofApp::draw(){
    eventsListener.draw();
}

void ofApp::intEventReceived(int & i){
	//Let's set the background color whenever we get this event 
	ofSetBackgroundColor(ofColor((int)floor(ofRandom(255)),(int)floor(ofRandom(255)),(int)floor(ofRandom(255))));
}

void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}

````


### Events propagation and priority

There are two other things that events have that make them even more powerful; being able to control their propagation and establishing their priority.

[TODO: finish this section]



### ofEvent<void>

Also you can have events that dont send any additional data.
If you declare an event as 
	
	ofEvent<void> voidEvent;
	
the callback function you associate with it need to have no argumets.

	void theCallbackFunction(){}
and you notify it just by calling `ofNotifyEvent(voidEvent);`







