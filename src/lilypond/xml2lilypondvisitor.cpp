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

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <locale>         // std::locale, std::tolower

#include "partsummary.h"
#include "rational.h"
#include "xml_tree_browser.h"
#include "xml2lilypondvisitor.h"
#include "xmlpart2lilypond.h"
#include "tree_browser.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
xml2lilypondvisitor::xml2lilypondvisitor( translationSwitches& sw ) :
  fSwitches(sw), 
  fCurrentStaffIndex(0)
  {}

//______________________________________________________________________________
Slilypondelement xml2lilypondvisitor::convert (const Sxmlelement& xml )
{
  Slilypondelement ly;
  if (xml) {
    // create a browser on this xml2lilypondvisitor
    tree_browser<xmlelement> browser(this);
    // browse the xmlelement tree
    browser.browse(*xml);
    // the stack top contains the resulting lilypondelement tree
    ly = scoreStackTop();
  }
  return ly;
}

//______________________________________________________________________________
// the score header contains information like title, author etc..
// it must be written only once, at the beginning  of the generated code,
// thus the function clears the data 
// when flushed so that further calls do nothing
//______________________________________________________________________________
void xml2lilypondvisitor::flushGlobalHeader ( globalHeader& header )
{
  if (! fGlobalHeaderHasBeenFlushed) {
    Slilypondelement tag = lilypondcmd::create("header = {\n");
    addToScoreStack (tag);
  
    if (header.fScorePartwise) {
      Slilypondelement tag =
        lilypondcmd::create("%MusicXMl_version =", lilypondcmd::kWithoutBackslash);
      string musicXMLVersion = header.fScorePartwise->getAttributeValue("version");
      tag->add (lilypondparam::create(musicXMLVersion));
      addToScoreStack (tag);
      header.fScorePartwise = 0;
     }
  
    if (header.fWorkNumber) {
      Slilypondelement tag =
        lilypondcmd::create("%work_number =", lilypondcmd::kWithoutBackslash);
      string workNumber = header.fWorkNumber->getValue();
      tag->add (lilypondparam::create(workNumber));
      addToScoreStack (tag);
      header.fWorkNumber = 0;
    }
  
    if (header.fWorkTitle) {
      Slilypondelement tag =
        lilypondcmd::create("%work_title = ", lilypondcmd::kWithoutBackslash);
      string workTitle = header.fWorkTitle->getValue();
      unsigned int pos = workTitle.find ('"');
      while (pos != string::npos) {
        workTitle = workTitle.replace (pos, 1, "'");
        pos = workTitle.find ('"', pos);
      }
      tag->add (lilypondparam::create(workTitle));
      addToScoreStack (tag);
      header.fWorkTitle = 0;
    }
    
    if (header.fMovementNumber) {
      Slilypondelement tag =
        lilypondcmd::create("%movement_number =", lilypondcmd::kWithoutBackslash);
      string movementNumber = header.fMovementNumber->getValue();
      tag->add (lilypondparam::create(movementNumber));
      addToScoreStack (tag);
      header.fMovementNumber = 0;
    }
    
    if (header.fMovementTitle) {
      Slilypondelement tag =
        lilypondcmd::create("title = ", lilypondcmd::kWithoutBackslash);
      string movementTitle = header.fMovementTitle->getValue();
      unsigned int pos = movementTitle.find ('"');
      while (pos != string::npos) {
        movementTitle = movementTitle.replace (pos, 1, "'");
        pos = movementTitle.find ('"', pos);
      }
      tag->add (lilypondparam::create(movementTitle));
      addToScoreStack (tag);
      header.fMovementTitle = 0;
    }
    
    vector<S_creator>::const_iterator i1;
    for (i1=header.fCreators.begin(); i1!=header.fCreators.end(); i1++) {
      string type = (*i1)->getAttributeValue("type");
      /* USER
      if ((type == "Composer") || (type == "composer")) {
        Slilypondelement tag = lilypondcmd::create("composer");
        */
      std::transform(type.begin(), type.end(), type.begin(), ::tolower);
      Slilypondelement tag =
        lilypondcmd::create(type+" = ", lilypondcmd::kWithoutBackslash);
      tag->add (lilypondparam::create((*i1)->getValue())); // quotes
      addToScoreStack (tag);
    } // for
    header.fCreators.clear();
    
    if (header.fRights) {
      Slilypondelement tag =
        lilypondcmd::create("%rights =", lilypondcmd::kWithoutBackslash);
      string rights = header.fRights->getValue();
      tag->add (lilypondparam::create(rights));
      addToScoreStack (tag);
      header.fRights = 0;
    }
    
    vector<S_software>::const_iterator i2;
    for (i2=header.fSoftwares.begin(); i2!=header.fSoftwares.end(); i2++) {
      Slilypondelement tag =
        lilypondcmd::create("%software =", lilypondcmd::kWithoutBackslash);
      tag->add (lilypondparam::create((*i2)->getValue())); // quotes
      addToScoreStack (tag);
    }
    header.fSoftwares.clear();
    
    if (header.fEncodingDate) {
      Slilypondelement tag =
        lilypondcmd::create("%encoding_date =", lilypondcmd::kWithoutBackslash);
      string encodingDate = header.fEncodingDate->getValue();
      tag->add (lilypondparam::create(encodingDate));
      addToScoreStack (tag);
      header.fEncodingDate = 0;
    }
    
    tag = lilypondcmd::create("}\n\n", lilypondcmd::kWithoutBackslash);
    addToScoreStack (tag);
    
    fGlobalHeaderHasBeenFlushed = true;
  }
}

