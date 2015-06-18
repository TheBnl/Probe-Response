/**
 * prSingleProbeRequest.cpp
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 18-06-15
 */

#include "prSingleProbeRequest.h"

prSingleProbeRequest::prSingleProbeRequest(void)
{
    ofLogNotice("prSingleProbeRequest") << "Single ProbeRequest is being created";
}


void prSingleProbeRequest::setup(string _name, float _signal, float _minSig, float _maxSig, float _minHue, float _maxHue)
{
    name = _name;
    finished = false;
    signal = _signal;
    minSig = _minSig;
    maxSig = _maxSig;
    minHue = _minHue;
    maxHue = _maxHue;
    
    fontOpacity.reset( 255 );
    fontOpacity.setRepeatType( PLAY_ONCE );
    fontOpacity.setCurve( LATE_EASE_IN_EASE_OUT );
    fontOpacity.setDuration(5);
    fontOpacity.animateTo(0);

    hue = hueFromSignalStrength(signal);
    color.setHsb(hue, 255, 255);
    
    int fontSize = 32;
    if (signal > 80){
        fontSize = 10; // 300
    } else if (signal < 80 && signal > 40) {
        fontSize = 28; // 150
    } else if (signal < 40 && signal > 20) {
        fontSize = 37; // 25
    } else if (signal < 20 && signal > 0) {
        fontSize = 42;
    }
    
    unica.loadFont("NeueHaasUnicaPro-Regular.ttf", fontSize);
    
    angle = ofRandom(0, 360);
    calculatePosition();
}

void prSingleProbeRequest::update(float _minSig, float _maxSig, float _minHue, float _maxHue)
{
    minSig = _minSig;
    maxSig = _maxSig;
    minHue = _minHue;
    maxHue = _maxHue;
    
    angle = angle >= 360 ? 0 + 0.02: angle + 0.02;
    calculatePosition();
    
    fontOpacity.update( 1.0f/60.0f );
    if (fontOpacity.hasFinishedAnimating()) {
        finished = true;
    }
}

void prSingleProbeRequest::draw()
{
    ofSetColor(color, fontOpacity.val());
    unica.drawString(name, x, y);
}


void prSingleProbeRequest::calculatePosition()
{
    float radian = angle * (pi/180);
    
    float scale = calculateScaleFromSignal(signal);
    
    float maxWidth = (ofGetWindowWidth()/2)*scale;
    float maxHeight = (ofGetWindowHeight()/2)*scale;
    
    x = maxWidth*cos(radian) + ofGetWindowWidth()/2;
    y = maxHeight*sin(radian) + ofGetWindowHeight()/2;
}


float prSingleProbeRequest::calculateScaleFromSignal(float signal)
{
    return ( 1 / (maxSig - minSig) ) * (signal - 30);
}


/**
 * Returns an hue value based on the given signal strenght.
 * @author Bram de Leeuw
 *
 * @param float signal
 * @return float hue
 */
float prSingleProbeRequest::hueFromSignalStrength(float signal)
{
    return (((maxHue - minHue) / (maxSig - minSig)) * (signal - minSig)) + minHue;
}
