
///<reference path="libmusicxml.d.ts"/>

//----------------------------------------------------------------------------
// the libMusicXML interface
//----------------------------------------------------------------------------
class libmusicxml {
	private fLibrary: libMusicXMLAdapter;

    async initialise():Promise<any> { 
        var module = MusicXMLModule();
        return new Promise( (success: any, failure: any) => {
            module['onRuntimeInitialized'] = () => {
				this.fLibrary = new module.libMusicXMLAdapter();
                // this.moduleInit (module);
                success ( this ); 
                }
        });
    }
	
	//------------------------------------------------------------------------
	// async initialization
	// moduleInit ( module ) {
	// 	this.fLibrary = new module.libMusicXMLAdapter();
	// }
		
	//------------------------------------------------------------------------
	// libMusicXML interface
	libVersion ()    : number						{ return this.fLibrary.libVersion (); }
	libVersionStr () : string						{ return this.fLibrary.libVersionStr (); }

	musicxml2guidoVersion ()    : number			{ return this.fLibrary.musicxml2guidoVersion (); }
	musicxml2guidoVersionStr ()	: string			{ return this.fLibrary.musicxml2guidoVersionStr (); }

    musicxml2lilypondVersion(): number				{ return this.fLibrary.musicxml2lilypondVersion (); }
    musicxml2lilypondVersionStr(): string			{ return this.fLibrary.musicxml2lilypondVersionStr (); }

    musicxml2brailleVersion(): number				{ return this.fLibrary.musicxml2brailleVersion (); }
    musicxml2brailleVersionStr(): string			{ return this.fLibrary.musicxml2brailleVersionStr (); }

	string2guido (xml: string, genBars: boolean)		{ return this.fLibrary.string2guido (xml, genBars); }
	xmlStringTranspose (xml: string, interval: number)	{ return this.fLibrary.xmlStringTranspose (xml, interval); }

	string2lily (xml: string, options: string): 	string		{ return this.fLibrary.string2lily (xml, options); }
	string2braille (xml: string, options: string): 	string		{ return this.fLibrary.string2braille (xml, options); }
}
