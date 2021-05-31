#pragma once

#include "../node.h"

namespace audio_graph {

    class SineNode : public Node {
        public:
            SineNode(): amplitude({1.0f}), frequency({440.0f}) {}
            LinearSmoothedFloat amplitude;
            LinearSmoothedFloat frequency;
        protected:
            static constexpr float TWO_PI = 2.0f * 3.1415926535f;
            void prepareToPlay(const PlaybackInitialisationInfo& info) override {
                this->sampleRate = info.sampleRate;
                amplitude.setSampleRate(sampleRate);
                frequency.setSampleRate(sampleRate);

                amplitude.setSmoothing(0.01f);
                frequency.setSmoothing(0.01f);
            }

            void prefetchBlock() override {
                amplitude.update();
                frequency.update();
            }

            void process(PositionalChannelBuffer& out) override {
                for (int frame = 0; frame < out.numFrames; frame++) {
                    float sample = sinf(phase) * amplitude.getNext();
                    for (int channel = 0; channel < out.numChannels; channel++) {
                        out.set(frame, channel, sample);
                    }
                    phase += frequency.getNext() * TWO_PI / sampleRate;
                    // Simple euler filter; assumes frequency is less than the sample rate.
                    if (phase > TWO_PI) phase -= TWO_PI;
                }
            }


        private:
            float phase;
            float sampleRate;
    };

}