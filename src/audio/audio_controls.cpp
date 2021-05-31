#include <typeinfo>
#include <stdio.h>
#include "audio/audio_controls.h"


LinearSmoothedFloat::LinearSmoothedFloat(float value, float transitionSeconds, float sampleRate) {
    // Debug warning for datatypes creating locks
    #ifndef NDEBUG
    if (!nextTarget.is_lock_free()) {
        fprintf(stderr, "WARNING: floats are creating locks!\n");
    }
    #endif
    this->value = value;
    this->nextTarget.store(value);
    this->target = value;
    // Convert transition length from seconds to samples
    this->transitionSeconds = transitionSeconds;
    this->sampleRate = sampleRate;
    setSmoothing();
}


void LinearSmoothedFloat::set(float target) {
    nextTarget.store(target);
}

float LinearSmoothedFloat::getTarget() {
    return nextTarget.load();
}

void LinearSmoothedFloat::setSampleRate(float rate) {
    this->sampleRate = rate;
    setSmoothing();
}

void LinearSmoothedFloat::setSmoothing(float length) {
    this->transitionSeconds = length;
    setSmoothing();
}

void LinearSmoothedFloat::setSmoothing() {
    this->transitionLength.store(static_cast<int>(this->transitionSeconds * this->sampleRate));
}


void LinearSmoothedFloat::update() {
    float newTarget = nextTarget.load();
    if (newTarget == target) {
        return;
    }
    target = newTarget;
    countdown = transitionLength;

    if (countdown <= 0) {
        value = target;
        return;
    }
    
    step = (target - value) / countdown;
}


float LinearSmoothedFloat::getNext() {
    if (countdown <= 0) return target;

    countdown--;
    value += step;

    return value;
}

float LinearSmoothedFloat::peek() {
    return countdown <= 0 ? target : value;
}

// ================================================================================================
// Linear smoothed Vec3
// ================================================================================================

LinearSmoothedVec3::LinearSmoothedVec3(Vec3 value, float transitionSeconds, float sampleRate) {
    this->value = value;
    this->nextX.store(value.x);
    this->nextY.store(value.y);
    this->nextZ.store(value.z);
    this->target = value;
    this->transitionLength = static_cast<int>(transitionSeconds * sampleRate);
}

void LinearSmoothedVec3::set(Vec3 target) {
    this->nextX.store(target.x);
    this->nextY.store(target.y);
    this->nextZ.store(target.z);
}

Vec3 LinearSmoothedVec3::getTarget() {
    return {this->nextX.load(), this->nextY.load(), this->nextZ.load()};
}

void LinearSmoothedVec3::update() {
    Vec3 newTarget = {nextX.load(), nextY.load(), nextZ.load()};
    if (newTarget == target) {
        return;
    }
    target = newTarget;
    countdown = transitionLength;

    if (countdown <= 0) {
        value = target;
        return;
    }

    step = (target - value) / countdown;
}

Vec3 LinearSmoothedVec3::getNext() {
    if (countdown <= 0) return target;

    countdown--;
    value += step;

    return value;
}

Vec3 LinearSmoothedVec3::peek() {
    if (countdown <= 0) return target;
    return value;
}