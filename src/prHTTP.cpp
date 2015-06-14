/**
 * prHTTP.cpp
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 08-05-15
 */

#include "prHTTP.h"

using namespace Poco::Net;
using namespace Poco;

/**
 * Setup the http session
 *
 * @param string baseurl
 */
void prHTTP::setup(string base)
{
    uri = base;
    
    session.setHost(uri.getHost());
    session.setPort(uri.getPort());
    session.setKeepAlive(true);
    
    ofLogNotice("prHTTP") << "Started HTTP session with: " << uri.getHost() << ":" << uri.getPort();
}


/**
 * Handles the GET requests
 *
 * @param string API path
 * @return string json response
 */
string prHTTP::GET(string path)
{
    string data;
    try {
        if (path.empty()) path = "/";
        
        HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        session.sendRequest(request);
        
        HTTPResponse response;
        ofLogNotice("prHTTP") << response.getStatus() << " " << response.getReason();
        
        istream &is = session.receiveResponse(response);
        
        // TODO: check efficientie
        istreambuf_iterator<char> eos;
        string responseBuff(istreambuf_iterator<char>(is), eos);
        data = responseBuff;
    }
    
    catch (Exception &ex) {
        ofLogError("prHTTP") << "Error in GET: " << ex.displayText();
        data = "";
    }
    
    return data;
}


/**
 * Post data to the api
 *
 * @param string path to the api endpoint
 * @param string data the data to send to the api in x-www-form-urlencoded format
 */
void prHTTP::POST(string path, string data)
{
    try {
        HTTPRequest request(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
        request.setContentType("application/x-www-form-urlencoded");
        request.setKeepAlive(true);
        request.setContentLength(data.length());
        
        string encodedData;
        uri.encode(data, ":", encodedData);
     
        ostream& myOStream = session.sendRequest(request);
        myOStream << data;

        request.write(cout);
        
        HTTPResponse response;
        istream& iStream = session.receiveResponse(response);
        
        ofLogNotice("prHTTP") << iStream.rdbuf();
    }
    
    catch (Exception &ex) {
        ofLogError("prHTTP") << "Error in POST: " << ex.displayText();
    }
}