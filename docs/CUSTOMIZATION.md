# Violetmod customization

Open **Geode > Violetmod > Settings** to change these options. Geode persists setting values between sessions.

## Interface and shortcut

| Setting | What it changes |
|---|---|
| Color theme | Violet, Midnight, Pink, Blue, Mint, Amber, or Monochrome accent |
| Interface scale | Scales popup text and controls |
| Compact layout | Reduces spacing and text size |
| Show subtitle | Toggles the popup subtitle |
| Menu button text | Changes the shortcut label; short text works best |
| Menu button size | Changes shortcut label scale |
| Menu button opacity | Changes shortcut transparency |
| Show menu shortcut | Enables/disables the main-menu entry |
| Popup width / height | Changes popup dimensions |
| Popup animations | Toggles the popup's elastic entrance |

## Feature defaults

The Frame Counter, Hitbox Viewer, Show Trajectory, practice utilities, level utilities, session statistics, and notifications have preference switches in the settings metadata. At this stage these are **configuration placeholders** for planned modules; they do not create those features by themselves.

## Custom icon

The main-menu shortcut currently uses a customizable text glyph. Set **Menu button text** to a short character or letter to personalize it. An arbitrary image-file picker and in-game icon editor are not implemented yet. Those require a safe asset-import/storage flow and will be added separately.

## Planned expanded customization

- Custom image icon import, crop, scale, and reset
- Independent primary, secondary, panel, and text colors
- Shortcut position and anchoring
- Per-overlay scale, opacity, position, and visibility
- Profiles/presets with import/export
- Reset individual sections or all Violetmod settings

## Compatibility

Settings are registered through Geode's mod metadata and persisted by Geode. Changes that affect the main-menu shortcut are applied when the menu is created again (re-enter the main menu or restart the game if needed).
