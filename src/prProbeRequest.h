/**
 * prProbeRequest.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created by  on 12-06-15.
 */

#ifndef _PR_PROBE_REQUEST
#define _PR_PROBE_REQUEST

#pragma once
#include "ofMain.h"
#include "ofxAnimatableFloat.h"

class prProbeRequest {
    
public:
    prProbeRequest(string name, float x, float y, int index);
    void update(float x, float y, int index, ofColor color);
    void draw();
    void newProbeRequest();
    void updatePosition(float _x, float _y, int _index);
    bool finished;
    
protected:
    int framesLeft;
    string name;
    ofTrueTypeFont unica;
    ofColor color;
    ofxAnimatableFloat fontHue;
    ofxAnimatableFloat fontOpacity;
    float hueFromSignalStrength(float signal);
    int fontSize;
    int lineHeight;
    int index;
    float x;
    float y;
    
};

#endif