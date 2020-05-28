
///<reference path="lib/libmusicxml.ts"/>
///<reference path="converter.ts"/>


//------------------------------------------------------------------------
function xmlversion (lxml: libmusicxml) : void {
	console.log( "LibMusicXML version " + lxml.libVersionStr());
	$("#lxmlversion").html (lxml.libVersionStr());
	console.log( "MusicXML to GMN converter version " + lxml.musicxml2guidoVersionStr());
	console.log( "MusicXML to Lilypond converter version " + lxml.musicxml2lilypondVersionStr());
// 	console.log( "MusicXML to Braille converter version " + lxml.musicxml2brailleVersionStr());
	$("#version").html (lxml.libVersionStr());
	$("#guidoversion").html (lxml.musicxml2guidoVersionStr());
	$("#lilyversion").html (lxml.musicxml2lilypondVersionStr());
	// $("#brailleversion").html (lxml.musicxml2brailleVersionStr());
}

const lxml = new libmusicxml();
const converter = new XMLConverter();
lxml.initialise ().then ( 
	(): void => {
		xmlversion(lxml);
		converter.initialize (lxml);
	}
);

document.domain = "grame.fr";
