/**
 * prThreadedHostNameIn.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created by  on 02-06-15.
 */

#ifndef _PR_THREADED_HOSTNAME_IN
#define _PR_THREADED_HOSTNAME_IN

#pragma once
#include "ofMain.h"
#include "ofThread.h"
#include "ofxNetwork.h"
#include "Poco/RegularExpression.h"
using Poco::RegularExpression;

class prThreadedHostNameIn: public ofThread {

public:
    string message;
    string hostName;
    string macAddress;
    
    /**
     * Set up threaded Host name is
     */
    prThreadedHostNameIn() {}
    
    
    /**
     * Start the thread, and set up all needed variables
     */
    void start(int port)
    {
        cout << "HN_IN listening on port: " + ofToString(port) << endl;
        connection.Create();
        connection.Bind( port );
        connection.SetNonBlocking( true );
        
        //1388666627 24:e3:14:5a:07:76 172.16.42.166 bramdeleeuw-3 01:24:e3:14:5a:07:76
        regex ="\\d+ (\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}) \\d+.\\d+.\\d+.\\d+ (\\S+) \\S+";
        ofThread::startThread();
    }
    
    
    /**
     * Stop the thread
     */
    void stop()
    {
        ofThread::stopThread();
    }
    
    
    /**
     * Listen on the opened port to any incoming data
     */
    void threadedFunction()
    {
        while(ofThread::isThreadRunning()) {
            if(ofThread::lock()) {
                char udpMessage[100000];
                int bytesReceived = connection.Receive(udpMessage,100000);
                
                
                if( bytesReceived > 0 ){
                    message = udpMessage;
                    
                    RegularExpression::Match match;
                    RegularExpression regEx(regex);
                    int found = regEx.match(message, match);
                    
                    if (found != 0) {
                        std::vector<std::string> data;
                        regEx.split(message, 0, data);
                        
                        macAddress = data[1];
                        hostName = data[2];
                        
                        cout << "HN_IN: FOUND: " << data[2] << " with hn: " << data[4] << endl;
                    }
                    
                    message.clear();
                }
                
                ofThread::unlock();
            } else {
                ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
            }
        }
    }
    
protected:
    string regex;
    ofxUDPManager connection;
    
};

#endif