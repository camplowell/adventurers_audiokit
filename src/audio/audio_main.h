#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "./node.h"

// Include soundio as a C library, not C++
extern "C" {
    #include "soundio/soundio.h"
}
namespace AudioModule {
    float getGlobalGain();
    void setGlobalGain(float);

    void initialize(audio_graph::Node*);
    void setRoot(audio_graph::Node*);

    int setup();
    void loop();
    void shutdown();
}