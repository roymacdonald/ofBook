# Events
Written by Roy J. Macdonald

openFrameworks has a super powerful events subsystem, which drives a lot of its internal workings and which you can make use to make your code a lot more cleaner, eficient and powerful.

## what is an ofEvent
An event is like a broadcasted message; sent from somewhere and listened by anyone that wants to. Each time that an event is sent (notified), the events subsystem will check if there are any listeners (other parts of the code that are expecting for this message) and if so it will call the callback functions that were registered with each listener.


## The events subsystem
OpenFrameworks has an underlaying system that is in charge of handling ofEvents.
In order to understand how it works imagine the following.

There's a big company with lots of different subdivisions, but a lot of the work that each subdivision has to do depends on another subdivision having finished its work.
Think of two subdivisions, `A` and `B` (in code each could be a different class). `A` depends on `B` having its work finished in order to make its own.

The company has an events management system (just like OF). When operations in `A` start each day, `A` will call the events management and tell "Hey, please let me know when `B` has finished its job. Call back to this subdivision and give us the information related to the job finished." 
So now, `A` can forget about whatever `B` is doing until it gets called back with the new information about the finished job.
When `B` finishes its job, it will call to events management and give it the relevant information about this just finished job. Then events management will call back to whom ever wanted to know that `B` finished its job. In this case it is just `A`, but it could be any number of subdivisions.
Once `A` has finished its labor day it will call to events management and say "I don't need to be notified about `B` finishing his job". This way it frees events management from spending time trying to reach an unoperative subdivision. 

This is more or less how the events work inside OF.

## What are events useful for

This has a lot of uses but the most important thing is that it makes the code more eficient, cleaner, flexible and way more powerful. [TODO: give reasons for these].

## Understanding events

There are two important things that we need to be able to notify and listen events; 
An **ofEvent instance** and a **callback function or method**.

###ofEvent instance
The ofEvent instance will probably be part of the class that will notify (send) this event. I say probably because it is not mandatory but it tends to be the common practice.
If you make this object public it will be available to any other class but if it is private only other objects of the same class will be able to listen this event notifications.

As said, events are like messages, hence these will pass some kind of information. Because of this, this information needs to be of a previously specified type. This can be from a simple `int` to any kind of class. Because so, the event and the callback need to have the same kind of information.

For example, we can have a class called `B` that has an ofEvent that  will send an `int`. Something like:

	class B {
	public:
	    //whatever else stuff of this class.
	    void update(){
	    	//when the desired conditions for sending the event are met we call
			int i = ofGetFrameNum();//this is the information that the event will pass.
	    	ofNotifyEvent(intEvent, i);
	    }
	    ofEvent<int> intEvent;
	};
	
Notice that the event type goes in between `ofEvent<` and `>`. let's call this the event's associated type.

###The callback

The callback function or method can be anywhere in your code, even a different class. It is up to you where you put it, it depends on what you want to do.

For example, let's declare a class called `A` that will have a callback function. Just for the sake of simplicity, let's make `A` the owner of the `B` instance that we want to listen to.
	
	class A {
	public:
		A(){// this is the class constructor.
		
			//Here we will register our listener just for simplicity but you could 
			//do this whenever you need to.
			ofAddListener(myBInstance.intEvent, this, &B::myCallBackFunction);
		}
		~A(){// this is the class destructor.
		
			//We will unregister from the event when this object gets destroyed.
			ofRemoveListener(myBInstance.intEvent, this, &B::myCallBackFunction);
		}

		void myCallBackFunction(int & i){// this is the callback method.
		
			//Here you write what you want to do when this event is received.
			//Probably using the passed argument, setting up a flag or calling some 
			//other functions or methods. In this example we will just print to the console.
			cout << "new event : "<< i << endl;
		}
		
		B myBInstance;//This is the instance of B that we want to listen to.
		
		// anything else that this class need for working.
	};
	
	
Notice that the argument type of the callback function is the same as the events asociated type -the one in between the `<` and `>`. Also notice the `&` that follows the type in the callback. It is super important that you add it. Otherwise it will not work. This `&` means that the argument is passed as reference. [TO DO: link to the explanation of references, hopefully in the ofBook]

##Adding and removing listeners

In order to make the callback function to react to the event notifications we need to register them together. This is like a kind of link between the event and the callback which is managed by OF's internal events subsystem.

For registering you use `ofAddListener(...)` and `ofRemoveListener(...)`;

