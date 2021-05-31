#pragma once
#include <vector>
#include <stdio.h>
#include <string>
#include "util/assert.h"
#include "./audio_controls.h"

template <typename T>
struct Buffer {
    virtual T get(int index) = 0;
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

    inline float get(int index) override {
        return values.at(index);
    }

    inline void set(int index, float value) override {
        values[index] = value;
    }

    inline void resize(int size) override {
        values.resize(size);
    }

    inline int size() override {
        return values.size();
    }

    inline void clear() override {
        std::fill(values.begin(), values.end(), 0.0f);
    }

    inline std::vector<float>::iterator begin() override {
        return values.begin();
    }

    inline std::vector<float>::iterator end() override {
        return values.end();
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
struct PositionalChannelBuffer {
    protected:
        AudioBuffer values = AudioBuffer::create();
        std::vector<LinearSmoothedVec3> positions;
        std::vector<LinearSmoothedFloat> radii;
        int frms;
        int chnls;
    public:
        inline float get(int frame, int channel = 0) {
            return values.get(index(frame, channel));
        }
        inline void set(int frame, int channel, float value) {
            values.set(index(frame, channel), value);
        }
        LinearSmoothedVec3& getLoc(int channel) {
            KIT_ASSERT(channel >= 0 && channel < chnls, "Channel out of range");
            return positions.at(channel);
        }
        LinearSmoothedFloat& getRadius(int channel) {
            KIT_ASSERT(channel >= 0 && channel < chnls, "Channel out of range");
            return radii.at(channel);
        }

        void resize(int numFrames, int numChannels = 1) {
            this->frms = numFrames;
            this->chnls = numChannels;
            values.resize(numFrames * numChannels);
            printf("Resized buffer: %d frames on %d channels\n", numFrames, numChannels);
        }
        void clear() {
            values.clear();
        }

        const int& numChannels = chnls;
        const int& numFrames = frms;

    protected:

        int index(int frame, int channel) {
            KIT_ASSERT(frame >= 0 && frame < frms, "Frame out of range" + std::to_string(frame));
            KIT_ASSERT(channel >= 0 && channel < chnls, "Channel out of range" + std::to_string(channel));
            return frame + (channel * frms);
        }
};