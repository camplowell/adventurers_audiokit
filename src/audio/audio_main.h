/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "./node.h"

// Include soundio as a C library, not C++
extern "C" {
    #include "soundio/soundio.h"
}
namespace AudioModule {
    /** Get the volume of the program output */
    float getGlobalGain();
    /** Set the volume of the program output */
    void setGlobalGain(float);

    /** Initialize an audio node with the current output info */
    void initialize(audio_graph::Node*);
    /** Set the node to feed directly into program out. */
    void setRoot(audio_graph::Node*);

    /** Open program audio. */
    int setup();
    /** Call each UI frame */
    void loop();
    /** Properly close program audio. */
    void shutdown();
}