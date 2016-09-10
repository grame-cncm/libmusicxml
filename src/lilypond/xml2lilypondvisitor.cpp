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

#include "utilities.h"
#include "partsummaryvisitor.h"
#include "rational.h"
#include "xml_tree_browser.h"
#include "xml2lilypondvisitor.h"
#include "xmlpart2lilypondvisitor.h"
#include "tree_browser.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
xml2lilypondvisitor::xml2lilypondvisitor( translationSwitches& sw ) :
  fSwitches(sw), 
  fCurrentStaffIndex(0)
{
  fGlobalHeaderHasBeenFlushed = false;
}

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
void xml2lilypondvisitor::startScoreStack   (Slilypondelement& elt) {
  bool doDebug = fSwitches.fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "!!! fScoreStack.push(elt) :" << std::endl;
  fScoreStack.push(elt);
  }
void xml2lilypondvisitor::addToScoreStack   (Slilypondelement& elt) {
  bool doDebug = fSwitches.fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "!!! fScoreStack.top()->add(elt) :" << std::endl;
  fScoreStack.top()->addElement (elt);
}
void xml2lilypondvisitor::pushToScoreStack  (Slilypondelement& elt) {
  bool doDebug = fSwitches.fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "!!!* addToScoreStack(elt); fScoreStack.push(elt) :" << std::endl;
  addToScoreStack(elt); fScoreStack.push(elt);
}
void xml2lilypondvisitor::popFromScoreStack () {
  bool doDebug = fSwitches.fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "!!! fScoreStack.pop() :" << std::endl;
 fScoreStack.pop();
}
Slilypondelement& xml2lilypondvisitor::scoreStackTop () {
  bool doDebug = fSwitches.fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "*** return fScoreStack.top() :" << std::endl;
  return fScoreStack.top();
}

