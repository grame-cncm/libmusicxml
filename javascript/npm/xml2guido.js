
"use strict";

const fs = require ("fs");
const lib = require ("./interface");

var engine = new lib().initialize ("./libmusicxml").then (xml2guido);


function xml2guido (lib) {
	process.argv.forEach((val, index) => {
		var content = fs.readFileSync (val, 'utf8');
		console.log (lib.string2guido (content.toString(), true));
	});
}

module.exports = xml2guido;
