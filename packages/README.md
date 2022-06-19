Welcome to the libmusicxml project
======================================================================

[Grame](http://www.grame.fr) - Centre National de Création Musicale
----------------------------------------------------------------------

The MusicXML library is a portable C++ library designed close to the MusicXML format and intended to facilitate MusicXML support.

[MusicXML](http://www.musicxml.com/) is a music interchange format designed for notation, analysis, retrieval, and performance applications. The MusicXML format has been initialy developped by Recordare. It is open for use by anyone under a royalty-free license.


The libmusicxml library runs on the main operating systems: Linux, MacOS, iOS, Windows, Android. It can also be compiled to JavaScript using [emscripten](http://emscripten.org) to be embedded in a web page.

This package includes the musicxml library and 2 converters:

- xml2guido: converts the MusicXML format to [Guido](http://guidodoc.grame.fr/);
- xml2midi: converts the MusicXML format to MIDI;

Previous included converters are now available from the [musicformats](https://github.com/jacques-menu/musicformats) repository:
- xml2ly (Jacques Menu) : converts the MusicXML format to [Lilypond](http://lilypond.org/)
- xml2brl (Jacques. Menu) : converts the MusicXML format to [braille music]


---
Many thanks to Arsia Cont who contributed to the Guido converter in a significant way.
