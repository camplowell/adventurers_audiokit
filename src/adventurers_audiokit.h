#pragma once

class adventurersAudiokit {
    public:
        adventurersAudiokit();
        void loop();
        ~adventurersAudiokit();
    private:
        int pitch;
        int gain;
};