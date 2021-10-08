# Core

Core is the only place where there should be OS/platform specific code,
so the code style is a little bit different from the rest of the engine

The comment for the functions will now be in the headers, and there is no
"core" prefix because everything is part of core

Core is considered its own module that is comprised of sub modules. All the submodules
are in their own folders

The goals
- Create OpenGL Context
- Handle events
- Create dialog boxes

## Known Issues

### X11
When minimizing a window with a dialog, and opening it again, the main window has a little animation
when popping up but the dialog just shows

When closing a dialog, for an instant there is corrupted video data on the window (If dialog'ed before the 2nd tick)
