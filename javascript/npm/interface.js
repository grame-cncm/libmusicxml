
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

	string2guido (xml, genBars)			{ return this.fLibrary.string2guido (xml, genBars); }
	xmlStringTranspose (xml, interval)	{ return this.fLibrary.xmlStringTranspose (xml, interval); }
}


if ((typeof process !== 'undefined') && (process.release.name === 'node')) {
	module.exports = libmusicxml;
}
