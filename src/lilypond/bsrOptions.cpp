/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "utilities.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "generalOptions.h"
#include "bsrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string facSimileKindAsString (
  bsrFacSimileKind facSimileKind)
{
  string result;

  switch (facSimileKind) {
    case kFacSimileYes:
      result = "facSimileYes";
      break;
    case kFacSimileNo:
      result = "facSimileNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_bsrFacSimileKindAtom bsrFacSimileKindAtom::create (
  string           shortName,
  string           longName,
  string           description,
  string           valueSpecification,
  string           variableName,
  bsrFacSimileKind bsrFacSimileKindVariable)
{
  bsrFacSimileKindAtom* o = new
    bsrFacSimileKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      bsrFacSimileKindVariable);
  assert(o!=0);
  return o;
}

bsrFacSimileKindAtom::bsrFacSimileKindAtom (
  string           shortName,
  string           longName,
  string           description,
  string           valueSpecification,
  string           variableName,
  bsrFacSimileKind bsrFacSimileKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fBsrFacSimileKindVariable (
      bsrFacSimileKindVariable)
{}

bsrFacSimileKindAtom::~bsrFacSimileKindAtom ()
{}

void bsrFacSimileKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'bsrFacSimileKindAtom'" <<
      endl;
  }
#endif

}

void bsrFacSimileKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsFacSimileKindAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "bsrFacSimileKindVariable" << " : \"" <<
    facSimileKindAsString (
      fBsrFacSimileKindVariable) <<
      "\"" <<
    endl;
}

void bsrFacSimileKindAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    facSimileKindAsString (
      fBsrFacSimileKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrFacSimileKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrTextsLanguageAtom bsrTextsLanguageAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  bsrTextsLanguageKind&
                     bsrTextsLanguageKindVariable)
{
  bsrTextsLanguageAtom* o = new
    bsrTextsLanguageAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      bsrTextsLanguageKindVariable);
  assert(o!=0);
  return o;
}

bsrTextsLanguageAtom::bsrTextsLanguageAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  bsrTextsLanguageKind&
                     bsrTextsLanguageKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fBsrTextsLanguageKindVariable (
      bsrTextsLanguageKindVariable)
{}

bsrTextsLanguageAtom::~bsrTextsLanguageAtom ()
{}

void bsrTextsLanguageAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'bsrTextsLanguageAtom'" <<
      endl;
  }
#endif

}

void bsrTextsLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsBsrTextsLanguageAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fBsrTextsLanguageKindVariable" << " : \"" <<
    bsrTextsLanguageKindAsString (
      fBsrTextsLanguageKindVariable) <<
    "\"" <<
    endl;
}

void bsrTextsLanguageAtom::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrTextsLanguageKindAsString (
      fBsrTextsLanguageKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrTextsLanguageAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_bsrOptions gBsrOptions;
S_bsrOptions gBsrOptionsUserChoices;
S_bsrOptions gBsrOptionsWithDetailedTrace;

S_bsrOptions bsrOptions::create (
  S_oahHandler handler)
{
  bsrOptions* o = new bsrOptions (
    handler);
  assert(o!=0);
  return o;
}

bsrOptions::bsrOptions (
  S_oahHandler handler)
  : oahGroup (
    "BSR",
    "hbsr", "help-bsr",
R"(These bsr control the way BSR data is handled.)",
    kElementVisibilityAlways,
    handler)
{
  // append this bsr group to the bsr handler
  // if relevant
  if (handler) {
    handler->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeBsrOptions (false);
}

bsrOptions::~bsrOptions ()
{}

void bsrOptions::initializeBsrDisplayOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    traceAndDisplaySubGroup =
      oahSubGroup::create (
        "Display",
        "hbsrtd", "help-bsr-display",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (traceAndDisplaySubGroup);

  // BSR

  fDisplayBsr = boolOptionsInitialValue;

  traceAndDisplaySubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "dbsr", "display-bsr",
R"(Write the contents of the BSR data to standard error.)",
        "displayBsr",
        fDisplayBsr,
        gBsrOptions->fDisplayBsr));

  // BSR details

  fDisplayBsrDetails = boolOptionsInitialValue;

  traceAndDisplaySubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "dbsrd", "display-bsr-details",
R"(Write the contents of the BSR data with more details to standard error.)",
        "displayBsrDetails",
        fDisplayBsrDetails,
        gBsrOptions->fDisplayBsrDetails));
}

