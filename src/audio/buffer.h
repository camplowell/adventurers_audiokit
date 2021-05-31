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

/**
 * @brief An abstract class for buffers of various types
 * 
 * @tparam T The datatype the buffer contains
 */
template <typename T>
struct Buffer {
    virtual T& get(int index) = 0;
    virtual void set(int index, T value) = 0;
    virtual void resize(int) = 0;
    virtual int size() = 0;
    virtual void clear() = 0;

    virtual typename std::vector<T>::iterator begin() = 0;
    virtual typename std::vector<T>::iterator end() = 0;
};

/** A buffer filled with floats */
struct AudioBuffer: Buffer<float> {
    std::vector<float> values;
    static inline AudioBuffer create() {
        AudioBuffer buffer;
        return buffer;
    }

    /** Get the value at a given index */
    inline float& get(int index) override {
        return values.at(index);
    }

    /** Set the value at a given index */
    inline void set(int index, float value) override {
        values[index] = value;
    }

    /** Resize the audio buffer to contain the given number of samples.
     * Not realtime or thread safe.
     */
    inline void resize(int size) override {
        values.resize(size);
    }

    /** Returns the number of samples in the buffer */
    inline int size() override {
        return values.size();
    }
    /** Fills the buffer with 0's without changing its size */
    inline void clear() override {
        std::fill(values.begin(), values.end(), 0.0f);
    }

    /** Used for iteration over the data in the buffer */
    inline std::vector<float>::iterator begin() override {
        return values.begin();
    }

    /** Used for iteration over the data in the buffer */
    inline std::vector<float>::iterator end() override {
        return values.end();
    }

    float& operator[](int index) {
        return values[index];
    }
};

/** A dummy audio buffer that always returns 0 */
/*
struct DummyAudioBuffer: Buffer<float> {
    static const DummyAudioBuffer instance;
    
    inline float get(int index) override { return 0.0f; }
    void set(int index, float value) override {}
    void resize(int size) override {}
    inline int size() override { return 0; }
    void clear() override {}

    std::vector<float>::iterator begin() override {return dummyVect.begin(); }
    std::vector<float>::iterator end() override {return dummyVect.end(); }

    private:
        static std::vector<float> dummyVect;
};
*/

/** A multi-channel buffer for positional audio */
struct PositionalChannelBuffer {
    protected:
        AudioBuffer values = AudioBuffer::create();
        std::vector<LinearSmoothedVec3> positions;
        std::vector<LinearSmoothedFloat> radii;
        int frms;
        int chnls;
    public:
        /** Gets the sample at the given frame and channel */
        inline float get(int frame, int channel = 0) {
            return values.get(index(frame, channel));
        }
        /** Sets the sample value at the given frame and channel */
        inline void set(int frame, int channel, float value) {
            values.set(index(frame, channel), value);
        }

        /** Get the 3D location of a given channel */
        LinearSmoothedVec3& getLoc(int channel) {
            KIT_ASSERT(channel >= 0 && channel < chnls, "Channel out of range");
            return positions.at(channel);
        }
        /** Get the apparent radius of the channel */
        LinearSmoothedFloat& getRadius(int channel) {
            KIT_ASSERT(channel >= 0 && channel < chnls, "Channel out of range");
            return radii.at(channel);
        }

        /** Resize the buffer to contain the given number of frames and channels.
         * Not realtime or thread safe.
        */
        void resize(int numFrames, int numChannels = 1) {
            this->frms = numFrames;
            this->chnls = numChannels;
            values.resize(numFrames * numChannels);
            printf("Resized buffer: %d frames on %d channels\n", numFrames, numChannels);
        }

        /** Fill the buffer with 0's without changing its size. */
        void clear() {
            values.clear();
        }

        /** The number of channels this buffer spans. */
        const int& numChannels = chnls;
        /** The number of frames this buffer contains. */
        const int& numFrames = frms;

    protected:
        /** Calculate an index from a frame and channel number. */
        int index(int frame, int channel) {
            KIT_ASSERT(frame >= 0 && frame < frms, "Frame out of range" + std::to_string(frame));
            KIT_ASSERT(channel >= 0 && channel < chnls, "Channel out of range" + std::to_string(channel));
            return frame + (channel * frms);
        }
};