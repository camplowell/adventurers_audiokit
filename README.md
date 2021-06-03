# Adventurer's Audiokit

Will eventually become a procedural audioscape generator for tabletop roleplaying.
Currently, it does very little, as the infrastructure to perform more advanced operations is not done yet.

## Objectives for alpha releasse
- Extensible, readable codebase
- Clean UI without sacrificing depth
- Drop in effects live
- Load and transition between rooms
- Granular player for flexible effects
- Looping player for non-granular sounds
- High-quality, efficient reverb
- Parametric EQ


### Foundational tasks ![Progress](https://progress-bar.dev/5/?scale=14&title=progress&color=babaca&suffix=%2F14)
- [x] Integrate Dear IMGUI into build process
- [x] Integrate Libsoundio into build process
- [x] Isolate platform-specific code
- [x] Smoothed numerical input to audio thread
- [x] Move audio calculation to node class
- [ ] Read audio (WAV) files from disk
- [ ] Implement mixer node
- [ ] Audio node graphs
  - [ ] Traversal robust to cycles
  - [ ] Single-threaded playback
  - [ ] Seamless topology changes
- [ ] Modularly link audio inputs to GUI elements
- [ ] Deal with audio device changes
- [ ] State machine for UI "pages"

### Before V0.0.1 (Proof of concept) ![Progress](https://progress-bar.dev/0/?scale=12&tittle=progress&color=babbaca&suffix=%2F12)
- [ ] Initialize node tree from JSON ("effect" units)
- [ ] Tree of effects from JSON ("room" units)
- [ ] Load gui elements from effect file
- [ ] Load element layout from room file
- [ ] Implement granular sampler
  - [ ] Random grain selection
  - [ ] Trigger protocol (MIDI?)
  - [ ] Manual trigger generator
  - [ ] Density-based trigger generator
  - [ ] Pattern and tempo trigger generator
- [ ] File picker
- [ ] Fade between rooms

## Building
Build management is done through GNU Make; 
This is natively supported on Mac and Linux, but requires a shell to run on Windows.
Make targets:
- all: Builds the code (Clang / Clang++)
- doc: Generates HTML documentation
- clean: Removes built files and clears the documentation folder.

## Dependencies
[Dear IMGUI](https://github.com/ocornut/imgui) (submodule)
- GLFW (other window managers possible)
- OpenGL2 (other renderers possible)

[Libsoundio](https://github.com/andrewrk/libsoundio) (submodule)
- cmake
- ALSA library (optional)
- libjack2 (optional)
- libpulseaudio (optional)

Optional dependencies will build if installed, and will be skipped if not present.
