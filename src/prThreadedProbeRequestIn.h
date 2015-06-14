/**
 * prThreadedProbeRequestIn.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Voorbeeld data via TCPDump:
 * 05:53:42.576005 2889891532us tsft 1.0 Mb/s 2412 MHz 11g -69dB signal antenna 1 BSSID:Broadcast DA:Broadcast SA:c0:d9:62:8a:b1:05 (oui Unknown) Probe Request (Ziggo395A4) [1.0* 2.0* 5.5* 6.0 9.0 11.0* 12.0 18.0 Mbit][|802.11]
 *
 * Belangrijke data:
 *  1: Timestamp        = 05:53:42.576005 2889891532us
 *  2: Signaal          = -69dB
 *  3: BSSID (router)   = mac addr
 *  4: DA (router)      = mac addr
 *  5: SA (device)      = c0:d9:62:8a:b1:05
 *  6: SSID             = Ziggo395A4
 *
 * Voorbeeld data via Karma:
 * Jan 1 00:12:09 KARMA: Probe Request from cc:29:f5:cb:f8:b8 for SSID 'newlucht-students'
 * Belangrijke data:
 *  1: Timestamp        = Jan 1 00:12:09
 *  5: macAddress       = cc:29:f5:cb:f8:b8
 *  6: SSID             = newlucht-students
 *
 * Created by  on 02-06-15.
 */

#ifndef _PR_THREADED_PROBE_REQUEST_IN
#define _PR_THREADED_PROBE_REQUEST_IN

#pragma once
#include "ofMain.h"
#include "ofThread.h"
#include "ofxNetwork.h"
#include "Poco/RegularExpression.h"
using Poco::RegularExpression;

class prThreadedProbeRequestIn: public ofThread {
    
public:
    prThreadedProbeRequestIn() {}
    
    string getTimestamp() {
        string _timestamp;
        if(ofThread::lock()) {
            _timestamp = timestamp;
            ofThread::unlock();
        }
        return _timestamp;
    }
    
    string getSignal() {
        string _signal;
        if(ofThread::lock()) { _signal = signal; ofThread::unlock(); }
        return _signal;
    }
    
    string getMacAddress() {
        string _macAddress;
        if(ofThread::lock()) { _macAddress = macAddress; ofThread::unlock(); }
        return _macAddress;
    }
    
    string getBSSID() {
        string _BSSID;
        if(ofThread::lock()) { _BSSID = BSSID; ofThread::unlock(); }
        return _BSSID;
    }
    
    string getDA() {
        string _DA;
        if(ofThread::lock()) { _DA = DA; ofThread::unlock(); }
        return _DA;
    }
    
    string getSSID() {
        string _SSID;
        if(ofThread::lock()) {
            _SSID = SSID;
            ofThread::unlock();
        }
        return _SSID;
    }
    
    
    /**
     * Start the thread, and set up all needed variables
     */
    void start(int port)
    {
        cout << "PR_IN listening on port: " + ofToString(port) << endl;
        connection.Create();
        connection.Bind( port );
        connection.SetNonBlocking( true );
        regex = "(.*?) (.*?) (.*?) (.*? .*?) (.*? .*?) (.*?) (.*?) (.*?) (.*? .*?) (BSSID:.*?) (DA:.*?) (SA:.*?) (.*? .*?) (.*? .*?) \\((.*?)\\) \\[(.*?)\\]";
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
                string data;
                int bytesReceived = connection.Receive(udpMessage,100000);
                
                //cout << "PR_IN bytes: " + ofToString(bytesReceived) << endl;
                
                /**
                 * Recieved some bytes, store these in a temporary string
                 */
                if( bytesReceived > 0 ){
                    data = udpMessage;
                    waiting = false;
                    //cout << "PR_IN data: " + data << endl;
                }
                
                /**
                 * Bytes recieved equals -1
                 * TODO: Check out what this means...
                 */
                if ( bytesReceived == -1 ) {
                    if (!waiting) {
                        cout << "PR_IN waiting for input.. " << bytesReceived << endl;
                        waiting = true;
                    }
                    // hmn?
                    //cout << "PR_IN noting found..: " << bytesReceived << endl;
                }

                /**
                 * The last caracter equals '\n' this means the message has ended and we can pocess further!
                 */
                else if (data[data.length()-1] == '\n') {

                    RegularExpression::Match match;
                    RegularExpression regEx(regex);
                    int found = regEx.match(message, match);

                    if (found != 0) {
                        std::vector<std::string> data;
                        regEx.split(message, 0, data);
                        
                        timestamp = data[1];
                        signal = data[7];
                        BSSID = data[10];
                        DA = data[11];
                        macAddress = data[12]; // SA
                        SSID = data[15];
                        
                        // trim the - & dB
                        signal.erase((signal.length() - 2), signal.length());
                        signal.erase(0, 1);
                        
                        // trim the SA:
                        macAddress.erase(0, 3);

                        cout << "PR_IN: "+ SSID +", with mac: "+ BSSID +"/"+ DA +" probed for by: "+ macAddress +", on: "+ timestamp +", signal: "+ signal << endl;
                        message.clear();
                    }
                }

                else {
                    data[bytesReceived] = '\0';
                    message.append(data);
                    cout << "PR_IN APPEND: " + data + " RESULT: " + message << endl;
                }
                
                
                ofThread::unlock();
                
            } else {
                ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
            }
            
        }
    }
    
    
protected:
    string message;
    string timestamp;
    string signal;
    string macAddress;
    string BSSID;
    string DA;
    string SSID;
    string regex;
    ofxUDPManager connection;
    bool waiting;
    
};

#endif