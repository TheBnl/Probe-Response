/**
 * prLight.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 03-05-15
 */

#ifndef _PR_LIGHT
#define _PR_LIGHT

#pragma once
#include "ofMain.h"

class prLight {
    
public:
    prLight(float hue);
    void update(float hue);
    void draw();
    void enable();
    void disable();
    
private:
    ofLight light;
    ofColor ambientColor;
    ofColor diffuseColor;
    ofColor specularColor;
    
};

#endif
