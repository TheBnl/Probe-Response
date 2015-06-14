/**
 * prHTTP.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 08-05-15
 */

#ifndef _PR_HTTP
#define _PR_HTTP

#pragma once
#include "ofMain.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"

class prHTTP {
    
public:
    void setup(string base);
    string GET(string path);
    void POST(string path, string data);
    
private:
    Poco::URI uri;
    Poco::Net::HTTPClientSession session;
};

#endif

