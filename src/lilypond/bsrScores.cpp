/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "bsrScores.h"

#include "version.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "xml2lyOptionsHandling.h"


using namespace std;

namespace MusicXML2 
{

/*
//______________________________________________________________________________
S_bsrScoreBlock bsrScoreBlock::create (
  int            inputLineNumber)
{
  bsrScoreBlock* o = new bsrScoreBlock (
    inputLineNumber);
  assert(o!=0);
  return o;
}

bsrScoreBlock::bsrScoreBlock (
  int            inputLineNumber)
    : bsrElement (inputLineNumber)
{
  // create the score command parallel music
  fScoreBlockParallelMusicBLock =
    bsrParallelMusicBLock::create (
      inputLineNumber,
      bsrParallelMusicBLock::kEndOfLine);
  
  // create the score command layout
  fScoreBlockLayout =
    bsrLayout::create (
      inputLineNumber);
  
  // create the score command midi
  string midiTempoDuration  = gLilypondOptions->fMidiTempo.first;
  int    midiTempoPerSecond = gLilypondOptions->fMidiTempo.second;
  
  fScoreBlockMidi =
    msrMidi::create (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
}

bsrScoreBlock::~bsrScoreBlock ()
{}

void bsrScoreBlock::appendPartGroupBlockToScoreBlock (
  S_bsrPartGroupBlock partGroupBlock)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> getPartGroupCombinedName () <<
       " to BSR score" <<
       endl;
  }
#endif

  fScoreBlockParallelMusicBLock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);
    
//               fScoreBlockElements.push_back(partGroupBlock);
}

/* JMI
void bsrScoreBlock::appendVoiceUseToParallelMusicBLock (
  S_bsrUseVoiceCommand voiceUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to BSR score" <<
       endl;
  }
#endif

  fScoreBlockParallelMusicBLock->
    addElementToParallelMusicBLock (voiceUse);
    
//     JMI             fScoreBlockElements.push_back(voiceUse);
}
                  
void bsrScoreBlock::appendLyricsUseToParallelMusicBLock (
  S_bsrNewLyricsBlock lyricsUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to BSR score" <<
       endl;
  }
#endif

  fScoreBlockParallelMusicBLock->
    addElementToParallelMusicBLock (lyricsUse);
}
* /

void bsrScoreBlock::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScoreBlock::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_bsrScoreBlock>*> (v)) {
        S_bsrScoreBlock elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrScoreBlock::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrScoreBlock::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScoreBlock::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_bsrScoreBlock>*> (v)) {
        S_bsrScoreBlock elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrScoreBlock::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrScoreBlock::browseData (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScoreBlock::browseData ()" <<
      endl;
  }

  {
    // browse the score command parallel music
    msrBrowser<bsrParallelMusicBLock> browser (v);    
    browser.browse (*fScoreBlockParallelMusicBLock);
  }

/ * JMI
  for (
    vector<S_msrElement>::const_iterator i = fScoreBlockElements.begin ();
    i != fScoreBlockElements.end ();
    i++) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
* /
  {
    // browse the score command layout
    msrBrowser<bsrLayout> browser (v);    
    browser.browse (*fScoreBlockLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);    
    browser.browse (*fScoreBlockMidi);
  }

  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% <== bsrScoreBlock::browseData ()" <<
      endl;
  }
}

void bsrScoreBlock::print (ostream& os)
{
  os << "ScoreBlock" << endl << endl;

  gIndenter++;

  os <<
    fScoreBlockParallelMusicBLock <<
    endl;
    
  os <<
    fScoreBlockLayout <<
    endl;
  
  os <<
    fScoreBlockMidi <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrScoreBlock& scr)
{
  scr->print (os);
  return os;
}
*/

//______________________________________________________________________________
S_bsrScore bsrScore::create (
  int            inputLineNumber,
  S_msrScore     mScore)
{
  bsrScore* o = new bsrScore (
    inputLineNumber, mScore);
  assert(o!=0);
  return o;
}

bsrScore::bsrScore (
  int            inputLineNumber,
  S_msrScore     mScore)
    : bsrElement (inputLineNumber)
{
  fMsrScore = mScore;

/*
  // create the LilyPond version assoc
  fLilypondVersion =
    bsrVarValAssoc::create (
      inputLineNumber,
      bsrVarValAssoc::kUncommented,
      bsrVarValAssoc::kWithBackSlash,
      bsrVarValAssoc::kLibraryVersion,
      bsrVarValAssoc::kSpace,
      bsrVarValAssoc::kQuotesAroundValue,
      "2.19",
      bsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      bsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      bsrVarValAssoc::kWithEndl);

  // should the initial comments about xml2ly and the options used
  // be generated?
  if (gLilypondOptions->fXml2lyInfos) {
    // create the input source name comment
    {
      stringstream s;
    
      s <<
        "Generated by xml2ly " << currentVersionNumber () <<
        " from ";
    
      if (gXml2lyOptions->fInputSourceName == "-") {
        s << "standard input";
      }
      else {
        s << "\"" << gXml2lyOptions->fInputSourceName << "\"";
      }
    
      s <<
        endl <<
        "% on " << gXml2lyOptions->fTranslationDate <<
        endl;
  
      fInputSourceNameComment =
        bsrComment::create (
          inputLineNumber,
          s.str (),
          bsrComment::kNoGapAfterwards);
    }
  
    // create the translation date comment
    {
      stringstream s;
      
      s <<
        "% Translation command was:";
          
      fTranslationDateComment =
        bsrComment::create (
          inputLineNumber,
          s.str (),
          bsrComment::kNoGapAfterwards);
    }
  
    // do the command line long and short options differ?
    bool
      longAndShortOptionsDiffer =
      gGeneralOptions->fCommandLineWithShortOptions
        !=
      gGeneralOptions->fCommandLineWithLongOptions;

    
  // create the command line long options comment
  {
    stringstream s;
    
    s <<
      gTab <<
      gGeneralOptions->fCommandLineWithLongOptions;
  
    if (longAndShortOptionsDiffer) {
      s <<
        endl <<
        "% or:";
    }
    
    fCommandLineLongOptionsComment =
      bsrComment::create (
        inputLineNumber,
        s.str (),
        longAndShortOptionsDiffer
          ? bsrComment::kNoGapAfterwards
          : bsrComment::kGapAfterwards);
  }

  if (longAndShortOptionsDiffer) {
    // create the command line short options comment
    stringstream s;
    
    s <<
      gTab <<
      gGeneralOptions->fCommandLineWithShortOptions;
    
    fCommandLineShortOptionsComment =
      bsrComment::create (
        inputLineNumber,
        s.str (),
        bsrComment::kGapAfterwards);
  }
}

  // create the global staff size assoc
  fGlobalStaffSizeAssoc =
    bsrSchemeVariable::create (
      inputLineNumber,
      bsrSchemeVariable::kUncommented,
      "set-global-staff-size",
      "20", // the LilyPond default
      "Comment or adapt next line as needed (default is 20)",
      bsrSchemeVariable::kWithEndlTwice);

  // initialize Scheme functions informations
  fTongueSchemeFunctionIsNeeded              = false;
  fCustomShortBarLineSchemeFunctionIsNeeded  = false;
  fEditorialAccidentalSchemeFunctionIsNeeded = false;

  if (gLilypondOptions->fLilypondCompileDate) {
    // create the date and time functions
    addDateAndTimeSchemeFunctionsToScore ();
  }

  if (gLilypondOptions->fPointAndClickOff) {
    // create the pointAndClickOff command
    addPointAndClickOffSchemeFunctionsToScore ();
  }

  if (gLilypondOptions->fPointAndClickOff) {
    // create the glissandoWithText command
    addGlissandoWithTextSchemeFunctionsToScore ();
  }


  if (gLilypondOptions->fJianpu) {
    // create the Jianpu include command JMI
  }

  
  // create the header
  fHeader =
    bsrHeader::create (
      inputLineNumber);

  // create the paper
  fPaper =
    bsrPaper::create (
      inputLineNumber);

  if (gLilypondOptions->fLilypondCompileDate) {
    // define headers and footers
    
    fPaper->
      setOddHeaderMarkup (
R"(\markup {
    \fill-line {
      \on-the-fly \not-first-page {
        \fromproperty #'page:page-number-string
        " "
        \fromproperty #'header:title
        " "
        \fromproperty #'header:subtitle
      }
    }
  }
)"
      );
      
    
    fPaper->
      setEvenHeaderMarkup (
R"(\markup {
    \fill-line {
      \on-the-fly \not-first-page {
        \fromproperty #'page:page-number-string
        " "
        \fromproperty #'header:title
        " "
        \fromproperty #'header:subtitle
      }
    }
  }
)"
      );

    stringstream s;

    s <<
R"(\markup {
    \tiny
    \column {
      \fill-line {
        #(string-append
)"
      <<
      "\"Music generated from MusicXML by xml2ly " <<
      currentVersionNumber () <<
      " and engraved by LilyPond \" (lilypond-version))" <<
R"(
      }
      \fill-line {
        "https://github.com/grame-cncm/libmusicxml/tree/lilypond - http://www.lilypond.org"
      }
      \fill-line { \italic { \modTimeAsString }}
    }
  }
)";
    
    fPaper->
      setOddFooterMarkup (
        s.str ());
  }
  
  // create the score layout
  fScoreLayout =
    bsrLayout::create (
      inputLineNumber);

  // create the 'myBreakIsBreak' assoc
  {
    bsrVarValAssoc::bsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? bsrVarValAssoc::kCommented
          : bsrVarValAssoc::kUncommented;
  
    fMyBreakIsBreakAssoc =
      bsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        bsrVarValAssoc::kWithoutBackSlash,
        bsrVarValAssoc::kLilypondMyBreak,
        bsrVarValAssoc::kEqualSign,
        bsrVarValAssoc::kNoQuotesAroundValue,
        "{ \\break }",
        bsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        "Pick your choice from the next two lines as needed",
        bsrVarValAssoc::kWithoutEndl);
  }
  
  // create the 'myBreakIsEmpty' assoc
  {
    bsrVarValAssoc::bsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? bsrVarValAssoc::kUncommented
          : bsrVarValAssoc::kCommented;
  
    fMyBreakIsEmptyAssoc =
      bsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        bsrVarValAssoc::kWithoutBackSlash,
        bsrVarValAssoc::kLilypondMyBreak,
        bsrVarValAssoc::kEqualSign,
        bsrVarValAssoc::kNoQuotesAroundValue,
        "{ }",
        bsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        bsrVarValAssoc::g_LilyPondVarValAssocNoComment,
        bsrVarValAssoc::kWithEndl);
  }

  // create the 'myPageBreakIsPageBreak' assoc
  {
    bsrVarValAssoc::bsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? bsrVarValAssoc::kCommented
          : bsrVarValAssoc::kUncommented;
  
    fMyPageBreakIsPageBreakAssoc =
      bsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        bsrVarValAssoc::kWithoutBackSlash,
        bsrVarValAssoc::kLilypondMyPageBreak,
        bsrVarValAssoc::kEqualSign,
        bsrVarValAssoc::kNoQuotesAroundValue,
        "{ \\pageBreak }",
        bsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        "Pick your choice from the next two lines as needed",
        bsrVarValAssoc::kWithoutEndl);
  }
  
  // create the 'myPageBreakIsEmpty' assoc
  {
    bsrVarValAssoc::bsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? bsrVarValAssoc::kUncommented
          : bsrVarValAssoc::kCommented;
  
    fMyPageBreakIsEmptyAssoc =
      bsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        bsrVarValAssoc::kWithoutBackSlash,
        bsrVarValAssoc::kLilypondMyPageBreak,
        bsrVarValAssoc::kEqualSign,
        bsrVarValAssoc::kNoQuotesAroundValue,
        "{ }",
        bsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        bsrVarValAssoc::g_LilyPondVarValAssocNoComment,
        bsrVarValAssoc::kWithEndl);
  }

  if (gLilypondOptions->fGlobal) {
    // create the 'global' assoc
    fGlobalAssoc =
      bsrVarValAssoc::create (
        inputLineNumber,
        bsrVarValAssoc::kUncommented,
        bsrVarValAssoc::kWithoutBackSlash,
        bsrVarValAssoc::kLilypondGlobal,
        bsrVarValAssoc::kEqualSign,
        bsrVarValAssoc::kNoQuotesAroundValue,
        "{ }",
        bsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        "Place whatever you need in the 'global' variable",
        bsrVarValAssoc::kWithEndl);
  }
  
  // create the score command
  fScoreBlock =
    bsrScoreBlock::create (
      inputLineNumber);
      */
}

bsrScore::~bsrScore ()
{}

void bsrScore::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScore::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrScore::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrScore::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScore::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrScore::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrScore::browseData (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrScore::browseData ()" <<
      endl;
  }

/*
  {
    // browse the score LilyPond version
    msrBrowser<bsrVarValAssoc> browser (v);
    browser.browse (*fLilypondVersion);
  }

  if (fInputSourceNameComment) {
    // browse the input source name comment
    msrBrowser<bsrComment> browser (v);
    browser.browse (*fInputSourceNameComment);
  }

  if (fTranslationDateComment) {
    // browse the translation date comment
    msrBrowser<bsrComment> browser (v);
    browser.browse (*fTranslationDateComment);
  }

  if (fCommandLineLongOptionsComment) {
    // browse the command line long options comment
    msrBrowser<bsrComment> browser (v);
    browser.browse (*fCommandLineLongOptionsComment);
  }

  if (fCommandLineShortOptionsComment) {
    // browse the command line short options comment
    msrBrowser<bsrComment> browser (v);
    browser.browse (*fCommandLineShortOptionsComment);
  }

  {
    // browse the score global size
    msrBrowser<bsrSchemeVariable> browser (v);
    browser.browse (*fGlobalStaffSizeAssoc);
  }

  {
    // browse the Scheme function map
    for (
      map<string, S_bsrSchemeFunction>::const_iterator i =
        fScoreSchemeFunctionsMap.begin ();
      i != fScoreSchemeFunctionsMap.end ();
      i++) {
      // browse the Scheme function
      msrBrowser<bsrSchemeFunction> browser (v);
      browser.browse (*(*i).second);
    } // for
  }

  {
    // browse the score header
    msrBrowser<bsrHeader> browser (v);
    browser.browse (*fHeader);
  }

  {
    // browse the score paper
    msrBrowser<bsrPaper> browser (v);
    browser.browse (*fPaper);
  }

  {
    // browse the score layout
    msrBrowser<bsrLayout> browser (v);
    browser.browse (*fScoreLayout);
  }

  {
    // browse the myBreakIsBreak assoc
    msrBrowser<bsrVarValAssoc> browser (v);
    browser.browse (*fMyBreakIsBreakAssoc);
  }
  {
    // browse the myBreakIsEmpty assoc
    msrBrowser<bsrVarValAssoc> browser (v);
    browser.browse (*fMyBreakIsEmptyAssoc);
  }

  {
    // browse the myPageBreakIsPageBreak assoc
    msrBrowser<bsrVarValAssoc> browser (v);
    browser.browse (*fMyPageBreakIsPageBreakAssoc);
  }
  {
    // browse the myPageBreakIsEmpty assoc
    msrBrowser<bsrVarValAssoc> browser (v);
    browser.browse (*fMyPageBreakIsEmptyAssoc);
  }

  if (fGlobalAssoc) {
    // browse the 'global' assoc
    msrBrowser<bsrVarValAssoc> browser (v);
    browser.browse (*fGlobalAssoc);
  }

  {
    // browse the voices and stanzas list
    for (
      list<S_msrElement>::const_iterator i = fScoreElements.begin ();
      i != fScoreElements.end ();
      i++) {
      // browse the element
      msrBrowser<msrElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  {
    // browse the score command
    msrBrowser<bsrScoreBlock> browser (v);    
    browser.browse (*fScoreBlock);
  }
*/

  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% <== bsrScore::browseData ()" <<
      endl;
  }
}

void bsrScore::print (ostream& os)
{
  os <<
    "BSR Score" <<
    endl <<
    endl;

  gIndenter++;

  // print the MSR structure (without the voices)
  fMsrScore->
    printSummary (os);
  os <<
    endl;

/*
  // print BSR basic information
  os <<
    fLilypondVersion <<
    endl <<
    
    fGlobalStaffSizeAssoc <<
    endl <<

    fHeader <<
    // no endl here

    fPaper <<
    endl <<

    fScoreLayout <<
    endl;

// myBreakAssoc,myPageBreakAssoc globalAssoc? JMI

  // print the voices
  if (fScoreElements.size ()) {  
    list<S_msrElement>::const_iterator
      iBegin = fScoreElements.begin (),
      iEnd   = fScoreElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    os <<
      endl;
  }

  // print the score block
  os <<
    fScoreBlock;
    */

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrScore& scr)
{
  scr->print (os);
  return os;
}


}
