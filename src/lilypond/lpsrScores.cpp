/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrScores.h"

#include "version.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "xml2lyOptionsHandling.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_lpsrScoreBlock lpsrScoreBlock::create (
  int            inputLineNumber)
{
  lpsrScoreBlock* o = new lpsrScoreBlock (
    inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrScoreBlock::lpsrScoreBlock (
  int            inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  // create the score command parallel music
  fScoreBlockParallelMusicBLock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);
  
  // create the score command layout
  fScoreBlockLayout =
    lpsrLayout::create (
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

lpsrScoreBlock::~lpsrScoreBlock ()
{}

void lpsrScoreBlock::appendPartGroupBlockToScoreBlock (
  S_lpsrPartGroupBlock partGroupBlock)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> getPartGroupCombinedName () <<
       " to LPSR score" <<
       endl;
  }
#endif

  fScoreBlockParallelMusicBLock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);
    
//               fScoreBlockElements.push_back(partGroupBlock);
}

/* JMI
void lpsrScoreBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       endl;
  }
#endif

  fScoreBlockParallelMusicBLock->
    addElementToParallelMusicBLock (voiceUse);
    
//     JMI             fScoreBlockElements.push_back(voiceUse);
}
                  
void lpsrScoreBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       endl;
  }
#endif

  fScoreBlockParallelMusicBLock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrScoreBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScoreBlock::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScoreBlock::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrScoreBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScoreBlock::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScoreBlock::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrScoreBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScoreBlock::browseData ()" <<
      endl;
  }

  {
    // browse the score command parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);    
    browser.browse (*fScoreBlockParallelMusicBLock);
  }

/* JMI
  for (
    vector<S_msrElement>::const_iterator i = fScoreBlockElements.begin ();
    i != fScoreBlockElements.end ();
    i++) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/
  {
    // browse the score command layout
    msrBrowser<lpsrLayout> browser (v);    
    browser.browse (*fScoreBlockLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);    
    browser.browse (*fScoreBlockMidi);
  }

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrScoreBlock::browseData ()" <<
      endl;
  }
}

void lpsrScoreBlock::print (ostream& os)
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

ostream& operator<< (ostream& os, const S_lpsrScoreBlock& scr)
{
  scr->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrScore lpsrScore::create (
  int            inputLineNumber,
  S_msrScore     mScore)
{
  lpsrScore* o = new lpsrScore (
    inputLineNumber, mScore);
  assert(o!=0);
  return o;
}

lpsrScore::lpsrScore (
  int            inputLineNumber,
  S_msrScore     mScore)
    : lpsrElement (inputLineNumber)
{
  fMsrScore = mScore;

  // create the LilyPond version assoc
  fLilypondVersion =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kUncommented,
      lpsrVarValAssoc::kWithBackSlash,
      lpsrVarValAssoc::kLibraryVersion,
      lpsrVarValAssoc::kSpace,
      lpsrVarValAssoc::kQuotesAroundValue,
      "2.19",
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kWithEndl);

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
        lpsrComment::create (
          inputLineNumber,
          s.str (),
          lpsrComment::kNoGapAfterwards);
    }
  
    // create the translation date comment
    {
      stringstream s;
      
      s <<
        "% Translation command was:";
          
      fTranslationDateComment =
        lpsrComment::create (
          inputLineNumber,
          s.str (),
          lpsrComment::kNoGapAfterwards);
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
      lpsrComment::create (
        inputLineNumber,
        s.str (),
        longAndShortOptionsDiffer
          ? lpsrComment::kNoGapAfterwards
          : lpsrComment::kGapAfterwards);
  }

  if (longAndShortOptionsDiffer) {
    // create the command line short options comment
    stringstream s;
    
    s <<
      gTab <<
      gGeneralOptions->fCommandLineWithShortOptions;
    
    fCommandLineShortOptionsComment =
      lpsrComment::create (
        inputLineNumber,
        s.str (),
        lpsrComment::kGapAfterwards);
  }
}

  // create the global staff size assoc
  fGlobalStaffSizeAssoc =
    lpsrSchemeVariable::create (
      inputLineNumber,
      lpsrSchemeVariable::kUncommented,
      "set-global-staff-size",
      "20", // the LilyPond default
      "Comment or adapt next line as needed (default is 20)",
      lpsrSchemeVariable::kWithEndlTwice);

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
    lpsrHeader::create (
      inputLineNumber);

  // create the paper
  fPaper =
    lpsrPaper::create (
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
    lpsrLayout::create (
      inputLineNumber);

  // create the 'myBreakIsBreak' assoc
  {
    lpsrVarValAssoc::lpsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? lpsrVarValAssoc::kCommented
          : lpsrVarValAssoc::kUncommented;
  
    fMyBreakIsBreakAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kLilypondMyBreak,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ \\break }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        "Pick your choice from the next two lines as needed",
        lpsrVarValAssoc::kWithoutEndl);
  }
  
  // create the 'myBreakIsEmpty' assoc
  {
    lpsrVarValAssoc::lpsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? lpsrVarValAssoc::kUncommented
          : lpsrVarValAssoc::kCommented;
  
    fMyBreakIsEmptyAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kLilypondMyBreak,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
        lpsrVarValAssoc::kWithEndl);
  }

  // create the 'myPageBreakIsPageBreak' assoc
  {
    lpsrVarValAssoc::lpsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? lpsrVarValAssoc::kCommented
          : lpsrVarValAssoc::kUncommented;
  
    fMyPageBreakIsPageBreakAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kLilypondMyPageBreak,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ \\pageBreak }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        "Pick your choice from the next two lines as needed",
        lpsrVarValAssoc::kWithoutEndl);
  }
  
  // create the 'myPageBreakIsEmpty' assoc
  {
    lpsrVarValAssoc::lpsrCommentedKind
      commentedKind =
        gLilypondOptions->fIgnoreLineBreaks
          ? lpsrVarValAssoc::kUncommented
          : lpsrVarValAssoc::kCommented;
  
    fMyPageBreakIsEmptyAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        commentedKind,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kLilypondMyPageBreak,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
        lpsrVarValAssoc::kWithEndl);
  }

  if (gLilypondOptions->fGlobal) {
    // create the 'global' assoc
    fGlobalAssoc =
      lpsrVarValAssoc::create (
        inputLineNumber,
        lpsrVarValAssoc::kUncommented,
        lpsrVarValAssoc::kWithoutBackSlash,
        lpsrVarValAssoc::kLilypondGlobal,
        lpsrVarValAssoc::kEqualSign,
        lpsrVarValAssoc::kNoQuotesAroundValue,
        "{ }",
        lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
        "Place whatever you need in the 'global' variable",
        lpsrVarValAssoc::kWithEndl);
  }
  
  // create the score command
  fScoreBlock =
    lpsrScoreBlock::create (
      inputLineNumber);
}

lpsrScore::~lpsrScore ()
{}

void lpsrScore::setGlobalStaffSize (float size)
{
  stringstream s;
  
  s << size;
  
  fGlobalStaffSizeAssoc->
    setVariableValue (s.str ());
}

void lpsrScore::setJianpuFileIncludeIsNeeded ()
{
  if (! fScmAndAccregSchemeModulesAreNeeded) {
    addJianpuFileIncludeToScore ();
    
    fJianpuFileIncludeIsNeeded = true;    
  }
}

void lpsrScore::setScmAndAccregSchemeModulesAreNeeded ()
{
  if (! fScmAndAccregSchemeModulesAreNeeded) {
    addAccordionRegistrationSchemeModulesToScore ();
    
    fScmAndAccregSchemeModulesAreNeeded = true;    
  }
}

void lpsrScore::setCustomShortBarLineSchemeFunctionIsNeeded ()
{
  if (! fCustomShortBarLineSchemeFunctionIsNeeded) {
    addCustomShortBarLineSchemeFunctionToScore ();
    
    fCustomShortBarLineSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setTongueSchemeFunctionIsNeeded ()
{
  if (! fTongueSchemeFunctionIsNeeded) {
    addTongueSchemeFunctionToScore ();
    
    fTongueSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setEditorialAccidentalSchemeFunctionIsNeeded ()
{
  if (! fEditorialAccidentalSchemeFunctionIsNeeded) {
    addEditorialAccidentalSchemeFunctionToScore ();
    
    fEditorialAccidentalSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setDynamicsSchemeFunctionIsNeeded ()
{
  if (! fDynamicsSchemeFunctionIsNeeded) {
    addDynamicsSchemeFunctionToScore ();
    
    fDynamicsSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setTupletsCurvedBracketsSchemeFunctionIsNeeded ()
{
  if (! fTupletsCurvedBracketsSchemeFunctionIsNeeded) {
    addTupletsCurvedBracketsSchemeFunctionToScore ();
    
    fTupletsCurvedBracketsSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setAfterSchemeFunctionIsNeeded ()
{
  if (! fAfterSchemeFunctionIsNeeded) {
    addAfterSchemeFunctionToScore ();
    
    fAfterSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setTempoRelationshipSchemeFunctionIsNeeded ()
{
  if (! fTempoRelationshipSchemeFunctionIsNeeded) {
    addTempoRelationshipSchemeFunctionToScore ();
    
    fTempoRelationshipSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setGlissandoWithTextSchemeFunctionIsNeeded ()
{
  if (! fGlissandoWithTextSchemeFunctionIsNeeded) {
    addGlissandoWithTextSchemeFunctionsToScore ();
    
    fGlissandoWithTextSchemeFunctionIsNeeded = true;    
  }
}

void lpsrScore::setDampMarkupIsNeeded ()
{
  if (! fDampMarkupIsNeeded) {
    addDampMarkupToScore ();
    
    fDampMarkupIsNeeded = true;    
  }
}

void lpsrScore::setDampAllMarkupIsNeeded ()
{
  if (! fDampAllMarkupIsNeeded) {
    addDampAllMarkupToScore ();
    
    fDampAllMarkupIsNeeded = true;    
  }
}

void lpsrScore::setWhiteNoteHeadsIsNeeded ()
{
  if (! fWhiteNoteHeadsIsNeeded) {
    addWhiteNoteHeadsToScore ();
    
    fWhiteNoteHeadsIsNeeded = true;    
  }
}

void lpsrScore::addDateAndTimeSchemeFunctionsToScore ()
{
  string
    schemeFunctionName =
      "date & time",
      
    schemeFunctionDescription =
R"(
% A set of functions to obtain a source file's modification time.
)",

    schemeFunctionCode =
R"(
#(define comml           (object->string (command-line)))
#(define loc             (+ (string-rindex comml #\space ) 2))
#(define commllen        (- (string-length comml) 2))
#(define filen           (substring comml loc commllen))
#(define siz             (object->string (stat:size (stat filen))))
#(define ver             (object->string (lilypond-version)))
#(define dat             (strftime "%d/%m/%Y" (localtime (current-time))))
#(define tim             (strftime "%H:%M:%S" (localtime (current-time))))
#(define modTime         (stat:mtime (stat filen)))
#(define modTimeAsString (strftime "%d/%m/%Y - %H:%M:%S" (localtime modTime)))
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme functions for '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addPointAndClickOffSchemeFunctionsToScore ()
{
  string
    schemeFunctionName =
      "pointAndClickOff",
      
    schemeFunctionDescription =
R"(
% \pointAndClickOff to reduce the size of the produced PDF file.
)",

    schemeFunctionCode =
R"(
\pointAndClickOff
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme functions for '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addGlissandoWithTextSchemeFunctionsToScore ()
{
  string
    schemeFunctionName =
      "glissandoWithText",
      
    schemeFunctionDescription =
R"(
% \\glissandoTextOn/Off to get text along glissandos.
)",

    schemeFunctionCode =
R"(
% thanks to Thomas Morley for contributing this code

%% c/p from lily-library.scm (it is not public)
#(define (sign x)
  (if (= x 0)
      0
      (if (< x 0) -1 1)))

#(define (radians->degree radians)
  (/ (* radians 180) PI))

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% Glissando with text
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% Reads from Glissando.details
%%%%   - glissando-text
%%%%   - glissando-text-font-size
%%%%   - glissando-text-padding

#(define* ((gliss-plus-text #:optional always-add-text) grob)
  (let*  ((orig-grob (ly:grob-original grob))
          (broken-beams (ly:spanner-broken-into orig-grob))
          (stil (ly:line-spanner::print grob)))
    (if (or (null? broken-beams)
            (and (pair? broken-beams)
                 (or
                    always-add-text
                    (equal? grob (car broken-beams)))))
        (if (not (ly:stencil? stil))
            #f
            (let* ((layout (ly:grob-layout grob))
                   (line-thickness
                     (ly:output-def-lookup layout 'line-thickness))
                   (props
                     (ly:grob-alist-chain
                       grob
                       (ly:output-def-lookup layout 'text-font-defaults)))
                   (font
                     (ly:paper-get-font
                     layout
                     (cons
                       '((font-encoding . fetaMusic) (font-name . #f))
                       props)))
                   (trill-glyph
                     (ly:font-get-glyph font "scripts.trill_element"))
                   (trill-glyph-height
                     (interval-length (ly:stencil-extent trill-glyph Y)))
                   (zigzag-width
                     (ly:grob-property grob 'zigzag-width))
                   (thickness (ly:grob-property grob 'thickness))
                   (thick
                     (if (number? thickness)
                         (/ thickness 10)
                         line-thickness))
                   (style (ly:grob-property grob 'style))
                   (corr
                     (cond ((eq? style 'trill)
                             trill-glyph-height)
                           ((eq? style 'zigzag)
                             (+ thick zigzag-width))
                           (else 0)))
                   (stil-x-ext (ly:stencil-extent stil X))
                   (stil-y-ext (ly:stencil-extent stil Y))
                   (stil-x-length (- (cdr stil-x-ext) (car stil-x-ext)))
                   (stil-y-length (- (cdr stil-y-ext) (car stil-y-ext)))
                   (details (ly:grob-property grob 'details))
                   (gliss-text
                     (assoc-get 'glissando-text details "gliss."))
                   (gliss-text-font-size
                     (assoc-get 'glissando-text-font-size details -5))
                   (gliss-text-padding
                     (assoc-get 'glissando-text-padding details 0.5))
                   (left-bound-info (ly:grob-property grob 'left-bound-info))
                   (y-left (assoc-get 'Y left-bound-info))
                   (right-bound-info (ly:grob-property grob 'right-bound-info))
                   (y-right (assoc-get 'Y right-bound-info))
                   (slant (sign (- y-right y-left)))
                   (gradient
                     (/ (- stil-y-length corr) stil-x-length))
                   ;; `ly:stencil-rotate' needs an angle in degrees...
                   ;; TODO use ly:angle ?
                   (alpha
                     (radians->degree
                       (angle
                         (make-rectangular
                           stil-x-length
                           (- stil-y-length corr line-thickness)))))
                   (text-stencil
                     (grob-interpret-markup
                       grob
                         (make-halign-markup CENTER
                           (make-fontsize-markup
                             gliss-text-font-size
                             gliss-text))))
                   (text-stencil-height
                     (interval-length (ly:stencil-extent text-stencil Y)))
                   (y-move
                     (+
                       (/ text-stencil-height 2)
                       gliss-text-padding))
                   (rotated-text-stil
                     (ly:stencil-rotate text-stencil (* slant alpha) 0 0))
                   (text-center-X
                     (interval-center (ly:stencil-extent rotated-text-stil X)))
                   (translated-text-stencil
                     (ly:stencil-translate
                       rotated-text-stil
                       ;; Constuction-helpers
                       ;text-stencil
                       ;(make-cross-stencil '(0 . 0)) ;; not included atm
                       (cons
                         (+
                           (car stil-x-ext)
                           (/ stil-x-length 2)
                           (* slant -1 y-move gradient))
                         (+
                           (car stil-y-ext)
                           (/ stil-y-length 2)
                           y-move)))))
            (ly:stencil-add
              stil
              ;; Construction-helpers
              ;;
              ;(make-line-stencil
              ;  0.1
              ;  (+ (car stil-x-ext)(/ stil-x-length 2))
              ;  (+ (car stil-y-ext)(/ stil-y-length 2))
              ;  (+ (car stil-x-ext)(/ stil-x-length 2)(* slant -1 10 gradient))
              ;  10)
              ;;
              ;; (2) colors the text-part
              ;;
              ;(stencil-with-color translated-text-stencil red)
              translated-text-stencil
              )))
        stil)))

glissandoTextOn =
  \temporary \override Glissando.stencil =
    #(gliss-plus-text
      ;; change to true, if added text is wished for both parts of a broken
      ;; glissando
      ;; TODO added text at second parts needs to be improved, vertical
      ;; positioning is sometimes bad
      #f)

glissandoTextOff = \revert Glissando.stencil
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme functions for '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addCustomShortBarLineSchemeFunctionToScore ()
{
  string
    schemeModulesName =
      "curstom short barline Scheme function",
      
    schemeModulesDescription =
R"(
% The function needed to produce curstom short barlines.
)",

    schemeModulesCode =
R"(
#(define ((make-custom-short-bar-line x y) grob extent)
   "Draw a short bar line."
   (let* ((short-staff (* 1/2 (ly:staff-symbol-staff-space grob)))
          (staff-line-thickness (ly:staff-symbol-line-thickness grob))
          (height (interval-end extent)))
     (bar-line::draw-filled-box
      (cons 0 (+ x staff-line-thickness))
      (cons (- height (* 7 short-staff) x) (- height short-staff x))
      staff-line-thickness
      extent
      grob)))

#(add-bar-glyph-print-procedure "/" (make-custom-short-bar-line 0.1 0.1))

#(define-bar-line "/" "/" #f #f)
)";


  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Including Jianpu definition file '" << schemeModulesName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeModulesName,
        schemeModulesDescription,
        schemeModulesCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeModulesName] =
    schemeFunction;
}

void lpsrScore::addJianpuFileIncludeToScore ()
{
  string
    schemeModulesName =
      "jianpu include file",
      
    schemeModulesDescription =
R"(
% The definitions needed to produce jianpu scores.
)",

    schemeModulesCode =
R"(
% From https://github.com/nybbs2003/lilypond-Jianpu
\include "jianpu10a.ly"
)";
  
  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Including Jianpu definition file '" << schemeModulesName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeModulesName,
        schemeModulesDescription,
        schemeModulesCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeModulesName] =
    schemeFunction;
}

void lpsrScore::addAccordionRegistrationSchemeModulesToScore ()
{
  string
    schemeModulesName =
      "scm & accreg",
      
    schemeModulesDescription =
R"(
% Two modules are to be used in the right order to use accordion registration.
)",

    schemeModulesCode =
R"(
#(use-modules (scm accreg))
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Using Scheme modules '" << schemeModulesName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeModulesName,
        schemeModulesDescription,
        schemeModulesCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeModulesName] =
    schemeFunction;
}

void lpsrScore::addTongueSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "tongue",
      
    schemeFunctionDescription =
R"(
% Creates multiple tongue technicals, argument is a number.
% Example: 'c4 -\tongue #3' creates a triple tongue.
)",

    schemeFunctionCode =
R"(
tongue =
#(define-music-function (parser location dots) (integer?)
   (let ((script (make-music 'ArticulationEvent
                   'articulation-type "staccato")))
     (set! (ly:music-property script 'tweaks)
           (acons 'stencil
             (lambda (grob)
               (let ((stil (ly:script-interface::print grob)))
                 (let loop ((count (1- dots)) (new-stil stil))
                   (if (> count 0)
                       (loop (1- count)
                         (ly:stencil-combine-at-edge new-stil X RIGHT stil 0.2))
                       (ly:stencil-aligned-to new-stil X CENTER)))))
             (ly:music-property script 'tweaks)))
     script))
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addEditorialAccidentalSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "editorialAccidental",
      
    schemeFunctionDescription =
R"(
% Craetes editorial accidentals as LilyPond musica ficta.
% Example: '\editorialAccidental cis4'.
)",

    schemeFunctionCode =
R"(
editorialAccidental =
#(define-music-function
  (note)
  (ly:music?)
  #{
    \once\accidentalStyle forget
    \once\set suggestAccidentals = ##t
    #note
  #})
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addDynamicsSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "dynamics",
      
    schemeFunctionDescription =
R"(
% Creates variables define dynamics not native to LilyPond.
)",

    schemeFunctionCode =
R"(
rf = #(make-dynamic-script "rf")
sfpp = #(make-dynamic-script "sfpp")
sffz = #(make-dynamic-script "sffz")
ppppp = #(make-dynamic-script "ppppp")
pppppp = #(make-dynamic-script "pppppp")
fffff = #(make-dynamic-script "fffff")
ffffff = #(make-dynamic-script "ffffff")
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addTupletsCurvedBracketsSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "tupletsCurvedBrackets",
      
    schemeFunctionDescription =
R"(
% A function to draw curved tuplets brackets, not native to LilyPond.
% Thanks to Ben, mailto:soundsfromsound@gmail.com
)",

    schemeFunctionCode =
R"(
tupletsCurvedBrackets = {
  % Use slur-stencil
  \override TupletBracket.stencil = #ly:slur::print
  %% Use 'thickness from Slur
  \override TupletBracket.thickness = #1.2
  %% 'control-points need to be set
  \override TupletBracket.control-points =
  #(lambda (grob)
     (let* ((x-pos (ly:grob-property grob 'X-positions))
            (pos (ly:grob-property grob 'positions))
            (x-ln (interval-length x-pos))
            (dir (ly:grob-property grob 'direction))
            ;; read out the height of the TupletBracket, may be
            ;; negative!
            (height (- (cdr pos) (car pos)))
            ;; height-corr is introduced because sometimes the shape
            ;; of the slur needs to be adjusted.
            ;; It is used in the 2nd/3rd control-point.
            ;; The value of 0.3 is found by trial and error
            (height-corr (* 0.3 dir height))
            (edge-height (ly:grob-property grob 'edge-height
                           '(0.7 . 0.7)))
            (pad 1.0))
       (list
        ;; first cp
        (cons
         (+ (car x-pos) 0.5)
         (- (+ (* dir pad) (+ (car pos) (* -1 dir
                                          (car edge-height))))
           (if (= dir -1)
               (if (> height 3)
                   (/ dir 2.0)
                   0.0)
               (if (< height -3)
                   (/ dir 2.0)
                   0.0))))
        ;; second cp
        (cons
         (+ (car x-pos) (* x-ln 1/4))
         (+ (* dir pad) (+ (car pos) (* dir (+ 0.5 height-corr)))))
        ;; third cp
        (cons
         (+ (car x-pos) (* x-ln 3/4))
         (+ (* dir pad) (+ (cdr pos) (* dir (- 0.5 height-corr)))))
        ;; fourth cp
        (cons
         (- (cdr x-pos) 0.5)
         (+ (* dir pad) (+ (cdr pos) (* -1 dir (cdr edge-height)))))
        )))
  \override TupletBracket.staff-padding = #' ()
  #(define (invert-direction x) (if (eq? UP
                                         (ly:tuplet-bracket::calc-direction x)) DOWN UP))
  % \override TupletBracket.direction = #invert-direction
}
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addAfterSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "after",
      
    schemeFunctionDescription =
R"(
% A function to create events after given music.
% Thanks to David Kastrup for the inspiration!
)",

    schemeFunctionCode =
R"(
after =
#(define-music-function (t e m) (ly:duration? ly:music? ly:music?)
   #{
     \context Bottom <<
       #m
       { \skip $t <> -\tweak extra-spacing-width #empty-interval $e }
     >>
   #})
)";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addTempoRelationshipSchemeFunctionToScore ()
{
  string
    schemeFunctionName =
      "tempoRelationship",
      
    schemeFunctionDescription =
R"(
% A function to create tempo relationships,
% such as 'b8 [ b8 ]' = '\tuplet 3/2 { b4 b8 }' for swing.
% See http://lsr.di.unimi.it/LSR/Item?id=204
)",

    schemeFunctionCode =
      // add ! before ( and after ) since the code contains )"
R"!(
tempoRelationshipStaffReduce = #-3

tempoRelationship =
#(define-music-function (parser location label parenthesized musicI musicII)
   (string? boolean? ly:music? ly:music?)
   (let* (
           (left-paren (if parenthesized "(" ""))
           (right-paren (if parenthesized ")" ""))
           )
     #{
       \tempo \markup {
         \line \general-align #Y #DOWN {
           % 1st column in line
           $label

           % 2nd column in line

           $left-paren

           \score {
             \new Staff \with {
               % reduce the font size a la cue
               fontSize = #tempoRelationshipStaffReduce
               \override StaffSymbol.staff-space = #(magstep tempoRelationshipStaffReduce)
               % hide the staff lines
               \override StaffSymbol.line-count = #0
               % align horizontally
               \override VerticalAxisGroup.Y-extent = #'(-0.85 . 0)
             }

             {
               % \override Score.SpacingSpanner.common-shortest-duration = #(ly:make-moment 1/2) % super-tight
               % \override Score.SpacingSpanner.common-shortest-duration = #(ly:make-moment 1/4) % tight
               % \override Score.SpacingSpanner.common-shortest-duration = #(ly:make-moment 3/16) % even
               \override Score.SpacingSpanner.common-shortest-duration = #(ly:make-moment 5/32) % even

               % the left music
               \relative c' { \stemUp $musicI }

               % the equivalence sign
               \once \override Score.TextScript.Y-offset = #-0.4
               s4.^\markup{
                 \halign #-1 "="
               }

               % the right music
               \relative c' { \stemUp $musicII }
             }

             \layout {
               indent = 0
               \context {
                 \Staff
                 \remove "Clef_engraver"
                 \remove "Time_signature_engraver"
               }
             } % layout end
           } % score end

           $right-paren

         } % line end
       } % markup end
     #}))
)!";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addDampMarkupToScore ()
{
  string
    schemeFunctionName =
      "dampMarkup",
      
    schemeFunctionDescription =
R"(
% A function to create damp markups,
)",

    schemeFunctionCode =
      // add ! before ( and after ) since the code contains )"
R"!(
damp = \markup {
%  \scale #'(5 . 5)
  {
    \center-column {
      {
        \override #'(thickness . 1.8)
        \combine \draw-line #'(-1.5 . 0)
        \combine \draw-line #'(0 . -1.5)
        \combine \draw-line #'(0 . 1.5)
        \combine \draw-line #'(1.5 . 0)
        \draw-circle #0.8 #0.2 ##f
      }
    }
  }

}
)!";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addDampAllMarkupToScore ()
{
  string
    schemeFunctionName =
      "dampAllMarkup",
      
    schemeFunctionDescription =
R"(
% A function to create damp all markups,
)",

    schemeFunctionCode =
      // add ! before ( and after ) since the code contains )"
R"!(
dampAll = \markup
%% do not use 'fontsize
%\scale #'(5 . 5)
{
  \combine \bold "O"
  \path #0.2
  #'((moveto -.4 .8)(lineto 2.2 .8)
      (closepath)
      (moveto .9 -.5)(lineto .9 2.1))
}
)!";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

void lpsrScore::addWhiteNoteHeadsToScore ()
{
  string
    schemeFunctionName =
      "whiteNoteHeads",
      
    schemeFunctionDescription =
R"(
% A function to display note shorter than a quarter with white heads,
% as in some ancient music scores
)",

    schemeFunctionCode =
      // add ! before ( and after ) since the code contains )"
R"!(
whiteNoteHeads =
#(define-music-function (music) (ly:music?)
   #{
     \temporary \override NoteHead.stencil = #ly:text-interface::print
     \temporary \override NoteHead.text = \markup {
       \musicglyph "noteheads.s1"
     }
     \omit \time 3/4
     \shiftDurations #1 #0 { #music }
     \omit \time 3/2
     \revert NoteHead.stencil
     \revert NoteHead.text
   #}
   )
)!";

  if (gLpsrOptions->fTraceSchemeFunctions) {
    gLogIOstream <<
      "Creating Scheme function '" << schemeFunctionName << "'" <<
      endl;
  }

  // create the Scheme function
  S_lpsrSchemeFunction
    schemeFunction =
      lpsrSchemeFunction::create (
        1, // inputLineNumber, JMI ???
        schemeFunctionName,
        schemeFunctionDescription,
        schemeFunctionCode);

  // register it in the Scheme functions map
  fScoreSchemeFunctionsMap [schemeFunctionName] =
    schemeFunction;
}

/* JMI
void lpsrScore::appendVoiceUseToStoreCommand (S_msrVoice voice)
{
  S_lpsrUseVoiceCommand
    useVoiceCommand =
      lpsrUseVoiceCommand::create (
        fInputLineNumber,
        voice);
  
  fScoreBlock->
    appendVoiceUseToParallelMusicBLock (useVoiceCommand);
}

void lpsrScore::appendLyricsUseToStoreCommand (S_msrStanza stanza)
{
  S_lpsrNewLyricsBlock
    newLyricsCommand =
      lpsrNewLyricsBlock::create (
        fInputLineNumber,
        stanza,
        stanza->getStanzaVoiceUplink ());
  
  fScoreBlock->
    appendLyricsUseToParallelMusicBLock (newLyricsCommand);
}
*/

