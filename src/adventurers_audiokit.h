/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */
#pragma once
#include "imgui.h"

namespace Audiokit_UI {
    void setupIO(ImGuiIO& io);
    void loadFonts(ImGuiIO& io);
    ImVec4 getClearColor();

    void setup();
    void loop();
    void shutdown();
}