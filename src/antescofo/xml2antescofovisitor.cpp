/*
  Copyright (C) 2013 Thomas Coffy

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  thomas.coffy@gmail.com
  http://repmus.ircam.fr/antescofo
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>
#include <string>

#include "partsummary.h"
#include "rational.h"
#include "xml_tree_browser.h"
#include "xml2antescofovisitor.h"
#include "xmlpart2antescofo.h"
#include "tree_browser.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
xml2antescofovisitor::xml2antescofovisitor(antescofowriter& _w, bool generateComments, bool generateStem, bool generateBar) :
	fGenerateComments(generateComments), fGenerateStem(generateStem),
	fGenerateBars(generateBar), fGeneratePositions(false),
	fCurrentStaffIndex(0),
	w(_w)
{}

//______________________________________________________________________________
Santescofoelement xml2antescofovisitor::convert (const Sxmlelement& xml)
{
	Santescofoelement as;
	if (xml) {
		tree_browser<xmlelement> browser(this);
		browser.browse(*xml);
		as = current();
	}
	return as;
}

//______________________________________________________________________________
// the score header contains information like title, author etc..
// it must be written only once, at the beginning of the first antescofo voice
// thus the function clears the data when flushed so that further calls do nothing
//______________________________________________________________________________
void xml2antescofovisitor::flushHeader ( scoreHeader& header )
{
	if (header.fTitle) {
		Santescofoelement tag = antescofotag::create("title");
		string title = header.fTitle->getValue();
		size_t pos = title.find ('"');
		while (pos != string::npos) {
			title = title.replace (pos, 1, "'");
			pos = title.find ('"', pos);
		}
		tag->add (antescofoparam::create(title));
		add (tag);
		header.fTitle = (void*)0;
	}
	vector<S_creator>::const_iterator i;
	for (i=header.fCreators.begin(); i!=header.fCreators.end(); i++) {
		string type = (*i)->getAttributeValue("type");
		if ((type == "Composer") || (type == "composer")) {
			Santescofoelement tag = antescofotag::create("composer");
			tag->add (antescofoparam::create((*i)->getValue()));
			//tag->add (antescofoparam::create("dy=4hs", false));
			add (tag);
		}
	}
	header.fCreators.clear();
}

//______________________________________________________________________________
// the part header contains information like part name
// it must be written only once, at the beginning of the corresponding antescofo voice
// thus the function clears the data when flushed so that further calls do nothing
//______________________________________________________________________________
void xml2antescofovisitor::flushPartHeader ( partHeader& header )
{
	if (header.fPartName && header.fPartName->getValue().size()) {
		Santescofoelement tag = antescofotag::create("instr");
		stringstream s1, s2;
		string instr = header.fPartName->getValue();
//		int offset = instr.size() * 2;

	/*	
		s1 << "dx=-" << offset << "hs";
		tag->add (antescofoparam::create(instr));
		tag->add (antescofoparam::create(s1.str(), false));
		tag->add (antescofoparam::create("dy=-5hs", false));
		add (tag);

		tag = antescofotag::create("systemFormat");
		tag->add (antescofoparam::create(""));
		s2 << "dx=" << offset << "hs";
		tag->add (antescofoparam::create(s2.str(), false));
		add (tag);
	*/
		header.fPartName = (void*)0;
	}
}

//______________________________________________________________________________
void xml2antescofovisitor::visitStart ( S_score_partwise& elt )
{
    Santescofoelement chord = antescofochord ::create();
    start(chord);
}

//______________________________________________________________________________
void xml2antescofovisitor::visitStart ( S_movement_title& elt )		{ fHeader.fTitle = elt; }
void xml2antescofovisitor::visitStart ( S_creator& elt )			{ fHeader.fCreators.push_back(elt); }
void xml2antescofovisitor::visitStart ( S_score_part& elt )			{ fCurrentPartID = elt->getAttributeValue("id"); }
void xml2antescofovisitor::visitStart ( S_part_name& elt )			{ fPartHeaders[fCurrentPartID].fPartName = elt; }

//______________________________________________________________________________
void xml2antescofovisitor::visitStart ( S_part& elt )
{
	partsummary ps;
	xml_tree_browser browser(&ps);
	browser.browse(*elt);
	
	// check we want to convert this part 
	string partname =	elt->getAttributeValue("id");
	bool badpart = w.write_parts.empty() ? false : true;
	for (vector<string>::const_iterator v = w.write_parts.begin(); v != w.write_parts.end(); v++) {
		if (*v == partname)
			badpart = false;
	}
	if (badpart)
		return;

	smartlist<int>::ptr voices = ps.getVoices ();
	int targetStaff = 0xffff;	// initialized to a value we'll unlikely encounter
	bool notesOnly = false;
	rational currentTimeSign (0,1);
	// browse the parts voice by voice: allows to describe voices that spans over several staves
	for (unsigned int i = 0; i < voices->size(); i++) {
		// check we want to convert this voice
		bool badvoice = w.write_voices.empty() ? false : true;
		for (vector<string>::const_iterator v = w.write_voices.begin(); v != w.write_voices.end(); v++) {
			stringstream ss;
			ss << i+1;
 			if (*v == ss.str())
				badvoice = false;
		}
		if (badvoice)
			continue;
		int targetVoice = (*voices)[i];
		int mainstaff = ps.getMainStaff(targetVoice);
		if (targetStaff == mainstaff) {
			notesOnly = true;
		}
		else {
			notesOnly = false;
			targetStaff = mainstaff;
			fCurrentStaffIndex++;
		}

		Santescofoelement seq = antescofoseq::create();
		push (seq);

		Santescofoelement tag = antescofotag::create("staff");
		tag->add (antescofoparam::create(fCurrentStaffIndex, false));
		add (tag);

		flushHeader (fHeader);
		flushPartHeader (fPartHeaders[elt->getAttributeValue("id")]);

		xmlpart2antescofo pv(w, fGenerateComments, fGenerateStem, fGenerateBars);

		pv.generatePositions (fGeneratePositions);
		xml_tree_browser browser(&pv);
		pv.initialize(seq, targetStaff, fCurrentStaffIndex, targetVoice, notesOnly, currentTimeSign);
		browser.browse(*elt);
		pop();
		currentTimeSign = pv.getTimeSign();
	}
}

//______________________________________________________________________________
void xml2antescofovisitor::addPosition	( Sxmlelement elt, Santescofoelement& tag, int yoffset)
{
}

}

