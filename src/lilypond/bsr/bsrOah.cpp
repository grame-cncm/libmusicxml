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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "generalOah.h"
#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string bsrFacSimileKindAsString (
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'bsrFacSimileKindAtom'" <<
      endl;
  }
#endif

}

void bsrFacSimileKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrFacSimileKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrFacSimileKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFacSimileKindAtom>*
    p =
      dynamic_cast<visitor<S_bsrFacSimileKindAtom>*> (v)) {
        S_bsrFacSimileKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrFacSimileKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrFacSimileKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrFacSimileKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string bsrFacSimileKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrFacSimileKindAsString (fBsrFacSimileKindVariable);

  return s.str ();
}

string bsrFacSimileKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrFacSimileKindAsString (fBsrFacSimileKindVariable);

  return s.str ();
}

void bsrFacSimileKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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
    bsrFacSimileKindAsString (
      fBsrFacSimileKindVariable) <<
      "\"" <<
    endl;
}

void bsrFacSimileKindAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrFacSimileKindAsString (
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'bsrTextsLanguageAtom'" <<
      endl;
  }
#endif

}

void bsrTextsLanguageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTextsLanguageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrTextsLanguageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTextsLanguageAtom>*
    p =
      dynamic_cast<visitor<S_bsrTextsLanguageAtom>*> (v)) {
        S_bsrTextsLanguageAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrTextsLanguageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTextsLanguageAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrTextsLanguageAtom::browseData ()" <<
      endl;
  }
#endif
}

string bsrTextsLanguageAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    bsrTextsLanguageKindAsString (fBsrTextsLanguageKindVariable);

  return s.str ();
}

string bsrTextsLanguageAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    bsrTextsLanguageKindAsString (fBsrTextsLanguageKindVariable);

  return s.str ();
}

void bsrTextsLanguageAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

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

void bsrTextsLanguageAtom::printAtomOptionsValues (
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
S_bsrOah gBsrOah;
S_bsrOah gBsrOahUserChoices;
S_bsrOah gBsrOahWithDetailedTrace;

S_bsrOah bsrOah::create (
  S_oahHandler handlerUpLink)
{
  bsrOah* o = new bsrOah (
    handlerUpLink);
  assert(o!=0);
  return o;
}

bsrOah::bsrOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "BSR",
    "hbsr", "help-bsr",
R"(These bsr control the way BSR data is handled.)",
    kElementVisibilityAlways,
    handlerUpLink)
{
  // append this bsr group to the bsr handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeBsrOah (false);
}

bsrOah::~bsrOah ()
{}

void bsrOah::initializeBsrDisplayOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Display",
        "hbsrd", "help-bsr-display",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // BSR

  fDisplayBsr = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "dbsr", "display-bsr",
R"(Write the contents of the BSR data to standard error.)",
        "displayBsr",
        fDisplayBsr));

  // BSR details

  fDisplayBsrDetails = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "dbsrd", "display-bsr-details",
R"(Write the contents of the BSR data with more details to standard error.)",
        "displayBsrDetails",
        fDisplayBsrDetails,
        fDisplayBsr));
}

void bsrOah::initializeBsrLanguagesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "hbsrl", "help-bsr-languages",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

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
      existingBsrTextsLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  const bsrTextsLanguageKind
    bsrTextsLanguageKindDefaultValue =
      fBsrTextsLanguageKind;

  subGroup->
    appendAtomToSubGroup (
      bsrTextsLanguageAtom::create (
        "btl", "bsr-texts-language",
        replaceSubstringInString (
          replaceSubstringInString (
            replaceSubstringInString (
R"(Use LANGUAGE to transcribe texts in the BSR logs and views,
as well as in the generated braille music.
The NUMBER texts languages available are:
TEXT_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              "NUMBER",
              to_string (gBsrTextsLanguageKindsMap.size ())),
            "TEXT_LANGUAGES",
            existingBsrTextsLanguageKinds (K_NAMES_LIST_MAX_LENGTH)),
          "DEFAULT_VALUE",
          bsrTextsLanguageKindAsString (
            bsrTextsLanguageKindDefaultValue)),
        "LANGUAGE",
        "bsr-texts-language",
        fBsrTextsLanguageKind));
}

void bsrOah::initializeBsrMiscellaneousOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Miscellaneous",
        "hlpm", "help-miscellaneous",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // braille music headings

  fNoBrailleMusicHeadings = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nmh", "no-music-headings",
R"(Don't generate any music headings in the Braille code.)",
        "noBrailleMusicHeadings",
        fNoBrailleMusicHeadings));

  // braille tempos

  fNoTempos = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nt", "no-tempos",
R"(Don't generate any tempos in the Braille code.)",
        "noTempos",
        fNoTempos));

  // braille page numbers

  fNoPageNumbers = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "npn", "no-page-numbers",
R"(Don't generate any page numbers in the Braille code.)",
        "noBraillePageNumbers",
        fNoPageNumbers));

  // braille line numbers

  fNoLineNumbers = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nln", "no-line-numbers",
R"(Don't generate any line numbers in the Braille code.)",
        "noBrailleLineNumbers",
        fNoLineNumbers));

  // braille measure numbers

  fNoMeasureNumbers = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nmn", "no-measure-numbers",
R"(Don't generate any measure numbers in the Braille code.)",
        "noMeasureNumbers",
        fNoMeasureNumbers));

  // braille lyrics

  fNoBrailleLyrics = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nl", "no-lyrics",
R"(Don't generate any lyrics in the Braille code.)",
        "noBrailleLyrics",
        fNoBrailleLyrics));

  // braille compile date

  fBrailleCompileDate = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lpcd", "braille-compile-date",
