# PopDict

PopDict is a Qt-based desktop utility that queries a local DICT server (`localhost:2628`) to retrieve definitions or translations for the current clipboard content and displays the results in a pop-up window.

## Requirements
- CMake 3.19+ (for building)
- Qt 6.5 (Core, Widgets, and Network modules)
- Local DICT server running on port `2628`

## Usage
1. Install the binary.
2. Set up a shortcut to launch the application (e.g., `SUPER+C`).
3. Copy text to the clipboard.
4. Launch PopDict using your shortcut to view definitions or translations.