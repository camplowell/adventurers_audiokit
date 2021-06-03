/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */

#pragma once

#include "../node.h"

namespace audio_graph {

    /** @brief Simple sinewave generator node; mainly for testing */
    class SineNode : public Node {
        public:
            SineNode(): amplitude({1.0f}), frequency({440.0f}) {}
            /// The amplitude of the generated wave
            LinearSmoothedFloat amplitude;
            /// The frequency of the generated wave
            LinearSmoothedFloat frequency;
        protected:
            static constexpr double TWO_PI = 2.0f * 3.14159265358979311599796346854f;
            void prepareToPlay(const PlaybackInitialisationInfo& info) override {
                this->sampleRate = info.sampleRate;
                amplitude.setSampleRate(sampleRate);
                frequency.setSampleRate(sampleRate);

                amplitude.setSmoothing(0.02f);
                frequency.setSmoothing(0.02f);
            }

            void prefetchBlock() override {
                amplitude.update();
                frequency.update();
            }

            void process(PositionalChannelBuffer& out) override {
                for (int frame = 0; frame < out.numFrames; frame++) {
                    float sample = static_cast<float>(sin(phase) * amplitude.getNext());
                    //if (sample >= 1.0f) sample = 0.9999f;
                    for (int channel = 0; channel < out.numChannels; channel++) {
                        out.set(frame, channel, sample);
                    }
                    phase += frequency.getNext() * TWO_PI / sampleRate;
                    // Simple euler filter; assumes frequency is less than the sample rate.
                    if (phase > TWO_PI) phase -= TWO_PI;
                }
            }

        private:
            double phase;
            double sampleRate;
    };

}