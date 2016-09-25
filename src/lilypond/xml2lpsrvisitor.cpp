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
#include "xml2lpsrvisitor.h"
#include "xmlpart2lpsrvisitor.h"
#include "tree_browser.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
xml2lpsrvisitor::xml2lpsrvisitor( translationSwitches& ts ) :
  fSwitches(ts), 
  fCurrentStaffIndex(0)
{}

//______________________________________________________________________________
SlpsrElement xml2lpsrvisitor::convertToLilyPond (const Sxmlelement& xml )
{
  SlpsrElement ly;
  if (xml) {
    // create a browser on this xml2lpsrvisitor
    tree_browser<xmlelement> browser(this);
    // browse the xmlelement tree
    browser.browse(*xml);
    // the stack top contains the resulting lpsrElement tree
    ly = fLpsrseq;
  }
  return ly;
}

//______________________________________________________________________________
void xml2lpsrvisitor::addElementToSequence (SlpsrElement& elt) {
  bool doDebug = fSwitches.fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "!!! addElementToSequence : " << elt << std::endl;
  fLpsrseq->addElementToSequence (elt);
}

//______________________________________________________________________________
void xml2lpsrvisitor::addPreamble () {
  SlpsrVariableValueAssociation vva1 =
        lpsrVariableValueAssociation:: create(
          "\\version", "2.19",
          lpsrVariableValueAssociation::kSpace,
          lpsrVariableValueAssociation::kQuotesAroundValue,
          lpsrVariableValueAssociation::kUncommented);
  fLpsrseq->addElementToSequence (vva1);

  SlpsrComment com = lpsrComment::create("uncomment the following to keep original scores global size");
  fLpsrseq->addElementToSequence (com);
  
  SlpsrSchemeVariableValueAssociation svva1 =
        lpsrSchemeVariableValueAssociation:: create(
          "set-global-staff-size", "26",
          lpsrSchemeVariableValueAssociation::kCommented);
  fLpsrseq->addElementToSequence (svva1);
}
void xml2lpsrvisitor::addPostamble () {
  SlpsrComment com1 = lpsrComment::create("choose \\break below to keep the original line breaks");
  fLpsrseq->addElementToSequence (com1);

  SlpsrVariableValueAssociation vva1 =
        lpsrVariableValueAssociation:: create(
          "myBreak", "{ \\break }",
          lpsrVariableValueAssociation::kEqualSign,
          lpsrVariableValueAssociation::kNoQuotesAroundValue,
          lpsrVariableValueAssociation::kUncommented);
  fLpsrseq->addElementToSequence (vva1);

  SlpsrComment com2 = lpsrComment::create("choose {} below to let lpsr determine where to break lines");
  fLpsrseq->addElementToSequence (com2);

  SlpsrVariableValueAssociation vva2 =
        lpsrVariableValueAssociation:: create(
          "myBreak", "{}",
          lpsrVariableValueAssociation::kEqualSign,
          lpsrVariableValueAssociation::kNoQuotesAroundValue,
          lpsrVariableValueAssociation::kCommented);
  fLpsrseq->addElementToSequence (vva2);
}

//______________________________________________________________________________
void xml2lpsrvisitor::visitStart ( S_score_partwise& elt )
{
  // create the implicit lpsrSequence element FIRST THING!
  fLpsrseq = lpsrSequence::create(lpsrSequence::kEndOfLine);
  
  // add standard preamble
  addPreamble ();

  // create the header element
  flpsrHeader = lpsrHeader::create();
  flpsrHeader->setScorePartwise(elt);
  // add is as the second lpsrSequence element
  SlpsrElement header = flpsrHeader;
  fLpsrseq->addElementToSequence (header);

  // create the paper element
  fLpsrpaper = lpsrPaper::create();
  // add is as the second lpsrSequence element
  SlpsrElement paper = fLpsrpaper;
  fLpsrseq->addElementToSequence (paper);

  // create the layout element
  fLpsrlayout = lpsrLayout::create();
  // add it as the third lpsrSequence element
  SlpsrElement layout = fLpsrlayout;
  fLpsrseq->addElementToSequence (layout);
  
  // add standard postamble
  addPostamble ();
}

//______________________________________________________________________________
void xml2lpsrvisitor::visitEnd ( S_score_partwise& elt )
{
  // create the score element
  fLpsrscore = lpsrScore::create();
  // add is as the last lpsrSequence element
  SlpsrElement score = fLpsrscore;
  fLpsrseq->addElementToSequence (score);
  
  // get score parallel music
  SlpsrParallel par = fLpsrscore->getScoreParallelMusic();
  
  // add the parts and lyrics to it
  lpsrPartsmap::const_iterator i;
  for (i = fLpsrpartsMap.begin(); i != fLpsrpartsMap.end(); i++) {
    // get part
    SlpsrPart part = (*i).second;
    
    // create a new staff comaand
    SlpsrNewstaffCommand nstf = lpsrNewstaffCommand::create();
    
    // add it to the score parallel music
    par->addElementToParallel(nstf);
    
    // add the part name to the new staff
    SlpsrVariableUseCommand cmd = lpsrVariableUseCommand::create(part->getPartName());
    nstf->addElementToNewStaff(cmd);
  } // for
}

