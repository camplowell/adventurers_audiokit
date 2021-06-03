/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */

#include "./buffer.h"

// ================================================================================================
// Channel buffer
// ================================================================================================

float ChannelBuffer::get(int frame, int channel) {
    return values.at(index(frame, channel));
}

void ChannelBuffer::set(int frame, int channel, float value) {
    values[index(frame, channel)] = value;
}

void ChannelBuffer::resize(int numFrames, int numChannels) {
    values.resize(numFrames * numChannels);
    this->n_frames = numFrames;
    this->n_channels = numChannels;
    printf("Resized buffer: %d frames on %d channels\n", numFrames, numChannels);
}

void ChannelBuffer::clear() {
    std::fill(values.begin(), values.end(), 0.0f);
}

int ChannelBuffer::index(int frame, int channel) {
    KIT_ASSERT(frame >= 0 && frame < n_frames, "Frame out of range" + std::to_string(frame));
    KIT_ASSERT(channel >= 0 && channel < n_channels, "Channel out of range" + std::to_string(channel));
    return frame + (channel * n_frames);
}

// ================================================================================================
// Positional channel buffer
// ================================================================================================

Vec3& PositionalChannelBuffer::getLoc(int channel) {
    KIT_ASSERT(channel >= 0 && channel < n_channels, "Channel out of range");
    return positions[channel];
}

float& PositionalChannelBuffer::getRadius(int channel) {
    KIT_ASSERT(channel >= 0 && channel < n_channels, "Channel out of range");
    return radii[channel];
}

void PositionalChannelBuffer::resize(int numFrames, int numChannels) {
    positions.resize(numChannels);
    radii.resize(numChannels);
    ChannelBuffer::resize(numFrames, numChannels);
    printf("Resized buffer: %d frames on %d channels\n", numFrames, numChannels);
}

void PositionalChannelBuffer::clear() {
    ChannelBuffer::clear();
    // Clear radius buffer
    std::fill(radii.begin(), radii.end(), 0.0f);
    // Clear vector buffer
    for (int i = 0; i < n_channels; i++) {
        positions[i] = {0.0, 0.0, 0.0};
    }
}