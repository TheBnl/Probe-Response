/**
 * prVictim.cpp
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 11-06-15
 */

#include "prVictim.h"

prVictim::prVictim(void)
{
    ofLogNotice("prVictim") << "Victim is being created";
}

void prVictim::setup(string _name, float _signal)
{
    name = _name;
    signal = _signal;
    fontSize = 32;
    
    animColor.setColor( ofColor::black );
    animColor.setDuration( 0.3f );
    animColor.setRepeatType(PLAY_ONCE);
    animColor.setCurve(SWIFT_GOOGLE);
    
    hue = hueFromSignalStrength(signal);
    ofColor color;
    color.setHsb(hue, 255, 255);
    animColor.animateTo( color );
    
    fontOpacity.reset( 255 );
    fontOpacity.setRepeatType( PLAY_ONCE );
    fontOpacity.setCurve( LATE_EASE_IN_EASE_OUT );
    fontOpacity.setDuration(5);
    fontOpacity.animateTo(0);
    
    unica.loadFont("NeueHaasUnicaPro-Regular.ttf", fontSize);

    minSig = 0;
    maxSig = 100;
    minHue = 75;
    maxHue = 255;
    
    // only random part in whole installation!
    // TODO: find a parpameter so this does not have to be random!
    angle = ofRandom(0, 360);
    
    calculatePosition();
}


void prVictim::update(float _minSig, float _maxSig, float _minHue, float _maxHue)
{
    minSig = _minSig;
    maxSig = _maxSig;
    minHue = _minHue;
    maxHue = _maxHue;
    
    angle = angle >= 360 ? 0 + 0.02 : angle + 0.02;
    calculatePosition();
    
    fontOpacity.update( 1.0f/60.0f );
    animColor.update( 1.0f/60.0f );
    
    if (fontOpacity.hasFinishedAnimating()) {
        finished = true;
    }
    
    for (int i=0; i < probeRequests.size(); i++) {
        if (probeRequests.at(i).finished) {
            probeRequests.erase(probeRequests.begin() + i);
        } else {
            ofColor color = animColor.getCurrentColor();
            probeRequests.at(i).update(x, y, i, color);
        }
    }
}


void prVictim::draw()
{

    ofColor color = animColor.getCurrentColor();
    ofSetColor(color, fontOpacity.val());
    
    float z = 0;
    if (signal > 80){
        z = -400; // 300
    } else if (signal < 80 && signal > 40) {
        z = -250; // 150
    } else if (signal < 40 && signal > 20) {
        z = -75; // 25
    } else if (signal < 20 && signal > 0) {
        z = 0;
    }
    
    ofPushMatrix();
    ofTranslate( 0, 0, z);
    unica.drawString(name, x, y);
    for (int i=0; i < probeRequests.size(); i++) {
        probeRequests.at(i).draw();
    }
    ofPopMatrix();
    
    ofSetColor(255,255,255,255);
}


void prVictim::newProbeRequest(string _name, float _signal)
{
    signal = _signal;
    
    std::vector<prProbeRequest>::iterator it;
    it = probeRequests.begin();
    prProbeRequest probeRequest = prProbeRequest( _name, x, y, 0);
    it = probeRequests.insert ( it , probeRequest );
    fontOpacity.setPercentDone(0);
    
    hue = hueFromSignalStrength(signal);
    ofColor color;
    color.setHsb(hue, 255, 255);
    animColor.animateTo( color );
}


void prVictim::calculatePosition()
{
    float radian = angle * (pi/180);
    
    float scale = calculateScaleFromSignal(signal);
    
    float maxWidth = (ofGetWindowWidth()/2)*scale;
    float maxHeight = (ofGetWindowHeight()/2)*scale;
    
    x = maxWidth*cos(radian) + ofGetWindowWidth()/2;
    y = maxHeight*sin(radian) + ofGetWindowHeight()/2;
}


float prVictim::calculateScaleFromSignal(float signal)
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
float prVictim::hueFromSignalStrength(float signal)
{
    return (((maxHue - minHue) / (maxSig - minSig)) * (signal - minSig)) + minHue;
}