void lpsrScore::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScore::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrScore>*
    p =
      dynamic_cast<visitor<S_lpsrScore>*> (v)) {
        S_lpsrScore elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScore::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrScore::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScore::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrScore>*
    p =
      dynamic_cast<visitor<S_lpsrScore>*> (v)) {
        S_lpsrScore elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScore::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrScore::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScore::browseData ()" <<
      endl;
  }

  {
    // browse the score LilyPond version
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fLilypondVersion);
  }

  if (fInputSourceNameComment) {
    // browse the input source name comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fInputSourceNameComment);
  }

  if (fTranslationDateComment) {
    // browse the translation date comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fTranslationDateComment);
  }

  if (fCommandLineLongOptionsComment) {
    // browse the command line long options comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fCommandLineLongOptionsComment);
  }

  if (fCommandLineShortOptionsComment) {
    // browse the command line short options comment
    msrBrowser<lpsrComment> browser (v);
    browser.browse (*fCommandLineShortOptionsComment);
  }

  {
    // browse the score global size
    msrBrowser<lpsrSchemeVariable> browser (v);
    browser.browse (*fGlobalStaffSizeAssoc);
  }

  {
    // browse the Scheme function map
    for (
      map<string, S_lpsrSchemeFunction>::const_iterator i =
        fScoreSchemeFunctionsMap.begin ();
      i != fScoreSchemeFunctionsMap.end ();
      i++) {
      // browse the Scheme function
      msrBrowser<lpsrSchemeFunction> browser (v);
      browser.browse (*(*i).second);
    } // for
  }

  {
    // browse the score header
    msrBrowser<lpsrHeader> browser (v);
    browser.browse (*fHeader);
  }

  {
    // browse the score paper
    msrBrowser<lpsrPaper> browser (v);
    browser.browse (*fPaper);
  }

  {
    // browse the score layout
    msrBrowser<lpsrLayout> browser (v);
    browser.browse (*fScoreLayout);
  }

  {
    // browse the myBreakIsBreak assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMyBreakIsBreakAssoc);
  }
  {
    // browse the myBreakIsEmpty assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMyBreakIsEmptyAssoc);
  }

  {
    // browse the myPageBreakIsPageBreak assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMyPageBreakIsPageBreakAssoc);
  }
  {
    // browse the myPageBreakIsEmpty assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMyPageBreakIsEmptyAssoc);
  }

  if (fGlobalAssoc) {
    // browse the 'global' assoc
    msrBrowser<lpsrVarValAssoc> browser (v);
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
    msrBrowser<lpsrScoreBlock> browser (v);    
    browser.browse (*fScoreBlock);
  }

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrScore::browseData ()" <<
      endl;
  }
}

void lpsrScore::print (ostream& os)
{
  os <<
    "LPSR Score" <<
    endl <<
    endl;

  gIndenter++;

  // print the MSR structure (without the voices)
  fMsrScore->
    printSummary (os);
  os <<
    endl;

  // are some Scheme functions needed?
  const int fieldWidth = 42;
  
  os << left <<
    setw (fieldWidth) <<
    "TongueSchemeFunctionIsNeeded" << " : " <<
    booleanAsString (
      fTongueSchemeFunctionIsNeeded) <<
    endl <<
    setw (fieldWidth) <<
    "EditorialAccidentalSchemeFunctionIsNeeded" << " : " <<
    booleanAsString (
      fEditorialAccidentalSchemeFunctionIsNeeded) <<
    endl <<
    endl;

  // print LPSR basic information
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

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrScore& scr)
{
  scr->print (os);
  return os;
}


}
