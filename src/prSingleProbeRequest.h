/**
 * prSingleProbeRequest.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created by Bram on 18-06-15.
 */

#ifndef _PR_SINGLE_PROBE_REQUEST
#define _PR_SINGLE_PROBE_REQUEST

#pragma once
#include "ofMain.h"
#include "ofxAnimatableFloat.h"

class prSingleProbeRequest {
    
public:
    prSingleProbeRequest();
    void setup(string name, float signal, float _minSig, float _maxSig, float _minHue, float _maxHue);
    void update(float minSig, float maxSig, float minHue, float maxHue);
    void draw();
    bool finished;
    
protected:
    void calculatePosition();
    float calculateScaleFromSignal(float signal);
    float hueFromSignalStrength(float signal);
    string name;
    ofColor color;
    ofTrueTypeFont unica;
    ofxAnimatableFloat fontOpacity;
    float maxSig;
    float minSig;
    float maxHue;
    float minHue;
    float hue;
    float signal;
    float angle;
    float x;
    float y;
    
};

#endif