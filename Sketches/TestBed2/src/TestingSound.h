#pragma once

#include "TestingBase.h"

#include "chronotext/sound/SoundEngine.h"

class TestingSound : public TestingBase, public chr::SoundEngine::Listener
{
public:
    static int MAX_CHANNELS;
    
    void setup() final;
    void shutdown() final;
    void update() final;
    
    void addTouch(int index, float x, float y) final;
    
    void handleEvent(const chr::SoundEngine::Event &event) final;
    
protected:
    std::shared_ptr<chr::SoundEngine> engine;
    std::vector<chr::Effect::Ref> effects;
};
