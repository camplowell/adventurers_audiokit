/*
 * Copyright (c) 2021 Lowell Camp
 * 
 * This file is part of the Adventurer's Audiokit,
 * which is licensed under the Mozilla Public License 2.0
 * See https://opensource.org/licenses/MPL-2.0
 */
#pragma once
#include "imgui.h"

/** \mainpage Index
 * 
 * \section Intro
 * Adventurer's Audiokit is (or will be) a procedural audioscape generator,
 * primarily targeted at tabletop roleplaying.
 * 
 */

/** @brief A namespace for handling the UI of the software.
 * 
 */
namespace Audiokit_UI {
    /** Set up the window IO. 
     * Should be called when setting up the window.
    */
    void setupIO(ImGuiIO& io);
    /** Load fonts.
     * Should be called after initializing the graphics backend.
     * */
    void loadFonts(ImGuiIO& io);
    /** Get the window background color. */
    ImVec4 getClearColor();

    /** General UI setup. */
    void setup();
    /** Processes user input and draws the UI. */
    void loop();
    /** Call before exiting the application. */
    void shutdown();
}