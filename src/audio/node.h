#pragma once
#include "./buffer.h"
#include "util/assert.h"

namespace audio_graph {

    struct PlaybackInitialisationInfo {
        float sampleRate;
        int blockSize;
    };

    class Node {
        public:
            virtual ~Node() {}
            void initialize(const PlaybackInitialisationInfo& info);
            void prepareForNextBlock();
            void process();
            bool doneProcessing();
            PositionalChannelBuffer& getOutput();
        protected:
            virtual void prepareToPlay(const PlaybackInitialisationInfo&) {};
            virtual void prefetchBlock() {}
            virtual void process(PositionalChannelBuffer& out) = 0;
        private:
            PositionalChannelBuffer outputBuffer;
            bool done;
            std::atomic_bool initialized;
    };

    class DummyNode: public Node {
        public:
            DummyNode() {}
        protected:
            void prepareToPlay(const PlaybackInitialisationInfo&) {}
            virtual void prefetchBlock() {}
            virtual void process(PositionalChannelBuffer& out) {}
    };
}