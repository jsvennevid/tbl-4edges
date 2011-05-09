4 EDGES
=======

A PlayStation 2 Demo by The Black Lotus

Full Credits
------------

emoon

   Main coding, engine coding, effects, converter tools, system coding and camera work.

Chip

   System coding, FFT generator, SH renderer (did not make it to the demo) and cdversion.

Nico

   Landscape graphics.

Louie500

   The rest of all graphics.

Little Bitchard

   Kick ass music.

Info
----

Hardware required: Playstation 2 compatible hardware able to boot homebrew CDs.

NTSC capable display.

Notice that this demo is only released as an ISO image for now as there
are some problems streaming the music over host with ps2link. 

About the code
--------------

Some general info about the code: Some parts of the code is a bit messy and the loading from
CD is kinda slow (my fault :) / emoon) as it's done file per file on startup. Overall the code
could be written in a way more optimized fashion but in general the (main) CPU usage is about 5% of 
the frame time and the rest is mostly spent inside the VU1 code and on the GS. 

But in the end the target of steady 60 fps was achieved.

Our new version of the engine is a full rewrite (in C99 instead of C++) so the code doesn't represent
what we actually have now.

Hopefully you will find the code somewhat useful

License
-------
Copyright (c) 2004-2006 Jesper Svennevid, Daniel Collin

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Compiling
--------

The build system in use is tundra, found at:
https://github.com/deplinenoise/tundra

When building for PlayStation 2 you also need PS2SDK, found here:
https://github.com/ps2dev/ps2sdk

Data
----
The final data can be found here:

https://github.com/downloads/jsvennevid/tbl-4edges/the_black_lotus_-_4_edges_-_iso.zip

Source data is not available for the public release.

Special thanks
--------------

Sparky (Morten Mikkelsen) for the post effect help and code to move channels in
the frame buffer (taken from sps2 for Ps2 linux)

jbit for fixing and helping out with the adpcm music playing and the
rest of the #ps2dev people: Neov, Oobles, Tyr, Boomint, adresd, Gillian,
Raizor, Nagra, gorim and the rest of the crew. 

