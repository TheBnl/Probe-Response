/**
 * prHostNameIn.cpp
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 11-06-15
 */

#include "prHostNameIn.h"

#include "Poco/RegularExpression.h"
using Poco::RegularExpression;


void prHostNameIn::setup(int port)
{
    ofLogNotice("prHostNameIn") << "Listening on port: " << port;
    connection.Create();
    connection.Bind( port );
    connection.SetNonBlocking( true );
    regex ="\\d+ (\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}:\\S{2}) \\d+.\\d+.\\d+.\\d+ (\\S+) \\S+";
}


void prHostNameIn::update()
{
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
            
            ofLogNotice("prHostNameIn") << "Found: " << data[1] << " with hostname: " << data[2];
        }
        
        message.clear();
    }
}

string prHostNameIn::getHostName() {
    return hostName;
}

string prHostNameIn::getMacAddress() {
    return macAddress;
}