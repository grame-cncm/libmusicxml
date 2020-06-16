/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "version.h"
#include "utilities.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "generalOah.h"

#include "msrOah.h"
#include "msr2LpsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msr2LpsrScoreOutputKindAtom msr2LpsrScoreOutputKindAtom::create (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  lpsrScoreOutputKind& lpsrScoreOutputKindVariable)
{
  msr2LpsrScoreOutputKindAtom* o = new
    msr2LpsrScoreOutputKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lpsrScoreOutputKindVariable);
  assert(o!=0);
  return o;
}

msr2LpsrScoreOutputKindAtom::msr2LpsrScoreOutputKindAtom (
  string               shortName,
  string               longName,
  string               description,
  string               valueSpecification,
  string               variableName,
  lpsrScoreOutputKind& lpsrScoreOutputKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLpsrScoreOutputKindVariable (
      lpsrScoreOutputKindVariable)
{}

msr2LpsrScoreOutputKindAtom::~msr2LpsrScoreOutputKindAtom ()
{}

S_oahValuedAtom msr2LpsrScoreOutputKindAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msr2LpsrScoreOutputKindAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msr2LpsrScoreOutputKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msr2LpsrScoreOutputKindAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msr2LpsrScoreOutputKindAtom'" <<
      endl;
  }
#endif

  map<string, lpsrScoreOutputKind>::const_iterator
    it =
      gLpsrScoreOutputKindsMap.find (
        theString);

  if (it == gLpsrScoreOutputKindsMap.end ()) {
    // no, score output kind is unknown in the map

    stringstream s;

    s <<
      "LPSR score output kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gLpsrScoreOutputKindsMap.size () <<
      " known LPSR score output kinds are:" <<
      endl;

    gIndenter++;

    s <<
      existingLpsrScoreOutputKinds (K_NAMES_LIST_MAX_LENGTH);

    gIndenter--;

    oahError (s.str ());
  }

  setLpsrScoreOutputKindVariable (
    (*it).second);
}

void msr2LpsrScoreOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrScoreOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2LpsrScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_msr2LpsrScoreOutputKindAtom>*> (v)) {
        S_msr2LpsrScoreOutputKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2LpsrScoreOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2LpsrScoreOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrScoreOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2LpsrScoreOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_msr2LpsrScoreOutputKindAtom>*> (v)) {
        S_msr2LpsrScoreOutputKindAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2LpsrScoreOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2LpsrScoreOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrScoreOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string msr2LpsrScoreOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

string msr2LpsrScoreOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    lpsrScoreOutputKindAsString (fLpsrScoreOutputKindVariable);

  return s.str ();
}

void msr2LpsrScoreOutputKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msr2LpsrScoreOutputKindAtom:" <<
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
    "fOptionsLpsrScoreOutputKindVariable" << " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;

  gIndenter--;
}

void msr2LpsrScoreOutputKindAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    lpsrScoreOutputKindAsString (
      fLpsrScoreOutputKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msr2LpsrScoreOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrOmitPartAtom msrOmitPartAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
{
  msrOmitPartAtom* o = new
    msrOmitPartAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert(o!=0);
  return o;
}

msrOmitPartAtom::msrOmitPartAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      stringSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrOmitPartAtom::~msrOmitPartAtom ()
{}

S_oahValuedAtom msrOmitPartAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrOmitPartAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrOmitPartAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrOmitPartAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be omitted

  string partName = theString;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> partName = \"" << partName << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  set<string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << partName << "\" occurs more that once" <<
      "in the '--msr-omit-part' option";

    oahError (s.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }
}

void msrOmitPartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrOmitPartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOmitPartAtom>*
    p =
      dynamic_cast<visitor<S_msrOmitPartAtom>*> (v)) {
        S_msrOmitPartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrOmitPartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrOmitPartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrOmitPartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrOmitPartAtom>*
    p =
      dynamic_cast<visitor<S_msrOmitPartAtom>*> (v)) {
        S_msrOmitPartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrOmitPartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrOmitPartAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrOmitPartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrOmitPartAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrOmitPartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrOmitPartAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrOmitPartAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : " <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void msrOmitPartAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringSetVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;

    gIndenter++;

    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msrOmitPartAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrKeepPartAtom msrKeepPartAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
{
  msrKeepPartAtom* o = new
    msrKeepPartAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert(o!=0);
  return o;
}

msrKeepPartAtom::msrKeepPartAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      stringSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrKeepPartAtom::~msrKeepPartAtom ()
{}

S_oahValuedAtom msrKeepPartAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrKeepPartAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrKeepPartAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrKeepPartAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be kept

  string partName = theString;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> partName = \"" << partName << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  set<string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << partName << "\" occurs more that once" <<
      "in the '--msr-keep-part' option";

    oahError (s.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }
}

void msrKeepPartAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrKeepPartAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrKeepPartAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrKeepPartAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrKeepPartAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrKeepPartAtom>*
    p =
      dynamic_cast<visitor<S_msrKeepPartAtom>*> (v)) {
        S_msrKeepPartAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrKeepPartAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrKeepPartAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrKeepPartAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrKeepPartAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrKeepPartAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrKeepPartAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrKeepPartAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringSetVariable" << " : " <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void msrKeepPartAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringSetVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;

    gIndenter++;

    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msrKeepPartAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________

S_msr2LpsrOah gMsr2LpsrOah;
S_msr2LpsrOah gMsr2LpsrOahUserChoices;
S_msr2LpsrOah gMsr2LpsrOahWithDetailedTrace;

S_msr2LpsrOah msr2LpsrOah::create (
  S_oahHandler handlerUpLink)
{
  msr2LpsrOah* o = new msr2LpsrOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

msr2LpsrOah::msr2LpsrOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Msr2LPSR",
    "hmsr2lpsr", "help-msr-to-lpsr",
R"(These options control the way MSR data is translated to LPSR.)",
    kElementVisibilityWhole,
    handlerUpLink)
{
/* JMI
  // sanity check
  msrAssert (
    handlerUpLink != nullptr,
    "handlerUpLink is null");
*/

  // append this options group to the options handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeMsr2LpsrOah (false);
}

msr2LpsrOah::~msr2LpsrOah ()
{}

#ifdef TRACE_OAH
void msr2LpsrOah::initializeTraceOah (
  bool boolOptionsInitialValue)
{}
#endif

void msr2LpsrOah::initializeScoreOutputOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "LilyPond score output",
        "hlpso", "help-lilypond-score-output",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // LPSR score output kind
  // --------------------------------------

  const lpsrScoreOutputKind
    lpsrScoreOutputKindDefaultValue =
      kScoreOnly; // default value

  fScoreOutputKind = lpsrScoreOutputKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msr2LpsrScoreOutputKindAtom::create (
        "m2lpsok", "msr2lpsr-score-output-kind",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use OUTPUT_KIND to create the LPSR blocks,
as well as in the generated LilyPond code.
The NUMBER LilyPond score output kinds available are:
  OUTPUT_KINDS.
'-one-file' means that LilyPond will produce a single file
containing all the scores and/or parts.
Otherwise, one file will be generated for each score and/or part.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gLpsrScoreOutputKindsMap.size ())),
            regex ("OUTPUT_KINDS"),
            existingLpsrScoreOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
          regex ("DEFAULT_VALUE"),
          lpsrScoreOutputKindAsString (
            lpsrScoreOutputKindDefaultValue)),
        "OUTPUT_KIND",
        "scoreOutputKind",
        fScoreOutputKind));
}

void msr2LpsrOah::initializeRepeatsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Repeats",
      "hmsrr", "help-msr-repeats",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // barlines
  // --------------------------------------

  fCreateImplicitInitialRepeatBarline = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ciirb", "create-implicit-initial-repeat-barline",
