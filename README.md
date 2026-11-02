# Violetmod

A unified, native Geometry Dash mod built with Geode. Violetmod is designed as one integrated toolkit with a shared interface, settings system and modular feature architecture.

## Current development line - 0.3.0

The current source contains the Geode project foundation, a main-menu popup, interface customization, and a broader feature-settings catalogue. The new gameplay/performance settings are **configuration entries only** at this stage; they do not mean the corresponding runtime hooks are implemented.

## Requested feature suite

### Gameplay and practice
- Speedhack and audio synchronization
- Noclip and limited noclip (local practice)
- Layout mode
- Show hitboxes, hitbox trail and trajectory
- Smart StartPos and StartPos switcher
- Show all attempts
- No glow and no shaders
- Pitch shifter
- Frame extrapolation

### Timing and performance
- Configurable physics TPS target
- FPS target/lock
- Speed multiplier
- Separate audio pitch control

Physics TPS and rendered FPS are different systems. A requested target cannot guarantee that the game engine or device will actually sustain that rate. The implementation must report the effective rate rather than promise unsupported performance.

### Safety and compatibility
- Manual safe mode
- Safe-mode warning popup
- Automatic safe mode fallback
- Local verification testing only; Violetmod will not falsify online verification or upload status.

### On-screen labels
Independent label toggles are planned for FPS, TPS, physics TPS, frame time, attempts, session time, speed, player coordinates/mode, level name, practice status, input, memory and display refresh rate. Label scale and opacity are also configurable.

## Level copying
Copy tools will operate on levels the user owns or is authorized to edit. Violetmod will not bypass passwords or access restrictions on other creators' protected levels.

## Interface customization

Open **Geode > Violetmod > Settings** to change the current interface settings. The shortcut currently uses a text glyph; custom PNG import/editor, independent color roles, shortcut positioning, profiles and import/export remain planned.

## Build and compatibility target

- Geode SDK 5.6.1
- Geometry Dash 2.2081
- Android ARM64 and Windows
- CMake 3.21+ and a compatible C++23 toolchain

Set `GEODE_SDK` to your Geode SDK directory and configure/build with CMake.

## Development status

The code has not yet been compiled or tested in-game. Feature settings added in 0.3.0 are a catalogue and configuration foundation, not working gameplay features. Every module must be implemented against public Geode APIs and validated separately before being marked complete.
