/**
 * prHostNameIn.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created by  on 11-06-15.
 */

#ifndef _PR_HOST_NAME_IN
#define _PR_HOST_NAME_IN

#pragma once
#include "ofMain.h"
#include "ofxNetwork.h"

class prHostNameIn {
    
public:
    void setup(int port);
    void update();
    string getHostName();
    string getMacAddress();
    
protected:
    string regex;
    string message;
    string hostName;
    string macAddress;
    bool waiting;
    ofxUDPManager connection;
    
};

#endif