/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "utilities.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"
#include "brailleOah.h"

#include "bsrBasicTypes.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_brailleOah gGlobalBrailleOah;
S_brailleOah gGlobalBrailleOahUserChoices;
S_brailleOah gGlobalBrailleOahWithDetailedTrace;

S_brailleOah brailleOah::create (
  S_oahHandler handlerUpLink)
{
  brailleOah* o = new brailleOah (
    handlerUpLink);
  assert(o!=0);
  return o;
}

brailleOah::brailleOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Braille",
    "hbrl", "help-braille",
R"(These options control how the Braille music code is generated.)",
    kElementVisibilityWhole,
    handlerUpLink)
{
  // append this braille group to the braille handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeBrailleOah (false);
}

brailleOah::~brailleOah ()
{}

void brailleOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOah>*
    p =
      dynamic_cast<visitor<S_brailleOah>*> (v)) {
        S_brailleOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOah>*
    p =
      dynamic_cast<visitor<S_brailleOah>*> (v)) {
        S_brailleOah elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching brailleOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> brailleOah::browseData ()" <<
      endl;
  }
#endif
}

void brailleOah::initializeBrailleOah (
  bool boolOptionsInitialValue)
{}

S_brailleOah brailleOah::createCloneWithDetailedTrace ()
{
  S_brailleOah
    clone =
      brailleOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the braille handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;;

  return clone;
}

//______________________________________________________________________________
void brailleOah::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void brailleOah::checkGroupOptionsConsistency ()
{
  // JMI
}

void brailleOah::printBrailleOahValues (int fieldWidth)
{
  gLogOstream <<
    "The Braille options are:" <<
    endl;

  gIndenter++;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_brailleOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBrailleOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
    gLogOstream <<
      "Initializing Braille braille handling" <<
      endl;
  }
