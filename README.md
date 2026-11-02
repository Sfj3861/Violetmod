# Violetmod

A modular Geometry Dash utility mod built with Geode, with a violet-themed and touch-friendly interface.

## Current state

Version **0.1.1** adds a navigable popup from the Geometry Dash main menu. It currently contains Home, Tools, and About sections. The Tools page is a roadmap, not a claim that those tools are already implemented.

## Planned features

- Frame Counter
- Hitbox Viewer
- Show Trajectory
- Persistent settings and modular feature controls

## Build

Requirements:
- Geode SDK 5.6.1
- CMake 3.21+
- A compatible C++23 toolchain for the target platform

Set the `GEODE_SDK` environment variable to your Geode SDK directory, then configure and build with CMake.

## Compatibility target

- Geometry Dash 2.2081
- Android ARM64
- Windows

## Development status

The source is an early implementation and has **not yet been compiled or tested in-game**. Contributions should use public Geode APIs and respect the licenses of any third-party code.
