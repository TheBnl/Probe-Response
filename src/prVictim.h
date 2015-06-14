/**
 * prVictim.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created by  on 12-06-15.
 */

#ifndef _PR_VICTIM
#define _PR_VICTIM

#pragma once
#include "ofMain.h"
#include "prProbeRequest.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfColor.h"

class prVictim {
    
public:
    prVictim();
    void setup(string name, float signal);
    void update(float minSig, float maxSig, float minHue, float maxHue);
    void draw();
    void newProbeRequest(string name, float signal);
    bool finished;
    
protected:
    void calculatePosition();
    float calculateScaleFromSignal(float signal);
    float hueFromSignalStrength(float signal);
    float signal;
    float angle;
    float hue;
    float maxSig;
    float minSig;
    float maxHue;
    float minHue;
    ofxAnimatableOfColor animColor;
    ofxAnimatableFloat fontOpacity;
    ofColor color;
    float x;
    float y;
    string name;
    int fontSize;
    ofTrueTypeFont unica;
    vector<prProbeRequest> probeRequests;
    
};

#endif