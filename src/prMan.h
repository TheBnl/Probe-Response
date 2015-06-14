/**
 * prMan.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 03-05-15
 */

#ifndef _PR_MAN
#define _PR_MAN

#pragma once
#include "ofMain.h"

class prMan {
    
public:
    prMan();
    void setup();
    void draw();
    
private:
    ofSpherePrimitive tempHead;
    
};

#endif
