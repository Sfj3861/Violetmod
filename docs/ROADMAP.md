# Violetmod roadmap

## Current status: 0.1.1 - interface foundation

- [x] Geode project skeleton and metadata
- [x] Main-menu entry point
- [x] Navigable popup with Home, Tools, and About sections
- [ ] Validate a full build against Geode SDK 5.6.1 on Android ARM64 and Windows
- [ ] Add persistent settings and UI preferences
- [ ] Add a modular feature registry and per-feature enable/disable controls

## Planned tools

### Frame Counter
- Display frame and timing information in a compact overlay.
- Keep the overlay independent of camera/world transforms.
- Make visibility and placement configurable.

### Hitbox Viewer
- Draw diagnostic hitbox overlays in the correct world coordinate space.
- Keep camera transforms and gameplay object transforms separate.
- Provide clear visibility controls.

### Show Trajectory
- Display a trajectory preview using the game's relevant object and physics data.
- Keep the preview aligned with the world rather than fixed to the camera.
- Allow the overlay to be toggled independently.

## Development principles

- Use public Geode SDK APIs and compatible open-source code only.
- Prefer small, isolated modules over one large implementation.
- Never label a feature as working until it has been built and tested in-game.
- Keep Android touch targets readable and avoid overcrowding the interface.
- Preserve compatibility with other Geode mods wherever possible.

## Current limitations

The popup is an early navigation shell. The listed tools are not implemented yet. This repository change has not been compiled or tested in Geometry Dash because a matching Geode SDK toolchain is not available in the current environment.
