//
//  httpRequestHandler.cpp
//  LeapCast
//
//  Created by Jia Chen on 1/18/14.
//  Copyright (c) 2014 Jia Chen. All rights reserved.
//

#include "httpRequestHandler.h"
#include <locale>
#include "mongoose.h"

void httpRequestHandler::setController(Leap::Controller *controller)
{
    pController=controller;
    if(!pController->isConnected())
    {
        cout<<"controller is not connected, please check....."<<endl;
    }
    pController->enableGesture(Gesture::TYPE_CIRCLE);
    pController->enableGesture(Gesture::TYPE_KEY_TAP);
    pController->enableGesture(Gesture::TYPE_SCREEN_TAP);
    pController->enableGesture(Gesture::TYPE_SWIPE);
    pController->setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);
}

string httpRequestHandler::getDeviceListString()
{
    DeviceList l=pController->devices();
    string res;
    if(l.count()==0)
    {
        return "";
    }
    if(l.count()==1)
    {
        Device d=l[0];
        return getDeviceString(d);
        
    }
    for (int i=0;i<l.count(); i++) {
        Device d=l[i];
        res.append(getDeviceString(d));
        if(i!=l.count()-1)
        {
            res.append(",");
        }
    }
    return res;
}
string httpRequestHandler::getDeviceString(Device& d)
{
    if(d.isValid())
    {
        char res[5000];
        sprintf(res,
                "{ \n desc: \"%s\" ,\n horizViewAngle:%f ,\n vertViewAngle:%f ,\n isValid: %d ,\n range: %f \n}",
                d.toString().c_str(),
                d.horizontalViewAngle(),
                d.verticalViewAngle(),
                d.isValid(),
                d.range()
                );
        return string(res);
    }
    return "{}";
}
void httpRequestHandler::handleRequest (mg_connection *conn)
{
    
    string uri=conn->uri;
    mg_send_header(conn, "Content-Typ", "application/json");
    if(pController==NULL)
    {
        mg_send_status(conn, 500);

        mg_printf_data(conn, "{\n error: \"controller not attached\" \n }");
        return;
    }
    Frame lastFrame=pController->frame();
    if (uri == "/controller")
    {

        mg_printf_data(conn,
                       "{\n devices: [%s],\n hasFocus:%d ,\n isConnected:%d ,\n policyFlags: POLICY_BACKGROUND_FRAMES \n }",
                       getDeviceListString().c_str(),
                       pController->hasFocus(),
                       pController->isConnected()
                       );
        return;
    }
    else if( uri=="/devices")
    {
        if(pController->devices().count()==0)
        {
            mg_send_status(conn, 500);
            mg_printf_data(conn, "{\n error: \"No device found\" \n }");
            return;
        }

        mg_printf_data(conn,
                       getDeviceListString().c_str()
                       );
                return;
 
    }
    else if( uri=="/frame")
       {
           if(!lastFrame.isValid())
           {
               mg_send_status(conn, 500);
               mg_printf_data(conn, "{\n error: \"No valid frame found\" \n }");
               return;
           }
           mg_printf_data(conn,
                          getFrameString(lastFrame).c_str()
                          );
                   return;
       }
    else if (uri == "/gestures")
    {
        if(!lastFrame.isValid())
        {
            mg_send_status(conn, 500);
            mg_printf_data(conn, "{\n error: \"No valid frame found\" \n }");
            return;
        }

        mg_printf_data(conn,
                       getGestureListString(lastFrame.gestures()).c_str()
                       );
                return;
    }
    else if( uri=="/fingers")
    {
        if(!lastFrame.isValid())
        {
            mg_send_status(conn, 500);
            mg_printf_data(conn, "{\n error: \"No valid frame found\" \n }");
            return;
        }

        mg_printf_data(conn,
                       getFingerListString(lastFrame.fingers()).c_str()
                       );
                return;
    }
    else if( uri=="/hands")
    {
        if(!lastFrame.isValid())
        {
            mg_send_status(conn, 500);
            mg_printf_data(conn, "{\n error: \"No valid frame found\" \n }");
            return;
        }
        const char* t=       getHandListString(lastFrame.hands()).c_str();
        mg_printf_data(conn,
                t
                       );
                return;
    }
    else if( uri=="/pointables")
    {
        if(!lastFrame.isValid())
        {
            mg_send_status(conn, 500);
            mg_printf_data(conn, "{\n error: \"No valid frame found\" \n }");
            return;
        }
        mg_printf_data(conn,
                       getPointableListString(lastFrame.pointables()).c_str()
                       );
                return;
    }
    else if( uri=="/tools")
    {
        if(!lastFrame.isValid())
        {
            mg_send_status(conn, 500);
            mg_printf_data(conn, "{\n error: \"No valid frame found\" \n }");
            return;
        }
        mg_printf_data(conn,
                      getToolListString(lastFrame.tools()).c_str()
                       );
                return;
    }
    else{
        mg_send_status(conn, 400);
        mg_printf_data(conn, "{\n error: \"Request resource not found\" \n }");
    }
           return;
}
string httpRequestHandler::getHandListString(const HandList& l)
{
    string res;
    if(l.count()==0)
    {
    return "{}";
    }
    if(l.count()==1)
    {
        Hand h=l[0];
        return getHandString(h);
        
    }
    for (int i=0;i<l.count(); i++) {
        Hand h=l[i];
        res.append(getHandString(h));
        if(i!=l.count()-1)
        {
            res.append(",");
        }
    }
    return res;
}




