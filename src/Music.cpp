#define MINIAUDIO_IMPLEMENTATION

#include "Music.hpp"
#include <iostream>

void playMusic() {
    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return;
    }

    ma_engine_play_sound(&engine, "../../sound/Imperial_March.mp3", NULL);

    printf("Press Enter to quit...");
    getchar();

    ma_engine_uninit(&engine);

    return;
}
