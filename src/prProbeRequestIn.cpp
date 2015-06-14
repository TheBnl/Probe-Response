/**
 * prProbeRequestIn.cpp
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 11-06-15
 */

#include "prProbeRequestIn.h"

#include "Poco/RegularExpression.h"
using Poco::RegularExpression;


void prProbeRequestIn::setup(int port)
{
    ofLogNotice("prProbeRequestIn") << "Listening on port: " << port;
    connection.Create();
    connection.Bind( port );
    connection.SetNonBlocking( true );
    regex = "(\\S+) .+ -(\\d+)\\S+ .+ BSSID:(\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}) DA:(\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}) SA:(\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}) .+ \\((.*?)\\) .+";
}


void prProbeRequestIn::update()
{
    char udpMessage[100000];
    string data;
    int bytesReceived = connection.Receive(udpMessage,100000);
    
    if( bytesReceived > 0 ){
        data = udpMessage;
        waiting = false;
    }
    

    if ( bytesReceived == -1 ) {
        if (!waiting) {
            ofLogNotice("prProbeRequestIn") << "Waiting for input...";
            waiting = true;
        }
    }

    else if (data[data.length()-1] == '\n') {
        
        RegularExpression::Match match;
        RegularExpression regEx(regex);
        int found = regEx.match(message, match);
        
        if (found != 0) {
            std::vector<std::string> data;
            regEx.split(message, 0, data);
            
            timestamp = data[1];
            signal = data[2];
            BSSID = data[3];
            DA = data[4];
            macAddress = data[5]; // SA
            SSID = data[6];
            ofLogNotice("prProbeRequestIn") << "Result: " << message;
            ofLogNotice("prProbeRequestIn") <<  SSID << ", with mac: " << BSSID << "/" << DA << " probed for by: " << macAddress << ", on: " << timestamp << ", signal: " << signal;
            message.clear();
        }
    }
    
    else {
        data[bytesReceived] = '\0';
        message.append(data);
    }
}


string prProbeRequestIn::getTimestamp() {
    return timestamp;
}

string prProbeRequestIn::getSignal() {
    return signal;
}

string prProbeRequestIn::getMacAddress() {
    return macAddress;
}

string prProbeRequestIn::getBSSID() {
    return BSSID;
}

string prProbeRequestIn::getDA() {
    return DA;
}

string prProbeRequestIn::getSSID() {
    return SSID;
}