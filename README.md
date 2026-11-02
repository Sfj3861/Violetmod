# Violetmod

A modular Geometry Dash utility mod built with Geode, with a customizable interface and Android-friendly controls.

## Current state - 0.2.0

- Main-menu shortcut opens a navigable Violetmod popup.
- Home, Tools, and Customization pages.
- Persistent Geode settings for theme, popup scale/dimensions, shortcut text/size/opacity/visibility, and animation preferences.
- Feature preference switches for planned utilities.

**Important:** feature switches are configuration placeholders. Frame Counter, Hitbox Viewer, Show Trajectory, and the other gameplay utilities are not implemented yet.

## Customization

Open **Geode > Violetmod > Settings**. You can choose a theme, adjust popup dimensions and scale, change the shortcut's text glyph, size and opacity, and hide the shortcut.

The current shortcut icon is a text glyph, not an uploaded image. Custom PNG import, an in-game icon editor, independent color editing, shortcut positioning, profiles, and import/export remain planned.

See [docs/CUSTOMIZATION.md](docs/CUSTOMIZATION.md) for the full setting guide.

## Planned tools

- Frame Counter
- Hitbox Viewer
- Show Trajectory
- FPS/frame-time and input/session diagnostics
- Practice and level utilities

## Build

Requirements:
- Geode SDK 5.6.1
- CMake 3.21+
- Compatible C++23 toolchain for the target platform

Set the `GEODE_SDK` environment variable to your Geode SDK directory, then configure and build with CMake.

## Compatibility target

- Geometry Dash 2.2081
- Android ARM64
- Windows

## Development status

The source is an early implementation and has **not yet been compiled or tested in-game**. Use public Geode APIs and respect third-party licenses.