#endif

  // create the braille variables
  // ------------------------------------------------------

  gGlobalBrailleOahUserChoices = brailleOah::create (
    handler);
  assert(gGlobalBrailleOahUserChoices != 0);

  gGlobalBrailleOah =
    gGlobalBrailleOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gGlobalBrailleOahWithDetailedTrace =
    gGlobalBrailleOah->
      createCloneWithDetailedTrace ();
      */
}


}


  /*
  // identification
  // --------------------------------------

  {
    // variables

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Identification",
          "hlpi", "help-braille-identification",
R"(These braille can be used to enforce values in the generated Braille code,
thus overriding the ones that may be present in the MSR data.
)",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // MusicXML informations

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "rights", "",
R"(Set the 'rights' to STRING in the Braille code.)",
          "STRING",
          "rights",
          fRights));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "composer", "",
R"(Set the 'composer' to STRING in the Braille code.)",
          "STRING",
          "composer",
          fComposer));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "arranger", "",
R"(Set the 'arranger' to STRING in the Braille code.)",
          "STRING",
          "arranger",
          fArranger));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "poet", "",
R"(Set the 'poet' to STRING in the Braille code.)",
          "STRING",
          "poet",
          fPoet));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "lyricist", "",
R"(Set the 'lyricist' to STRING in the Braille code.)",
          "STRING",
          "lyricist",
          fLyricist));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "software", "",
R"(Set the 'software' to STRING in the Braille code.)",
          "STRING",
          "software",
          fSoftware));

    // Braille informations

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "dedication", "",
R"(Set 'dedication' to STRING in the \header.)",
          "STRING",
          "dedication",
          fDedication));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "piece", "",
R"(Set 'piece' to STRING in the \header.)",
          "STRING",
          "piece",
          fPiece));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "opus", "",
R"(Set 'opus' to STRING in the \header.)",
          "STRING",
          "opus",
          fOpus));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "title", "",
R"(Set 'title' to STRING in the \header.)",
          "STRING",
          "title",
          fTitle));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "stitle", "subtitle",
R"(Set 'subtitle' to STRING in the \header.)",
          "STRING",
          "subTitle",
          fSubTitle));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "sstitle", "subsubtitle",
R"(Set 'subsubtitle' to STRING in the \header.)",
          "STRING",
          "subSubTitle",
          fSubSubTitle));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "instrument", "",
R"(Set 'instrument' to STRING in the \header.)",
          "STRING",
          "instrument",
          fInstrument));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "meter", "",
R"(Set 'meter' to STRING in the \header.)",
          "STRING",
          "meter",
          fMeter));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "tagline", "",
R"(Set 'tagline' to STRING in the \header.)",
          "STRING",
          "tagline",
          fTagline));

    subGroup->
      appendAtomToSubGroup (
        oahStringAtom::create (
          "copyright", "",
R"(Set 'copyright' to STRING in the \header.)",
          "STRING",
          "copyright",
          fCopyright));
  }


  // notes
  // --------------------------------------

  {
    // variables

    fAbsoluteOctaves  = boolOptionsInitialValue;

    fAllDurations  = boolOptionsInitialValue;

    fRomanStringNumbers = boolOptionsInitialValue;
    fAvoidOpenStrings    = boolOptionsInitialValue;

    fCompressMultiMeasureRests = boolOptionsInitialValue;

    fInputLineNumbers = boolOptionsInitialValue;

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Notes",
          "hlpn", "help-braille-notes",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "abs", "absolute",
R"(Generate Braille absolute note octaves.
By default, relative octaves are generated.)",
          "absoluteOctaves",
          fAbsoluteOctaves));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "alldurs", "all-durations",
R"(Generate all Braille durations.
By default, a duration equal to preceding one found in the current voice
is omitted for code conciseness.)",
          "allDurations",
          fAllDurations));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "rsn", "roman-string-numbers",
R"(Generate '\romanStringNumbers' in each voice
for Braille to generate roman instead of arabic string numbers.)",
          "romanStringNumbers",
          fRomanStringNumbers));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "aos", "avoid-open-strings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice
to prevent Braille from using open strings.)",
          "avoidOpenStrings",
          fAvoidOpenStrings));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "niln", "note-input-line-numbers",
          regex_replace (
R"(Generate after each note and barline a comment containing
its MusicXML input line number.
This is useful when debugging EXECUTABLE.)",
            regex ("EXECUTABLE"),
            gGlobalOahOahGroup->fHandlerExecutableName),
          "noteInputLineNumbers",
          fInputLineNumbers));
  }


  // bars
  // --------------------------------------

  {
    // variables

    fShowAllBarNumbers = boolOptionsInitialValue;

    // braille

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Bars",
          "hlpb", "help-braille-bars",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "abn", "all-bar-numbers",
R"(Generate Braille code to show all bar numbers.)",
          "showAllBarNumbers",
          fShowAllBarNumbers));
  }


  // line breaks
  // --------------------------------------

  {
    // variables

    fIgnoreLineBreaks                    = boolOptionsInitialValue;

    fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;

    // braille

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Line breaks",
          "hlplb", "help-braille-line-breaks",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "ilb", "ignore-line-breaks",
R"(Ignore the line breaks from the MusicXML input
and let Braille decide about them.)",
          "ignoreLineBreaks",
          fIgnoreLineBreaks));

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "blairm", "break-lines-at-incomplete-right-measures",
R"(Generate a '\break' command at the end of incomplete right measures
which is handy in popular folk dances and tunes.)",
          "breakLinesAtIncompleteRightMeasures",
          fBreakLinesAtIncompleteRightMeasures));
  }


  // page breaks
  // --------------------------------------

  {
    // variables

    fIgnorePageBreaks = boolOptionsInitialValue;

    // braille

    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Page breaks",
          "hlppb", "help-braille-page-breaks",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    subGroup->
      appendAtomToSubGroup (
        oahBooleanAtom::create (
          "ipb", "ignore-page-breaks",
R"(Ignore the page breaks from the MusicXML input
and let Braille decide about them.)",
          "ignorePageBreaks",
          fIgnorePageBreaks));
  }
        */
