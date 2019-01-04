
var fs = require("fs");

eval(fs.readFileSync("../libmusicxml.js").toString());
var module = new MusicXMLModule();
var xml = new module.libMusicXMLAdapter();
console.log ("libMusicXML version   : " + xml.libVersionStr());
console.log ("MusicXML2Guido version: " + xml.musicxml2guidoVersionStr());

var args = typeof process !== 'undefined' ?  process.argv : [];
for (var i=2; i<args.length; i++) {
	let file = args[i];
	var content = fs.readFileSync(file);
	console.log (file + " ==>");
	console.log (xml.string2guido (content, false));
}