void bsrOptions::initializeBsrLanguagesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    languagesSubGroup =
      oahSubGroup::create (
        "Languages",
        "hbsrl", "help-bsr-languages",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (languagesSubGroup);

  // texts language

  if (! setBsrTextsLanguage ("english")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "BSR texts language 'english' is unknown" <<
      endl <<
      "The " <<
      gBsrTextsLanguageKindsMap.size () <<
      " known BSR texts languages are:" <<
      endl;

    gIndenter++;

    s <<
      existingBsrTextsLanguageKinds ();

    gIndenter--;

    optionError (s.str ());
  }

  const bsrTextsLanguageKind
    bsrTextsLanguageKindDefaultValue =
      fBsrTextsLanguageKind;

  languagesSubGroup->
    appendAtom (
      bsrTextsLanguageAtom::create (
        "btl", "bsr-texts-language",
        replaceSubstringInString (
          replaceSubstringInString (
            replaceSubstringInString (
R"(Use LANGUAGE to transcribe texts in the BSR logs and views,
as well as in the generated braille music.
The 4 NUMBER texts languages available are:
TEXT_LANGUAGES.
english, german, italian and french.
The default is 'DEFAULT_VALUE'.)",
              "NUMBER",
              to_string (gBsrTextsLanguageKindsMap.size ())),
            "TEXT_LANGUAGES",
            existingBsrTextsLanguageKinds ()),
          "DEFAULT_VALUE",
          bsrTextsLanguageKindAsString (
            bsrTextsLanguageKindDefaultValue)),
        "LANGUAGE",
        "bsr-texts-language",
        fBsrTextsLanguageKind));
}

void bsrOptions::initializeBsrMiscellaneousOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    miscellaneousGenerationSubGroup =
      oahSubGroup::create (
        "Miscellaneous",
        "hlpm", "help-miscellaneous",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (miscellaneousGenerationSubGroup);

  // braille lyrics

  fNoBrailleLyrics      = boolOptionsInitialValue;

  miscellaneousGenerationSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "nolpl", "no-braille-lyrics",
R"(Don't generate any lyrics in the Braille code.)",
        "noBrailleLyrics",
        fNoBrailleLyrics));

  // braille compile date

  fBrailleCompileDate   = boolOptionsInitialValue;

  miscellaneousGenerationSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "lpcd", "braille-compile-date",
R"(Generate code to include the compilation date
when Braille creates the score.)",
        "brailleCompileDate",
        fBrailleCompileDate));

  // facsimile

  fFacSimileKind        = kFacSimileNo;

  miscellaneousGenerationSubGroup->
    appendAtom (
      bsrFacSimileKindAtom::create (
        "fs", "facsimile",
R"(Generate facsimile Braille nusic code.
By default, non-facsimile code is generated.)",
        "YES_OR_NO",
        "facSimileKind",
        fFacSimileKind));

  // clefs

  fIncludeClefs         = boolOptionsInitialValue;

  miscellaneousGenerationSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "clefs", "",
R"(Include clefs in BSR. By default, they are not.)",
        "includeClefs",
        fIncludeClefs));
}

void bsrOptions::initializeBsrExitAfterSomePassesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    exitAfterSomePassesSubGroup =
      oahSubGroup::create (
        "Exit after some passes",
        "hbe", "help-bsr-exit",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroup (exitAfterSomePassesSubGroup);

  // '-exit-3a' is hidden...

  fExit3a = boolOptionsInitialValue;

  S_oahBooleanAtom
    exit2aOptionsBooleanAtom =
      oahBooleanAtom::create (
        "e3a", "exit-3a",
R"(Exit after pass 3a, i.e. after conversion
of the MSR to the first BSR score.)",
        "exit3a",
        fExit3a);
        /* JMI
  exit2aOptionsBooleanAtom->
    setIsHidden ();
    */

  exitAfterSomePassesSubGroup->
    appendAtom (
      exit2aOptionsBooleanAtom);

  // '-exit-3b' is hidden... JMI ???

  fExit3b = boolOptionsInitialValue;

  S_oahBooleanAtom
    exit2bOptionsBooleanAtom =
      oahBooleanAtom::create (
        "e3b", "exit-3b",
R"(Exit after pass 3b, i.e. after conversion
of the first BSR to the second BSR.)",
        "exit3b",
        fExit3b);
        /* JMI
  exit2bOptionsBooleanAtom->
    setIsHidden ();
    */

  exitAfterSomePassesSubGroup->
    appendAtom (
      exit2bOptionsBooleanAtom);
}