//______________________________________________________________________________
// the part header contains information like part name
// it must be written only once, at the beginning 
// of the corresponding lilypond voice, thus the function clears the data
//  when flushed so that further calls do nothing
//______________________________________________________________________________
void xml2lilypondvisitor::flushPartHeader ( partHeader& header )
{
  if (header.fPartName && header.fPartName->getValue().size()) {
    Slilypondelement tag = lilypondcmd::create("set Staff.instrumentName ="); // USER
    stringstream s1, s2;
    string instr = header.fPartName->getValue();
    // int offset = instr.size() * 2;

    // USER s1 << "dx=-" << offset << "hs";
    tag->add (lilypondparam::create(instr));
    //tag->add (lilypondparam::create(s1.str(), false));
    //tag->add (lilypondparam::create("dy=-5hs", false));
    addToScoreStack (tag);

    //tag = lilypondcmd::create("systemFormat");
    //tag->add (lilypondparam::create(""));
    // USER s2 << "dx=" << offset << "hs";
    //tag->add (lilypondparam::create(s2.str(), false));
    // addToScoreStack (tag);
    
    header.fPartName = 0;
  }
}


/*
  movement-title
   instrument
   
  score-part
score-partwise

senza-misura 
  
  staccato
  * 
  * 
  * accent
  * strong-accent
tenuto

breath-mark
* 
* coda
creator

ending
fermata

grace
* 
* metronome
per-minute
* 
* octave
octave-shift
* 
part
part
part-name

sign
* 
* time-modification
type
unpitched
voice


  */

//______________________________________________________________________________
void xml2lilypondvisitor::visitStart ( S_score_partwise& elt )
{
  fGlobalHeader.fScorePartwise = elt;
  
  Slilypondelement chord = lilypondchord ::create();
  startScoreStack(chord);
}

//______________________________________________________________________________
void xml2lilypondvisitor::visitStart ( S_work_number& elt )
  { fGlobalHeader.fWorkNumber = elt; }

void xml2lilypondvisitor::visitStart ( S_work_title& elt )
  { fGlobalHeader.fWorkTitle = elt; }

void xml2lilypondvisitor::visitStart ( S_movement_number& elt )
  { fGlobalHeader.fMovementNumber = elt; }

void xml2lilypondvisitor::visitStart ( S_movement_title& elt )
  { fGlobalHeader.fMovementTitle = elt; }