string httpRequestHandler::getVectorString(const Vector& v)
{
    if(v.isValid())
    {
        char res[5000];
        sprintf(res,
                "{ \n   desc:\"%s\" ,\n  magnitude:%f ,\n   magnitudeSquared:%f ,\n   pitch:%f ,\n   roll:%f ,\n   yaw:%f \n}",
                v.toString().c_str(),
                v.magnitude(),
                v.magnitudeSquared(),
                v.pitch(),
                v.roll(),
                v.yaw()
                );
        return string(res);
    }
    return "{}";
}

string httpRequestHandler::getHandString(Hand& h)
{
    if(h.isValid())
    {
        char res[5000];
        sprintf(res,
                "{ \n desc: \"%s\", \n id:%d, \n direction: %s , \n FingerList:%s , \n palmNormal:%s , \n palmPosition:%s , \n palmVelocity:%s , \n pointableList:%s , \n sphereCenter:%s , \n sphereRadius;%f, \n stablizedPalmPosition:%s , \n timeVisible:%f, \n toolList:%s , \n }",
                h.toString().c_str(),
                h.id(),
                getVectorString(h.direction()).c_str(),
                getFingerListString(h.fingers()).c_str(),
                getVectorString(h.palmNormal()).c_str(),
                getVectorString(h.palmPosition()).c_str(),
                getVectorString(h.palmVelocity()).c_str(),
                getPointableListString(h.pointables()).c_str(),
                getVectorString(h.sphereCenter()).c_str(),
                h.sphereRadius(),
                getVectorString(h.stabilizedPalmPosition()).c_str(),
                h.timeVisible(),
                getToolListString(h.tools()).c_str()
                );
        return string(res);
    }
    return "{}";

}

string httpRequestHandler::getFingerListString(const FingerList& l)
{
    string res;
    if(l.count()==0)
    {
        return "{}";
    }
    if(l.count()==1)
    {
        Finger h=l[0];
        return getFingerString(h);
        
    }
    for (int i=0;i<l.count(); i++) {
        Finger h=l[i];
        res.append(getFingerString(h));
        if(i!=l.count()-1)
        {
            res.append(",");
        }
    }
    return res;
}

string httpRequestHandler::getFingerString(Finger& p)
{
    char res[5000];
    sprintf(res,
            "{ \n desc: \"%s\" \n }",
            p.toString().c_str());
    return string(res);
}

string httpRequestHandler::getPointableListString(const    PointableList& l)
{

    string res;
    if(l.count()==0)
    {
        return "{}";
    }
    if(l.count()==1)
    {
        Pointable h=l[0];
        return getPointableString(h);
        
    }
    for (int i=0;i<l.count(); i++) {
        Pointable h=l[i];
        res.append(getPointableString(h));
        if(i!=l.count()-1)
        {
            res.append(",");
        }
    }
    return res;
}