#ifdef TRACE_OPTIONS
void bsrOptions::initializeBsrTraceOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    traceSubGroup =
      oahSubGroup::create (
        "Trace",
        "hbst", "help-bsr-trace",
R"(Note: the bsr in this group imply '-tbsr, -trace-bsr'.)",
// JMI        kElementVisibilityHiddenByDefault,
      kElementVisibilityAlways,
      this);

  appendSubGroup (traceSubGroup);

  // BSR

  fTraceBsr = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tbsr", "trace-bsr",
R"(Write a trace of the BSR graphs visiting activity to standard error.)",
        "traceBsr",
        fTraceBsr));

  // pages

  fTracePages = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tpages", "trace-pages",
R"()",
        "tracePages",
        fTracePages,
        gTraceOptions->fTracePasses));

  // lines

  fTraceLines = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tlines", "trace-lines",
R"()",
        "traceLines",
        fTraceLines));

  // spaces

  fTraceSpaces = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tspaces", "trace-spaces",
R"(Write a trace of the BSR spaces activity to standard error.)",
        "traceSpaces",
        fTraceSpaces));

  // numbers

  fTraceNumbers = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tnums", "trace-numbers",
R"(Write a trace of the BSR numbers activity to standard error.)",
        "traceNumbers",
        fTraceNumbers));

  // parallels

  fTraceParallels = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahTwoBooleansAtom::create (
        "tpars", "trace-parallels",
R"()",
        "traceParallels",
        fTraceParallels,
        gTraceOptions->fTracePasses));

  // BSR visitors

  fTraceBsrVisitors     = boolOptionsInitialValue;

  traceSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "tbsrv", "trace-bsr-visitors",
R"(Write a trace of the BSR tree visiting activity to standard error.)",
        "traceBsrVisitors",
        fTraceBsrVisitors));
}
#endif

void bsrOptions::initializeBsrOptions (
  bool boolOptionsInitialValue)
{
  // display
  // --------------------------------------
  initializeBsrDisplayOptions (
    boolOptionsInitialValue);

  // languages
  // --------------------------------------
  initializeBsrLanguagesOptions (
    boolOptionsInitialValue);

  // miscellaneous
  // --------------------------------------
  initializeBsrMiscellaneousOptions (
    boolOptionsInitialValue);

  // exit after some passes
  // --------------------------------------
  initializeBsrExitAfterSomePassesOptions (
    boolOptionsInitialValue);

#ifdef TRACE_OPTIONS
  // trace
  // --------------------------------------
  initializeBsrTraceOptions (
    boolOptionsInitialValue);
#endif
}

S_bsrOptions bsrOptions::createCloneWithDetailedTrace ()
{
  S_bsrOptions
    clone =
      bsrOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the bsr handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;


  // display
  // --------------------------------------

  clone->fDisplayBsr =
    true;
  clone->fDisplayBsrDetails =
    true;


  // miscellaneous
  // --------------------------------------


  // exit after some passes
  // --------------------------------------


  return clone;
}

