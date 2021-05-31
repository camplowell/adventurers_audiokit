/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */

#include "./audio_main.h"
#include "./audio_controls.h"

#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

using namespace AudioModule;

// Libsoundio information
struct SoundIo *soundio;
struct SoundIoDevice *device;
struct SoundIoOutStream *outstream;

// The global volume property 
LinearSmoothedFloat globalGain = {0.0f};

// Playback info
std::atomic<float> latency;
std::atomic_int bufferSize;

// The audio node to play from
audio_graph::Node *root;

int setupAudio();
int openStream(int device = soundio_default_output_device_index(soundio));
void closeStream();
void closeAudio();

int AudioModule::setup() {
    // Open an audio stream
    int err;
    if (err = setupAudio()) {
        return err;
    }

    // Set up globalGain smoothing
    globalGain.setSampleRate(static_cast<float>(outstream->sample_rate));
    globalGain.setSmoothing(0.01f);

    globalGain.set(1.0f);

    while(!bufferSize.load()) {}
    
    return 0;
}

void AudioModule::shutdown() {
    // Set gain to 0 after 0.01s
    globalGain.setSmoothing(0.01f);
    globalGain.set(0.0f);
    // Wait until the gain is actually 0
    while(globalGain.peek() > 0.0f) {}
    // Wait until we are sure the buffer has been sent
    Sleep(static_cast<int>(ceil(latency.load())));
    // Close the audio channel
    closeAudio();
}

void AudioModule::loop() {
    soundio_flush_events(soundio);
}

float AudioModule::getGlobalGain() {
    return globalGain.getTarget();
}

void AudioModule::setGlobalGain(float amp) {
    globalGain.set(amp);
}

void AudioModule::initialize(audio_graph::Node* node) {
    printf("Initializing node\n");
    audio_graph::PlaybackInitialisationInfo info = {
        static_cast<float>(outstream->sample_rate),
        bufferSize.load()
    };
    node->initialize(info);
    printf("Finished initializing node\n");
}

void AudioModule::setRoot(audio_graph::Node* node) {
    root = node;
}

void write_callback(struct SoundIoOutStream *outstream,
        int frame_count_min, int frame_count_max)
{
    // Get the channel layout and playback information
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    float float_sample_rate = outstream->sample_rate;

    struct SoundIoChannelArea *areas;
    int frames_left = frame_count_max;
    int err;

    // Get the real latency of the system; this lets us know how long we have to wait for a smooth exit
    {
        double lat;
        if (err = soundio_outstream_get_latency(outstream, &lat)) {
            fprintf(stderr, "Failed to get output stream latency: %s\n", soundio_strerror(err));
            exit(1);
        }
        latency.store(static_cast<float>(lat * 1000));
        bufferSize.store(frames_left);
    }

    // Update inputs
    globalGain.update();

    // Write to the buffer
    while (frames_left > 0) {
        int frame_count = frames_left;
        // Signal we want to write to the output stream
        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
            fprintf(stderr, "%s\n", soundio_strerror(err));
            exit(1);
        }

        if (!frame_count)
            break;

        if (root) {
            // Process audio using the node
            root->prepareForNextBlock();
            root->process();
            PositionalChannelBuffer& buffer = root->getOutput();
            // Copy the node's output into audio out.
            for (int frame = 0; frame < frames_left; frame++) {
                float sample = buffer.get(frame, 0);
                float globalMult = globalGain.getNext();
                for(int channel = 0; channel < layout->channel_count; channel++) {
                    float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                    *ptr = sample * globalMult;
                }
            }
        } else {
            // No root node; play silence.
            for (int frame = 0; frame < frames_left; frame++) {
                for (int channel = 0; channel < layout->channel_count; channel++) {
                    float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                    *ptr = 0.0f;
                }
            }
        }

        // Set audio buffer contents here
        for (int frame = 0; frame < frames_left; frame++) {
            for (int channel = 0; channel < layout->channel_count; channel++) {
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                
            }
        }
        // Signal we're done writing to the output stream
        if ((err = soundio_outstream_end_write(outstream))) {
            fprintf(stderr, "%s\n", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
    }
}

int setupAudio() {
    int err;
    soundio = soundio_create();
    if (!soundio) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    if ((err = soundio_connect(soundio))) {
        fprintf(stderr, "error connecting: %s", soundio_strerror(err));
        return 1;
    }

    soundio_flush_events(soundio);

    return openStream();
}

int openStream(int index) {
    if (index < 0) {
        fprintf(stderr, "Device index out of bounds: %d", index);
        return 1;
    }

    device = soundio_get_output_device(soundio, index);
    if (!device) {
        fprintf(stderr, "Couldn't find device: %d", index);
        return 1;
    }

    fprintf(stderr, "Output device: %s\n", device->name);

    outstream = soundio_outstream_create(device);
    outstream->format = SoundIoFormatFloat32NE;
    outstream->write_callback = write_callback;

    int err = 0;
    if ((err = soundio_outstream_open(outstream))) {
        fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
        return 1;
    }

    if (outstream->layout_error)
        fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

    if ((err = soundio_outstream_start(outstream))) {
        fprintf(stderr, "unable to start device: %s", soundio_strerror(err));
        return 1;
    }

    return 0;
}

void closeStream() {
    // Close the output stream
    soundio_outstream_destroy(outstream);
    printf("Audio channel closed\n");
    // Unreference the output device
    soundio_device_unref(device);
    printf("Unreferenced audio device\n");
}

void closeAudio() {
    // First stop playback and close the device
    closeStream();
    // Close the soundio context
    soundio_destroy(soundio);
}