R"(Create an implicit repeat barline at the beginning of the stave
in case there is none, as is usual in scores.
By default, no such barline is added.)",
        "createImplicitInitialRepeatBarline",
        fCreateImplicitInitialRepeatBarline));
}

void msr2LpsrOah::initializeNotesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Notes",
      "hmsr2lpsrn", "help-msr-to-lpsr-notes",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // the 'dr' prefix
  // --------------------------------------

  S_oahPrefix
    shortDelayRestsPrefix =
      oahPrefix::create (
        "dr",
        "dr",
        "'-dr=abc,yz' is equivalent to '-drabc, -dryz'");
  fHandlerUpLink->
    appendPrefixToHandler (shortDelayRestsPrefix);

  // the 'delay-rests' prefix
  // --------------------------------------

  S_oahPrefix
    longDelayRestsPrefix =
      oahPrefix::create (
        "delay-rests",
        "delay-rests-",
        "'-delay-rests=abc,yz' is equivalent to '-delay-rests-abc, -delay-rests-yz'");
  fHandlerUpLink->
    appendPrefixToHandler (longDelayRestsPrefix);

  // the 'delay rests' multiplex booleans atom
  // --------------------------------------

  S_oahMultiplexBooleansAtom
    delayRestsMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Delay SHORT_NAME/LONG_NAME that occur on a rest until the next note.",
        "SHORT_NAME",
        "LONG_NAME",
        shortDelayRestsPrefix,
        longDelayRestsPrefix);

  subGroup->
    appendAtomToSubGroup (
      delayRestsMultiplexBooleansAtom);

  // delay rests dynamics
  // --------------------------------------

  fDelayRestsDynamics = false;

  S_oahBooleanAtom
    delayRestsDynamicsAtom =
      oahBooleanAtom::create (
        "drdyns", "delay-rests-dynamics",
R"()",
        "delayRestsDynamics",
        fDelayRestsDynamics);
  subGroup->
    appendAtomToSubGroup (
      delayRestsDynamicsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsDynamicsAtom);

  // delay rests words
  // --------------------------------------

  fDelayRestsWords = false;

  S_oahBooleanAtom
    delayRestsWordsAtom =
      oahBooleanAtom::create (
        "drwords", "delay-rests-words",
R"()",
        "delayRestsWords",
        fDelayRestsWords);
  subGroup->
    appendAtomToSubGroup (
      delayRestsWordsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsWordsAtom);

  // delay rests beams
  // --------------------------------------

  fDelayRestsBeams = false;

  S_oahBooleanAtom
    delayRestsBeamsAtom =
      oahBooleanAtom::create (
        "drbeams", "delay-rests-beams",
R"()",
        "delayRestsBeams",
        fDelayRestsBeams);
  subGroup->
    appendAtomToSubGroup (
      delayRestsBeamsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsBeamsAtom);

  // delay rests slurs
  // --------------------------------------

  fDelayRestsSlurs = false;

  S_oahBooleanAtom
    delayRestsSlursAtom =
      oahBooleanAtom::create (
        "drslurs", "delay-rests-slurs",
R"()",
        "delayRestsSlurs",
        fDelayRestsSlurs);
  subGroup->
    appendAtomToSubGroup (
      delayRestsSlursAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsSlursAtom);

  // delay rests ligatures
  // --------------------------------------

  fDelayRestsLigatures = false;

  S_oahBooleanAtom
    delayRestsLigaturesAtom =
      oahBooleanAtom::create (
        "drligs", "delay-rests-ligatures",
R"(<bracket/> in MusicXML, '\[... \}' in LilyPond)",
        "delayRestsLigatures",
        fDelayRestsLigatures);
  subGroup->
    appendAtomToSubGroup (
      delayRestsLigaturesAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsLigaturesAtom);

  // delay rests pedals
  // --------------------------------------

  fDelayRestsPedals = false;

  S_oahBooleanAtom
    delayRestsPedalsAtom =
      oahBooleanAtom::create (
        "drpeds", "delay-rests-pedals",
R"()",
        "delayRestsPedals",
        fDelayRestsPedals);
  subGroup->
    appendAtomToSubGroup (
      delayRestsPedalsAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsPedalsAtom);

  // delay rests slashes
  // --------------------------------------

  fDelayRestsSlashes = false;

  S_oahBooleanAtom
    delayRestsSlashesAtom =
      oahBooleanAtom::create (
        "drslashes", "delay-rests-slashes",
R"('<slash/>' in MusicXML)",
        "delayRestsSlashes",
        fDelayRestsSlashes);
  subGroup->
    appendAtomToSubGroup (
      delayRestsSlashesAtom);

  // delay rests wedges
  // --------------------------------------

  fDelayRestsWedges = false;

  S_oahBooleanAtom
    delayRestsWedgesAtom =
      oahBooleanAtom::create (
        "drwedges", "delay-rests-wedges",
R"('<wedge/>' in MusicXML, '<!' in LilyPond)",
        "delayRestsWedges",
        fDelayRestsWedges);
  subGroup->
    appendAtomToSubGroup (
      delayRestsWedgesAtom);
  delayRestsMultiplexBooleansAtom->
    addBooleanAtom (
      delayRestsWedgesAtom);

  // slash all grace notes
  // --------------------------------------

  fSlashAllGraceNotes = false;

  S_oahBooleanAtom
    slashAllGraceNotesAtom =
      oahBooleanAtom::create (
        "slashagn", "slash-all-grace-notes",
R"(Add a slash to all grace notes)",
        "slashAllGraceNotes",
        fSlashAllGraceNotes);
  subGroup->
    appendAtomToSubGroup (
      slashAllGraceNotesAtom);

  // slur all grace notes
  // --------------------------------------

  fSlurAllGraceNotes = false;

  S_oahBooleanAtom
    slurAllGraceNotesAtom =
      oahBooleanAtom::create (
        "sluragn", "slur-all-grace-notes",
R"(Add a slur to all grace notes)",
        "slurAllGraceNotes",
        fSlurAllGraceNotes);
  subGroup->
    appendAtomToSubGroup (
      slurAllGraceNotesAtom);

  // beam all grace notes
  // --------------------------------------

  fBeamAllGraceNotes = false;

  S_oahBooleanAtom
    beamAllGraceNotesAtom =
      oahBooleanAtom::create (
        "beamagn", "beam-all-grace-notes",
R"(Add a beam to all grace notes)",
        "beamAllGraceNotes",
        fBeamAllGraceNotes);
  subGroup->
    appendAtomToSubGroup (
      beamAllGraceNotesAtom);
}

