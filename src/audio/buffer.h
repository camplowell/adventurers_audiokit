/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */
#pragma once
#include <vector>
#include <stdio.h>
#include <string>
#include "util/assert.h"
#include "./audio_controls.h"


/** @brief A multi-channel audio buffer */
struct ChannelBuffer {
    protected:
    std::vector<float> values;
    int n_frames;
    int n_channels;

    public:
    /** The number of channels this buffer spans. */
    const int& numChannels = n_channels;
    /** The number of frames this buffer contains. */
    const int& numFrames = n_frames;

    /** Gets the sample at the given frame and channel */
    float get(int frame, int channel = 0);
    /** Sets the sample value at the given frame and channel */
    void set(int frame, int channel, float value);

    /** Resize the buffer to contain the given number of frames and channels.
     * @warning Not realtime or thread safe.
    */
    virtual void resize(int numFrames, int numChannels = 1);

    /** Fill the buffer with 0's without changing its size. */
    virtual void clear();

    protected:
    /** Calculate an index from a frame and channel number. */
    int index(int frame, int channel);
};

/** @brief A multi-channel buffer for positional audio */
struct PositionalChannelBuffer : public ChannelBuffer {
    protected:
    std::vector<Vec3> positions;
    std::vector<float> radii;

    public:
    /** Get the 3D location of a given channel */
    Vec3& getLoc(int channel);
    /** Get the apparent radius of the channel */
    float& getRadius(int channel);

    void resize(int numFrames, int numChannels = 1) override;

    void clear() override;
};