#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Include soundio as a C library, not C++
extern "C" {
    #include "soundio/soundio.h"
}
namespace AudioModule {
    float getFrequency();
    void setFrequency(float);
    float getAmplitude();
    void setAmplitude(float);

    int setup();
    void loop();
    void shutdown();
}