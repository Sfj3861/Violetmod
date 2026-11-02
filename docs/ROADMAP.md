# Violetmod roadmap

## Current status: 0.3.0 - unified feature catalogue

- [x] Geode project skeleton and metadata
- [x] Main-menu entry point and navigable popup
- [x] Persistent interface settings
- [x] Feature preference catalogue for requested gameplay, timing, safety and label modules
- [ ] Compile and validate against Geode SDK 5.6.1 on Android ARM64 and Windows
- [ ] Implement runtime modules and mark each only after build/runtime testing

## Requested modules

| Module | Scope | Status |
|---|---|---|
| Speedhack | Adjustable game speed | Settings/catalogue only |
| Speedhack audio | Keep audio synchronized with speed changes | Settings/catalogue only |
| Noclip | Local practice collision override | Settings/catalogue only |
| Limit Noclip | Constrain local noclip usage | Settings/catalogue only |
| Layout mode | Layout/practice display mode | Settings/catalogue only |
| Show hitboxes | Player/object hitbox visualization | Settings/catalogue only |
| Show hitbox trail | Historical hitbox visualization | Settings/catalogue only |
| Show trajectory | Predicted player path overlay | Settings/catalogue only |
| Smart StartPos | Assisted start-position workflow | Settings/catalogue only |
| StartPos switcher | Switch saved start positions | Settings/catalogue only |
| All attempts | Display attempt history | Settings/catalogue only |
| No glow / No shaders | Visual-effect toggles | Settings/catalogue only |
| Pitch shifter | Audio pitch adjustment | Settings/catalogue only |
| Frame extrapolation | Experimental visual interpolation | Settings/catalogue only |
| Physics TPS | Requested simulation update rate | Settings/catalogue only |
| FPS lock | Requested render target | Settings/catalogue only |
| Safe mode | Disable experimental modules | Settings/catalogue only |
| Safe mode popup | Explain safety fallback state | Settings/catalogue only |
| Auto safe mode | Fallback after detected instability | Settings/catalogue only |
| Labels | Configurable information overlays | Settings/catalogue only |
| Local verification testing | Local test indicator only | Settings/catalogue only |

## Label catalogue

FPS, TPS, physics TPS, frame time, attempts, session time, speed multiplier, player X/Y, player mode, level name, practice status, input display, memory usage and display refresh rate. Shared scale and opacity controls are included in settings metadata.

## Level access and verification

- Copy tools may support levels owned by the user or explicitly authorized for editing.
- Do not bypass passwords or access restrictions on protected levels.
- Verification tools must remain local/testing-only and must not falsify online verification, completion records or upload status.

## Implementation order

1. **Foundation and diagnostics:** label framework, FPS/frame-time display, attempt/session labels, safe-mode state and warning popup.
2. **Visual debugging:** hitboxes, hitbox trail, trajectory, layout mode, no glow and no shaders.
3. **Practice workflow:** StartPos manager/switcher, smart StartPos, local noclip and bounded noclip.
4. **Timing/audio:** speedhack, audio synchronization, pitch shifter, physics TPS target, FPS target and frame extrapolation.
5. **Validation:** test every module independently on Android ARM64 and Windows, including interactions between modules.

## Technical constraints

- Physics TPS is not the same as display refresh rate or rendered FPS. The game engine may clamp or ignore requested values.
- An FPS target cannot create display refresh capability the device does not have; report measured and effective rates separately.
- Frame extrapolation and custom timing can cause instability or desynchronization and must be isolated behind safe mode.
- Use public Geode SDK APIs and compatible open-source code only.
- Keep overlays in the correct world/screen coordinate space and make Android touch targets accessible.
- Never label a feature as working until it has been built and tested in-game.

## Remaining interface work

- [ ] Custom PNG icon import, crop and reset
- [ ] Independent panel/text/accent colors
- [ ] Shortcut positioning and anchoring
- [ ] Per-overlay position, scale, opacity and visibility
- [ ] Profiles, import/export and reset-to-defaults

## Current limitations

0.3.0 expands Geode settings and the unified feature catalogue. It does not yet implement the listed runtime gameplay hooks. The branch has not been compiled or tested in Geometry Dash in the current environment.
