/*
 * PLAYGROUND IN SYNC WITH develop BRANCH IN new-chronotext-toolkit
 *
 * OSX: COMMIT 84b2d82
 * IOS: COMMIT 84b2d82
 * ANDROID: COMMIT 84b2d82
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "TestingBase.h"

class Sketch : public chr::CinderSketch
{
public:
    void setup() final;
    void shutdown() final;
    
    void update() final;
    void draw() final;
    
protected:
    std::shared_ptr<TestingBase> frameTest;
};
