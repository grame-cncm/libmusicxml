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

#include "lpsrUtilities.h"
#include "xml2LpsrVisitor.h"
#include "xmlPartSummaryVisitor.h"
#include "xmlPart2LpsrVisitor.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
xml2LpsrVisitor::xml2LpsrVisitor( S_translationSettings& ts )
{
  fTranslationSettings = ts;
  
  fMillimeters     = -1;
  fGlobalStaffSize = -1.0;
  fTenths          = -1;
  
  fCurrentStaffIndex = 0;
  
  // create the header element
  fLpsrHeader = lpsrHeader::create ();

  // create the paper element
  fLpsrPaper = lpsrPaper::create ();
  
  // create the layout element
  fLpsrLayout = lpsrLayout::create ();
   
  // create the score element
  fLpsrScore = lpsrScore::create ();

   // create the implicit lpsrSequence element FIRST THING!
  fLpsrSeq = lpsrSequence::create (lpsrSequence::kEndOfLine);
    
  // append the header to the lpsrSequence
  S_lpsrElement header = fLpsrHeader;
  fLpsrSeq->appendElementToSequence (header);

  // append the paper to the lpsrSequence
  S_lpsrElement paper = fLpsrPaper;
  fLpsrSeq->appendElementToSequence (paper);

  // append the layout to the lpsrSequence
  S_lpsrElement layout = fLpsrLayout;
  fLpsrSeq->appendElementToSequence (layout);

  // add the "indent" association to the layout
  S_lpsrLilypondVarValAssoc indent =
    lpsrLilypondVarValAssoc::create (
      "indent", "0",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
      lpsrLilypondVarValAssoc::kUncommented,
      "\\cm");
  fLpsrLayout->addlpsrLilypondVarValAssoc (indent);
  
  // add the "indent" association to the layout
  S_lpsrLilypondVarValAssoc pageCount =
    lpsrLilypondVarValAssoc::create (
      "page-cout", "0",
      lpsrLilypondVarValAssoc::kEqualSign,
      lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
      lpsrLilypondVarValAssoc::kCommented);
  fLpsrLayout->addlpsrLilypondVarValAssoc (pageCount);
  
  // add the "indent" association to the layout
  S_lpsrLilypondVarValAssoc systemCount =
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
S_lpsrElement xml2LpsrVisitor::convertToLpsr (const Sxmlelement& xml )
{
  S_lpsrElement lpsr;
  if (xml) {
    // create a browser on this xml2LpsrVisitor
    tree_browser<xmlelement> browser (this);
    
    // browse the xmlelement tree
    browser.browse (*xml);
    
    // the stack top contains the resulting lpsrElement tree
    lpsr = fLpsrSeq;
  }
  return lpsr;
}

//______________________________________________________________________________
void xml2LpsrVisitor::appendElementToSequence (S_lpsrElement& elt) {
  bool doDebug = fTranslationSettings->fDebug;
//  bool doDebug = false;

  if (doDebug) cout << "!!! appendElementToSequence : " << elt << std::endl;
  fLpsrSeq->appendElementToSequence (elt);
}

//______________________________________________________________________________
void xml2LpsrVisitor::prependPreamble () {
  // prepending elements in reverse order

  if (fGlobalStaffSize > 0.0) {
    S_lpsrSchemeVarValAssoc staffSize =
          lpsrSchemeVarValAssoc:: create(
            "set-global-staff-size", fGlobalSfaffSizeAsString,
            lpsrSchemeVarValAssoc::kCommented);
    fLpsrSeq->prependElementToSequence (staffSize);
  
    S_lpsrComment com =
      lpsrComment::create (
        "uncomment the following to keep original score global size");
    fLpsrSeq->prependElementToSequence (com);
  }
  
  S_lpsrLilypondVarValAssoc version =
        lpsrLilypondVarValAssoc:: create(
          "\\version", "2.19",
          lpsrLilypondVarValAssoc::kSpace,
          lpsrLilypondVarValAssoc::kQuotesAroundValue,
          lpsrLilypondVarValAssoc::kUncommented);
  fLpsrSeq->prependElementToSequence (version);
}
 
void xml2LpsrVisitor::appendPostamble () {
  S_lpsrComment com1 =
    lpsrComment::create (
      "choose \\break below to keep the original line breaks");
  fLpsrSeq->appendElementToSequence (com1);

  S_lpsrLilypondVarValAssoc myBreak1 =
        lpsrLilypondVarValAssoc:: create(
          "myBreak", "{ \\break }",
          lpsrLilypondVarValAssoc::kEqualSign,
          lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
          lpsrLilypondVarValAssoc::kUncommented);
  fLpsrSeq->appendElementToSequence (myBreak1);

  S_lpsrComment com2 =
    lpsrComment::create (
      "choose {} below to let lpsr determine where to break lines");
  fLpsrSeq->appendElementToSequence (com2);

  S_lpsrLilypondVarValAssoc myBreak2 =
        lpsrLilypondVarValAssoc:: create(
          "myBreak", "{}",
          lpsrLilypondVarValAssoc::kEqualSign,
          lpsrLilypondVarValAssoc::kNoQuotesAroundValue,
          lpsrLilypondVarValAssoc::kCommented);
  fLpsrSeq->appendElementToSequence (myBreak2);
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_score_timewise& elt )
{
  lpsrMusicXMLError("score-timewise MusicXML data is not supported");
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_score_partwise& elt )
{
  // store the element in the header
// JMI  fLpsrHeader->setScorePartwise(elt);
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_part& elt )
{
  string partID = elt->getAttributeValue ("id");
  
  // browse the part contents for the first time with a xmlPartSummaryVisitor
  if (fTranslationSettings->fTrace)
    cerr << "Extracting part \"" << partID << "\" summary information" << endl;

  xmlPartSummaryVisitor xpsv (fTranslationSettings);
  xml_tree_browser      browser (&xpsv);
  browser.browse (*elt);

  if (fTranslationSettings->fTrace)
    cerr << "Getting the part voices IDs" << endl;
    
  smartlist<int>::ptr partSummaryVoiceIDsList =
    xpsv.getVoicesIDsList ();

  int size = partSummaryVoiceIDsList->size();

  if (size > 1)
    cerr << "Theare are " << size << " voices" << std::endl;
  else
    cerr << "There is 1 voice" << std::endl;
    
  int      targetStaff = -1;
  bool     notesOnly = false;
  rational currentTimeSign (0,1);
  
  // browse the parts voice by voice: 
  // this allows to describe voices that spans over several staves
  // and to collect the voice's lyrics
    
  if (fTranslationSettings->fTrace)
    cerr << "Extracting part \"" << partID << "\" voices information" << endl;

  for (unsigned int i = 0; i < partSummaryVoiceIDsList->size(); i++) {
    
    int targetVoice            = (*partSummaryVoiceIDsList) [i];
    int targetVoiceMainStaffID = xpsv.getVoiceMainStaffID (targetVoice);

    if (targetStaff == targetVoiceMainStaffID) {
      notesOnly = true;
    }
    else {
      notesOnly = false;
      targetStaff = targetVoiceMainStaffID;
      fCurrentStaffIndex++;
    }

    if (fTranslationSettings->fTrace)
      cerr << 
        "Handling part \"" << partID << 
        "\" contents, targetVoiceMainStaffID = " << targetVoiceMainStaffID <<
        ", targetStaff = " << targetStaff <<
        ", targetVoice = " << targetVoice << endl;

    string
      partName =
        "Part" + stringNumbersToEnglishWords (partID);
    
    string
      voiceName =
        partName + "_Voice" + int2EnglishWord (targetVoice);
      
    // create the part
    S_lpsrPart
      part =
        lpsrPart::create (
          partName,
          fCurrentInstrumentName,
          fTranslationSettings->fGenerateAbsoluteCode,
          fTranslationSettings->fGenerateNumericalTime);
      
    // register it in this visitors's part map
    fLpsrPartsMap[partID] = part;

 /*   
    // create the voice
    S_lpsrVoice
      voice =
        lpsrVoice::create (
          voiceName,
          fTranslationSettings->fGenerateAbsoluteCode,
          fTranslationSettings->fGenerateNumericalTime);

    // append the voice to the lpsrElement sequence
    S_lpsrElement v = voice;
    appendElementToSequence (v);
*/
    // browse the part contents once more with an xmlPart2LpsrVisitor
    xmlPart2LpsrVisitor
      xp2lv (
        fTranslationSettings,
        part,
        targetStaff,
        fCurrentStaffIndex,
        targetVoice,
        notesOnly,
        currentTimeSign);
    xml_tree_browser browser (&xp2lv);
    browser.browse (*elt);

    // JMI currentTimeSign = xp2lv.getTimeSign();

/*
    // add the voice to the part
    part->addVoiceToPart (voice);
*/
        
    // extract the part lyrics
    if (fTranslationSettings->fTrace)
      cerr << "Extracting part \"" << partID << "\" lyrics information" << endl;

  } // for
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
  S_lpsrLayout
    layout =
      fLpsrScore->getScoreLayout();

  if (fGlobalStaffSize > 0.0) {
    S_lpsrSchemeVarValAssoc
      staffSize =
        lpsrSchemeVarValAssoc::create (
          "layout-set-staff-size", fGlobalSfaffSizeAsString,
          lpsrSchemeVarValAssoc::kCommented);
    layout->addLpsrSchemeVarValAssoc (staffSize);  
  }

  // get score parallel music
  S_lpsrParallelMusic
    parallel =
      fLpsrScore->getScoreParallelMusic();
  
  // add the parts and lyrics to the score parallel music

  if (fTranslationSettings->fTrace) {
    int size = fLpsrPartsMap.size();

    if (size > 1)
      cerr << "Theare are " << size << " parts" << std::endl;
    else
      cerr << "There is 1 part" << std::endl;
  }

  lpsrPartsmap::const_iterator i;
  for (i = fLpsrPartsMap.begin(); i != fLpsrPartsMap.end(); i++) {
    
    // get part and part name
    S_lpsrPart   part     = (*i).second;
    string  partName = part->getPartName ();
     
    // create a staff
    cout << "--> creating a new staff command" << std::endl;
    S_lpsrNewstaffCommand
      newStaffCommand =
        lpsrNewstaffCommand::create();
    
    // get the part voices
    std::vector<S_lpsrVoice>
      partVoices =
        part->getPartVoices ();
 
    // add the voices lyrics to the staff command
    cout <<
      "--> add the lyrics to the staff, " << partVoices.size() << " voices found" << std::endl;

    std::vector<S_lpsrVoice>::const_iterator i;
    for (i = partVoices.begin(); i != partVoices.end(); i++) {
      string voiceName = (*i)->getVoiceName();
      cout <<
        "--> add the lyrics to the staff, " << voiceName << std::endl;
  
      // create the voice
      S_lpsrContext
        voiceContext =
          lpsrContext::create (
            lpsrContext::kNewContext, "Voice", voiceName);
          
      // add a use of the part name to the voice
      S_lpsrVariableUseCommand
        variableUse =
          lpsrVariableUseCommand::create (voiceName);
      voiceContext->addElementToContext (variableUse);
  
      // add the voice to the staff
      newStaffCommand->addElementToNewStaff (voiceContext);
  
      std::vector<S_lpsrLyrics>
        voiceLyrics =
          (*i)->getVoiceLyrics ();

      std::vector<S_lpsrLyrics>::const_iterator i;
      for (i = voiceLyrics.begin(); i != voiceLyrics.end(); i++) {
        S_lpsrLyrics lyrics     = (*i);
        string  lyricsName = lyrics->getLyricsName();
            
        // create the lyrics command
        S_lpsrNewlyricsCommand
          lyricsUse =
            lpsrNewlyricsCommand::create (
              lyricsName, voiceName);
              
        // add the lyrics use to the  staff
        newStaffCommand->addElementToNewStaff (lyricsUse);
      } // for
    } // for
    
    // add the staff to the score parallel music
    S_lpsrParallelMusic
      parallelMusic =
        fLpsrScore->getScoreParallelMusic ();
    parallelMusic->addElementToParallelMusic (newStaffCommand);


    /*
       \new Staff <<
            \set Staff.instrumentName = "Violins 1"
            \context Staff << 
                \context Voice = "PartPOneVoiceOne" { \voiceOne \PartPOneVoiceOne }
                \new Lyrics \lyricsto "PartPOneVoiceOne" \PartPOneVoiceOneLyricsOne
                \new Lyrics \lyricsto "PartPOneVoiceOne" \PartPOneVoiceOneLyricsTwo
                \new Lyrics \lyricsto "PartPOneVoiceOne" \PartPOneVoiceOneLyricsThree
                \new Lyrics \lyricsto "PartPOneVoiceOne" \PartPOneVoiceOneLyricsFour
                \new Lyrics \lyricsto "PartPOneVoiceOne" \PartPOneVoiceOneLyricsFive
                \new Lyrics \lyricsto "PartPOneVoiceOne" \PartPOneVoiceOneLyricsSix
                \context Voice = "PartPOneVoiceTwo" { \voiceTwo \PartPOneVoiceTwo }
                >>
            >>
      */
  } // for
  
  // append the score to the lpsrSequence
  // only now to place it after the postamble
  S_lpsrElement
    score = fLpsrScore;
  fLpsrSeq->appendElementToSequence (score);
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_work_number& elt )
  { fLpsrHeader->setWorkNumber (elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_work_title& elt )
  { fLpsrHeader->setWorkTitle (elt->getValue()); }
  
void xml2LpsrVisitor::visitStart ( S_movement_number& elt )
  { fLpsrHeader->setMovementNumber (elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_movement_title& elt )
  { fLpsrHeader->setMovementTitle (elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_creator& elt )
{
  string type = elt->getAttributeValue ("type");
  fLpsrHeader->addCreator (type, elt->getValue());
}

void xml2LpsrVisitor::visitStart ( S_rights& elt )
  { fLpsrHeader->setRights (elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_software& elt )
  { fLpsrHeader->addSoftware (elt->getValue()); }

void xml2LpsrVisitor::visitStart ( S_encoding_date& elt )
  { fLpsrHeader->setEncodingDate (elt->getValue()); }

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
  if (fTranslationSettings->fTrace)
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
    //cout << "--> pageHeight = " << pageHeight << endl;
    fLpsrPaper->setPaperHeight (1.0*pageHeight*fMillimeters/fTenths/10);  
  }
}
void xml2LpsrVisitor::visitStart ( S_page_width& elt )
{
  if (fVisitingPageLayout) {
    int pageWidth = (int)(*elt);
    //cout << "--> pageWidth = " << pageWidth << endl;
    fLpsrPaper->setPaperWidth (1.0*pageWidth*fMillimeters/fTenths/10);
  }
}

void xml2LpsrVisitor::visitStart ( S_left_margin& elt )
{
  if (fVisitingPageLayout) {
    int leftMargin = (int)(*elt);
    //cout << "--> leftMargin = " << leftMargin << endl;
    fLpsrPaper->setLeftMargin (1.0*leftMargin*fMillimeters/fTenths/10);  
  }
}
void xml2LpsrVisitor::visitStart ( S_right_margin& elt )
{
  if (fVisitingPageLayout) {
    int rightMargin = (int)(*elt);
    //cout << "--> rightMargin = " << rightMargin << endl;
    fLpsrPaper->setRightMargin (1.0*rightMargin*fMillimeters/fTenths/10);  
  }
}
void xml2LpsrVisitor::visitStart ( S_top_margin& elt )
{
  if (fVisitingPageLayout) {
    int topMargin = (int)(*elt);
    //cout << "--> topMargin = " << topMargin << endl;
    fLpsrPaper->setTopMargin (1.0*topMargin*fMillimeters/fTenths/10);  
  }
}
void xml2LpsrVisitor::visitStart ( S_bottom_margin& elt )
{
  if (fVisitingPageLayout) {
    int bottomMargin = (int)(*elt);
    //cout << "--> bottomMargin = " << bottomMargin << endl;
    fLpsrPaper->setBottomMargin (1.0*bottomMargin*fMillimeters/fTenths/10);  
  }
}

//______________________________________________________________________________
void xml2LpsrVisitor::visitStart ( S_instrument_name& elt )
  { fCurrentInstrumentName = elt->getValue(); }

void xml2LpsrVisitor::visitStart ( S_score_part& elt )
  { fCurrentPartID = elt->getAttributeValue("id"); }

void xml2LpsrVisitor::visitStart ( S_part_name& elt )
  { fCurrentPartName = elt->getValue(); }

//______________________________________________________________________________
void xml2LpsrVisitor::addPosition ( 
  Sxmlelement elt, S_lpsrElement& cmd, int yoffset)
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
   instrument
     
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
* 
* octave
octave-shift
* 

sign
* 
* time-modification
type
unpitched
voice


  */



/*
    std::map<string, xmlPartSummaryVisitor::stanzaContents> 
      stanzas = xpsv.getStanzas();
    for (std::map<string, xmlPartSummaryVisitor::stanzaContents> ::iterator 
        it1=stanzas.begin(); it1!=stanzas.end(); ++it1) {

      string 
        lyricsName =
          voiceName + 
          "LyricsStanza"+
          int2EnglishWord (atoi(it1->first.c_str()));
      string result;
      
      for (std::list<std::list<string> > ::iterator 
          it2=it1->second.begin(); it2!=it1->second.end(); ++it2) {    

        std::list<string> ::const_iterator 
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
      S_lpsrLyrics
        lyrics =
          lpsrLyrics::create(lyricsName, result);
      
      // append lyrics to the sequence
      S_lpsrElement elem = lyrics;  
      appendElementToSequence (elem);
      
      // add the lyrics to the voice
      cout << // JMIJMI
        "--> adding lyrics " << lyrics->getLyricsName() <<
        " to voice " << voiceName << std::endl;
      voice->addLyricsToVoice (lyrics);
 */
