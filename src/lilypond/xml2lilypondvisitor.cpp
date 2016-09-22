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
xml2lilypondvisitor::xml2lilypondvisitor( translationSwitches& ts ) :
  fSwitches(ts), 
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
    ly = fLilypondseq;
  }
  return ly;
}

//______________________________________________________________________________
void xml2lilypondvisitor::addElementToSequence (Slilypondelement& elt) {
  bool doDebug = fSwitches.fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "!!! addElementToSequence : " << elt << std::endl;
  fLilypondseq->addElementToSequence (elt);
}

//______________________________________________________________________________
void xml2lilypondvisitor::addPreamble () {
  Slilypondvariablevalueassociation vva1 =
        lilypondvariablevalueassociation:: create(
          "\\version", "2.19",
          lilypondvariablevalueassociation::kSpace,
          lilypondvariablevalueassociation::kQuotesAroundValue,
          lilypondvariablevalueassociation::kUncommented);
  fLilypondseq->addElementToSequence (vva1);

  Slilypondcomment com = lilypondcomment::create("uncomment the following to keep original scores global size");
  fLilypondseq->addElementToSequence (com);
  
  Slilypondschemevariablevalueassociation svva1 =
        lilypondschemevariablevalueassociation:: create(
          "set-global-staff-size", "26",
          lilypondschemevariablevalueassociation::kCommented);
  fLilypondseq->addElementToSequence (svva1);
}
void xml2lilypondvisitor::addPostamble () {
  Slilypondcomment com1 = lilypondcomment::create("choose \\break below to keep the original line breaks");
  fLilypondseq->addElementToSequence (com1);

  Slilypondvariablevalueassociation vva1 =
        lilypondvariablevalueassociation:: create(
          "myBreak", "{ \\break }",
          lilypondvariablevalueassociation::kEqualSign,
          lilypondvariablevalueassociation::kNoQuotesAroundValue,
          lilypondvariablevalueassociation::kUncommented);
  fLilypondseq->addElementToSequence (vva1);

  Slilypondcomment com2 = lilypondcomment::create("choose {} below to let lilypond determine where to break lines");
  fLilypondseq->addElementToSequence (com2);

  Slilypondvariablevalueassociation vva2 =
        lilypondvariablevalueassociation:: create(
          "myBreak", "{}",
          lilypondvariablevalueassociation::kEqualSign,
          lilypondvariablevalueassociation::kNoQuotesAroundValue,
          lilypondvariablevalueassociation::kCommented);
  fLilypondseq->addElementToSequence (vva2);
}

//______________________________________________________________________________
void xml2lilypondvisitor::visitStart ( S_score_partwise& elt )
{
  // create the implicit lilypondseq element FIRST THING!
  fLilypondseq = lilypondseq::create(lilypondseq::kEndOfLine);
    
  addPreamble ();

  // create the header element
  fLilypondheader = lilypondheader::create();
  fLilypondheader->setScorePartwise(elt);
  // add is as the second lilypondseq element
  Slilypondelement header = fLilypondheader;
  fLilypondseq->addElementToSequence (header);

  // create the paper element
  fLilypondpaper = lilypondpaper::create();
  // add is as the second lilypondseq element
  Slilypondelement paper = fLilypondpaper;
  fLilypondseq->addElementToSequence (paper);

  // create the layout element
  fLilypondlayout = lilypondlayout::create();
  // add it as the third lilypondseq element
  Slilypondelement layout = fLilypondlayout;
  fLilypondseq->addElementToSequence (layout);
  
  addPostamble ();

}

//______________________________________________________________________________
void xml2lilypondvisitor::visitStart ( S_work_number& elt )
  { fLilypondheader->setWorkNumber(elt); }

void xml2lilypondvisitor::visitStart ( S_work_title& elt )
  { fLilypondheader->setWorkTitle(elt); }
  
void xml2lilypondvisitor::visitStart ( S_movement_number& elt )
  { fLilypondheader->setMovementNumber(elt); }

void xml2lilypondvisitor::visitStart ( S_movement_title& elt )
  { fLilypondheader->setMovementTitle(elt); }

void xml2lilypondvisitor::visitStart ( S_creator& elt )
  { fLilypondheader->addCreator(elt); }

void xml2lilypondvisitor::visitStart ( S_rights& elt )
  { fLilypondheader->setRights(elt); }

void xml2lilypondvisitor::visitStart ( S_software& elt )
  { fLilypondheader->addSoftware(elt); }

void xml2lilypondvisitor::visitStart ( S_encoding_date& elt )
  { fLilypondheader->setEncodingDate(elt); }

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
  {
    /* JMI
  std::string partName = elt->getAttributeValue("id");
 
  // create the part description
  Slilypondpart part = lilypondpart::create(partName+"BOF", fSwitches.fGenerateAbsoluteCode);
  Slilypondelement p = part;
  
  // register it
  fLilypondpartsMap[fCurrentPartID] = part; 
  
  // add it to the lilypondelement sequence
  fLilypondseq->addElementToSequence (p);
  */
}

void xml2lilypondvisitor::visitStart ( S_part& elt )
{
  std::string partID = elt->getAttributeValue("id");
  
  // browse the part contents for the first time with a partsummaryvisitor
  if (fSwitches.fTrace) cerr << "Extracting part \"" << partID << "\" summary information" << endl;
  partsummaryvisitor ps;
  xml_tree_browser browser(&ps);
  browser.browse(*elt);

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
    s1 <<
      "Part" << stringnumberstoenglishwords(partID) <<
      "Voice" << int2EnglishWord(targetVoice);
    string partName = s1.str();
        
    // create the lilypondpart
    Slilypondpart part = lilypondpart::create(
      partName, fSwitches.fGenerateAbsoluteCode, fSwitches.fGenerateNumericalTime);
    // register it
    fLilypondpartsMap[partID] = part;
    // add it to the lilypondelement sequence
    Slilypondelement p = part;
    addElementToSequence (p);
    
   // browse the part contents for the second time with an xmlpart2lilypondvisitor
    xmlpart2lilypondvisitor xp2lv(fSwitches, part);
    xp2lv.generatePositions (fSwitches.fGeneratePositions);
    xml_tree_browser browser(&xp2lv);
    xp2lv.initialize(part, targetStaff, fCurrentStaffIndex, targetVoice, notesOnly, currentTimeSign);
    browser.browse(*elt);
    //popFromScoreStack();

    // JMI currentTimeSign = xp2lv.getTimeSign();

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
      //addToScoreStackTop (lyrics);   
      addElementToSequence (lyrics);
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
