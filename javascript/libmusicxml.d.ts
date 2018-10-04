declare var MusicXMLModule : any;

// libMusicXML
//--------------------------------------------------------------

interface TConversion {
    str:	string;
    err:	number;
}

interface libMusicXMLAdapter {
    constructor: libMusicXMLAdapter;
        
    libVersion () : 	number;
    libVersionStr():	string;
    
    musicxml2guidoVersion(): 	number;
    musicxml2guidoVersionStr(): string;

	string2guido (xml: string, generateBars: boolean): 	string;
}
