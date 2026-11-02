# Violetmod roadmap

## Current status: 0.2.0 - customization foundation

- [x] Geode project skeleton and metadata
- [x] Main-menu entry point
- [x] Navigable popup with Home, Tools, and Customization sections
- [x] Persistent Geode settings for theme, scale, dimensions, shortcut text, size, opacity, and visibility
- [x] Basic feature preference switches
- [ ] Build and runtime validation against Geode SDK 5.6.1 on Android ARM64 and Windows
- [ ] Custom image icon import and in-game editor
- [ ] Independent panel/text/accent colors and shortcut placement
- [ ] Preset profiles and import/export

## Feature catalogue

### Interface and quality of life
- Theme presets and layout density
- Popup sizing and scale
- Shortcut glyph, scale, opacity, visibility
- Animation preference
- Persistent settings

### Gameplay diagnostics (planned)
- Frame Counter
- Hitbox Viewer
- Show Trajectory
- FPS and frame-time display
- Input display and session statistics
- Practice/session utilities

### Level and editor utilities (planned)
- Level information and session tools
- Object/trigger inspection helpers
- Editor quality-of-life controls

### Practice and recording utilities (planned)
- Start-position and practice helpers
- Attempt/session tracking
- Input/macro-related tools only where technically appropriate and transparently labeled

## Customization roadmap

- [x] Text glyph shortcut
- [x] Theme presets
- [x] Popup scale and dimensions
- [x] Shortcut size, opacity, and visibility
- [ ] User-selected PNG icon with validation, crop, and reset
- [ ] Full color editor with independent UI color roles
- [ ] Drag-to-position shortcut with safe screen bounds
- [ ] Overlay-specific controls
- [ ] Profiles, import/export, and reset-to-defaults

## Development principles

- Use public Geode SDK APIs and compatible open-source code only.
- Keep feature modules isolated and configurable.
- Never label a feature as working until it has been built and tested in-game.
- Keep Android touch targets readable and avoid overcrowding the interface.
- Keep overlays in the correct world/screen coordinate space.
- Clearly distinguish implemented controls from planned feature placeholders.

## Current limitations

The 0.2.0 branch adds a customizable interface shell and Geode settings. The gameplay feature switches are preferences for future modules, not implementations of those modules. A custom image picker/editor is not implemented. The branch has not been compiled or tested in Geometry Dash because a matching Geode SDK/toolchain is not available in the current environment.