//______________________________________________________________________________
bool bsrOptions::setBsrTextsLanguage (string language)
{
  // is language in the chords languages map?
  map<string, bsrTextsLanguageKind>::const_iterator
    it =
      gBsrTextsLanguageKindsMap.find (language);

  if (it == gBsrTextsLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fBsrTextsLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void bsrOptions::enforceQuietness ()
{
  fDisplayBsr = false;
  fDisplayBsrDetails = false;
}

//______________________________________________________________________________
void bsrOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void bsrOptions::printBsrOptionsValues (int fieldWidth)
{
  gLogOstream <<
    "The BSR bsr are:" <<
    endl;

  gIndenter++;

  // display
  // --------------------------------------
  gLogOstream <<
    "Display:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "displayBsr" << " : " <<
    booleanAsString (fDisplayBsr) <<
    endl <<

    setw (fieldWidth) << "displayBsrDetails" << " : " <<
    booleanAsString (fDisplayBsrDetails) <<
    endl;

  gIndenter--;

  // exit after some passes
  // --------------------------------------

  gLogOstream <<
    "Exit after some passes:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "exit3a" << " : " <<
    booleanAsString (fExit3a) <<
    endl <<
    setw (fieldWidth) << "exit3b" << " : " <<
    booleanAsString (fExit3b) <<
    endl;

  gIndenter--;

  // miscellaneous
  // --------------------------------------

  gLogOstream <<
    "Miscellaneous:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "noBrailleLyrics" << " : " <<
      booleanAsString (fNoBrailleLyrics) <<
      endl <<
    setw (fieldWidth) << "brailleCompileDate" << " : " <<
      booleanAsString (fBrailleCompileDate) <<
      endl <<
    setw (fieldWidth) << "facSimileKind" << " : " <<
      booleanAsString (fFacSimileKind) <<
      endl <<
    setw (fieldWidth) << "includeClefs" << " : " <<
      booleanAsString (fIncludeClefs) <<
      endl;

  gIndenter--;

  // trace
  // --------------------------------------

#ifdef TRACE_OPTIONS
  gLogOstream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "traceBsr" << " : " <<
    booleanAsString (fTraceBsr) <<
    endl <<
    setw (fieldWidth) << "tracePages" << " : " <<
    booleanAsString (fTracePages) <<
    endl <<
    setw (fieldWidth) << "traceLines" << " : " <<
    booleanAsString (fTraceLines) <<
    endl <<
    setw (fieldWidth) << "traceSpaces" << " : " <<
    booleanAsString (fTraceSpaces) <<
    endl <<
    setw (fieldWidth) << "traceNumbers" << " : " <<
    booleanAsString (fTraceNumbers) <<
    endl <<
    setw (fieldWidth) << "traceParallels" << " : " <<
    booleanAsString (fTraceParallels) <<
    endl <<
    setw (fieldWidth) << "traceBsrVisitors" << " : " <<
    booleanAsString (fTraceBsrVisitors) <<
    endl;

  gIndenter--;
#endif

  gIndenter--;
}

S_oahValuedAtom bsrOptions::handleAtom (
  indentedOstream&  os,
  S_oahAtom atom)
{
  S_oahValuedAtom result;

  if (
    // BSR texts language atom?
    S_bsrTextsLanguageAtom
      textsLanguageAtom =
        dynamic_cast<bsrTextsLanguageAtom*>(&(*atom))
  ) {
#ifdef TRACE_OPTIONS
    if (gOahBasicOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'bsrTextsLanguageAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = textsLanguageAtom;
  }

  else if (
    // chords language atom?
    S_bsrTextsLanguageAtom
      textsLanguageAtom =
        dynamic_cast<bsrTextsLanguageAtom*>(&(*atom))
  ) {
#ifdef TRACE_OPTIONS
    if (gOahBasicOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'bsrTextsLanguageAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = textsLanguageAtom;
  }

  return result;
}

void bsrOptions::handleValuedAtomValue (
  indentedOstream&      os,
  S_oahAtom atom,
  string        theString)
{
}

ostream& operator<< (ostream& os, const S_bsrOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBsrOptionsHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    gLogOstream <<
      "Initializing BSR bsr handling" <<
      endl;
  }
#endif

  // create the bsr variables
  // ------------------------------------------------------

  gBsrOptionsUserChoices = bsrOptions::create (
    handler);
  assert(gBsrOptionsUserChoices != 0);

  gBsrOptions =
    gBsrOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
  gBsrOptionsWithDetailedTrace =
    gBsrOptions->
      createCloneWithDetailedTrace ();
}


}
