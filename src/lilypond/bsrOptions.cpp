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
S_optionsFacSimileKindItem optionsFacSimileKindItem::create (
  string           oahAtomShortName,
  string           oahAtomLongName,
  string           oahAtomDescription,
  string           optionsValueSpecification,
  string           optionsFacSimileKindItemVariableDisplayName,
  bsrFacSimileKind optionsFacSimileKindItemVariable)
{
  optionsFacSimileKindItem* o = new
    optionsFacSimileKindItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsFacSimileKindItemVariableDisplayName,
      optionsFacSimileKindItemVariable);
  assert(o!=0);
  return o;
}

optionsFacSimileKindItem::optionsFacSimileKindItem (
  string           oahAtomShortName,
  string           oahAtomLongName,
  string           oahAtomDescription,
  string           optionsValueSpecification,
  string           optionsFacSimileKindItemVariableDisplayName,
  bsrFacSimileKind optionsFacSimileKindItemVariable)
  : optionsValuedItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsFacSimileKindItemVariableDisplayName),
    fOptionsFacSimileKindItemVariable (
      optionsFacSimileKindItemVariable)
{}

optionsFacSimileKindItem::~optionsFacSimileKindItem ()
{}

void optionsFacSimileKindItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsFacSimileKindItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "optionsFacSimileKindItemVariable" << " : \"" <<
    facSimileKindAsString (
      fOptionsFacSimileKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsFacSimileKindItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : \"" <<
    facSimileKindAsString (
      fOptionsFacSimileKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsFacSimileKindItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsBsrTextsLanguageItem optionsBsrTextsLanguageItem::create (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsBsrTextsLanguageKindItemVariableDisplayName,
  bsrTextsLanguageKind&
                     optionsBsrTextsLanguageKindItemVariable)
{
  optionsBsrTextsLanguageItem* o = new
    optionsBsrTextsLanguageItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsBsrTextsLanguageKindItemVariableDisplayName,
      optionsBsrTextsLanguageKindItemVariable);
  assert(o!=0);
  return o;
}

optionsBsrTextsLanguageItem::optionsBsrTextsLanguageItem (
  string             oahAtomShortName,
  string             oahAtomLongName,
  string             oahAtomDescription,
  string             optionsValueSpecification,
  string             optionsBsrTextsLanguageKindItemVariableDisplayName,
  bsrTextsLanguageKind&
                     optionsBsrTextsLanguageKindItemVariable)
  : optionsValuedItem (
      oahAtomShortName,
      oahAtomLongName,
      oahAtomDescription,
      optionsValueSpecification,
      optionsBsrTextsLanguageKindItemVariableDisplayName),
    fOptionsBsrTextsLanguageKindItemVariable (
      optionsBsrTextsLanguageKindItemVariable)
{}

optionsBsrTextsLanguageItem::~optionsBsrTextsLanguageItem ()
{}

void optionsBsrTextsLanguageItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsBsrTextsLanguageItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableDisplayName" << " : " <<
    fOptionsItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsBsrTextsLanguageKindItemVariable" << " : \"" <<
    bsrTextsLanguageKindAsString (
      fOptionsBsrTextsLanguageKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsBsrTextsLanguageItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableDisplayName <<
    " : \"" <<
    bsrTextsLanguageKindAsString (
      fOptionsBsrTextsLanguageKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsBsrTextsLanguageItem& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_bsrOptions gBsrOptions;
S_bsrOptions gBsrOptionsUserChoices;
S_bsrOptions gBsrOptionsWithDetailedTrace;

S_bsrOptions bsrOptions::create (
  S_oahHandler oahHandler)
{
  bsrOptions* o = new bsrOptions (
    oahHandler);
  assert(o!=0);
  return o;
}

bsrOptions::bsrOptions (
  S_oahHandler oahHandler)
  : oahGroup (
    "BSR",
    "hbsr", "help-bsr",
R"(These options control the way BSR data is handled.)",
    oahHandler)
{
  // append this options group to the options handler
  // if relevant
  if (oahHandler) {
    oahHandler->
      appendOptionsGroupToHandler (this);
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
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (traceAndDisplaySubGroup);

  // BSR

  fDisplayBsr = boolOptionsInitialValue;

  traceAndDisplaySubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "dbsr", "display-bsr",
R"(Write the contents of the BSR data to standard error.)",
        "displayBsr",
        fDisplayBsr,
        gBsrOptions->fDisplayBsr));

  // BSR details

  fDisplayBsrDetails = boolOptionsInitialValue;

  traceAndDisplaySubGroup->
    appendOptionsItem (
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
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (languagesSubGroup);

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
    appendOptionsItem (
      optionsBsrTextsLanguageItem::create (
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
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (miscellaneousGenerationSubGroup);

  // braille lyrics

  fNoBrailleLyrics      = boolOptionsInitialValue;

  miscellaneousGenerationSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "nolpl", "no-braille-lyrics",
R"(Don't generate any lyrics in the Braille code.)",
        "noBrailleLyrics",
        fNoBrailleLyrics));

  // braille compile date

  fBrailleCompileDate   = boolOptionsInitialValue;

  miscellaneousGenerationSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "lpcd", "braille-compile-date",
R"(Generate code to include the compilation date
when Braille creates the score.)",
        "brailleCompileDate",
        fBrailleCompileDate));

  // facsimile

  fFacSimileKind        = kFacSimileNo;

  miscellaneousGenerationSubGroup->
    appendOptionsItem (
      optionsFacSimileKindItem::create (
        "fs", "facsimile",
R"(Generate facsimile Braille nusic code.
By default, non-facsimile code is generated.)",
        "YES_OR_NO",
        "facSimileKind",
        fFacSimileKind));

  // clefs

  fIncludeClefs         = boolOptionsInitialValue;

  miscellaneousGenerationSubGroup->
    appendOptionsItem (
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
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (exitAfterSomePassesSubGroup);

  // '-exit-3a' is hidden...

  fExit3a = boolOptionsInitialValue;

  S_oahBooleanAtom
    exit2aOptionsBooleanItem =
      oahBooleanAtom::create (
        "e3a", "exit-3a",
R"(Exit after pass 3a, i.e. after conversion
of the MSR to the first BSR score.)",
        "exit3a",
        fExit3a);
        /* JMI
  exit2aOptionsBooleanItem->
    setOptionsElementIsHidden ();
    */

  exitAfterSomePassesSubGroup->
    appendOptionsItem (
      exit2aOptionsBooleanItem);

  // '-exit-3b' is hidden... JMI ???

  fExit3b = boolOptionsInitialValue;

  S_oahBooleanAtom
    exit2bOptionsBooleanItem =
      oahBooleanAtom::create (
        "e3b", "exit-3b",
R"(Exit after pass 3b, i.e. after conversion
of the first BSR to the second BSR.)",
        "exit3b",
        fExit3b);
        /* JMI
  exit2bOptionsBooleanItem->
    setOptionsElementIsHidden ();
    */

  exitAfterSomePassesSubGroup->
    appendOptionsItem (
      exit2bOptionsBooleanItem);
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
R"(Note: the options in this group imply '-tbsr, -trace-bsr'.)",
// JMI        oahSubGroup::kHideDescriptionByDefault,
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (traceSubGroup);

  // BSR

  fTraceBsr = boolOptionsInitialValue;

  traceSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "tbsr", "trace-bsr",
