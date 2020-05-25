
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

	string2guido (xml: string, genBars: boolean)		{ return this.fLibrary.string2guido (xml, genBars); }
	xmlStringTranspose (xml: string, interval: number)	{ return this.fLibrary.xmlStringTranspose (xml, interval); }
}
