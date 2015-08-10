# concoord-SDL2-gfx

A tool for converting .svg files to a header containing short arrays to 
be passed to libsdl2-gfx-dev as a polygon's point's X & Y coordinates.

Concoord has no external dependencies and compiles with
```
g++ -o concoord concoord.cpp
```

Concoord respects the standard argc argv[] conventions of the *nix 
terminal. Convert one or more files like
```
./concoord /path/to/svg1.svg ../to/svg2.svg
```
to return an accompanying
```
/path/to/svg1.h ../to/svg2.h
```
in the original files' respective directories.

Concoord is licensed as zlib, see more in LICENSE.