R"(Write a trace of the BSR graphs visiting activity to standard error.)",
        "traceBsr",
        fTraceBsr));

  // pages

  fTracePages = boolOptionsInitialValue;

  traceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tpages", "trace-pages",
R"()",
        "tracePages",
        fTracePages,
        gTraceOptions->fTracePasses));

  // lines

  fTraceLines = boolOptionsInitialValue;

  traceSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "tlines", "trace-lines",
R"()",
        "traceLines",
        fTraceLines));

  // spaces

  fTraceSpaces = boolOptionsInitialValue;

  traceSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "tspaces", "trace-spaces",
R"(Write a trace of the BSR spaces activity to standard error.)",
        "traceSpaces",
        fTraceSpaces));

  // numbers

  fTraceNumbers = boolOptionsInitialValue;

  traceSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "tnums", "trace-numbers",
R"(Write a trace of the BSR numbers activity to standard error.)",
        "traceNumbers",
        fTraceNumbers));

  // parallels

  fTraceParallels = boolOptionsInitialValue;

  traceSubGroup->
    appendOptionsItem (
      oahTwoBooleansAtom::create (
        "tpars", "trace-parallels",
R"()",
        "traceParallels",
        fTraceParallels,
        gTraceOptions->fTracePasses));

  // BSR visitors

  fTraceBsrVisitors     = boolOptionsInitialValue;

  traceSubGroup->
    appendOptionsItem (
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

  // set the options handler upLink
  clone->
    setOptionsHandlerUpLink (
      fOptionsHandlerUpLink);


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
  gLogIOstream <<
    "The BSR options are:" <<
    endl;

  gIndenter++;

  // display
  // --------------------------------------
  gLogIOstream <<
    "Display:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "displayBsr" << " : " <<
    booleanAsString (fDisplayBsr) <<
    endl <<

    setw (fieldWidth) << "displayBsrDetails" << " : " <<
    booleanAsString (fDisplayBsrDetails) <<
    endl;

  gIndenter--;

  // exit after some passes
  // --------------------------------------

  gLogIOstream <<
    "Exit after some passes:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "exit3a" << " : " <<
    booleanAsString (fExit3a) <<
    endl <<
    setw (fieldWidth) << "exit3b" << " : " <<
    booleanAsString (fExit3b) <<
    endl;

  gIndenter--;

  // miscellaneous
  // --------------------------------------

  gLogIOstream <<
    "Miscellaneous:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
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
  gLogIOstream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
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

S_optionsValuedItem bsrOptions::handleOptionsItem (
  ostream&      os,
  S_oahAtom item)
{
  S_optionsValuedItem result;

  if (
    // BSR texts language item?
    S_optionsBsrTextsLanguageItem
      textsLanguageItem =
        dynamic_cast<optionsBsrTextsLanguageItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'optionsBsrTextsLanguageItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = textsLanguageItem;
  }

  else if (
    // chords language item?
    S_optionsBsrTextsLanguageItem
      bsrTextsLanguageItem =
        dynamic_cast<optionsBsrTextsLanguageItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'optionsBsrTextsLanguageItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = bsrTextsLanguageItem;
  }

  return result;
}

void bsrOptions::handleOptionsItemValue (
  ostream&      os,
  S_oahAtom item,
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
  S_oahHandler oahHandler)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    gLogIOstream <<
      "Initializing BSR options handling" <<
      endl;
  }
#endif

  // create the options variables
  // ------------------------------------------------------

  gBsrOptionsUserChoices = bsrOptions::create (
    oahHandler);
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
