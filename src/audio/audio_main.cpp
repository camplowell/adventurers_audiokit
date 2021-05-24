#include "audio/audio_main.h"

using namespace AudioModule;
static const float TWO_PI = 2.0f * 3.1415926535f;

struct SoundIo *soundio;
struct SoundIoDevice *device;
struct SoundIoOutStream *outstream;

float frequency;
float gain;

float phase;

int setupAudio();
int openStream(int device = soundio_default_output_device_index(soundio));
void closeStream();
void closeAudio();

int AudioModule::setup() {
    frequency = 440.0f;
    gain = 1.0f;

    return setupAudio();
}

void AudioModule::loop() {
    soundio_flush_events(soundio);
}

void AudioModule::shutdown() {
    closeAudio();
}

float* AudioModule::getFrequency() {
    return &frequency;
}

float* AudioModule::getAmplitude() {
    return &gain;
}

static void write_callback(struct SoundIoOutStream *outstream,
        int frame_count_min, int frame_count_max)
{
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    float float_sample_rate = outstream->sample_rate;

    struct SoundIoChannelArea *areas;
    int frames_left = frame_count_max;
    int err;

    while (frames_left > 0) {
        int frame_count = frames_left;

        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
            fprintf(stderr, "%s\n", soundio_strerror(err));
            exit(1);
        }

        if (!frame_count)
            break;

        float radians_per_sample = (frequency * TWO_PI) / float_sample_rate;
        for (int frame = 0; frame < frame_count; frame += 1) {
            float sample = sinf(phase) * gain;
            for (int channel = 0; channel < layout->channel_count; channel += 1) {
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                *ptr = sample;
            }
            phase += radians_per_sample;
        }
        phase = fmodf(phase, TWO_PI);

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
    soundio_outstream_destroy(outstream);
    soundio_device_unref(device);
}

void closeAudio() {
    closeStream();
    soundio_destroy(soundio);
}