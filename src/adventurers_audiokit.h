#pragma once
#include "imgui.h"

namespace Audiokit_UI {
    void setupIO(ImGuiIO& io);
    void loadFonts(ImGuiIO& io);
    ImVec4 getClearColor();

    void setup();
    void loop();
    void exit();
}