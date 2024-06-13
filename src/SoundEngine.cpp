#define MINIAUDIO_IMPLEMENTATION
#include "SoundEngine.hpp"

#include <iostream>

SoundEngine::SoundEngine() {
    ma_result const result { ma_engine_init(NULL, &_sound_engine) };
    if (result != MA_SUCCESS) {
        std::cerr << "Unable to init sound engine" << std::endl;
    }
}

SoundEngine::~SoundEngine() {
    ma_engine_uninit(&_sound_engine);
}

SoundEngine& SoundEngine::GetInstance() {
    static SoundEngine instance;
    return instance;
}

ma_engine& SoundEngine::GetEngine() {
    return GetInstance()._sound_engine;
}