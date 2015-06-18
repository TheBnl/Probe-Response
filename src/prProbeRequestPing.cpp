/**
 * prProbeRequestPing.cpp
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 18-06-15
 */

#include "prProbeRequestPing.h"

prProbeRequestPing::prProbeRequestPing(void)
{
    ofLogNotice("prProbeRequestPing") << "ProbeRequest Ping is being created";
}


void prProbeRequestPing::setup(float signal, float minSig, float maxSig, float minHue, float maxHue)
{
    finished = false;
    scaleFromSignal = calculateScaleFromSignal(signal, minSig, maxSig);
    
    opacity.reset( 255 );
    opacity.setRepeatType( PLAY_ONCE );
    opacity.setCurve( LATE_EASE_IN_EASE_OUT );
    opacity.setDuration(1);
    opacity.animateTo(0);
    
    scale.reset(0);
    scale.setRepeatType( PLAY_ONCE );
    scale.setCurve( EASE_OUT );
    scale.setDuration(scaleFromSignal * 4);
    scale.animateTo(1 - scaleFromSignal);
    
    float hue = hueFromSignalStrength(signal, minSig, maxSig, minHue, maxHue);
    color.setHsb(hue, 255, 255);
    
    angle = ofRandom(0, 360);
    calculatePosition();
}

void prProbeRequestPing::update()
{
    angle = angle >= 360 ? 0 + 0.02: angle + 0.02;
    calculatePosition();
    
    opacity.update( 1.0f/60.0f );
    scale.update( 1.0f/60.0f );
    if (opacity.hasFinishedAnimating()) {
        finished = true;
    }
}

void prProbeRequestPing::draw()
{
    ofSetColor(color, opacity.val());
    ofNoFill();
    ofCircle(x, y, scale.val() * 100);
}


void prProbeRequestPing::calculatePosition()
{
    float radian = angle * (pi/180);
    
    float maxWidth = (ofGetWindowWidth()/2) * scaleFromSignal;
    float maxHeight = (ofGetWindowHeight()/2) * scaleFromSignal;
    
    x = maxWidth*cos(radian) + ofGetWindowWidth()/2;
    y = maxHeight*sin(radian) + ofGetWindowHeight()/2;
}


float prProbeRequestPing::calculateScaleFromSignal(float signal, float minSig, float maxSig)
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
float prProbeRequestPing::hueFromSignalStrength(float signal, float minSig, float maxSig, float minHue, float maxHue)
{
    return (((maxHue - minHue) / (maxSig - minSig)) * (signal - minSig)) + minHue;
}
