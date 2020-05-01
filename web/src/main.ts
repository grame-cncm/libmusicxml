
///<reference path="lib/libmusicxml.ts"/>
///<reference path="converter.ts"/>


//------------------------------------------------------------------------
function xmlversion (lxml: libmusicxml) : void {
	console.log( "LibMusicXML version " + lxml.libVersionStr());
	$("#lxmlversion").html (lxml.libVersionStr());
	console.log( "MusicXML to GMN converter version " + lxml.musicxml2guidoVersionStr());
	$("#xml2guidoversion").html (lxml.musicxml2guidoVersionStr());
}

const lxml = new libmusicxml();
const converter = new XMLConverter();
lxml.initialise ().then ( 
	(): void => {
		xmlversion(lxml);
		converter.initialize (lxml);
	}
);