void msr2LpsrOah::initializeLyricsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Lyrics",
      "hmsrlyrd", "help-msr-lyrics",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

}

void msr2LpsrOah::initializeMsr2LpsrOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeTraceOah (
    boolOptionsInitialValue);
#endif

  // score output kind
  // --------------------------------------
  initializeScoreOutputOptions (
    boolOptionsInitialValue);

  // repeats
  // --------------------------------------
  initializeRepeatsOptions (
    boolOptionsInitialValue);

  // notes
  // --------------------------------------
  initializeNotesOptions (
    boolOptionsInitialValue);

  // lyrics
  // --------------------------------------
  initializeLyricsOptions (
    boolOptionsInitialValue);
}

S_msr2LpsrOah msr2LpsrOah::createCloneWithDetailedTrace ()
{
  S_msr2LpsrOah
    clone =
      msr2LpsrOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;

  // LilyPond output kind
  // --------------------------------------

  clone->fScoreOutputKind =
    fScoreOutputKind;

  // repeats
  // --------------------------------------

  clone->fCreateImplicitInitialRepeatBarline =
    fCreateImplicitInitialRepeatBarline;

  // notes
  // --------------------------------------

  clone->fDelayRestsDynamics =
    fDelayRestsDynamics;
  clone->fDelayRestsWords =
    fDelayRestsWords;
  clone->fDelayRestsSlurs =
    fDelayRestsSlurs;
  clone->fDelayRestsLigatures =
    fDelayRestsLigatures;
  clone->fDelayRestsPedals =
    fDelayRestsPedals;
  clone->fDelayRestsSlashes =
    fDelayRestsSlashes;
  clone->fDelayRestsWedges =
    fDelayRestsWedges;

  clone->fSlashAllGraceNotes =
    fSlashAllGraceNotes;
  clone->fSlurAllGraceNotes =
    fSlurAllGraceNotes;
  clone->fBeamAllGraceNotes =
    fBeamAllGraceNotes;

  return clone;
}

