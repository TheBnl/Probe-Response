/**
 * prHostNames.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 04-06-15
 */

#ifndef _PR_HOST_NAMES
#define _PR_HOST_NAMES

#pragma once
#include "ofMain.h"
#include "ofxJSON.h"

class prHostNames {
    
public:
    void setup(string knownHostNames);
    string resolveName(string macAddress);
    string saveHostName(string macAddress, string hostName);
    string macAddress;
    
private:
    std::vector< std::pair <string,string> > hostNames;
    ofxJSONElement json;
    
};

#endif