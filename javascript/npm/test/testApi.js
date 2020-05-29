

//----------------------------------------------------------------------------
// Misc. functions
//----------------------------------------------------------------------------
function misc (lib, log )
{
	log( "Versions :");
	log ("  libmusicxml version           " + lib.libVersion());
	log ("  libmusicxml version str       " + lib.libVersionStr());

	log ("  musicxml2guido version        " + lib.musicxml2guidoVersion());
	log ("  musicxml2guido version str    " + lib.musicxml2guidoVersionStr());
	log ("  musicxml2lilypond version     " + lib.musicxml2lilypondVersion());
	log ("  musicxml2lilypond version str " + lib.musicxml2lilypondVersionStr());
	log ("  musicxml2braille version      " + lib.musicxml2brailleVersion());
	log ("  musicxml2braille version str  " + lib.musicxml2brailleVersionStr());
}

function gmnconvert (xml, lib, log )
{
	log( "MusicXML -> GMN :");
	log ("  string2guido  :")
	log (lib.string2guido (xml, false));
	log ("  xmlStringTranspose :");
	log (lib.string2guido (lib.xmlStringTranspose(xml, 7), false));
}

function lilyconvert (xml, lib, log )
{
	log( "MusicXML -> Lilypond :");
	log ("  string2lily  :");
	log (lib.string2lily (xml, ""));
	log ("  string2lily help :");
	log (lib.string2lily ("", "-help"));
}

function brailleconvert (xml, lib, log )
{
	log( "MusicXML -> Braille :");
	log ("  string2braille  :")
	log (lib.string2braille (xml, ""));
}


function run (engine, log, xml) {
 	misc 	(engine, log);
	if (xml) {
		log( "<br /><hr />\n");
		gmnconvert (xml, engine, log );
		log( "<br /><hr />\n");
		lilyconvert (xml, engine, log );
		// log( "<br /><hr />\n");
		// brailleconvert (xml, engine, log );
	}
}

if ((typeof process !== 'undefined') && (process.release.name === 'node')) {
	module.exports = run;
}
