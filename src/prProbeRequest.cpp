/**
 * prProbeRequest.cpp
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 12-06-15
 */

#include "prProbeRequest.h"

prProbeRequest::prProbeRequest(string _name, float _x, float _y, int _index)
{
    name = _name;
    finished = false;
    fontSize = 32;
    lineHeight = 42;
    
    fontOpacity.reset( 255 );
    fontOpacity.setRepeatType( PLAY_ONCE );
    fontOpacity.setCurve( LATE_EASE_IN_EASE_OUT );
    fontOpacity.setDuration(5);
    fontOpacity.animateTo(0);
    
    unica.loadFont("NeueHaasUnicaPro-Regular.ttf", fontSize);
    updatePosition(_x, _y, _index);
}

void prProbeRequest::update(float _x, float _y, int _index, ofColor _color)
{
    index = _index + 1;
    color = _color;
    updatePosition(_x, _y, _index);
    
    fontOpacity.update( 1.0f/60.0f );
    if (fontOpacity.hasFinishedAnimating()) {
        finished = true;
    }
}

void prProbeRequest::draw()
{
    ofSetColor(color, fontOpacity.val());
    string line = "Searched for "+ name;
    unica.drawString(line, x, y);
}


void prProbeRequest::updatePosition(float _x, float _y, int _index)
{
    y = _y + (lineHeight * index);
    x = _x + lineHeight;
}
