# ofEvents

Written by Roy J. Macdonald

openFrameworks has a super powerful events system which you can use to write cleaner and more flexible code. The system comes with openFrameworks-specific lifecycle events that you can hook into, like `draw` and `mouseMoved`. It also allows you to create your own custom events, so that you can do things like create a self-contained button class.

## What is an Event?

The event system is based around the idea of broadcasting messages. A part of your code can send messages to other parts that are listening. This is called the ["observer pattern"](https://en.wikipedia.org/wiki/Observer_pattern). There are "subjects" (senders) and "observers" (listeners). Observers subscribe to the particular subject they are interested in listening to. When a subject decides to send a message, any listeners that are subscribed will be notified of that message.

In openFrameworks, an `ofEvent` instance is a subject. A listener can subscribe to an `ofEvent` via  [`ofAddListener`](http://openframeworks.cc/documentation/events/ofEventUtils/#!show_ofAddListener). When a listener registers with an `ofEvent`, it specifies a "callback" - a function that will be invoked when the event is triggered.

**MH: It would be useful to include an image here. Check out the Auctioneer image [here](https://sourcemaking.com/design_patterns/observer).**

**MH: I dropped the "The events subsystem" section. I think the example is confusing when using abstract "A" and "B". It could be added back with a more concrete example using something from every day life. E.g. maybe you could write it using a chatroom or mailing list analogy. You explicitly join/subscribe to a chat/mailing list. Once you've joined, you get notifications.**

## Why are Events Useful?

Events allow you to write certain types of code in a clean and flexible way.

[TODO: give reasons for these].
**MH: Some reasons that come to mind are decoupling components and creating one-to-many relationships. You might want to explicitly mention that event systems are often used in UI programming. See [Game Programming chapter on Observers](http://gameprogrammingpatterns.com/observer.html): "That's what the observer pattern is for. It lets one piece of code announce that something interesting happened without actually caring who receives the notification."**

## Understanding ofEvents

There are two parts to using the openFrameworks event system: an **`ofEvent` instance** (a subject) and a **callback function or method** (a listener).

### ofEvent Instance

The `ofEvent` instance is a subject that can be listened to by other parts of your code. If you make this `ofEvent` object public, any other class will be able to access it and add a listener. If you make it private only other objects of the same class will be able to listen to the event.

ofEvents are like messages so these usually will involve passing some information when an event is triggered. In C++, this information needs to be specified with a type. This can be something simple like an `int` or a custom class or even it could be void.  The ofEvent and the callback both need to have the same type specified.

For example, we can have a class called `EventSender` that has an ofEvent that  will send an `int`. Something like:

```cpp
class EventSender {
public:
    //whatever else stuff of this class.
    void someFunction(){
        //when the desired conditions for sending the ofEvent are met we call
        int i = ofGetFrameNum();//this is the information that the ofEvent will pass.
        ofNotifyEvent(intEvent, i);
    }
    ofEvent<int> intEvent;
};
```

Notice that the ofEvent type goes in between `ofEvent<` and `>`. Let's call this the ofEvent's associated type.

### The Callback

The callback function or method can be anywhere in your code, usually in a different class. It can be a class-member (method) or non-class-member function. It needs to have a single parameter, a reference to a variable whose type matches the event's associated type. A callback for the `intEvent` of the previous class `EventSender` would look something like:

```cpp
void myCallBackFunction(int & frameNum){// this is the callback method.

    //Here you write what you want to do when this ofEvent is received.
    //Probably using the passed argument, setting up a flag or calling some
    //other functions or methods. In this example we will just print to the console.
    cout << "new ofEvent : "<< frameNum << endl;
}
```

Notice that the callback's parameter type matches the ofEvents associated type - e.g. `ofEvent<int>`. Also notice the `&` that follows the type in the callback. This `&` means that the argument is passed as reference. This is necessary, and without it, the code won't work. For more information, check out [this section](https://github.com/openframeworks/ofBook/blob/master/chapters/cplusplus_basics/unabridged.md#dynamic-allocation-and-pointers) of the unabridged "C++ Language Basics" chapter.

**MH: added a link to the C++ basics chapter that deals with references, but I would recommend linking to an additional, external C++ tutorial.**

### ofEvent<void>

As said, the ofEvent associated type can be `void`, meaning that no information is sent along. You declare such as:

```cpp
ofEvent<void> voidEvent;
```

The callback function needs no arguments, like:

```cpp
void theCallbackFunction(){}
```

And for sending this event you call

```cpp
ofNotifyEvent(voidEvent);
```

### Adding and Removing Listeners

In order to make a callback function react to an ofEvent's notifications, we need to register them together. For registering and unregistering, you use [`ofAddListener(...)`](http://openframeworks.cc/documentation/events/ofEventUtils/#!show_ofAddListener) and [`ofRemoveListener(...)`](http://openframeworks.cc/documentation/events/ofEventUtils/#!show_ofRemoveListener) respectively.

For example, let's declare a class called `EventListener` that will have a callback function. Just for the sake of simplicity, let's make `EventListener` the owner of the `EventSender` instance that we want to listen to.

```cpp
class EventSender {
public:
    //whatever else stuff of this class.
    void someFunction(){
        //when the desired conditions for sending the ofEvent are met we call
        int i = ofGetFrameNum();//this is the information that the ofEvent will pass.
        ofNotifyEvent(intEvent, i);
    }
    ofEvent<int> intEvent;
};

class EventListener {
public:
    EventListener(){// this is the class constructor.

        //Here we will register our listener just for simplicity but you could
        //do this whenever you need to.
        ofAddListener(myEventSenderInstance.intEvent, this, &EventListener::myCallBackFunction);
    }
    ~EventListener(){// this is the class destructor.

        //We will unregister from the ofEvent when this object gets destroyed.
        ofRemoveListener(myEventSenderInstance.intEvent, this, &EventListener::myCallBackFunction);
    }

    void myCallBackFunction(int & i){// this is the callback method.

        //Here you write what you want to do when this ofEvent is received.
        //Probably using the passed argument, setting up a flag or calling some
        //other functions or methods. In this example we will just print to the console.
        cout << "new ofEvent : "<< i << endl;
    }

    EventSender myEventSenderInstance;//This is the instance of EventSender that we want to listen to.

    // anything else that this class need for working.

    //The following are needed in order to comply with the rule of 3 (or 5)
    //because this class has an explicitly declared destructor.
    //Read about the rule of 3 (or 5) towards the end of this chapter

    EventListener(const EventListener &) =default;//default copy constructor
    EventListener & operator=(const EventListener &) =default;//default copy assignment operator
    EventListener & operator=(EventListener &&) =default;//default move assignment operator
    EventListener(EventListener &&) =default;//default move constructor

};
```

The arguments passed in to `ofAddListener` and `ofRemoveListener` are the same, these are:

```cpp
ofAddListener(myEventSenderInstance.intEvent, // this is the instance of the ofEvent that we want to listen to.
                this, // this is the pointer to the object that has the callback.
                & EventListener::myCallBackFunction // this is the pointer to the callback.
                );
```

Here is what these arguments mean:

* `myEventSenderInstance.intEvent` is the ofEvent you want to listen to.
* `this` could be a bit confusing for newcomers. If you don't know what it means see this resource. [TODO: Put link to adequate resource.]
* `&EventListener::myCallBackFunction` has two important parts. `EventListener::myCallBackFunction` means find the function `myCallBackFunction` that is a member of the `EventListener` class. The `&` is the "address-of" operator that allows us to find the memory address of the function. Read [this section](https://github.com/openframeworks/ofBook/blob/master/chapters/cplusplus_basics/unabridged.md#dynamic-allocation-and-pointers) about pointers and references if you don't know what it means.

To stop a callback function from reacting to an ofEvent, we must unregister it. Use `ofRemoveListener`, passing in the exact same arguments you passed in to `ofAddListener`.

Now let's see some real world examples.

## openFrameworks Core ofEvents

One simple way to use ofEvents is to listening to an openFrameworks core event. openFrameworks has ofEvents that correspond to `draw`, `keyPressed`, etc. We only have to worry about creating an appropriate listener - by providing a callback function and registering it to the desired ofEvent. You can access the core events by calling `ofEvents()`. The core events are:

```cpp
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
```

To listen to one of these events, you need to create a callback function or method with one parameter whose type matches the specific ofEvent you want to listen to. E.g. you would need a parameter of type `ofMouseEventArgs` for the `mouseMoved` event. For more information on these types, see the [events documentation page](http://openframeworks.cc/documentation/events/).

You'll notice that these ofEvents have the same name as the default functions that are created in your `ofApp` class (e.g. `draw`, `setup`, etc.). `ofApp` inherits from `ofBaseApp` which handles registering these functions as listeners of the corresponding OF core ofEvents (e.g. `draw()` listens to `ofEvent<ofEventArgs> draw`). Go and open `ofBaseApp` and take a look at what it is going on there.

### Example: Color Changing Rectangle

**MH: a picture or GIF would really help make it immediately clear what the example is doing.**

Open the Color_changing_rectangle_example `chapters/events/code/`
[TODO: link to instructions for generating the examples using project generator].

This is the SimpleEventsListener.h file

```cpp
//This class is so simple and short that there's no need to make a .cpp file for it.
class SimpleEventsListener{
public:
    SimpleEventsListener(){//this is the class constructor
        //let's start listening to the mouseMoved ofEvent when this object gets created.
        ofAddListener(ofEvents().mouseMoved, this, &SimpleEventsListener::mouseMoved);
        //let's set the rectangle we are going to draw.
        rect.set(200,300, 100,100);
    }
    ~SimpleEventsListener(){//this is the class destructor
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

```
ofApp.h:

```cpp
#pragma once
#include "ofMain.h"
#include "SimpleEventsListener.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        SimpleEventsListener eventsListener;
};
```

and this in ofApp.cpp:

```cpp
#include "ofApp.h"

void ofApp::setup(){}
void ofApp::update(){}

void ofApp::draw(){
    eventsListener.draw();
}
```

Compile and run it. Move the mouse over the rectangle, and you will see the rectangle changes to a random color. Not very impressive but notice that there's nothing else but the `eventsListener.draw();` call in the `ofApp.cpp` file. This means `ofApp` does not have to worry about whatever `eventsListener` is doing. This makes `SimpleEventsListener` and `ofApp` independent of one another, meaning that you could be able to have another `SimpleEventsListener` object anywhere else and expect the same behavior. Try to expand this example and make a button class.

**MH: not sure what you are trying to say here. I think you want to say that SimpleEventsListener and ofApp are decoupled? That you could change the inner workings of SimpleEventsListener without breaking or having to change ofApp?**

### Using All the Mouse ofEvents

If you want to register listeners to all the mouse ofEvents, you would need to declare the following callback methods and use `ofAddListener` for each:

```cpp
void mouseMoved(ofMouseEventArgs& args);
void mouseDragged(ofMouseEventArgs& args);
void mousePressed(ofMouseEventArgs& args);
void mouseReleased(ofMouseEventArgs& args);
void mouseScrolled(ofMouseEventArgs& args);
void mouseEntered(ofMouseEventArgs& args);
void mouseExited(ofMouseEventArgs& args);
```

Instead of having to start listening to each individual ofEvent, openFrameworks comes with some handy helper functions for listening to all events of a particular type - all the mouse events, all the keyboard events or all the touch events. To start listening to all the mouse ofEvents in an example class named `ListenerClass`, you just need to use:

```cpp
ofRegisterMouseEvents(this);
```

Which is equivalent to writing:

```cpp
ofAddListener(ofEvents().mouseDragged, this,&ListenerClass::mouseDragged);
ofAddListener(ofEvents().mouseMoved,   this,&ListenerClass::mouseMoved);
ofAddListener(ofEvents().mousePressed, this,&ListenerClass::mousePressed);
ofAddListener(ofEvents().mouseReleased,this,&ListenerClass::mouseReleased);
ofAddListener(ofEvents().mouseScrolled,this,&ListenerClass::mouseScrolled);
ofAddListener(ofEvents().mouseEntered, this,&ListenerClass::mouseEntered);
ofAddListener(ofEvents().mouseExited,  this,&ListenerClass::mouseExited);
```

To stop listening just call:

```cpp
ofUnregisterMouseEvents(this);
```

Super handy! :)

The same pattern applies for key and touch.

```cpp
ofRegisterKeyEvents(this);
ofRegisterTouchEvents(this);

ofUnregisterKeyEvents(this);
ofUnregisterTouchEvents(this);
```

Note: you still need to have methods defined with the appropriate names (e.g. `mouseDragged`, `mouseMoved`, etc.).

## Custom Events

Now that we've seen how to use openFrameworks core ofEvents, it is time to see how to use custom ofEvents. These all you to create an event for any purpose (e.g. an event for when a button is pressed). Let's expand the previous example so `ofApp` can be notified when the mouse is over the `SimpleEventsListener` rectangle.

### Example: Mouse Over Event

**MH: a GIF would really help make it immediately clear what the example is doing.**

MouseOverListener.h

```cpp
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
```

ofApp.h:

```cpp
#pragma once

#include "ofMain.h"
#include "MouseOverListener.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void exit(); 

        void intEventReceived(int & i);

        MouseOverListener eventsListener;
};
```

ofApp.cpp:

```cpp

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
```
Run this example. When you move the mouse over the rectangle, the background of the app changes to a random color.

### Example: Many Buttons

**MH: a GIF would really help make it immediately clear what the example is doing.**

Let's take this a little bit further.

RandomColorButton.h:

```cpp
#pragma once
#include "ofMain.h"

class RandomColorButton{
public:
    RandomColorButton(){
        //In this case we will be listening to mouseReleased instead of mouseMoved
        ofAddListener(ofEvents().mouseReleased, this, &RandomColorButton::mouseReleased);
        //Also we will listen to the draw ofEvent so this class draws by itself.
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

```
ofApp.h:

```cpp
#pragma once
#include "ofMain.h"
#include "RandomColorButton.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();
        void exit();

        // This is the callback method. Notice that it's argument is an ofColor which matches the RandomColorButton ofEvent.
        void colorEventReceived(ofColor & color);

        //This will be the collection of buttons
        vector<RandomColorButton> buttons;
};

```

ofApp.cpp:

```cpp
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
```

Run this code. This will create 100 randomly positioned, sized and colored rectangles. If you clic any rectangle the app's background will change to the color of the clicked rectangle. This is a very basic self-contained button.
Can you expand it to make a fully functional button?


## Dynamically Starting and Stopping Listening

In all the previous examples we were adding and removing the listeners when the app begun and exited, respectively. In a lot of cases you might not want it to happen this way. Instead, you may want it to happen dynamically. This is done just like in the previous examples, but with extra attention to registering and unregistering. You need make sure to not add a listener that has already been added, because it might lead to unexpected behaviors. Even more important, you need to make sure to not unregister a listener before it has been added because the app will crash. Don't worry - there's a simple and safe way to handle this.

### Example: Buttons That Can Be Enabled and Disabled

**MH: a GIF would really help make it immediately clear what the example is doing. It would also help to describe the example before jumping into code.**

Notice what's going on in the `enableMouse()` and `disableMouse()` methods of `ToggleableButton`.

ToggleableButton.h:

```cpp
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
```

ofApp.h:

```cpp
#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        void keyReleased(int key);
        
        ToggleableButton button;
};
```

ofApp.cpp:

```cpp
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
```

## ofEvent Advanced Features

There are other features of ofEvents that make them even more flexible: event priority, event propagation and static events.

### Priority

The OF events system lets you specify the order in which the listeners for the same ofEvent are called. This allows you to do things like specify the order in which `draw` listeners are called (a z-index of sorts). It will also become important in the next section - event propagation.

You define the order by giving each listener a priority (an `int`) when you add it to an event. Listeners with a lower priority `int` are called before listeners with a higher priority `int`. By default, when you call `ofAddListener(...)` the priority is set to `OF_EVENT_ORDER_AFTER_APP`, meaning that the listener will get called after the `ofApp` events are called. You can set the priority yourself by passing an `int` as the fourth argument of `ofAddListener(...)`. You will need to pass the same priority `int` when you remove the listener with `ofRemoveListener(...)`.

`OF_EVENT_ORDER_AFTER_APP` is a preprocessor define constant equal to 200. There are two other constants: `OF_EVENT_ORDER_BEFORE_APP` which equals zero and `OF_EVENT_ORDER_APP` which equals 100.

### Event Propagation

Think of the following scenario where you have a button and an `ofEasyCam`. The button gets drawn to a certain place inside the window, and it reacts to mouse events there. At the same time, you have an `ofEasyCam` that reacts to mouse events on the whole window. When you press the button, the `ofEasyCam` will also react to the mouse interaction causing an unwanted behavior. 

There is an example in the code folder named `No_Propagation_Example` which depicts the scenario just mentioned.

How can we avoid this undesired behavior?

One simple and elegant solution is to use event propagation. Event propagation allows a listener with a low priority `int` to stop all listeners with a higher priority `int` from being notified of an event. If the button's listeners have a lower priority than the `ofEasyCam` listeners, then the button can stop certain mouse events from reaching `ofEasyCam`.

In all the previous examples, the callbacks have had `void` as their return type. In order to be able to deal with the event propagation the callback's return type needs to be of type `bool`. When the callback returns `true`, the event will stop propagating and any listeners that haven't already been called will not be notified.

The following example implements a solution to the button and `ofEasyCam` problem.
The important piece of code is in the following file/class. Notice how the event order is super important in order to use the event propagation successfully.


DraggableRect.h:

```cpp
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
```
ofApp.h

```cpp

#pragma once
#include "ofMain.h"
#include "DraggableRect.h"

class ofApp : public ofBaseApp{

    public:
        void draw();

        DraggableRect rect;

        ofEasyCam cam;
};

```

ofApp.cpp

```cpp
#include "ofApp.h"

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
    msg << "Clic and drag elsewhere to move the camera, hence rotate the magenta box";
    ofDrawBitmapStringHighlight(msg.str(), 20,20);
}
```

### Static ofEvents

You can have static ofEvents. This can become useful in several situations. Take a look at the example in `ofFolder/examples/events/simpleEventsExample`.

[TODO: explain this thoroughly].

**MH: I think you can get away with a brief explanation and then point them towards the example.**

## The Rule of 3 (or 5)

In C++ there is a rule, called the rule of 3, which became the rule of 5 in C++11. Read about it [here](https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)). In short, it says that if you one of the following methods to a class, you must add all of them:

*  destructor `~ClassName(){...}`
*  copy constructor `ClassName(ClassName& other)`
*  copy assignment operator `ClassName& operator=(ClassName& other)`
*  move assignment operator (C++11 only) `ClassName& operator=(ClassName&& other)`
*  move constructor (C++11 only) `ClassName(ClassName&& other)`

So, in order to have exception-safe code you must follow this rule. When listening to any `ofEvent` in your custom classes, it is a good idea to stop listening to these when the class instance is destroyed. This means that you will declare the class destructor, and hence, you'll have to follow this rule.

Even if you don't follow this rule, your code will compile with no errors or warnings, and it will probably work well, but it will not be exception-safe, and you run the risk of having several undesired side-effects, which can be really hard to debug.

**MH: it would be good to add a note with a concrete example that might arise if you declare a destructor without the other 4...**

## The New Way

From openFrameworks v0.10 onwards (not yet released as of 07/03/17), there is a new way for registering listeners. The old way, the one we've been using throughout this chapter, still works. By using this new way, you can avoid having to declare the class destructor, hence no extra stuff to comply with the rule of 3 (or 5).

The key ingredient in this new way is the new `ofEventListener` class. In order to register a listener to an ofEvent, you go through an `ofEventListener` object. This intermediary object allows you to easily unregister a listener without needing to specify any parameters (e.g. no need to remember the priority).

Taking the previous example from the ["Adding and Removing Listeners" section](#adding-and-removing-listeners). Using this new way would look like this:

```cpp
class B {
public:
    void someFunction(){
        int i = ofGetFrameNum();
        ofNotifyEvent(intEvent, i);
    }
    ofEvent<int> intEvent;
};

class A {
public:
    A(){
        listener = myBInstance.intEvent.newListener(this, &B::myCallBackFunction);
    }
    //There's no need to have a class destructor, because when the listener is destroyed
    //it will automatically unregister itself.

    void myCallBackFunction(int & i){// this is the callback method.
        cout << "new ofEvent : "<< i << endl;
    }
    void unregisterListener(){
        listener.unsubscribe();
    }		
    B myBInstance;
    ofEventListener listener;

};
```

So, to add a listener you have to call the `newListener(...)` method of the `ofEvent` you want to listen to and assign it to the `ofEventListener` object, like in the constructor of class `A`. To unregister a listener just call its `unsubscribe()` method. Look at class `A`'s `unregisterListener` method.

### ofEvents and Lambda Functions

This new way for listening to events allows you to use lambda functions as the callback. If you don't know about Lambda functions see the C++11 chapter. Lambda functions are anonymous functions. You can assign them dynamically and pass them as a parameter. You don't have to have to declare them before using them. It is possible to use a lambda function as the callback of any event.

For example, the code from the previous section using a lambda function would look like:

```cpp
class B {
public:
    void someFunction(){
        int i = ofGetFrameNum();
        ofNotifyEvent(intEvent, i);
    }
    ofEvent<int> intEvent;
};

class A {
public:
    A(){
        listener = myBInstance.intEvent.newListener([this](int & i){
            cout << "new ofEvent : "<< i << endl;
        });
    }
    void unregisterListener(){
        listener.unsubscribe();
    }		
    B myBInstance;
    ofEventListener listener;

};
```
