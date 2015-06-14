/**
 * prLight.h
 * @author Bram de Leeuw
 * @package probeResponse
 *
 * Created on 03-05-15
 */

#include "prLight.h"

prLight::prLight(float hue) {
    light.enable();
    light.setPointLight();
    
    ambientColor.setHsb(hue, 20, 255);
    diffuseColor.setHsb(hue, 255, 25);
    specularColor.setHsb(hue, 255, 255);
    
    light.setAmbientColor(ambientColor);
    light.setDiffuseColor(diffuseColor);
    light.setSpecularColor(specularColor);
}


void prLight::update(float hue)
{
    light.setPosition(ofGetWidth()*0.5, 20, -40);
    
    ambientColor.setHsb(hue, 200, 255);
    diffuseColor.setHsb(hue, 255, 125);
    specularColor.setHsb(hue, 255, 255);
    
    light.setAmbientColor(ambientColor);
    light.setDiffuseColor(diffuseColor);
    light.setSpecularColor(specularColor);
}


void prLight::draw()
{
    light.draw();
}


void prLight::enable()
{
    light.enable();
}


void prLight::disable()
{
    light.disable();
}