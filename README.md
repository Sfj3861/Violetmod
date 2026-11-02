# Violetmod

A modular Geometry Dash utility mod built with Geode.

## Project goals

- Violet/purple visual identity with a clean, touch-friendly interface.
- No mandatory account or external login.
- Modular feature architecture so tools can be enabled and configured independently.
- Android-first testing, with Windows support where practical.
- Reuse compatible open-source Geode patterns and libraries when their licences allow it.

## Current status

**Foundation / early development.** The repository contains the initial Geode project scaffold and a basic main-menu entry that opens a placeholder alert. This is not yet a feature-complete or release-ready mod.

## Build

1. Install the Geode SDK matching the target Geode release.
2. Set the `GEODE_SDK` environment variable to the SDK directory.
3. Configure and build with CMake using a supported Geode toolchain.
4. Package and test the resulting mod on the matching Geometry Dash and Geode versions.

Target game version: Geometry Dash 2.2081  
Target Geode version: 5.6.1

## Planned modules

- Violet-themed interface and settings framework
- Frame Counter
- Hitbox display
- Trajectory visualizer
- Replay and recording utilities, subject to compatibility and feasibility
- Import/export for Violetmod's own documented formats

## Suggestions and contributions

Feature proposals should be opened as GitHub Issues. Include the expected behavior, platform, and a short use case. Proposals will be reviewed for feasibility, compatibility, accessibility, and licensing.

## Licensing

Third-party code and assets will only be incorporated when their licences permit it. Attribution and licence notices will be preserved.
