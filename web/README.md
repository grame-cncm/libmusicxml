# LibMusicXML Online Tools

The [libmusicxml online tools](https://libmusicxml.grame.fr) is based on the [libmusicxml library](git+https://github.com/grame-cncm/libmusicxml.git), a library to support the MusicXML format.


## Features

The libmusicxml online tools provides conversion from MusicXML to various music score representation formats:

- [Guido Music Notation](https://guidodoc.grame.fr)
- [Lilypond](http://lilypond.org/)
- [Braille Music Notation](https://en.wikipedia.org/wiki/Braille_music)


## Building

Firstly ensure that you have [Git](https://git-scm.com/downloads) and [Node.js](https://nodejs.org/) installed.

Clone a copy of the repo then change to the `web` directory:

~~~~~~
git clone https://github.com/grame-cncm/libmusicxml.git
cd web
~~~~~~

Install dev dependencies:
~~~~~~
npm install
~~~~~~

To build everything: 
~~~~~~
make
~~~~~~

To test, launch an http server from the ../docs folder.  
Using python3;
~~~~~~
python -m http.server
~~~~~~
the server should be available from [http://localhost:8000](http://localhost:8000)

