/**
 * prHostNames.cpp
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 04-06-15
 */
#include "prHostNames.h"
#include "compairFirst.h"


/**
 * Setup the Host names with data from the database
 * the database can be found at localhost:5000/api/victimswithhostnames
 */
void prHostNames::setup(string knownHostNames)
{
    if (!knownHostNames.empty()) {
        json.parse(knownHostNames);
        
        for(unsigned int i = 0; i < json.size(); i++)
        {
            std::pair <string,string> macHostPair;
            string hostname = json[i]["hostName"].asString();
            string macAddress = json[i]["macAddress"].asString();
            macHostPair = std::make_pair(macAddress, hostname);
            hostNames.push_back( macHostPair );
        }
        
        ofLogNotice("prHostNames") << "Got " << hostNames.size() << " hostNames";
        
        for(int i = 0; i < hostNames.size(); i++)
        {
            std::pair <string,string> macHostPair = hostNames[i];
            ofLogNotice("prHostNames") << "MAC: " << macHostPair.first << " HOST: " << macHostPair.second;
        }
    }
    
    else {
        ofLogNotice("prHostNames") << "Started empty";
    }
    
}


/**
 * Translate a mac adress into a hostname or return Someone
 * @param string macAddress
 */
string prHostNames::resolveName(string macAddress)
{
    string resolvedHostName;
    vector< pair <string, string> >::iterator macHostPair = std::find_if( hostNames.begin(), hostNames.end(), CompareFirst(macAddress) );
    if (macHostPair != hostNames.end()) {
        resolvedHostName = macHostPair->second;
    }
    return resolvedHostName.empty() ? "Someone" : resolvedHostName;
}


/**
 * Save a hostname in the local lookup vector
 * @param string macAddress
 * @param string hostName
 */
string prHostNames::saveHostName(string macAddress, string hostName)
{
    ofLogNotice("prHostNames") << "Saving Hostname: " << hostName << " and mac addr: " << macAddress;
    std::pair <string,string> macHostPair;
    macHostPair = std::make_pair(macAddress, hostName);
    hostNames.push_back( macHostPair );
}
