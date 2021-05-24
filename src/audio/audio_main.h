#pragma once
// Include soundio as a C library, not C++
extern "C" {
    #include "soundio/soundio.h"
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
namespace AudioModule {
    float* getFrequency();
    float* getAmplitude();

    int setup();
    void loop();
    void shutdown();
}