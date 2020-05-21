
"use strict";

//----------------------------------------------------------------------------
// the libMusicXML interface
//----------------------------------------------------------------------------
class libmusicxml {
	constructor() {
		this.fLibrary = 0;
	}
	
	//------------------------------------------------------------------------
	// async initialization
	initialize (lib) {
		if ((typeof process !== 'undefined') && (process.release.name === 'node')) {
			var module = require (lib);
			return new Promise ( (success) => {
					module().then ( (instance) => {
					this.moduleInit ( instance );
					success ( this ); 
				});
			});
		}
		else {
			var module = MusicXMLModule();
			return new Promise ( (success, failure) => {
				module['onRuntimeInitialized'] = () => {
				this.moduleInit ( module );
				success ( this ); 
				}
			});
		}
	}
	
	//------------------------------------------------------------------------
	// async initialization
	moduleInit ( module ) {
		this.fLibrary = new module.libMusicXMLAdapter();
	}
		
	//------------------------------------------------------------------------
	// libMusicXML interface
	libVersion ()						{ return this.fLibrary.libVersion (); }
	libVersionStr ()					{ return this.fLibrary.libVersionStr (); }

	musicxml2guidoVersion ()			{ return this.fLibrary.musicxml2guidoVersion (); }
	musicxml2guidoVersionStr ()			{ return this.fLibrary.musicxml2guidoVersionStr (); }

	musicxml2lilypondVersion ()			{ return this.fLibrary.musicxml2lilypondVersion (); }
	musicxml2lilypondVersionStr ()		{ return this.fLibrary.musicxml2lilypondVersionStr (); }

	musicxml2brailleVersion ()			{ return this.fLibrary.musicxml2brailleVersion (); }
	musicxml2brailleVersionStr ()		{ return this.fLibrary.musicxml2brailleVersionStr (); }

	string2guido (xml, genBars)			{ return this.fLibrary.string2guido (xml, genBars); }
	string2lily (xml, options)			{ return this.fLibrary.string2lily (xml, options); }
	string2braille (xml, options)		{ return this.fLibrary.string2braille (xml, options); }

	xmlStringTranspose (xml, interval)	{ return this.fLibrary.xmlStringTranspose (xml, interval); }
}


if ((typeof process !== 'undefined') && (process.release.name === 'node')) {
	module.exports = libmusicxml;
}
