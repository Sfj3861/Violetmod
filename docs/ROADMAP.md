# Violetmod roadmap

## Current status: 0.4.0 - diagnostics runtime foundation

- [x] Geode project skeleton and metadata
- [x] Main-menu entry point and navigable popup
- [x] Persistent interface settings
- [x] Feature preference catalogue for requested gameplay, timing, safety and label modules
- [x] Live FPS estimate, smoothed frame-time and session-time labels
- [x] Per-level diagnostic state reset and rolling PlayLayer TPS measurement
- [x] Attempts, clicks, click-rate, player coordinates, level name and practice-state labels
- [x] Configurable overlay position, line spacing and refresh interval
- [x] Practice-only speedhack gated by Safe Mode; active/inactive state shown honestly
- [x] Explicit unavailable state for independent physics TPS instead of duplicating PlayLayer TPS
- [x] In-game safe-mode status indicator
- [ ] Compile and validate against Geode SDK 5.6.1 on Android ARM64 and Windows
- [x] Add approximate player-bounds overlay, historical bounds trail and world-space movement trace
- [ ] Validate visual debug overlays in-game on Android ARM64 and Windows
- [ ] Implement true collision geometry and physics-based future trajectory prediction
- [ ] Implement remaining runtime modules and mark each only after build/runtime testing

## Requested modules

| Module | Scope | Status |
|---|---|---|
| Speedhack | Adjustable game speed in Practice Mode only | Implemented in runtime; compile/device validation pending |
| Speedhack audio | Keep audio synchronized with speed changes | Settings/catalogue only |
| Noclip | Local practice collision override | Settings/catalogue only |
| Limit Noclip | Constrain local noclip usage | Settings/catalogue only |
| Layout mode | Layout/practice display mode | Settings/catalogue only |
| Show hitboxes | Player rendered-bounds visualization (approximation, not true collision geometry) | Runtime overlay implemented; build/device validation pending |
| Show hitbox trail | Recent historical outlines of rendered player bounds | Runtime overlay implemented; build/device validation pending |
| Show trajectory | Recent movement path in PlayLayer coordinates; not future physics prediction | Runtime overlay implemented; build/device validation pending |
| Smart StartPos | Assisted start-position workflow | Settings/catalogue only |
| StartPos switcher | Switch saved start positions | Settings/catalogue only |
| All attempts | Display attempt history | Settings/catalogue only |
| No glow / No shaders | Visual-effect toggles | Settings/catalogue only |
| Pitch shifter | Audio pitch adjustment | Settings/catalogue only |
| Frame extrapolation | Experimental visual interpolation | Settings/catalogue only |
| Physics TPS | Requested simulation update rate | Settings/catalogue only |
| FPS lock | Requested render target | Settings/catalogue only |
| Safe mode | Disable experimental modules | State indicator implemented; module gating pending |
| Safe mode popup | Explain safety fallback state | Settings/catalogue only |
| Auto safe mode | Fallback after detected instability | Settings/catalogue only |
| Labels | FPS, frame-time, TPS, attempts, session, click rate, coordinates and status | Expanded runtime overlay implemented; compile/device validation pending |
| All Copyable | Level-copy utility for the 2.2 copy system | Settings/catalogue only |
| Verify Hack | Permit eligible unverified uploads and append an explicit `UNVERIFIED` marker to the level description | Settings/catalogue only |

## Label catalogue

FPS, TPS, physics TPS, frame time, attempts, session time, speed multiplier, player X/Y, player mode, level name, practice status, input display, memory usage and display refresh rate. Shared scale and opacity controls are included in settings metadata.

## Level access and verification

- **All Copyable:** add a level-copy utility compatible with Geometry Dash 2.2's current level-copy system. As noted, 2.2 levels do not use the old copy-password restriction, so this is not a password-bypass feature.
- **Verify Hack:** the requested behavior is to permit uploading a level without completing the normal in-game verification, and automatically append `UNVERIFIED` to its description. It must never claim the level is verified or fabricate a legitimate completion.

## Implementation order

1. **Foundation and diagnostics:** label framework, FPS/frame-time display, attempt/session labels, safe-mode state and warning popup.
2. **Visual debugging:** approximate player bounds, historical bounds trail and recent world-space movement trace are now implemented. True collision geometry, future physics prediction, layout mode, no glow and no shaders remain pending.
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

0.4.0 adds a first runtime diagnostics overlay (FPS, frame time, session timer) and a safe-mode status indicator. The visual debug overlay now draws approximate rendered player bounds, historical bounds outlines and a recent movement trace. It does not claim to expose the engine's exact collision geometry or predict future physics. Other gameplay hooks in the catalogue remain unimplemented. The branch has not been compiled or tested in Geometry Dash in the current environment.
