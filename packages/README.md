Welcome to the libmusicxml project
======================================================================

[Grame](http://www.grame.fr) - Centre National de Création Musicale
----------------------------------------------------------------------

The MusicXML library is a portable C++ library designed close to the MusicXML format and intended to facilitate MusicXML support.

[MusicXML](http://www.musicxml.com/) is a music interchange format designed for notation, analysis, retrieval, and performance applications. The MusicXML format has been initialy developped by Recordare. It is open for use by anyone under a royalty-free license.


The libmusicxml library runs on the main operating systems: Linux, MacOS, iOS, Windows, Android. It can also be compiled to JavaScript (using [emscripten](http://emscripten.org)) to be embedded in a web page.

This package includes the musicxml library and 2 converters:

- xml2guido : converts the MusicXML format to [Guido](http://guidolib.sf.net/).
- xml2ly    : converts the MusicXML format to [Lilypond](http://lilypond.org/)

### Installation instructions

#### MacOS
You should copy the libmusicxml2.framework folder to `/Library/Frameworks`

---
