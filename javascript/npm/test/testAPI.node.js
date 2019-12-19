
"use strict";

const fs = require ("fs");
const lib = require ("../interface");
const run = require ("./testAPI");

var content = fs.readFileSync('test.xml', 'utf8');

var engine = new lib().initialize ("../libmusicxml").then (doit);

function log(str) 		{ console.log (str); }
function svg(str, name) { 
	fs.writeFileSync (name + ".svg", str, "utf8" );
	log ("  SVG flushed to " + name);
}

function doit (engine) {
 	run ( engine, log, content.toString() );
}
