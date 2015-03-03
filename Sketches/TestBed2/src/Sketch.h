/*
 * PLAYGROUND IN SYNC WITH:
 
 * 1) develop BRANCH IN https://github.com/new-chronotext-toolkit
 *    OSX: COMMIT e128512
 *    IOS: COMMIT e128512
 *    ANDROID: COMMIT e128512
 *
 * 2) Target1: master BRANCH IN https://github.com/arielm/FMOD
 *
 * 3) Target2: master BRANCH IN https://github.com/arielm/POCO
 *
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
    
    bool keyDown(const ci::app::KeyEvent &keyEvent) final;
    bool keyUp(const ci::app::KeyEvent &keyEvent) final;
    
protected:
    std::vector<std::shared_ptr<TestingBase>> frameTests;
    
    template<class TestingBase>
    static void executeTest(bool proceed = true, bool force = false);
    
    template<class TestingBase>
    void addFrameTest(bool proceed = true);
};
