/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */

#pragma once
#include <atomic>
#include "util/vec.h"

/** A superclass for lock-free and thread safe Node inputs */
template <typename T>
class AudioControl {
    public:
        /** Set the value of the control */
        virtual void set(T value) = 0;

        /** Get the most recently set value */
        virtual T getTarget() = 0;

        /** Update the audio thread's view of the value */
        virtual void update() = 0;
};

/** A linearly smoothed, lock-free and thread-safe float parameter. */
class LinearSmoothedFloat : public AudioControl<float> {
    public:
        LinearSmoothedFloat(float value, float transitionSeconds = 0.01f, float sampleRate = 44100.0f);

        void set(float target) override;

        float getTarget() override;

        /** Set the sample rate for calculating smoothing time */
        void setSampleRate(float);

        /** Set how long the smoothing takes */
        void setSmoothing(float transitionSeconds);

        /** Update the audio thread's view of the target value */
        void update() override;

        /** Iterate to the next value and return */
        float getNext();

        /** Retrieve the current value without iterating */
        float peek();
    private:
        void setSmoothing();
        // Accessed by UI thread
        float sampleRate;
        float transitionSeconds;
        // Accessed by both
        std::atomic<float> nextTarget;
        std::atomic_int transitionLength;
        // Accessed by audio thread
        float target;
        float step;
        float value;
        int countdown;
        
};

// A Vec3 with linearly smoothed channels
class LinearSmoothedVec3 : public AudioControl<Vec3>{
    public:
        LinearSmoothedVec3(Vec3 value, float transitionSeconds, float sampleRate);
        
        void set(Vec3 target) override;

        Vec3 getTarget() override;

        void update() override;

        Vec3 getNext();

        /** Retrieve the current value without iterating */
        Vec3 peek();
    protected:
        std::atomic<float> nextX;
        std::atomic<float> nextY;
        std::atomic<float> nextZ;
        Vec3 target;
        Vec3 step;
        Vec3 value;
        int countdown;
        int transitionLength;
};
/*
template <typename T>
class SwappingControl {
    public:
        virtual T* swap(T* newValue);
        virtual T* getValue();
    protected:
        T* value;
};
*/