const char* convertTouchZoneToString(Pointable::Zone z)
{
    switch(z)
    {
        case Leap::Pointable::ZONE_NONE:
            return "ZONE_NONE";
        case Leap::Pointable::ZONE_TOUCHING:
            return "ZONE_TOUCHING";
        case Leap::Pointable::ZONE_HOVERING:
            return "ZONE_HOVERING";
    }
    return "ZONE_NONE";
}
string httpRequestHandler::getPointableString(Pointable& p)
{
    
    if(p.isValid())
    {
        char res[5000];
        sprintf(res,
                "{ \n desc: \"%s\", \n id:%d, \n direction: %s , \n isFinger:%d , \n isTool:%d, , \n length:%f,\n stablizedTipPosition:%s, \n timeVisible:%f, \n tipPosition:%s , \n tipVelocity:%s , \n touchDistance:%f , \n touchZone:%s , \n  width:%f  \n }",
                p.toString().c_str(),
                p.id(),
                getVectorString( p.direction()).c_str(),
                p.isFinger(),
                p.isTool(),
                p.length(),
                getVectorString(p.stabilizedTipPosition()).c_str(),
                p.timeVisible(),
                getVectorString(p.tipPosition()).c_str(),
                getVectorString(p.tipVelocity()).c_str(),
                p.touchDistance(),
                convertTouchZoneToString( p.touchZone()),
                p.width()
                );
        return string(res);
    }
    return "{}";
}

string httpRequestHandler::getToolListString(const ToolList& l)
{

    string res;
    if(l.count()==0)
    {
        return "{}";
    }
    if(l.count()==1)
    {
        Tool h=l[0];
        return getToolString(h);
        
    }
    for (int i=0;i<l.count(); i++) {
        Tool h=l[i];
        res.append(getToolString(h));
        if(i!=l.count()-1)
        {
            res.append(",");
        }
    }
    return res;
    
}

string httpRequestHandler::getToolString(Tool& p)
{
    char res[5000];
    sprintf(res,
            "{ \n desc: \"%s\" \n }",
            p.toString().c_str());
    return string(res);
}
string httpRequestHandler::getGestureListString(const GestureList& l)
{
    string res;
    if(l.count()==0)
    {
         return "{}";
    }
    if(l.count()==1)
    {
        Gesture h=l[0];
        return getGestureString(h);
        
    }
    for (int i=0;i<l.count(); i++) {
        Gesture h=l[i];
        res.append(getGestureString(h));
        if(i!=l.count()-1)
        {
            res.append(",");
        }
    }
    return res;
    
}

const char* convertStateToString(Gesture::State s)
{
    switch (s) {
        case Leap::Gesture::STATE_INVALID:
            return "STATE_INVALID";
        case Leap::Gesture::STATE_START:
            return "STATE_START";
        case Leap::Gesture::STATE_UPDATE:
            return "STATE_UPDATE";
        case Leap::Gesture::STATE_STOP:
            return "STATE_STOP";
    }
    return "STATE_INVALID";
}
const char* convertTypeToString(Gesture::Type t)
{
    switch (t) {
    case Leap::Gesture::TYPE_INVALID:
        return "TYPE_INVALID";
    case Leap::Gesture::TYPE_SWIPE:
        return "TYPE_SWIPE";
    case Leap::Gesture::TYPE_CIRCLE:
        return "TYPE_CIRCLE";
    case Leap::Gesture::TYPE_SCREEN_TAP:
        return "TYPE_SCREEN_TAP";
    case Leap::Gesture::TYPE_KEY_TAP:
        return "TYPE_KEY_TAP";
    }

    return "TYPE_INVALID";
}
string httpRequestHandler::getGestureString(Gesture& p)
{
    if(p.isValid())
    {
        char res[5000];
        sprintf(res,
                "{ \n desc:\"%s\", \n id:%d, \n duration: %lld, \n durationSections: %f, \n hands:%s, \n pointables:%s, \n state:%s, \n type:%s  \n }",
                p.toString().c_str(),
                p.id(),
                p.duration(),
                p.durationSeconds(),
                getHandListString(p.hands()).c_str(),
                getPointableListString(p.pointables()).c_str(),
                convertStateToString(p.state()),
                convertTypeToString(p.type())
                                );
        return string(res).c_str();
    }
    return "{}";
}

string httpRequestHandler::getFrameString(Frame lastFrame)
{
    if(lastFrame.isValid())
    {
        char res[5000];
        sprintf(res,
                "{ \n desc:\"%s\", \n id:%lld, \n time_stamp: %lld, \n hands:%s, \n pointables:%s, \n fingers:%s, \n tools:%s,  \n gestures:%s, \n}",
                lastFrame.toString().c_str(),
                lastFrame.id(),
                lastFrame.timestamp(),
                getHandListString(lastFrame.hands()).c_str(),
                getPointableListString(lastFrame.pointables()).c_str(),
                getFingerListString(lastFrame.fingers()).c_str(),
                getToolListString(lastFrame.tools()).c_str(),
                getGestureListString(lastFrame.gestures()).c_str()
                );
        return string(res);
    }
    return "{}";
}