//______________________________________________________________________________
// the score header contains information like title, author etc..
// it must be written only once, at the beginning of the generated code,
// thus the function clears the data 
// when flushed so that further calls do nothing
//______________________________________________________________________________
void xml2lilypondvisitor::flushGlobalHeader ( globalHeader& header )
{
  if (! fGlobalHeaderHasBeenFlushed) {
    Slilypondelement cmd = lilypondcmd::create("header = {\n");
    addToScoreStack (cmd);
  
    if (header.fScorePartwise) {
      Slilypondelement cmd =
        lilypondcmd::create("%MusicXMl_version =", lilypondcmd::kWithoutBackslash);
      string musicXMLVersion = header.fScorePartwise->getAttributeValue("version");
      cmd->addParam (lilypondparam::create(musicXMLVersion));
      addToScoreStack (cmd);
      header.fScorePartwise = 0;
     }
  
    if (header.fWorkNumber) {
      Slilypondelement cmd =
        lilypondcmd::create("%work_number =", lilypondcmd::kWithoutBackslash);
      string workNumber = header.fWorkNumber->getValue();
      cmd->addParam (lilypondparam::create(workNumber));
      addToScoreStack (cmd);
      header.fWorkNumber = 0;
    }
  
    if (header.fWorkTitle) {
      Slilypondelement cmd =
        lilypondcmd::create("%work_title = ", lilypondcmd::kWithoutBackslash);
      string workTitle = header.fWorkTitle->getValue();
      unsigned int pos = workTitle.find ('"');
      while (pos != string::npos) {
        workTitle = workTitle.replace (pos, 1, "'");
        pos = workTitle.find ('"', pos);
      }
      cmd->addParam (lilypondparam::create(workTitle));
      addToScoreStack (cmd);
      header.fWorkTitle = 0;
    }
    
    if (header.fMovementNumber) {
      Slilypondelement cmd =
        lilypondcmd::create("%movement_number =", lilypondcmd::kWithoutBackslash);
      string movementNumber = header.fMovementNumber->getValue();
      cmd->addParam (lilypondparam::create(movementNumber));
      addToScoreStack (cmd);
      header.fMovementNumber = 0;
    }
    
    if (header.fMovementTitle) {
      Slilypondelement cmd =
        lilypondcmd::create("title = ", lilypondcmd::kWithoutBackslash);
      string movementTitle = header.fMovementTitle->getValue();
      unsigned int pos = movementTitle.find ('"');
      while (pos != string::npos) {
        movementTitle = movementTitle.replace (pos, 1, "'");
        pos = movementTitle.find ('"', pos);
      }
      cmd->addParam (lilypondparam::create(movementTitle));
      addToScoreStack (cmd);
      header.fMovementTitle = 0;
    }
    
    vector<S_creator>::const_iterator i1;
    for (i1=header.fCreators.begin(); i1!=header.fCreators.end(); i1++) {
      string type = (*i1)->getAttributeValue("type");
      /* USER
      if ((type == "Composer") || (type == "composer")) {
        Slilypondelement cmd = lilypondcmd::create("composer");
        */
      std::transform(type.begin(), type.end(), type.begin(), ::tolower);
      Slilypondelement cmd =
        lilypondcmd::create(type+" = ", lilypondcmd::kWithoutBackslash);
      cmd->addParam (lilypondparam::create((*i1)->getValue())); // quotes
      addToScoreStack (cmd);
    } // for
    header.fCreators.clear();
    
    if (header.fRights) {
      Slilypondelement cmd =
        lilypondcmd::create("%rights =", lilypondcmd::kWithoutBackslash);
      string rights = header.fRights->getValue();
      cmd->addParam (lilypondparam::create(rights));
      addToScoreStack (cmd);
      header.fRights = 0;
    }
    
    vector<S_software>::const_iterator i2;
    for (i2=header.fSoftwares.begin(); i2!=header.fSoftwares.end(); i2++) {
      Slilypondelement cmd =
        lilypondcmd::create("%software =", lilypondcmd::kWithoutBackslash);
      cmd->addParam (lilypondparam::create((*i2)->getValue())); // quotes
      addToScoreStack (cmd);
    }
    header.fSoftwares.clear();
    
    if (header.fEncodingDate) {
      Slilypondelement cmd =
        lilypondcmd::create("%encoding_date =", lilypondcmd::kWithoutBackslash);
      string encodingDate = header.fEncodingDate->getValue();
      cmd->addParam (lilypondparam::create(encodingDate));
      addToScoreStack (cmd);
      header.fEncodingDate = 0;
    }
    
    cmd = lilypondcmd::create("}\n\n", lilypondcmd::kWithoutBackslash);
    addToScoreStack (cmd);
    
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
    Slilypondelement cmd = lilypondcmd::create("set Staff.instrumentName ="); // USER
    stringstream s1, s2;
    string instr = header.fPartName->getValue();
    // int offset = instr.size() * 2;

    // USER s1 << "dx=-" << offset << "hs";
    cmd->addParam (lilypondparam::create(instr));
    //cmd->add (lilypondparam::create(s1.str(), false));
    //cmd->add (lilypondparam::create("dy=-5hs", false));
    addToScoreStack (cmd);

    //cmd = lilypondcmd::create("systemFormat");
    //cmd->add (lilypondparam::create(""));
    // USER s2 << "dx=" << offset << "hs";
    //cmd->add (lilypondparam::create(s2.str(), false));
    // addToScoreStack (cmd);
    
    header.fPartName = 0;
  }
}

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
void xml2lilypondvisitor::visitStart ( S_instrument_name& elt )
  { 
    // JMI fCurrentPartID = elt->getAttributeValue("id"); 
    }

void xml2lilypondvisitor::visitStart ( S_score_part& elt )
  { fCurrentPartID = elt->getAttributeValue("id"); }

/*
  <part-list>
    <score-part id="P1">
      <part-name print-object="no">Violins 1</part-name>
      <score-instrument id="P1-I1">
        <instrument-name>Violins 1</instrument-name>
        </score-instrument>
      <midi-device id="P1-I1" port="1"></midi-device>
      <midi-instrument id="P1-I1">
        <midi-channel>1</midi-channel>
        <midi-program>49</midi-program>
        <volume>78.7402</volume>
        <pan>0</pan>
        </midi-instrument>
      </score-part>
    <score-part id="P2">
      <part-name print-object="no">Violoncellos</part-name>
      <score-instrument id="P2-I1">
        <instrument-name>Violoncellos</instrument-name>
        </score-instrument>
      <midi-device id="P2-I1" port="1"></midi-device>
      <midi-instrument id="P2-I1">
        <midi-channel>4</midi-channel>
        <midi-program>49</midi-program>
        <volume>78.7402</volume>
        <pan>0</pan>
        </midi-instrument>
      </score-part>
    </part-list>
  <part id="P1">
*/

void xml2lilypondvisitor::visitStart ( S_part_name& elt )
  { fPartHeader[fCurrentPartID].fPartName = elt; }

