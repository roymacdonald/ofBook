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

There are two important things that we need to be able to notify and to listen ofEvents; 
An **ofEvent instance** and a **callback function or method**.

### ofEvent instance
The ofEvent instance will probably be part of the class that will notify (send) it. I say probably because it is not mandatory but it tends to be the common practice.
If you make this object public it will be available to any other class but if it is private only other objects of the same class will be able to listen this ofEvent notifications.

As said, ofEvents are like messages, hence these will pass some kind of information. Because of this, this information needs to be of a previously specified type. This can be from a simple `int` to any kind of class. Because so, the ofEvent and the callback need to have the same kind of information.

For example, we can have a class called `B` that has an ofEvent that  will send an `int`. Something like:

	class B {
	public:
	    //whatever else stuff of this class.
	    void someFunction(){
	    	//when the desired conditions for sending the ofEvent are met we call
			int i = ofGetFrameNum();//this is the information that the ofEvent will pass.
	    	ofNotifyEvent(intEvent, i);
	    }
	    ofEvent<int> intEvent;
	};
	
Notice that the ofEvent type goes in between `ofEvent<` and `>`. let's call this the ofEvent's associated type.

### The callback

The callback function or method can be anywhere in your code, usually in a different class. It is up to you where you put it, it depends on what you want to do. It can be a class-member (method) or non-class-member function.
It need to have a single argument, as a reference of the same type of the event's associated type.
A callback for the `intEvent` of class `B` (the one in the previous code block) will look something like:
	
		void myCallBackFunction(int & i){// this is the callback method.
			
			//Here you write what you want to do when this ofEvent is received.
			//Probably using the passed argument, setting up a flag or calling some 
			//other functions or methods. In this example we will just print to the console.
			cout << "new ofEvent : "<< i << endl;
		}
			
	
Notice that the argument type of the callback function is the same as the ofEvents asociated type -the one in between the `<` and `>`. Also notice the `&` that follows the type in the callback. It is super important that you add it. Otherwise it will not work. This `&` means that the argument is passed as reference. [TO DO: link to the explanation of references, hopefully in the ofBook]

### Adding and removing listeners

In order to make the callback function to react to the ofEvent notifications we need to register them together. This is like a kind of link between the ofEvent and the callback which is managed by OF's internal events subsystem.

For registering you use `ofAddListener(...)` and `ofRemoveListener(...)`;

For example, let's declare a class called `A` that will have a callback function. Just for the sake of simplicity, let's make `A` the owner of the `B` instance that we want to listen to.
	
	
	class B {
	public:
	    //whatever else stuff of this class.
	    void someFunction(){
	    	//when the desired conditions for sending the ofEvent are met we call
			int i = ofGetFrameNum();//this is the information that the ofEvent will pass.
	    	ofNotifyEvent(intEvent, i);
	    }
	    ofEvent<int> intEvent;
	};
	
	class A {
	public:
		A(){// this is the class constructor.
		
			//Here we will register our listener just for simplicity but you could 
			//do this whenever you need to.
			ofAddListener(myBInstance.intEvent, this, &B::myCallBackFunction);
		}
		~A(){// this is the class destructor.
			
			//We will unregister from the ofEvent when this object gets destroyed.
			ofRemoveListener(myBInstance.intEvent, this, &B::myCallBackFunction);
		}
		
		void myCallBackFunction(int & i){// this is the callback method.
		
			//Here you write what you want to do when this ofEvent is received.
			//Probably using the passed argument, setting up a flag or calling some 
			//other functions or methods. In this example we will just print to the console.
			cout << "new ofEvent : "<< i << endl;
		}
		
		B myBInstance;//This is the instance of B that we want to listen to.
		
		// anything else that this class need for working.

		//The following are needed in order to comply with the rule of 3 (or 5)
		//because this class has an explicitly declared destructor.		//Read about the rule of 3 (or 5) towards the end of this chapter
		
		A(const A &) =default;//default copy constructor
		A & operator=(const A &) =default;//default copy assignment operator
    	A & operator=(A &&) =default;//default move assignment operator
		A(A &&) =default;//default move constructor
		
	};
	
	