As you can see from the previous piece of code, the arguments passed are the same in `ofAddListener` and `ofRemoveListener`, these are:

		ofAddListener(myBInstance.intEvent,// this is the instance of the event that we want to listen to.
					  this,// this is the pointer to the object that has the callback 
					  &B::myCallBackFunction// this is the pointer to the callback.
					  );
These arguments mean:

* `myBInstance.intEvent` is quite straight forwards. You just pass the event that you want to listen to.
* `this` could be a bit confusing for newcomers. If you dont know what it means read here.[TODO: Put link to adecuate resource.]
* `&B::myCallBackFunction`, has a bit more things. Firts there's a `&` which is used to dereference (turn something into a pointer). Read here [TODO: Put link to adecuate resource.] about pointers and references if you dont know what it means. Then there is `::`. It is used for specifying a certain something of a class. In this case it is the function `myCallbackFunction`of the class `B`.


To stop a callback function from reacting to an event, we unregister it; we remove a listener. Use `ofRemoveListener` passing the exact same arguments you passed in `ofAddListener`.

let's see some real world examples.


## openFrameworks core events

The most simple way to use ofEvents is by listening to openFrameworks core events.
In this case we only have to worry about providing an adecuate callback function and registering to the desired event. OF will take care of the rest internally.
You can access these by calling `ofEvents()`.

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

The adecuate callback function or method is one that has the same argument type as the associated type of the event you want to listen to.

You can notice that these events are named the same as the functions that are declared by default in your `ofApp`. Also you can notice that `ofApp` inherits from `ofBaseApp`, which is the base class that is registernig to these OF core events and calling the according function of `ofApp`. Go and open `ofBaseApp` and take a look at what it is going on there. 


#####Example Code 1
Make a new empty project with the project generator and put the following in `ofApp.h`


````
#pragma once
#include "ofMain.h"

//This class is so simple and short that there's no need to make a new file for it.
class SimpleEventsListener{
public:
    SimpleEventsListener(){
        //let's start listening to the mouseMoved event when this object gets created.
        ofAddListener(ofEvents().mouseMoved, this, &SimpleEventsListener::mouseMoved);
        //let's set the rectangle we are going to draw.
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


Compile and run it. Move the mouse over the rectangle. It changes its color to a random one when we move the mouse over it. Not very impresive but did you notice that there's nothing else but the `eventsListener.draw();` call in the `ofApp.cpp` file. This makes `ofApp` not having to worry about whatever `eventsListener` has to do. This makes `SimpleEventsListener`  very independent from `ofApp`, meaning that you could be able to have another `SimpleEventsListener` object anywhere else and expect the same behavior. Try to expand this example and make a button class.


###Using all the mouse events
If you want to register to all the mouse events you'll need to declare the following callback methods.

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

and to stop listening just call:

	ofUnregisterMouseEvents(this);

Super handy! :)

The same pattern applies for key and touch.


	ofRegisterKeyEvents(this);
	ofRegisterTouchEvents(this);