//______________________________________________________________________________
void msr2LpsrOah::setAllMsr2LpsrTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void msr2LpsrOah::enforceQuietness ()
{}

//______________________________________________________________________________
void msr2LpsrOah::checkOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2LpsrOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2LpsrOah>*
    p =
      dynamic_cast<visitor<S_msr2LpsrOah>*> (v)) {
        S_msr2LpsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2LpsrOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2LpsrOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2LpsrOah>*
    p =
      dynamic_cast<visitor<S_msr2LpsrOah>*> (v)) {
        S_msr2LpsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msr2LpsrOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2LpsrOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msr2LpsrOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msr2LpsrOah::printMsr2LpsrOahValues (int valueFieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  // LilyPond output kind
  // --------------------------------------

  gLogOstream <<
    "LilyPond output kind:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "scoreOutputKind" << " : " <<
    lpsrScoreOutputKindAsString (fScoreOutputKind) <<
    endl;

  gIndenter--;

  // repeats
  // --------------------------------------

  gLogOstream <<
    "Repeats:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) <<
    "createImplicitInitialRepeatBarline" << " : " <<
    booleanAsString (fCreateImplicitInitialRepeatBarline) <<
    endl;

  gIndenter--;

  // notes
  // --------------------------------------

  gLogOstream <<
     "Notes:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "delayRestsDynamics" << " : " <<
    booleanAsString (fDelayRestsDynamics) <<
    endl <<

    setw (valueFieldWidth) << "delayRestsWords" << " : " <<
    booleanAsString (fDelayRestsWords) <<
    endl <<

    setw (valueFieldWidth) << "delayRestsSlurs" << " : " <<
    booleanAsString (fDelayRestsSlurs) <<
    endl <<

    setw (valueFieldWidth) << "delayRestsLigatures" << " : " <<
    booleanAsString (fDelayRestsLigatures) <<
    endl <<

    setw (valueFieldWidth) << "delayRestsPedals" << " : " <<
    booleanAsString (fDelayRestsPedals) <<
    endl <<

    setw (valueFieldWidth) << "delayRestsSlashes" << " : " <<
    booleanAsString (fDelayRestsSlashes) <<
    endl <<

    setw (valueFieldWidth) << "delayRestsWedges" << " : " <<
    booleanAsString (fDelayRestsWedges) <<
    endl <<

    setw (valueFieldWidth) << "slashAllGraceNotes" << " : " <<
    booleanAsString (fSlashAllGraceNotes) <<
    endl <<
    setw (valueFieldWidth) << "slurAllGraceNotes" << " : " <<
    booleanAsString (fSlurAllGraceNotes) <<
    endl <<
    setw (valueFieldWidth) << "beamAllGraceNotes" << " : " <<
    booleanAsString (fBeamAllGraceNotes) <<
    endl;

  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msr2LpsrOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMsr2LpsrOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah && ! gGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing MusicXML options handling" <<
      endl;
  }
#endif

  // create the MusicXML options
  // ------------------------------------------------------

  gMsr2LpsrOahUserChoices = msr2LpsrOah::create (
    handler);
  assert(gMsr2LpsrOahUserChoices != 0);

  gMsr2LpsrOah =
    gMsr2LpsrOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gMsr2LpsrOahWithDetailedTrace =
    gMsr2LpsrOah->
      createCloneWithDetailedTrace ();
      */
}


}