As you can see from the previous piece of code, the arguments passed are the same in `ofAddListener` and `ofRemoveListener`, these are:

		ofAddListener(myBInstance.intEvent,// this is the instance of the ofEvent that we want to listen to.
					  this,// this is the pointer to the object that has the callback 
					  &B::myCallBackFunction// this is the pointer to the callback.
					  );
These arguments mean:

* `myBInstance.intEvent` is quite straight forwards. You just pass the ofEvent that you want to listen to.
* `this` could be a bit confusing for newcomers. If you dont know what it means read here.[TODO: Put link to adecuate resource.]
* `&B::myCallBackFunction`, has a bit more things. Firts there's a `&` which is used to dereference (turn something into a pointer). Read here [TODO: Put link to adecuate resource.] about pointers and references if you dont know what it means. Then there is `::`. It is used for specifying a certain something of a class. In this case it is the function `myCallbackFunction`of the class `B`.


To stop a callback function from reacting to an ofEvent, we unregister it; we remove a listener. Use `ofRemoveListener` passing the exact same arguments you passed in `ofAddListener`.

let's see some real world examples.


## openFrameworks core ofEvents

The most simple way to use ofEvents is by listening to openFrameworks core ofEvents.
In this case we only have to worry about providing an adecuate callback function and registering to the desired ofEvent. OF will take care of the rest internally.
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

The adecuate callback function or method is one that has the same argument type as the associated type of the ofEvent you want to listen to.

You can notice that these ofEvents are named the same as the functions that are declared by default in your `ofApp`. Also you can notice that `ofApp` inherits from `ofBaseApp`, which is the base class that is registernig to these OF core ofEvents and calling the according function of `ofApp`. Go and open `ofBaseApp` and take a look at what it is going on there. 


