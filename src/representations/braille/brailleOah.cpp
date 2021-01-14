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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"
#include "brailleOah.h"

#include "bsrBasicTypes.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_brailleOahGroup gGlobalBrailleOahGroup;

S_brailleOahGroup brailleOahGroup::create ()
{
  brailleOahGroup* o = new brailleOahGroup ();
  assert (o != nullptr);
  return o;
}

brailleOahGroup::brailleOahGroup ()
  : oahGroup (
    "Braille group",
    "hbrl-group", "help-braille-group",
R"(These options control how the Braille music code is generated.)",
    kElementVisibilityWhole)
{
  initializeBrailleOahGroup ();
}

brailleOahGroup::~brailleOahGroup ()
{}

void brailleOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOahGroup>*
    p =
      dynamic_cast<visitor<S_brailleOahGroup>*> (v)) {
        S_brailleOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void brailleOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_brailleOahGroup>*
    p =
      dynamic_cast<visitor<S_brailleOahGroup>*> (v)) {
        S_brailleOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching brailleOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void brailleOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> brailleOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void brailleOahGroup::initializeBrailleOahGroup ()
{}

//______________________________________________________________________________
void brailleOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void brailleOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

void brailleOahGroup::printBrailleOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The Braille options are:" <<
    endl;

  ++gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_brailleOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_brailleOahGroup createGlobalBrailleOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global braille OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalBrailleOahGroup) {
    // create the global options group
    gGlobalBrailleOahGroup =
      brailleOahGroup::create ();
    assert (gGlobalBrailleOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalBrailleOahGroup;
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

    fAbsoluteOctaves  = false;

    fAllDurations  = false;

    fRomanStringNumbers = false;
    fAvoidOpenStrings    = false;

    fCompressMultiMeasureRests = false;

    fInputLineNumbers = false;

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
            gGlobalOahOahGroup->getHandlerExecutableName ()),
          "noteInputLineNumbers",
          fInputLineNumbers));
  }


  // bars
  // --------------------------------------

  {
    // variables

    fShowAllBarNumbers = false;

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

    fIgnoreLineBreaks                    = false;

    fBreakLinesAtIncompleteRightMeasures = false;

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

    fIgnorePageBreaks = false;

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