//______________________________________________________________________________
void xml2lpsrvisitor::visitStart ( S_work_number& elt )
  { flpsrHeader->setWorkNumber(elt); }

void xml2lpsrvisitor::visitStart ( S_work_title& elt )
  { flpsrHeader->setWorkTitle(elt); }
  
void xml2lpsrvisitor::visitStart ( S_movement_number& elt )
  { flpsrHeader->setMovementNumber(elt); }

void xml2lpsrvisitor::visitStart ( S_movement_title& elt )
  { flpsrHeader->setMovementTitle(elt); }

void xml2lpsrvisitor::visitStart ( S_creator& elt )
  { flpsrHeader->addCreator(elt); }

void xml2lpsrvisitor::visitStart ( S_rights& elt )
  { flpsrHeader->setRights(elt); }

void xml2lpsrvisitor::visitStart ( S_software& elt )
  { flpsrHeader->addSoftware(elt); }

void xml2lpsrvisitor::visitStart ( S_encoding_date& elt )
  { flpsrHeader->setEncodingDate(elt); }

//______________________________________________________________________________
void xml2lpsrvisitor::visitStart ( S_instrument_name& elt )
{}

void xml2lpsrvisitor::visitStart ( S_score_part& elt )
  { fCurrentPartID = elt->getAttributeValue("id"); }

void xml2lpsrvisitor::visitStart ( S_part_name& elt )
{}

void xml2lpsrvisitor::visitStart ( S_part& elt )
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
      "Part" << stringNumbersToEnglishWords(partID) <<
      "Voice" << int2EnglishWord(targetVoice);
    string partName = s1.str();
        
    // create the lpsrPart
    SlpsrPart part = lpsrPart::create(
      partName, fSwitches.fGenerateAbsoluteCode, fSwitches.fGenerateNumericalTime);
    // register it
    fLpsrpartsMap[partID] = part;
    // add it to the lpsrElement sequence
    SlpsrElement p = part;
    addElementToSequence (p);
    
    // browse the part contents for the second time with an xmlpart2lpsrvisitor
    xmlpart2lpsrvisitor xp2lv(fSwitches, part);
    xp2lv.generatePositions (fSwitches.fGeneratePositions);
    xml_tree_browser browser(&xp2lv);
    xp2lv.initialize(part, targetStaff, fCurrentStaffIndex, targetVoice, notesOnly, currentTimeSign);
    browser.browse(*elt);
    //popFromScoreStack();

    // JMI currentTimeSign = xp2lv.getTimeSign();

    // extract the part lyrics
    if (fSwitches.fTrace) cerr << "Extracting part \"" << partID << "\" lyrics information" << endl;
    std::map<std::string, partsummaryvisitor::stanzaContents> 
      stanzas = ps.getStanzas();
    for (std::map<std::string, partsummaryvisitor::stanzaContents> ::iterator 
        it1=stanzas.begin(); it1!=stanzas.end(); ++it1) {
  
      string   lyricsName;
      string   result;
      
      lyricsName =
        "Part"+stringNumbersToEnglishWords(partID)+
        "LyricsStanza"+int2EnglishWord(atoi(it1->first.c_str()));
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
 
      // create lyrics
      SlpsrLyrics lyrics = lpsrLyrics::create(lyricsName, result);
      // add it to the sequence
      SlpsrElement elem = lyrics;  
      addElementToSequence (elem);
      // add the lyrics to the part
      part->addLyricsToPart(lyrics);
    } // for

  ps.clearStanzas(); // for next voice
  } // for
}

//______________________________________________________________________________
void xml2lpsrvisitor::addPosition ( 
  Sxmlelement elt, SlpsrElement& cmd, int yoffset)
{
  float posx = elt->getAttributeFloatValue("default-x", 0) + elt->getAttributeFloatValue("relative-x", 0);
  if (posx) {
    posx = (posx / 10) * 2;   // convert to half spaces
    stringstream s;
    s << "dx=" << posx << "hs";
  }
  
  float posy = elt->getAttributeFloatValue("default-y", 0) + elt->getAttributeFloatValue("relative-y", 0);
  if (posy) {
    posy = (posy / 10) * 2;   // convert to half spaces
    posy += yoffset;      // anchor point convertion (defaults to upper line in xml)
    stringstream s;
    s << "dy=" << posy << "hs";
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
