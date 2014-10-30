/*
 * PLAYGROUND IN SYNC WITH ContextRework BRANCH IN new-chronotext-toolkit
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch(void *context, void *delegate = nullptr);
    
    void setup();
    void draw();
};