void xml2lilypondvisitor::visitStart ( S_part& elt )
{
  string partID = elt->getAttributeValue("id");
  
  // browse the part contents for the first time with a partsummaryvisitor
  if (fSwitches.fTrace) cerr << "Extracting part \"" << partID << "\" summary information" << endl;
  partsummaryvisitor ps;
  xml_tree_browser browser(&ps);
  browser.browse(*elt);

  flushGlobalHeader (fGlobalHeader);

  if (fSwitches.fTrace) cerr << "Extracting part \"" << partID << "\" voices information" << endl;
  smartlist<int>::ptr voices = ps.getVoices ();
  int targetStaff = 0xffff; // initialized to a value we'll unlikely encounter
  bool notesOnly = false;
  rational currentTimeSign (0,1);
  
  // browse the parts voice by voice: 
  // this allows to describe voices that spans over several staves
  // and to collect the voice's lyrics
  for (unsigned int i = 0; i < voices->size(); i++) {
    int targetVoice = (*voices)[i];
    int mainStaff = ps.getMainStaff(targetVoice);
    if (targetStaff == mainStaff) {
      notesOnly = true;
    }
    else {
      notesOnly = false;
      targetStaff = mainStaff;
      fCurrentStaffIndex++;
    }

    if (fSwitches.fTrace)
      cerr << 
        "Handling part \"" << partID << 
        "\" contents, mainStaff = " << mainStaff <<
        ", targetStaff = " << targetStaff <<
        ", targetVoice = " << targetVoice << endl;

    stringstream s1;
    s1 << "Part" << partID << "Voice" << targetVoice;
    string partName = s1.str();
    
    Slilypondelement part = lilypondpart::create(partName, fSwitches.fGenerateAbsoluteCode);
    pushToScoreStack (part);
    // JMI2 addToScoreStack(part);
    
    stringstream s2;
    s2 << partName << " = { \%xml2lilypondvisitor::visitStart ( S_part" << endl;
    string partHeader = s2.str();
    Slilypondelement cmd = lilypondcmd::create(partHeader, lilypondcmd::kWithoutBackslash);
    //cmd->add (lilypondparam::create(comment, false)); // no quotes
    addToScoreStack (cmd); // JMI
  
    flushPartHeader (fPartHeader[partID]);

    // browse the part contents for the second time with an xmlpart2lilypondvisitor
    xmlpart2lilypondvisitor pv(fSwitches);
    pv.generatePositions (fSwitches.fGeneratePositions);
    xml_tree_browser browser(&pv);
    pv.initialize(part, targetStaff, fCurrentStaffIndex, targetVoice, notesOnly, currentTimeSign);
    browser.browse(*elt);
    popFromScoreStack();
    currentTimeSign = pv.getTimeSign();

    if (fSwitches.fTrace) cerr << "Extracting part \"" << partID << "\" lyrics information" << endl;
    std::map<std::string, partsummaryvisitor::stanzaContents> 
      stanzas = ps.getStanzas();
    for (std::map<std::string, partsummaryvisitor::stanzaContents> ::iterator 
        it1=stanzas.begin(); it1!=stanzas.end(); ++it1) {
  
      string   lyricsName;
      string   result;
      
      lyricsName = "LyricsForPart"+partID+"Stanza"+int2EnglishWord(atoi(it1->first.c_str()));
      for (std::list<std::list<std::string> > ::iterator 
          it2=it1->second.begin(); it2!=it1->second.end(); ++it2) {    
        std::list<std::string> ::const_iterator 
          it2Begin = it2->begin(),
          it2End   = it2->end(),
          it3      = it2Begin;
  
        for ( ; ; ) {
          result+=*it3;
          if (++it3 == it2End) break;
          result+=" -- ";
        } // for
        result+=" ";
      } // for
 
      Slilypondelement lyrics = lilypondlyrics::create(lyricsName, result);
      addToScoreStack (lyrics);   
    } // for

  ps.clearStanzas(); // for next voice
  } // for
}

//______________________________________________________________________________
void xml2lilypondvisitor::addPosition ( 
  Sxmlelement elt, Slilypondelement& cmd, int yoffset)
{
  float posx = elt->getAttributeFloatValue("default-x", 0) + elt->getAttributeFloatValue("relative-x", 0);
  if (posx) {
    posx = (posx / 10) * 2;   // convert to half spaces
    stringstream s;
    s << "dx=" << posx << "hs";
    // cmd->add (lilypondparam::create(s.str(), false)); // USER
  }
  
  float posy = elt->getAttributeFloatValue("default-y", 0) + elt->getAttributeFloatValue("relative-y", 0);
  if (posy) {
    posy = (posy / 10) * 2;   // convert to half spaces
    posy += yoffset;      // anchor point convertion (defaults to upper line in xml)
    stringstream s;
    s << "dy=" << posy << "hs";
    // cmd->add (lilypondparam::create(s.str(), false)); // USER
  }
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
