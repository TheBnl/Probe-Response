/**
 * prProbeRequestPing.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created by Bram on 18-06-15.
 */

#ifndef _PR_PROBE_REQUEST_PING
#define _PR_PROBE_REQUEST_PING

#pragma once
#include "ofMain.h"
#include "ofxAnimatableFloat.h"

class prProbeRequestPing {
    
public:
    prProbeRequestPing();
    void setup(float signal, float _minSig, float _maxSig, float _minHue, float _maxHue);
    void update();
    void draw();
    bool finished;
    
protected:
    void calculatePosition();
    float calculateScaleFromSignal(float signal, float maxSig, float minSig);
    float hueFromSignalStrength(float signal, float minSig, float maxSig, float minHue, float maxHue);
    ofColor color;
    ofxAnimatableFloat opacity;
    ofxAnimatableFloat scale;
    float scaleFromSignal;
    float angle;
    float x;
    float y;
    
};

#endif