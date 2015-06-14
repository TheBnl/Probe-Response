/**
 * prMan.cpp
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 03-05-15
 */

#include "prMan.h"

prMan::prMan() {}

void prMan::setup()
{
    tempHead.setRadius(ofGetWidth()*.15);
}


void prMan::draw()
{
    tempHead.setPosition(ofGetWidth()*0.5, ofGetHeight()*0.5, 0);
    tempHead.draw();
}