##### Example Code 1
Make a new empty project with the project generator and put the following in `ofApp.h`



    #pragma once
    #include "ofMain.h"
    
    //This class is so simple and short that there's no need to make a new file for it.
    class SimpleEventsListener{
    public:
        SimpleEventsListener(){
            //let's start listening to the mouseMoved ofEvent when this object gets created.
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
		


and this in ofApp.cpp



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
    



Compile and run it. Move the mouse over the rectangle. It changes its color to a random one when we move the mouse over it. Not very impresive but did you notice that there's nothing else but the `eventsListener.draw();` call in the `ofApp.cpp` file. This makes `ofApp` not having to worry about whatever `eventsListener` has to do. This makes `SimpleEventsListener`  very independent from `ofApp`, meaning that you could be able to have another `SimpleEventsListener` object anywhere else and expect the same behavior. Try to expand this example and make a button class.


### Using all the mouse ofEvents
If you want to register to all the mouse ofEvents you'll need to declare the following callback methods.

	void mouseMoved(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void mouseScrolled(ofMouseEventArgs& args);
	void mouseEntered(ofMouseEventArgs& args);
	void mouseExited(ofMouseEventArgs& args);

Instead of having to start and stop listening to each individual ofEvent, openFrameworks comes with some handy helper functions for listening to mouse ofEvents, as well as keyboard and touch ofEvents.

So to start listening to the mouse ofEvents in a class, named for example `ListenerClass` you just need to use
	
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


## Custom Events

As we have already seen how to use openFrameworks core ofEvents it is time to see how to use custom ofEvents.
Let's expand/modify a litle bit the previous example so `ofApp` reacts when the `SimpleEventsListener` object sends when the mouse is over its rectangle.


##### Example Code 2
ofApp.h



    #pragma once
    
    #include "ofMain.h"
    
    class SimpleEventsListener{
    public:
        SimpleEventsListener(){
            //let's start listening to the mouseMoved ofEvent when this object gets created.
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
    		


ofApp.cpp


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
    


Run this example. Now when you move the mouse over the rectangle the background changes to a random color.

##### Example Code 3
Let's take this a little bit further.

ofApp.h


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
        
            // This is the callback method. Notice that it's argument is an ofColor which matches the RandomColorButton ofEvent.
            void colorEventReceived(ofColor & color);
        
            vector<RandomColorButton> buttons;
    };


ofApp.cpp


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
    //There's no need to draw the buttons. These draw by themselves. :)
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


Run this code and see what happens.



## ofEvent<void>

Also you can have ofEvents that dont send any additional data.
If you declare an ofEvent as 
	
	ofEvent<void> voidEvent;
	
the callback function you associate with it need to have no argumets.

	void theCallbackFunction(){}
and you notify it just by calling `ofNotifyEvent(voidEvent);`


## Start and stop listening as desired.
In all the previous examples we were adding and removing the listeners when the app begun and exited, respectively. In a lot of cases you might not want it to happen this way, instead making this happen dynamically. This is done just like in the previous examples, but the important thing you need to care of is to not add a listener that's already added, because it might lead to unexpected behaviors. Even more important is not removing a listener that has not been added yet because the app will crash.
But don't worry, there's a simple and safe way for handling this.

##### Example Code 4

Notice whats going on in the `enableMouse()` and `disableMouse()` methods of `ToggleableButton`.

ofApp.h


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


ofApp.cpp


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




## ofEvent advanced features

There are a few other things that ofEvents have that make them even more powerful and flexible.

### Priority

The OF events subsystem lets you specify the order in which the callbacks for the same ofEvent are called. This is done by setting its priority. 
By default, when you call `ofAddListener(...)` the priority is set to `OF_EVENT_ORDER_AFTER_APP`, meaning that the callback will get called after the `ofApp` events are called.
The ofEvent priority is set when the listener is added, by passing the priority, which is an `int`, as the fourth argument of `ofAddListener(...)`.

`OF_EVENT_ORDER_AFTER_APP` is just a preprocessor define equal to 200;
There are also `OF_EVENT_ORDER_BEFORE_APP` that equals zero and `OF_EVENT_ORDER_APP` which equals 100.

It is important to notice that you need to pass to `ofRemoveListener(...)` the same priority as the one used in `ofAddListener(...)`.


You might be wondering how this is useful. For example, if you have several objects listening to the `draw` core event you can controll which object one is drawn first and which last.


### Events propagation

Think of the following scenario. You have a button, similar to the ones of the previous examples; it gets drawn on a certain place inside your window and it reacts to the mouse events. At the same time you have an `ofEasyCam` object that its interaction area is the whole window. When you press the button the `ofEasyCam` will also react to the mouse interaction causing an unwanted behavior. How can we avoid this? There are several different way, but most are very cumbersome to use, having redundat code and not very flexible.

There is a much nicer and elegant solution. This is by stopping the event propagation. As seen before, the events have a specific order for excecuting the callbacks, defined by their priority. If a certain condition is met while a callback is runned you can stop the following callbacks from getting called. It is also super simple to code.

As we've seen in all the previous examples, all the callbacks have `void` as their return type. If you change the callback return type to `bool` and you make it to return `true` the propagation will stop.

The following code example will implement what was described at the begining of this part.

If you comment out the line that says  `#define STOP_EVENT_PROPAGATION`, the mouse events will not stop propagation.

ofApp.h


    #pragma once
    #include "ofMain.h"
    
    //Comment out the following line in order to allow event propagation.
    #define STOP_EVENT_PROPAGATION
    class DraggableRect{
    public:
        
        DraggableRect(){
            rect.set(100,100, 130,100);
            //I added all the mouse events call back just to show how handy the following function is.
            #ifdef STOP_EVENT_PROPAGATION
            //The ofEasyCam registers it's mouse events with OF_EVENT_ORDER_AFTER_APP, so in order 
            //to be able to efectively stop propagation we need to make this class to register
            //with a lower priority value; it will get called earlier, so if propagation stops the
            //ofEasyCam will not get the notified mouse events.
            ofRegisterMouseEvents(this,OF_EVENT_ORDER_AFTER_APP -1);
            #else
            ofRegisterMouseEvents(this);
            #endif
        }
        ~DraggableRect(){
            #ifdef STOP_EVENT_PROPAGATION
            ofUnregisterMouseEvents(this,OF_EVENT_ORDER_AFTER_APP -1);
            #else
            ofUnregisterMouseEvents(this);
            #endif
        }
        #ifdef STOP_EVENT_PROPAGATION
        bool mouseDragged( ofMouseEventArgs & mouse ){
        #else
        void mouseDragged( ofMouseEventArgs & mouse ){
        #endif
            
            if(bDragging){
                rect.x = mouse.x - offset.x;
                rect.y = mouse.y - offset.y;
            }
        #ifdef STOP_EVENT_PROPAGATION
            return bDragging;
        #endif
        }
        #ifdef STOP_EVENT_PROPAGATION
        bool mousePressed( ofMouseEventArgs & mouse ){
        #else
        void mousePressed( ofMouseEventArgs & mouse ){
        #endif
    
            if(rect.inside(mouse)){//notice that you can pass the mouse argument directly to the ofRectangle's inside method.
                bDragging = true;
                offset = mouse - rect.getPosition();
                return true;
            }
        #ifdef STOP_EVENT_PROPAGATION
            return bDragging;
        #endif
        }
        void mouseReleased(ofMouseEventArgs & mouse){
            bDragging  = false;
        }
        //Even when the following mouse callbacks are not being used we need to have these declared because otherwise the function ofRegisterMouseEvents will not work.
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
    
    class ofApp : public ofBaseApp{
    
    	public:
    		void draw();
    
            DraggableRect rect;
    		    
            ofEasyCam cam;
    };
    



ofApp.cpp


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
        msg << "Clic and drag elsewhere to move the camera, hence rotate the magenta box";
        ofDrawBitmapStringHighlight(msg.str(), 20,20);
    
    }
    


### static ofEvents

You can have static ofEvents. This can become useful in several situations.
Take a look at the example in `ofFolder/examples/events/simpleEventsExample`.

[TODO: explain this thoroughly].




## THE RULE OF 3 (or 5)
In C++ there is a rule, called the rule of 3, which became the rule of 5 in C++11.
Read about it [here](https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)).
In short it says that if you add to a class any of the following you must add them all. 

*  destructor `~ClassName(){...}`
*  copy constructor `ClassName(ClassName& other)`
*  copy assignment operator `ClassName& operator=(ClassName& other)`
*  move assignment operator (C++11 only) `ClassName& operator=(ClassName&& other)`
*  move constructor (C++11 only) `ClassName(ClassName&& other)`

So, in order to have exception-safe code you must follow this rule.

When listening to any `ofEvent` in your custom classes, it is a good idea to stop listening to these when the class instance is destroyed. This means that you will declare the class destructor, hence you'll have to follow this rule. 

Even if you don't follow this rule, your code will compile with no errors or warnings and it will probably work well, but it will not be exception-safe, and you run the risk of having several undesired side-effects, which can be really hard to debug.



## The new way
As of openFrameworks v0.10 onwards, there is a new way for registering listeners. Yet the old way, the one we've seen throughout this chapter, still works.
By using this new way you can avoid having to declare the class destructor, hence no extra stuff to comply with the rule of 3 (or 5). 

The key ingredient of this new way is the addition of the `ofEventListener` class.
In order to register to listen ofEvents in this new way you need to have an `ofEventListener` object for each callback you might want to link with an `ofEvent`.

Using this new way the code in [Adding and removing listeners](#adding-and-removing-listeners)

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
	
So, to add a listener you have to call the `newListener()` method of the event you want to listen to and asign it to the `ofEventListener` object, like in the constructor of class `A`.

To unregister a listener just call its `unsubscribe()` method. Look at class `A`'s `unregisterListener` method.



### ofEvents and Lambda functions
This new way for listening to events allows you to use lambda functions as the callback.
If you don't know about Lambda functions read ofBook's chapter about C++11. Even though when just a section of it is about Lambda functions read the whole chapter as it describes several super useful features. 
Lambda functions are annonymous functions. This means that you dont have to declare these previously in order to use, you are able to assign these dynamically and pass as  function argument.

It is possible to use a lambda function as the callback of any event. 

For example, the previous code using a lambda function as the callback would be:

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

