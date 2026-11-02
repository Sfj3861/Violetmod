# Violetmod roadmap

## Phase 1 — foundation
- [x] Geode metadata for Geometry Dash 2.2081
- [x] CMake project configuration
- [x] Main-menu entry point
- [x] Initial popup/alert proving the menu hook is active
- [ ] Violet-themed reusable UI components
- [ ] Persistent settings registry
- [ ] Validate layout and touch targets on Android

## Phase 2 — core utilities
- [ ] Frame Counter: visibility, position, scale, and display options
- [ ] Hitbox display with world-space rendering
- [ ] Trajectory visualizer with world-space rendering
- [ ] Independent enable/disable controls for each module

## Phase 3 — file workflows
- [ ] Import/export for Violetmod settings
- [ ] Document native file formats before implementing them
- [ ] Validate malformed, oversized, or incompatible files

## Design principles
1. Prefer stable Geode APIs over fragile hooks.
2. Render world overlays in world coordinates, not camera-relative screen coordinates.
3. Separate UI, settings, rendering, and file I/O.
4. Keep Android touch targets comfortable without hiding advanced settings.
5. Test each feature on its target platform before marking it complete.
6. Reuse external code only when its licence permits it; preserve attribution.

## Community feedback
At the time of this initial review, the repository had no Issues or Pull Requests containing feature proposals. New suggestions can be opened as GitHub Issues and reviewed for feasibility, compatibility, accessibility, and licensing.
