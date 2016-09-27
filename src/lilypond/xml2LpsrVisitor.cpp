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

#include "tree_browser.h"
#include "xml_tree_browser.h"

#include "rational.h"
#include "utilities.h"

#include "xml2LpsrVisitor.h"
#include "xmlPartSummaryVisitor.h"
#include "xmlPart2LpsrVisitor.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
xml2LpsrVisitor::xml2LpsrVisitor( translationSwitches& ts )
  :
    fTranslationSwitches(ts), 
    fCurrentStaffIndex(0)
{
  fMillimeters     = -1;
  fGlobalStaffSize = -1.0;
  fTenths          = -1;
  
  // create the header element
  fLpsrHeader = lpsrHeader::create();

  // create the paper element
  fLpsrPaper = lpsrPaper::create();
  
  // create the layout element
  fLpsrLayout = lpsrLayout::create();
   
  // create the score element
  fLpsrScore = lpsrScore::create();

   // create the implicit lpsrSequence element FIRST THING!
  fLpsrSeq = lpsrSequence::create(lpsrSequence::kEndOfLine);
    
  // append the header to the lpsrSequence
  SlpsrElement header = fLpsrHeader;
  fLpsrSeq->appendElementToSequence (header);

  // append the paper to the lpsrSequence
  SlpsrElement paper = fLpsrPaper;
  fLpsrSeq->appendElementToSequence (paper);

  // append the layout to the lpsrSequence
  SlpsrElement layout = fLpsrLayout;
  fLpsrSeq->appendElementToSequence (layout);

  // add the "indent" association to the layout
  SlpsrLilypondVarValAssoc indent =
    lpsrLilypondVarValAssoc::create (
      "indent", "0",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented,
      "\\cm");
  fLpsrLayout->addlpsrLilypondVarValAssoc (indent);
  
  // add the "indent" association to the layout
  SlpsrLilypondVarValAssoc pageCount =
    lpsrLilypondVarValAssoc::create (
      "page-cout", "0",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
      lpsrLilypondVarValAssoc::kCommented);
  fLpsrLayout->addlpsrLilypondVarValAssoc (pageCount);
  
  // add the "indent" association to the layout
  SlpsrLilypondVarValAssoc systemCount =
    lpsrLilypondVarValAssoc::create (
      "system-count", "0",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
      lpsrLilypondVarValAssoc::kCommented);
  fLpsrLayout->addlpsrLilypondVarValAssoc (systemCount);
  
  // add standard postamble
  appendPostamble ();
  
  fVisitingPageLayout = false;
}

//______________________________________________________________________________
SlpsrElement xml2LpsrVisitor::convertToLpsr (const Sxmlelement& xml )
{
  SlpsrElement ly;
  if (xml) {
    // create a browser on this xml2LpsrVisitor
    tree_browser<xmlelement> browser(this);
    // browse the xmlelement tree
    browser.browse(*xml);
    // the stack top contains the resulting lpsrElement tree
    ly = fLpsrSeq;
  }
  return ly;
}

//______________________________________________________________________________
void xml2LpsrVisitor::appendElementToSequence (SlpsrElement& elt) {
  bool doDebug = fTranslationSwitches.fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "!!! appendElementToSequence : " << elt << std::endl;
  fLpsrSeq->appendElementToSequence (elt);
}

//______________________________________________________________________________
void xml2LpsrVisitor::prependPreamble () {
  // prepending elements in reverse order

  SlpsrSchemeVarValAssoc staffSize =
        lpsrSchemeVarValAssoc:: create(
          "set-global-staff-size", fGlobalSfaffSizeAsString,
          lpsrSchemeVarValAssoc::kCommented);
  fLpsrSeq->prependElementToSequence (staffSize);

  SlpsrComment com =
    lpsrComment::create("uncomment the following to keep original scores global size");
  fLpsrSeq->prependElementToSequence (com);
  
  SlpsrLilypondVarValAssoc version =
        lpsrLilypondVarValAssoc:: create(
          "\\version", "2.19",
          lpsrLilypondVarValAssoc::kSpace,
          lpsrLilypondVarValAssoc::kQuotesAroundValue,
          lpsrLilypondVarValAssoc::kUncommented);
  fLpsrSeq->prependElementToSequence (version);
}
 
