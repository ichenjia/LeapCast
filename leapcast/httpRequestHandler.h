//
//  httpRequestHandler.h
//  LeapCast
//
//  Created by Jia Chen on 1/18/14.
//  Copyright (c) 2014 Jia Chen. All rights reserved.
//

#ifndef __LeapCast__httpRequestHandler__
#define __LeapCast__httpRequestHandler__

#include <iostream>
#include "mongoose.h"
#include <string>
#include "Leap.h"
#include "LeapMath.h"
using namespace std;
using namespace Leap;


    

enum Resources
{
    rGesture=0, // current gesture in progress or the last gesture
    rConfig,
    rController,
    rDevice,// current device info
    rFingers, // list all fingers captured currently
    rFrame, // current frame
    rHands, // list of all hands captured currently
    rListener, // info about current listener
    rPointables, // list of all current pointables captured
    rTools, // list of tools captured
    rUnknown // not supported type
};
enum Gesture_Type
{
    gCircle=0, gSwipe, gKeyTap, gScreenTap
};
    

class httpRequestHandler{

public:
    void handleRequest (mg_connection *conn);
    httpRequestHandler(){

        pController=NULL;
    };
    
    void setController(Controller* controller);

   private:
    const Leap::Controller* pController;
    string getDeviceListString();
    string getDeviceString(Device& d);

    string getHandListString(const HandList& l);

    string getPointableListString(const PointableList& l);

    string getFingerListString(const FingerList& l);

    string getToolListString(const ToolList& l);

    string getGestureListString(const GestureList& l);
    string getHandString(Hand& h);
    string getPointableString(Pointable& p);
    string getFingerString(Finger& p);
    string getToolString(Tool& p);
    string getGestureString(Gesture& p);
    string getVectorString(const Vector& v);
    string getFrameString(Frame lastFrame);

    
};

#endif /* defined(__LeapCast__httpRequestHandler__) */