	ofUnregisterKeyEvents(this);
	ofUnregisterTouchEvents(this);


##Custom Events

As we have already seen how to use openFrameworks core events it is time to see how to use custom events.
Let's expand/modify a litle bit the previous example so `ofApp` reacts when the `SimpleEventsListener` object sends when the mouse is over its rectangle.


#####Example Code 2
ofApp.h


````
#pragma once

#include "ofMain.h"

class SimpleEventsListener{
public:
    SimpleEventsListener(){
        //let's start listening to the mouseMoved event when this object gets created.
        ofAddListener(ofEvents().mouseMoved, this, &SimpleEventsListener::mouseMoved);
        //let's set the rectangle we are going to draw.
        rect.set(200,300, 100,100);
        
        //let's set rectangle's color to red.
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
		void exit();
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
void ofApp::exit(){
	ofRemoveListener(eventsListener.intEvent, this, &ofApp::intEventReceived);
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

Run this example. Now when you move the mouse over the rectangle the background changes to a random color.

#####Example Code 3
Let's take this a little bit further.

ofApp.h

````
#pragma once

#include "ofMain.h"


class RandomColorButton{
public:
    RandomColorButton(){
        //In this case we will be listening to mouseReleased instead of mouseMoved
        ofAddListener(ofEvents().mouseReleased, this, &RandomColorButton::mouseReleased);
        //Also we will listen to the draw event so this class draws by itself.
        ofAddListener(ofEvents().draw, this, &RandomColorButton::draw);
        
        //let's set the rectangle we are going to draw to a random size and position.
        //Instead of calling ofRandom directly inside rect.set() the following code avoids that
        //the rectangle falls out of the window.
        float w = ofRandom(30, 300);
        float h = ofRandom(30, 300);
        float x = ofRandom(ofGetWidth() - w);
        float y = ofRandom(ofGetHeight() - h);
        rect.set(x,y,w,h);
        
        //let's set rectangle's color to a random color
        color = ofColor((int)floor(ofRandom(255)),(int)floor(ofRandom(255)),(int)floor(ofRandom(255)));
        
    }
    ~RandomColorButton(){
        //this is the destructor. We need to rem
        ofRemoveListener(ofEvents().mouseMoved, this, &RandomColorButton::mouseReleased);
        ofRemoveListener(ofEvents().draw, this, &RandomColorButton::draw);
    }
    void mouseReleased(ofMouseEventArgs& args){
        if(rect.inside(args.x, args.y)){
            // now when you release the mouse inside this rectangle it will broadcast an event with its color
            // This way ofApp will be able to change its background to the color of the clicked rectangle.
			ofNotifyEvent(colorEvent, color);
        }
    }
    
    void draw(ofEventArgs&){
        ofPushStyle();
        ofSetColor(color);
        ofDrawRectangle(rect);
        ofPopStyle();
    }
    
    ofEvent<ofColor> colorEvent;
    
protected:
    ofRectangle rect;
    ofColor color;

};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

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
    
        // This is the callback method. Notice that it's argument is an ofColor which matches the RandomColorButton event.
        void colorEventReceived(ofColor & color);
    
        vector<RandomColorButton> buttons;
};
````

ofApp.cpp

````
#include "ofApp.h"


void ofApp::setup(){
    //Make 100 buttons
    buttons.resize(100);
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
//There's no need to draw the buttons. :)
}
//--------------------------------------------------------------
void ofApp::colorEventReceived(ofColor & color){
	//Let's set the background color to the one sent in the received event
    ofSetBackgroundColor(color);
}
//--------------------------------------------------------------
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

Run this code and see what happens.



## ofEvent<void>

Also you can have events that dont send any additional data.
If you declare an event as 
	
	ofEvent<void> voidEvent;
	
the callback function you associate with it need to have no argumets.

	void theCallbackFunction(){}
and you notify it just by calling `ofNotifyEvent(voidEvent);`


## Start and stop listening as desired.
In all the previous examples we were Adding and removing the listeners when the app begun and exited, respectively. In a lot of cases you might not want it to happen this way, instead making this happen dynamically. This is done just like in the previous examples, but the important thing you need to care of is to not add a listener that's already added, because it might lead to unexpected behaviors. Even more important is not removing a listener that has not been added yet because the app will crash.
But don't worry, there's a simple and safe way for handling this.

#####Example Code 4

Notice whats going on in the `enableMouse()` and `disableMouse()` methods of `ToggleableButton`.

ofApp.h

````
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
    //We will use this boo to check if the events are registered or not.
    bool bListeningMouseEvent = false;

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
    
        ToggleableButton button;
};
````

ofApp.cpp

````
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
//--------------------------------------------------------------
void ofApp::keyPressed(int key){}
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



## ofEvent advanced features

There are a few other things that events have that make them even more powerful and flexible.

### Events priority

The OF events subsystem lets you specify the order in which the callbacks for the same event are called. This is done by setting its priority. 
By default, when you call `ofAddListener(...)` the priority is set to `OF_EVENT_ORDER_AFTER_APP` so this will get called after the `ofApp` events are called.
The event priority is set when the listener is added, by passing the priority, which is an `int`, as the fourth argument of `ofAddListener(...)`.

`OF_EVENT_ORDER_AFTER_APP` is just a preprocessor define equal to 200;
There are also `OF_EVENT_ORDER_BEFORE_APP` that equals zero and `OF_EVENT_ORDER_APP` which equals 100.

It is important to notice that you need to pass to `ofRemoveListener(...)` the same priority as the one used in `ofAddListener(...)`.


You might be wondering how this is useful. For example, if you have several objects listening to the `draw` core event you can controll which object one is drawn first and which last.


### Events propagation






[TODO: finish this section]


### ofEvents and Lambda functions


### static ofEvents

## The new way
ofEventListener

