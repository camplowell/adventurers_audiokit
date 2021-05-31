/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */
#pragma once
#include "./buffer.h"
#include "util/assert.h"

/**
 * @brief A namespace for audio nodes and their properties
 * 
 */
namespace audio_graph {

    /**
     * @brief A struct for conveying stream-specific parameters to Nodes.
     */
    struct PlaybackInitialisationInfo {
        float sampleRate;
        int blockSize;
    };

    /**
     * @brief A modular unit for audio processing.
     * 
     * \paragraph Usage Using nodes
     * First, instantiate the node in a way specific to the superclass. <br>
     * Then, you need to initialize the node to the current playback properties.
     * You can do this by calling AudioModule::initialize(audio_graph::Node*) <br>
     * Once the node is fully initialized, it can then be set as the root node
     * by calling AudioModule::setRoot(audio_graph::Node*)
     * 
     * \paragraph Implementing Implementing a node
     * When implementing a node, you need to overload: <br>
     * prepareToPlay(const PlaybackInitialisationInfo&) <br>
     * prefetchBlock() <br>
     * process(PositionalChannelBuffer& out)
     */
    class Node {
        public:
            virtual ~Node() {}
            /** Call to finish node initialization with the given playback settings. */
            void initialize(const PlaybackInitialisationInfo& info);
            /** Called before any processing is performed on each block of audio */
            void prepareForNextBlock();
            /** Called to process each block of audio */
            void process();
            /** Returns true if processing is done and it is safe to access the results */
            bool doneProcessing();
            /** Retrieves the processed output */
            PositionalChannelBuffer& getOutput();
        protected:
            /** Prepares the node to process audio with the given playback settings. */
            virtual void prepareToPlay(const PlaybackInitialisationInfo&) {};
            /** Called before any processing is performed on each block of audio.
             * Use to prefetch audio data, 
            */
            virtual void prefetchBlock() {}
            /** Processes audio data, filling the given output buffer. */
            virtual void process(PositionalChannelBuffer& out) = 0;
        private:
            PositionalChannelBuffer outputBuffer;
            bool done;
            std::atomic_bool initialized;
    };

    /** 
     * A Dummy audio node, used for testing purposes.
    */
    class DummyNode: public Node {
        public:
            const char ID;
            DummyNode(char id) : ID(id) {}
        protected:
            void prepareToPlay(const PlaybackInitialisationInfo&) {}
            virtual void prefetchBlock() {}
            virtual void process(PositionalChannelBuffer& out) {}
    };
}