void xml2lilypondvisitor::visitStart ( S_creator& elt )
  { fGlobalHeader.fCreators.push_back(elt); }

void xml2lilypondvisitor::visitStart ( S_rights& elt )
  { fGlobalHeader.fRights = elt; }

void xml2lilypondvisitor::visitStart ( S_software& elt )
  { fGlobalHeader.fSoftwares.push_back(elt); }

void xml2lilypondvisitor::visitStart ( S_encoding_date& elt )
  { fGlobalHeader.fEncodingDate = elt; }

//______________________________________________________________________________
void xml2lilypondvisitor::visitStart ( S_score_part& elt )
  { fCurrentPartID = elt->getAttributeValue("id"); }

void xml2lilypondvisitor::visitStart ( S_part_name& elt )
  { fPartHeader[fCurrentPartID].fPartName = elt; }

void xml2lilypondvisitor::visitStart ( S_part& elt )
{
  string partid = elt->getAttributeValue("id");
  
  // browse the part contents for the first time with a partsummary visitor
  if (fSwitches.fTrace) cerr << "Extracting part \"" << partid << "\" summary information" << endl;
  partsummary ps;
  xml_tree_browser browser(&ps);
  browser.browse(*elt);

  flushGlobalHeader (fGlobalHeader);
  
  if (fSwitches.fTrace) cerr << "Extracting part \"" << partid << "\" voices information" << endl;
  smartlist<int>::ptr voices = ps.getVoices ();
  int targetStaff = 0xffff; // initialized to a value we'll unlikely encounter
  bool notesOnly = false;
  rational currentTimeSign (0,1);
  
  // browse the parts voice by voice: 
  // this allows to describe voices that spans over several staves
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

    if (fSwitches.fTrace)
      cerr << 
        "Handling part \"" << partid << 
        "\" contents, voice " << targetVoice << endl;

    Slilypondelement seq = lilypondseq::create();
    pushToScoreStack (seq);

    stringstream s1;
    s1 << "Part" << partid << "Voice" << targetVoice;
    string partName = s1.str();
    stringstream s2;
    s2 << partName << " = { " << endl;
    string partHeader = s2.str();
    Slilypondelement tag = lilypondcmd::create(partHeader, lilypondcmd::kWithoutBackslash);
    //tag->add (lilypondparam::create(comment, false)); // no quotes
    addToScoreStack (tag);
  
    flushPartHeader (fPartHeader[partid]);

    // browse the part contents for the second time with an xmlpart2lilypond visitor
    xmlpart2lilypond pv(fSwitches);
    pv.generatePositions (fSwitches.fGeneratePositions);
    xml_tree_browser browser(&pv);
    pv.initialize(seq, targetStaff, fCurrentStaffIndex, targetVoice, notesOnly, currentTimeSign);
    browser.browse(*elt);
    popFromScoreStack();
    currentTimeSign = pv.getTimeSign();

    tag = lilypondcmd::create("}\n", lilypondcmd::kWithoutBackslash);
    addToScoreStack (tag);
  } // for
}

//______________________________________________________________________________
void xml2lilypondvisitor::addPosition ( 
  Sxmlelement elt, Slilypondelement& tag, int yoffset)
{
  float posx = elt->getAttributeFloatValue("default-x", 0) + elt->getAttributeFloatValue("relative-x", 0);
  if (posx) {
    posx = (posx / 10) * 2;   // convert to half spaces
    stringstream s;
    s << "dx=" << posx << "hs";
    // tag->add (lilypondparam::create(s.str(), false)); // USER
  }
  
  float posy = elt->getAttributeFloatValue("default-y", 0) + elt->getAttributeFloatValue("relative-y", 0);
  if (posy) {
    posy = (posy / 10) * 2;   // convert to half spaces
    posy += yoffset;      // anchor point convertion (defaults to upper line in xml)
    stringstream s;
    s << "dy=" << posy << "hs";
    // tag->add (lilypondparam::create(s.str(), false)); // USER
  }
}

}

