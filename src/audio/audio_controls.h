#pragma once
#include <atomic>
#include "util/vec.h"

template <typename T>
class AudioControl {
    public:
        /** Set the value of the control */
        virtual void set(T value) = 0;
        virtual T getTarget() = 0;
        /** Update the audio thread's view of the value */
        virtual void update() = 0;
};

/** A linearly smoothed, lock-free and thread-safe float parameter. */
class LinearSmoothedFloat : public AudioControl<float> {
    public:
        LinearSmoothedFloat(float value, float transitionSeconds, float sampleRate);
        /** Set the next target value */
        void set(float target) override;
        float getTarget() override;
        /** Set how long the smoothing takes */
        void setSmoothing(float transitionSeconds, float sampleRate);
        /** Update the audio thread's view of the target value */
        void update() override;
        /** Iterate to the next value and return */
        float getNext();
        /** Retrieve the current value without iterating */
        float peek();
    protected:
        std::atomic<float> nextTarget;
        float target;
        float step;
        float value;
        int countdown;
        std::atomic_int transitionLength;
};

// A Vec3 with linearly smoothed channels
class LinearSmoothedVec3 : public AudioControl<Vec3>{
    public:
        LinearSmoothedVec3(Vec3 value, float transitionSeconds, float sampleRate);
        /** Set the next target value */
        void set(Vec3 target) override;
        Vec3 getTarget() override;
        /** Update the audio thread's view of the target value */
        void update() override;
        /** Iterate to the next value and return */
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