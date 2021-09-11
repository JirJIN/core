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

Avert your eyes, I'm only putting this on GitHub because that's the easiest way
for me to develop this on both Linux and Windows, I don't really know what I am doing