R"(Generate code to include the compilation date
when Braille creates the score.)",
        "brailleCompileDate",
        fBrailleCompileDate));

  // facsimile

  fFacSimileKind = kFacSimileNo;

  subGroup->
    appendAtomToSubGroup (
      bsrFacSimileKindAtom::create (
        "fs", "facsimile",
R"(Generate facsimile Braille nusic code.
By default, non-facsimile code is generated.)",
        "YES_OR_NO",
        "facSimileKind",
        fFacSimileKind));

  // clefs

  fIncludeClefs = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "clefs", "",
R"(Include clefs in BSR. By default, they are not.)",
        "includeClefs",
        fIncludeClefs));
}

void bsrOah::initializeBsrExitAfterSomePassesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Exit after some passes",
        "hbexit", "help-bsr-exit",
R"()",
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // '-exit-3a' is hidden...

  fExit3a = boolOptionsInitialValue;

  S_oahBooleanAtom
    exit2aOahBooleanAtom =
      oahBooleanAtom::create (
        "e3a", "exit-3a",
R"(Exit after pass 3a, i.e. after conversion
of the MSR to the first BSR score.)",
        "exit3a",
        fExit3a);

  subGroup->
    appendAtomToSubGroup (
      exit2aOahBooleanAtom);

  fExit3b = boolOptionsInitialValue;

  S_oahBooleanAtom
    exit2bOahBooleanAtom =
      oahBooleanAtom::create (
        "e3b", "exit-3b",
R"(Exit after pass 3b, i.e. after conversion
of the first BSR to the second BSR.)",
        "exit3b",
        fExit3b);

  subGroup->
    appendAtomToSubGroup (
      exit2bOahBooleanAtom);
}

#ifdef TRACE_OAH
void bsrOah::initializeBsrTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hbst", "help-bsr-trace",
R"(  Note: the options in this subgroup imply '-tbsr, -trace-bsr'.)",
// JMI        kElementVisibilityHiddenByDefault,
      kElementVisibilityAlways,
      this);

  appendSubGroupToGroup (subGroup);

  // BSR

  fTraceBsr = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tbsr", "trace-bsr",
R"(Write a trace of the BSR graphs visiting activity to standard error.)",
        "traceBsr",
        fTraceBsr));

  // pages

  fTracePages = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "tpages", "trace-pages",
R"()",
        "tracePages",
        fTracePages,
        gTraceOah->fTracePasses));

  // lines

  fTraceLines = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tlines", "trace-lines",
R"()",
        "traceLines",
        fTraceLines));

  // spaces

  fTraceSpaces = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tspaces", "trace-spaces",
R"(Write a trace of the BSR spaces activity to standard error.)",
        "traceSpaces",
        fTraceSpaces));

  // numbers

  fTraceNumbers = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tnums", "trace-numbers",
R"(Write a trace of the BSR numbers activity to standard error.)",
        "traceNumbers",
        fTraceNumbers));

  // parallels

  fTraceParallels = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "tpars", "trace-parallels",
R"()",
        "traceParallels",
        fTraceParallels,
        gTraceOah->fTracePasses));

  // BSR visitors

  fTraceBsrVisitors     = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tbsrv", "trace-bsr-visitors",
R"(Write a trace of the BSR tree visiting activity to standard error.)",
        "traceBsrVisitors",
        fTraceBsrVisitors));
}
#endif

void bsrOah::initializeBsrOah (
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

#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeBsrTraceOah (
    boolOptionsInitialValue);
#endif
}

S_bsrOah bsrOah::createCloneWithDetailedTrace ()
{
  S_bsrOah
    clone =
      bsrOah::create (0);
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
bool bsrOah::setBsrTextsLanguage (string language)
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
void bsrOah::enforceQuietness ()
{
  fDisplayBsr = false;
  fDisplayBsrDetails = false;
}

//______________________________________________________________________________
void bsrOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void bsrOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrOah>*
    p =
      dynamic_cast<visitor<S_bsrOah>*> (v)) {
        S_bsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrOah>*
    p =
      dynamic_cast<visitor<S_bsrOah>*> (v)) {
        S_bsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching bsrOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> bsrOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void bsrOah::printBsrOahValues (int fieldWidth)
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
    setw (fieldWidth) << "noBrailleMusicHeadings" << " : " <<
      booleanAsString (fNoBrailleMusicHeadings) <<
      endl <<

    setw (fieldWidth) << "noTempos" << " : " <<
      booleanAsString (fNoTempos) <<
      endl <<

    setw (fieldWidth) << "noBraillePageNumbers" << " : " <<
      booleanAsString (fNoPageNumbers) <<
      endl <<
    setw (fieldWidth) << "noBrailleLineNumbers" << " : " <<
      booleanAsString (fNoLineNumbers) <<
      endl <<
    setw (fieldWidth) << "noMeasureNumbers" << " : " <<
      booleanAsString (fNoMeasureNumbers) <<
      endl <<

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

#ifdef TRACE_OAH
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

ostream& operator<< (ostream& os, const S_bsrOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBsrOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing BSR bsr handling" <<
      endl;
  }
#endif

  // create the bsr variables
  // ------------------------------------------------------

  gBsrOahUserChoices = bsrOah::create (
    handler);
  assert(gBsrOahUserChoices != 0);

  gBsrOah =
    gBsrOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
/* JMI
  gBsrOahWithDetailedTrace =
    gBsrOah->
      createCloneWithDetailedTrace ();
      */
}


}
