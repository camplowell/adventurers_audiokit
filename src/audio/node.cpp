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