#pragma once

#include "miniaudio.h"
#include <vector>

class SoundEngine {
    public:
        static SoundEngine& GetInstance();
        static ma_engine& GetEngine();

        SoundEngine(SoundEngine const& other) = delete;
        SoundEngine& operator=(const SoundEngine &) = delete;

    private:

        SoundEngine();
        ~SoundEngine();

        ma_engine _sound_engine;

};