/*
 * PLAYGROUND IN SYNC WITH:
 
 * 1) develop BRANCH IN https://github.com/new-chronotext-toolkit
 *    OSX: COMMIT e226461
 *    IOS: COMMIT e226461
 *    ANDROID: COMMIT e226461
 *
 * 2) master BRANCH IN https://github.com/arielm/POCO
 *
 * 3) master BRANCH IN https://github.com/arielm/FMOD
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
    
    void addTouch(int index, float x, float y) final;
    void updateTouch(int index, float x, float y) final;
    void removeTouch(int index, float x, float y) final;
    
protected:
    std::shared_ptr<TestingBase> frameTest;
};
