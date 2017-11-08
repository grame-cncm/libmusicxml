/*
 MusicXML Library
 Copyright (C) Grame 2006-2013
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
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
#include "xml2guidovisitor.h"
#include "xmlpart2guido.h"
#include "tree_browser.h"

using namespace std;

namespace MusicXML2
{
    
    //______________________________________________________________________________
    xml2guidovisitor::xml2guidovisitor(bool generateComments, bool generateStem, bool generateBar) :
    fGenerateComments(generateComments), fGenerateStem(generateStem),
    fGenerateBars(generateBar), fGeneratePositions(true),
    fCurrentStaffIndex(0), previousStaffHasLyrics(false), fCurrentAccoladeIndex(0)
    {}
    
    //______________________________________________________________________________
    Sguidoelement xml2guidovisitor::convert (const Sxmlelement& xml)
    {
        Sguidoelement gmn;
        if (xml) {
            tree_browser<xmlelement> browser(this);
            browser.browse(*xml);
            gmn = current();
        }
        return gmn;
    }
    
    //______________________________________________________________________________
    // the score header contains information like title, author etc..
    // it must be written only once, at the beginning of the first guido voice
    // thus the function clears the data when flushed so that further calls do nothing
    //______________________________________________________________________________
    void xml2guidovisitor::flushHeader ( scoreHeader& header )
    {
        if (header.fTitle) {
            Sguidoelement tag = guidotag::create("title");
            string title = header.fTitle->getValue();
            size_t pos = title.find ('"');
            while (pos != string::npos) {
                title = title.replace (pos, 1, "'");
                pos = title.find ('"', pos);
            }
            tag->add (guidoparam::create(title));
            add (tag);
            header.fTitle = (void*)0;
        }
        vector<S_creator>::const_iterator i;
        for (i=header.fCreators.begin(); i!=header.fCreators.end(); i++) {
            string type = (*i)->getAttributeValue("type");
            if ((type == "Composer") || (type == "composer")) {
                Sguidoelement tag = guidotag::create("composer");
                tag->add (guidoparam::create((*i)->getValue()));
                tag->add (guidoparam::create("dy=4hs", false));
                add (tag);
            }
        }
        header.fCreators.clear();
    }
    
    //______________________________________________________________________________
    // the part header contains information like part name
    // it must be written only once, at the beginning of the corresponding guido voice
    // thus the function clears the data when flushed so that further calls do nothing
    //______________________________________________________________________________
    void xml2guidovisitor::flushPartHeader ( partHeader& header )
    {
        if ( (header.visited==false) && header.fPartName.size()) {
            Sguidoelement tag = guidotag::create("instr");
            stringstream s1, s2;
            string instr = header.fPartName;
            size_t offset = instr.size() * 2;
            
            s1 << "dx=-" << offset << "hs";
            tag->add (guidoparam::create(instr));
            tag->add (guidoparam::create(s1.str(), false));
            tag->add (guidoparam::create("dy=-5hs", false));
            add (tag);
            
            tag = guidotag::create("systemFormat");
            tag->add (guidoparam::create(""));
            s2 << "dx=" << offset << "hs";
            tag->add (guidoparam::create(s2.str(), false));
            add (tag);
            header.visited = true;
        }
    }
    
    void xml2guidovisitor::flushPartGroup (std::string partID)
    {
        //cerr<< "Entering flushPartGroup with ID "<<partID<<endl;
        /// Add groupings (accolade and barformat)
        // search if this part ID exists in any grouping
        // Guido Limitation: One \accol tag per staff ONLY (for nested definitions)
        // IDEA: Treat the FIRST occurence of partID in grouping and get rid of it.
        partGroup* partGroupIt = find_first_of_partID_inGroup(partID);
        
        if (partGroupIt != NULL)
        {
            /// something was found. Generate Accolades and BarFormat if any
            
            if (partGroupIt->bracket)
            {
                std::string accolParams = "id=1, range="+partGroupIt->guidoRange;
                
                Sguidoelement tag3 = guidotag::create("accol");
                tag3->add (guidoparam::create(accolParams, false));
                add (tag3);
            }
            
            if (partGroupIt->barlineGrouping)
            {
                std::string barformatParams = "style= \"system\", range="+partGroupIt->guidoRange;
                
                Sguidoelement tag4 = guidotag::create("barFormat");
                tag4->add (guidoparam::create(barformatParams, false));
                add (tag4);
            }
            
            /// Make sure that this group pattern won't be visited next time
            partGroupIt->visited = true;
        }
    }
    
    //______________________________________________________________________________
    void xml2guidovisitor::visitStart ( S_score_partwise& elt )
    {
        Sguidoelement chord = guidochord ::create();
        start(chord);
    }
    
    //______________________________________________________________________________
    void xml2guidovisitor::visitStart ( S_movement_title& elt )		{ fHeader.fTitle = elt; }
    void xml2guidovisitor::visitStart ( S_creator& elt )			{ fHeader.fCreators.push_back(elt); }
    //void xml2guidovisitor::visitStart ( S_score_part& elt )			{ fCurrentPartID = elt->getAttributeValue("id"); }
    //void xml2guidovisitor::visitStart ( S_part_name& elt )			{ fPartHeaders[fCurrentPartID].fPartName = elt; }
    
    //______________________________________________________________________________
    void xml2guidovisitor::visitStart ( S_part& elt )
    {
        partsummary ps;
        xml_tree_browser browser(&ps);
        browser.browse(*elt);
        
        smartlist<int>::ptr voices = ps.getVoices ();
        int targetStaff = 0xffff;	// initialized to a value we'll unlikely encounter
        bool notesOnly = false;
        rational currentTimeSign (0,1);
        
        staffClefMap.clear();
        timePositions.clear();
        
        // browse the parts voice by voice: allows to describe voices that spans over several staves
        for (unsigned int i = 0; i < voices->size(); i++) {
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
            
            Sguidoelement seq = guidoseq::create();
            push (seq);
            
            Sguidoelement tag = guidotag::create("staff");
            tag->add (guidoparam::create(fCurrentStaffIndex, false));
            add (tag);
            
            //// Add staffFormat if needed
            // Case1: If previous staff has Lyrics, then move current staff lower to create space: \staffFormat<dy=-5>
            int stafflines = elt->getIntValue(k_staff_lines, 0);
            
            if ((previousStaffHasLyrics)||stafflines)
            {
                Sguidoelement tag2 = guidotag::create("staffFormat");
                if (previousStaffHasLyrics)
                {
                    tag2->add (guidoparam::create("dy=-5", false));
                }
                
                if (stafflines>0)
                {
                    stringstream staffstyle;
                    staffstyle << "style=\"" << stafflines<<"-line\"";
                    tag2->add (guidoparam::create(staffstyle.str(),false));
                }
                add (tag2);
            }
            ////
            
            flushHeader (fHeader);
            flushPartHeader (fPartHeaders[elt->getAttributeValue("id")]);
            flushPartGroup  (elt->getAttributeValue("id"));
            
            //// Add Accolade if countStaves on this Part is >1, and we are entering span
            if ((ps.countStaves()>1)&&(fCurrentStaffIndex>fCurrentAccoladeIndex))
            {
                int rangeEnd = fCurrentStaffIndex + ps.countStaves() - 1;

				stringstream accol;
				accol << "id=" << fCurrentAccoladeIndex << ", range=\"" << fCurrentStaffIndex << "-" << rangeEnd << "\"";
				stringstream barformat;
				barformat << "style= \"system\", range=\"" << fCurrentStaffIndex << "-" << rangeEnd << "\"";
				
                Sguidoelement tag3 = guidotag::create("accol");
                tag3->add (guidoparam::create(accol.str(), false));
                add (tag3);
                
                // add new barFormat tag
                Sguidoelement tag4 = guidotag::create("barFormat");
                tag4->add (guidoparam::create(barformat.str(), false));
                add (tag4);
				
                fCurrentAccoladeIndex = rangeEnd;
            }else {
                
                // if we have multiple staves, and this staff has NO barFormat, then we should add one like \barFormat<style="system", range="1">
                // Otherwise there'll be no bar lines!!!
                if (checkLonelyBarFormat(fCurrentStaffIndex))
                {
					stringstream barformat;
					barformat << "style= \"system\", range=\"" << fCurrentStaffIndex << "\"";
                    Sguidoelement tag4 = guidotag::create("barFormat");
                    tag4->add (guidoparam::create(barformat.str(), false));
                    add (tag4);
                }
            }
            
            ////
            
            xmlpart2guido pv(fGenerateComments, fGenerateStem, fGenerateBars);
            pv.generatePositions (fGeneratePositions);
            xml_tree_browser browser(&pv);
            pv.initialize(seq, targetStaff, fCurrentStaffIndex, targetVoice, notesOnly, currentTimeSign);
            pv.staffClefMap = staffClefMap;
            pv.timePositions = timePositions;
            browser.browse(*elt);
            pop();
            currentTimeSign = pv.getTimeSign();
            previousStaffHasLyrics = pv.hasLyrics();
            staffClefMap = pv.staffClefMap;
            timePositions = pv.timePositions;
        }
    }
    
    //______________________________________________________________________________
    void xml2guidovisitor::addPosition	( Sxmlelement elt, Sguidoelement& tag, int yoffset)
    {
        float posx = elt->getAttributeFloatValue("default-x", 0) + elt->getAttributeFloatValue("relative-x", 0);
        if (posx) {
            posx = (posx / 10) * 2;   // convert to half spaces
            stringstream s;
            s << "dx=" << posx << "hs";
            tag->add (guidoparam::create(s.str(), false));
        }
        float posy = elt->getAttributeFloatValue("default-y", 0) + elt->getAttributeFloatValue("relative-y", 0);
        if (posy) {
            posy = (posy / 10) * 2;   // convert to half spaces
            posy += yoffset;		  // anchor point convertion (defaults to upper line in xml)
            stringstream s;
            s << "dy=" << posy << "hs";
            tag->add (guidoparam::create(s.str(), false));
        }
    }
    
    //______________________________________________________________________________
    void xml2guidovisitor::addPosition	( Sxmlelement elt, Sguidoelement& tag, int yoffset, int xoffset)
    {
        float posx = elt->getAttributeFloatValue("default-x", 0) + elt->getAttributeFloatValue("relative-x", 0);
        if (posx) {
            posx = (posx / 10) * 2;   // convert to half spaces
            posx += xoffset;
            stringstream s;
            s << "dx=" << posx << "hs";
            tag->add (guidoparam::create(s.str(), false));
        }
        float posy = elt->getAttributeFloatValue("default-y", 0) + elt->getAttributeFloatValue("relative-y", 0);
        if (posy) {
            posy = (posy / 10) * 2;   // convert to half spaces
            posy += yoffset;		  // anchor point convertion (defaults to upper line in xml)
            stringstream s;
            s << "dy=" << posy << "hs";
            tag->add (guidoparam::create(s.str(), false));
        }
    }
    
    //______________________________________________________________________________
    void xml2guidovisitor::addPosY	( Sxmlelement elt, Sguidoelement& tag, int yoffset, int ymultiplier = 1.0)
    {
        float posy = elt->getAttributeFloatValue("default-y", 0) + elt->getAttributeFloatValue("relative-y", 0);
        if (posy) {
            posy = (posy / 10) * 2;   // convert to half spaces
            posy += yoffset;		  // anchor point convertion (defaults to upper line in xml)
            posy = posy * ymultiplier;
            stringstream s;
            s << "dy=" << posy << "hs";
            tag->add (guidoparam::create(s.str(), false));
        }
    }
    
    void xml2guidovisitor::addPlacement	( Sxmlelement elt, Sguidoelement& tag)
    {
        string placement = elt->getAttributeValue("placement");
        
        if (placement.size()) {
            
            stringstream s;
            s << "position=\"" << placement << "\"";
            tag->add (guidoparam::create(s.str(), false));
        }
    }
    
}