void xml2LpsrVisitor::appendPostamble () {
  SlpsrComment com1 =
    lpsrComment::create("choose \\break below to keep the original line breaks");
  fLpsrSeq->appendElementToSequence (com1);

  SlpsrLilypondVarValAssoc myBreak1 =
        lpsrLilypondVarValAssoc:: create(
          "myBreak", "{ \\break }",
          lpsrLilypondVarValAssoc::kEqualSign,
          lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
          lpsrLilypondVarValAssoc::kUncommented);
  fLpsrSeq->appendElementToSequence (myBreak1);

  SlpsrComment com2 =
    lpsrComment::create("choose {} below to let lpsr determine where to break lines");
  fLpsrSeq->appendElementToSequence (com2);

  SlpsrLilypondVarValAssoc myBreak2 =
        lpsrLilypondVarValAssoc:: create(
          "myBreak", "{}",
          lpsrLilypondVarValAssoc::kEqualSign,
          lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
          lpsrLilypondVarValAssoc::kCommented);
  fLpsrSeq->appendElementToSequence (myBreak2);
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_score_partwise& elt )
{
  // store the element in the header
// JMI  fLpsrHeader->setScorePartwise(elt);
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitEnd ( S_score_partwise& elt )
{
  // now we can insert the global staff size where needed
  stringstream s;

  s << fGlobalStaffSize;
  s >> fGlobalSfaffSizeAsString;
  
  // add standard preamble ahead of the part element sequence
  // only now because set-global-staff-size needs information
  // from the <scaling> element
  prependPreamble ();

  // get score layout
  SlpsrLayout layout = fLpsrScore->getScoreLayout();

  // add the "layout-set-staff-size" scheme variable to the score layout
  SlpsrComment com =
    lpsrComment::create("uncomment the following to keep original scores global size");
  fLpsrSeq->prependElementToSequence (com);

  SlpsrSchemeVarValAssoc staffSize =
    lpsrSchemeVarValAssoc::create (
      "layout-set-staff-size", fGlobalSfaffSizeAsString,
      lpsrSchemeVarValAssoc::kCommented);
  layout->addLpsrSchemeVarValAssoc (staffSize);  

  // get score parallel music
  SlpsrParallelMusic parallel = fLpsrScore->getScoreParallelMusic();
  
  // add the parts and lyrics to it
  lpsrPartsmap::const_iterator i;
  for (i = fLpsrPartsMap.begin(); i != fLpsrPartsMap.end(); i++) {
    // get part
    SlpsrPart part = (*i).second;
    
    // create a new staff comaand
    SlpsrNewstaffCommand nstf = lpsrNewstaffCommand::create();
    
    // add it to the score parallel music
    SlpsrParallelMusic parallelMusic = fLpsrScore->getScoreParallelMusic();
    parallelMusic->addElementToParallelMusic(nstf);
    parallelMusic->addElementToParallelMusic(nstf); // JMI TEST
    parallelMusic->addElementToParallelMusic(nstf); // JMI TEST
    
    // add the part name to the new staff
    SlpsrVariableUseCommand variableUse =
      lpsrVariableUseCommand::create(part->getPartName());
    nstf->addElementToNewStaff(variableUse);
  } // for
  
  // append the score to the lpsrSequence
  // only now to place it after the postamble
  SlpsrElement score = fLpsrScore;
  fLpsrSeq->appendElementToSequence (score);
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_work_number& elt )
  { fLpsrHeader->setWorkNumber(elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_work_title& elt )
  { fLpsrHeader->setWorkTitle(elt->getValue()); }
  
void xml2LpsrVisitor::visitStart ( S_movement_number& elt )
  { fLpsrHeader->setMovementNumber(elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_movement_title& elt )
  { fLpsrHeader->setMovementTitle(elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_creator& elt )
  { fLpsrHeader->addCreator(elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_rights& elt )
  { fLpsrHeader->setRights(elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_software& elt )
  { fLpsrHeader->addSoftware(elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_encoding_date& elt )
  { fLpsrHeader->setEncodingDate(elt->getValue()); }

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_millimeters& elt )
{ 
  fMillimeters = (int)(*elt);
//  cout << "--> fMillimeters = " << fMillimeters << endl;
  
  fGlobalStaffSize = fMillimeters*72.27/25.4;
//  cout << "--> fGlobalStaffSize = " << fGlobalStaffSize << endl;
}

void xml2LpsrVisitor::visitStart ( S_tenths& elt )
{
  fTenths = (int)(*elt);
//  cout << "--> fTenths = " << fTenths << endl;
}

void xml2LpsrVisitor::visitEnd ( S_scaling& elt)
{
  if (fTranslationSwitches.fTrace)
    cerr <<
      "There are " << fTenths << " tenths for " << 
      fMillimeters << " millimeters, hence a global staff size of " <<
      fGlobalStaffSize << std::endl;
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_system_distance& elt )
{
  int systemDistance = (int)(*elt);
//  cout << "--> systemDistance = " << systemDistance << endl;
  fLpsrPaper->setBetweenSystemSpace (1.0*systemDistance*fMillimeters/fTenths/10);  
}
void xml2LpsrVisitor::visitStart ( S_top_system_distance& elt )
{
  int topSystemDistance = (int)(*elt);
//  cout << "--> fTopSystemDistance = " << topSystemDistance << endl;
  fLpsrPaper->setPageTopSpace (1.0*topSystemDistance*fMillimeters/fTenths/10);  
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_page_layout& elt )
{
  fVisitingPageLayout = true;
}
void xml2LpsrVisitor::visitEnd ( S_page_layout& elt )
{
  fVisitingPageLayout = false;
}

void xml2LpsrVisitor::visitStart ( S_page_height& elt )
{
  if (fVisitingPageLayout) {
    int pageHeight = (int)(*elt);
    cout << "--> pageHeight = " << pageHeight << endl;
    fLpsrPaper->setPaperHeight (1.0*pageHeight*fMillimeters/fTenths/10);  
  }
}
void xml2LpsrVisitor::visitStart ( S_page_width& elt )
{
  if (fVisitingPageLayout) {
    int pageWidth = (int)(*elt);
    cout << "--> pageWidth = " << pageWidth << endl;
    fLpsrPaper->setPaperWidth (1.0*pageWidth*fMillimeters/fTenths/10);
  }
}

void xml2LpsrVisitor::visitStart ( S_left_margin& elt )
{
  if (fVisitingPageLayout) {
    int leftMargin = (int)(*elt);
    cout << "--> leftMargin = " << leftMargin << endl;
    fLpsrPaper->setLeftMargin (1.0*leftMargin*fMillimeters/fTenths/10);  
  }
}
void xml2LpsrVisitor::visitStart ( S_right_margin& elt )
{
  if (fVisitingPageLayout) {
    int rightMargin = (int)(*elt);
    cout << "--> rightMargin = " << rightMargin << endl;
    fLpsrPaper->setRightMargin (1.0*rightMargin*fMillimeters/fTenths/10);  
  }
}
void xml2LpsrVisitor::visitStart ( S_top_margin& elt )
{
  if (fVisitingPageLayout) {
    int topMargin = (int)(*elt);
    cout << "--> topMargin = " << topMargin << endl;
    fLpsrPaper->setTopMargin (1.0*topMargin*fMillimeters/fTenths/10);  
  }
}
void xml2LpsrVisitor::visitStart ( S_bottom_margin& elt )
{
  if (fVisitingPageLayout) {
    int bottomMargin = (int)(*elt);
    cout << "--> bottomMargin = " << bottomMargin << endl;
    fLpsrPaper->setBottomMargin (1.0*bottomMargin*fMillimeters/fTenths/10);  
  }
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_instrument_name& elt )
{}

void xml2LpsrVisitor::visitStart ( S_score_part& elt )
  { fCurrentPartID = elt->getAttributeValue("id"); }

void xml2LpsrVisitor::visitStart ( S_part_name& elt )
{}

void xml2LpsrVisitor::visitStart ( S_part& elt )
{
  std::string partID = elt->getAttributeValue("id");
  
  // browse the part contents for the first time with a xmlPartSummaryVisitor
  if (fTranslationSwitches.fTrace)
    cerr << "Extracting part \"" << partID << "\" summary information" << endl;
  xmlPartSummaryVisitor ps(fTranslationSwitches);
  xml_tree_browser browser(&ps);
  browser.browse(*elt);

  if (fTranslationSwitches.fTrace)
    cerr << "Extracting part \"" << partID << "\" voices information" << endl;
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

    if (fTranslationSwitches.fTrace)
      cerr << 
        "Handling part \"" << partID << 
        "\" contents, mainStaff = " << mainStaff <<
        ", targetStaff = " << targetStaff <<
        ", targetVoice = " << targetVoice << endl;

    stringstream s1;
    s1 <<
      "Part" << stringNumbersToEnglishWords (partID) <<
      "Voice" << int2EnglishWord (targetVoice);
    string partName = s1.str();
        
    // create the lpsrPart
    SlpsrPart part = lpsrPart::create(
      partName,
      fTranslationSwitches.fGenerateAbsoluteCode,
      fTranslationSwitches.fGenerateNumericalTime);
      
    // register it in this visitors's map
    fLpsrPartsMap[partID] = part;
    
    // append it to the lpsrElement sequence
    SlpsrElement p = part;
    appendElementToSequence (p);
    
    // browse the part contents once more with an xmlpart2lpsrvisitor
    xmlpart2lpsrvisitor xp2lv(fTranslationSwitches, part);
    xp2lv.generatePositions (fTranslationSwitches.fGeneratePositions);
    xml_tree_browser browser(&xp2lv);
    xp2lv.initialize(part, targetStaff, fCurrentStaffIndex, targetVoice, notesOnly, currentTimeSign);
    browser.browse(*elt);

    // JMI currentTimeSign = xp2lv.getTimeSign();

    // extract the part lyrics
    if (fTranslationSwitches.fTrace)
      cerr << "Extracting part \"" << partID << "\" lyrics information" << endl;
    std::map<std::string, xmlPartSummaryVisitor::stanzaContents> 
      stanzas = ps.getStanzas();
    for (std::map<std::string, xmlPartSummaryVisitor::stanzaContents> ::iterator 
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
 
      // create the lyrics
      SlpsrLyrics lyrics = lpsrLyrics::create(lyricsName, result);
      
      // append it to the sequence
      SlpsrElement elem = lyrics;  
      appendElementToSequence (elem);
      
      // add the lyrics to the part
      part->addLyricsToPart(lyrics);
    } // for

  ps.clearStanzas(); // for next voice
  } // for
}

//______________________________________________________________________________
void xml2LpsrVisitor::addPosition ( 
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
