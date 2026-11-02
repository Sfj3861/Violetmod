# Violetmod

A unified, native Geometry Dash mod built with Geode. Violetmod is designed as one integrated toolkit with a shared interface, settings system and modular feature architecture.

## Current development line - 0.4.0

The current source contains the Geode project foundation, main-menu popup, interface customization, and a first runtime diagnostics overlay. FPS, frame time and process-session time labels are live; the safe-mode state is shown in-game. The remaining gameplay/performance switches are configuration entries until their runtime modules are completed and validated.

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
- Verify Hack: allow an eligible level upload without a completed in-game verification, while clearly marking the level description as `UNVERIFIED`.

### On-screen labels
Independent label toggles are available for FPS, frame time and session time in the first runtime pass. TPS, physics TPS, attempts, speed, player coordinates/mode, level name, practice status, input, memory and display refresh rate remain planned. Label scale and opacity are also configurable.

## Level copying and Verify Hack
**All Copyable** will use Geometry Dash 2.2's current copy system. The old copy-password restriction is not used for 2.2 levels, so this is a compatibility utility rather than a password bypass.

## Interface customization

Open **Geode > Violetmod > Settings** to change the current interface settings. The shortcut currently uses a text glyph; custom PNG import/editor, independent color roles, shortcut positioning, profiles and import/export remain planned.

## Build and compatibility target

- Geode SDK 5.6.1
- Geometry Dash 2.2081
- Android ARM64 and Windows
- CMake 3.21+ and a compatible C++23 toolchain

Set `GEODE_SDK` to your Geode SDK directory and configure/build with CMake.

## Development status

The new runtime overlay has not yet been compiled or tested in-game in this environment. Do not treat unimplemented settings as working features. The next milestone is SDK compilation and device testing before adding invasive gameplay hooks. Every module must be implemented against public Geode APIs and validated separately before being marked complete.
