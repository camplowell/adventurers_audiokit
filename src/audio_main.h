// Include soundio as a C library, not C++
extern "C" {
    #include "soundio/soundio.h"
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static const float TWO_PI = 2.0F * 3.1415926535f;
static float seconds_offset = 0.0f;
static float frequency = 440.0f;
static float phase = 0.0f;
static float gain = 1.0f;

struct SoundIo *soundio;
struct SoundIoDevice *device;
struct SoundIoOutStream *outStream;

static void write_callback(
    struct SoundIoOutStream *outstream, 
    int frame_count_min, int frame_count_max
) {
    const struct SoundIoChannelLayout *layout = &(outstream->layout);
    float float_sample_rate = outstream->sample_rate;

    struct SoundIoChannelArea *areas;
    int frames_left = frame_count_max;
    int err;

    while (frames_left > 0) {
        int frame_count = frames_left;
        // Begin writing to buffer
        if (err = soundio_outstream_begin_write(outstream, &areas, &frame_count)) {
            fprintf(stderr, "Couldn't write to outstream: %s", soundio_strerror(err));
            exit(1);
        }
        if (!frame_count)
            break;

        float radians_per_second = frequency * TWO_PI;
        // Loop through all 'frames' (1 sample for all channels)
        for (int frame = 0; frame < frame_count; frame++) {
            phase += radians_per_second / float_sample_rate;
            float sample = sin(phase) * gain;
            // Loop through each channel, setting the sample value
            for (int channel = 0; channel < layout->channel_count; channel++) {
                // Find the sample pointer so we can set it
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);

                *ptr = sample;
            }
        }

        // Bring phase back between 0 and TWO_PI
        phase = fmodf(phase, TWO_PI);

    }
}
static float* getGain() {
    return &gain;
}

static float* getPitch() {
    return &frequency;
}

static int sound_init() {
    int err;
    soundio = soundio_create();
    if (err = soundio_connect(soundio)) {
        // Some error code
        fprintf(stderr, "Error connecting: %s", soundio_strerror(err));
        return 1;
    }

    // Update our view of the connected devices and their information
    // Very performant; merely flips a pointer
    soundio_flush_events(soundio);

    int default_out_device_index = soundio_default_output_device_index(soundio);
    if (default_out_device_index < 0) {
        fprintf(stderr, "No output device found");
        return 1;
    }

    device = soundio_get_output_device(soundio, default_out_device_index);
    if (!device) {
        fprintf(stderr, "Out of memory");
        return 1;
    }

    printf("Output device: %s\n", device->name);

    outStream = soundio_outstream_create(device);
    outStream->format = SoundIoFormatFloat32NE;
    outStream->write_callback = write_callback;

    if ((err = soundio_outstream_open(outStream))) {
        fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
    }
}

static void update() {
    soundio_flush_events(soundio);
}

static void sound_deinit() {
    soundio_outstream_destroy(outStream);
    soundio_device_unref(device);
    soundio_destroy(soundio);
}
