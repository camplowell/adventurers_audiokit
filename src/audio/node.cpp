/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */

#include "./node.h"

using namespace audio_graph;

void Node::initialize(const PlaybackInitialisationInfo& info) {
    prepareToPlay(info);

    outputBuffer.resize(info.blockSize);
    initialized.store(true);
}

void Node::prepareForNextBlock() {
    done = false;

    prefetchBlock();
}

void Node::process() {
    outputBuffer.clear();
    process(outputBuffer);
    done = true;
}

bool Node::doneProcessing() {
    return done;
}

PositionalChannelBuffer& Node::getOutput() {
    KIT_ASSERT(done, "Not done processing yet!");
    return outputBuffer;
}