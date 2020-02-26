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
#include "mxmlTreeOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrDalSegnoAtom msrDalSegnoAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringDalSegnoKindMapVariable)
{
  msrDalSegnoAtom* o = new
    msrDalSegnoAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringDalSegnoKindMapVariable);
  assert(o!=0);
  return o;
}

msrDalSegnoAtom::msrDalSegnoAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringDalSegnoKindMapVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringDalSegnoKindMapVariable (
      stringDalSegnoKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrDalSegnoAtom::~msrDalSegnoAtom ()
{}

S_oahValuedAtom msrDalSegnoAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrDalSegnoAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrDalSegnoAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrDalSegnoAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno specification

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrDalSegnoAtom'" <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, msrDalSegno::msrDalSegnoKind>::iterator
    it =
      fStringDalSegnoKindMapVariable.find (theString);

  if (it != fStringDalSegnoKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Dal segno value \"" << theString << "\" occurs more that once" <<
      "in a '--dal-segno' option";

    oahError (s.str ());
  }

  else {
    fStringDalSegnoKindMapVariable [theString] = msrDalSegno::kDalSegno;
  }
}

void msrDalSegnoAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrDalSegnoAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_msrDalSegnoAtom>*> (v)) {
        S_msrDalSegnoAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrDalSegnoAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrDalSegnoAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrDalSegnoAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_msrDalSegnoAtom>*> (v)) {
        S_msrDalSegnoAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrDalSegnoAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrDalSegnoAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrDalSegnoAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrDalSegnoAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringDalSegnoKindMapVariable.begin (),
      iEnd   = fStringDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrDalSegnoAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringDalSegnoKindMapVariable.begin (),
      iEnd   = fStringDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrDalSegnoAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrDalSegnoAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringDalSegnoKindMapVariable" << " : " <<
    endl;

  if (! fStringDalSegnoKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringDalSegnoKindMapVariable.begin (),
      iEnd   = fStringDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i).first << "\"" <<
        " --> " <<
        "\"" << msrDalSegno::dalSegnoKindAsString ((*i).second) << "\"";
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void msrDalSegnoAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringDalSegnoKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringDalSegnoKindMapVariable.begin (),
      iEnd   = fStringDalSegnoKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i).first << "\"" <<
        " --> " <<
        "\"" << msrDalSegno::dalSegnoKindAsString ((*i).second) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msrDalSegnoAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrReplaceClefAtom msrReplaceClefAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<msrClefKind, msrClefKind>&
                      clefKindClefKindMapVariable)
{
  msrReplaceClefAtom* o = new
    msrReplaceClefAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      clefKindClefKindMapVariable);
  assert(o!=0);
  return o;
}

msrReplaceClefAtom::msrReplaceClefAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<msrClefKind, msrClefKind>&
                      clefKindClefKindMapVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fClefKindClefKindMapVariable (
      clefKindClefKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrReplaceClefAtom::~msrReplaceClefAtom ()
{}

S_oahValuedAtom msrReplaceClefAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a msrReplaceClefAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void msrReplaceClefAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'msrReplaceClefAtom'" <<
      endl;
  }
#endif

  // theString contains the replace clef specification
  // decipher it to extract the old and new clef names

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  string regularExpression (
    "(.*)"
    "="
    "(.*)");
//    "[[:space:]]*(.*)[[:space:]]*" JMI
//    "="
//    "[[:space:]]*(.*)[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for part transpose string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceOah) {
      for (unsigned i = 0; i < smSize; ++i) {
        os <<
          "[" << sm [i] << "] ";
      } // for
      os << endl;
    }
#endif
  }

  else {
    stringstream s;

    s <<
      "-marTransposePart argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    originalClefhName    = sm [1],
    destinationClefhName = sm [2];

#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    os <<
      "--> originalClefhName = \"" << originalClefhName << "\", " <<
      "--> destinationClefhName = \"" << destinationClefhName << "\"" <<
      endl;
  }
#endif

  // is originalClefhName in the replace clef map?
  msrClefKind
    originalClefKind =
       clefKindFromString (originalClefhName);

  map<msrClefKind, msrClefKind>::iterator
    it =
      fClefKindClefKindMapVariable.find (originalClefKind);

  if (it != fClefKindClefKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Replace clef value \"" << theString << "\" occurs more that once" <<
      "in a '--dal-segno' option";

    oahError (s.str ());
  }

  else {
    msrClefKind
      destinationClefKind =
         clefKindFromString (destinationClefhName);

    fClefKindClefKindMapVariable [originalClefKind] = destinationClefKind;
  }
}

void msrReplaceClefAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrReplaceClefAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrReplaceClefAtom>*
    p =
      dynamic_cast<visitor<S_msrReplaceClefAtom>*> (v)) {
        S_msrReplaceClefAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrReplaceClefAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrReplaceClefAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrReplaceClefAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrReplaceClefAtom>*
    p =
      dynamic_cast<visitor<S_msrReplaceClefAtom>*> (v)) {
        S_msrReplaceClefAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching msrReplaceClefAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrReplaceClefAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> msrReplaceClefAtom::browseData ()" <<
      endl;
  }
#endif
}

string msrReplaceClefAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fClefKindClefKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindClefKindMapVariable.begin (),
      iEnd   = fClefKindClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string msrReplaceClefAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fClefKindClefKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindClefKindMapVariable.begin (),
      iEnd   = fClefKindClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void msrReplaceClefAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "msrReplaceClefAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "clefKindClefKindMapVariable" << " : " <<
    endl;

  if (! fClefKindClefKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindClefKindMapVariable.begin (),
      iEnd   = fClefKindClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << clefKindAsString ((*i).first) << "\"" <<
        " --> " <<
        "\"" << clefKindAsString ((*i).second) << "\"";
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;
}

void msrReplaceClefAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fClefKindClefKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindClefKindMapVariable.begin (),
      iEnd   = fClefKindClefKindMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << clefKindAsString ((*i).first) << "\"" <<
        " --> " <<
        "\"" << clefKindAsString ((*i).second) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    gIndenter--;
  }
}

ostream& operator<< (ostream& os, const S_msrReplaceClefAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________

S_mxmlTreeOah gMxmlTreeOah;
S_mxmlTreeOah gMxmlTreeOahUserChoices;
S_mxmlTreeOah gMxmlTreeOahWithDetailedTrace;

S_mxmlTreeOah mxmlTreeOah::create (
  S_oahHandler handlerUpLink)
{
  mxmlTreeOah* o = new mxmlTreeOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

mxmlTreeOah::mxmlTreeOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "MusicXML",
    "hmxml", "help-musicxml",
R"(These options control the way MusicXML data is translated to MSR.)",
    kElementVisibilityAlways,
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
  initializeMxmlTreeOah (false);
}

mxmlTreeOah::~mxmlTreeOah ()
{}

#ifdef TRACE_OAH
void mxmlTreeOah::initializeMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hmxmlt", "help-musicxml-trace",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // fetch the 't' prefix

  S_oahPrefix
    shortTracePrefix =
      fHandlerUpLink->
        fetchPrefixInMapByItsName (
          "t");

  msrAssert (
    shortTracePrefix != nullptr,
    "shortTracePrefix is null");

  // fetch the 'trace' prefix

  S_oahPrefix
    longTracePrefix =
      fHandlerUpLink->
        fetchPrefixInMapByItsName (
          "trace");

  msrAssert (
    longTracePrefix != nullptr,
    "longTracePrefix is null");

  // the 'MusicXML' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    musicXMLMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME when analyzing MusicXML data.",
        "SHORT_NAME",
        "LONG_NAME",
        shortTracePrefix,
        longTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      musicXMLMultiplexBooleansAtom);

  // encoding

  fTraceEncoding = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceEncodingAtom =
      oahTwoBooleansAtom::create (
        "tenc", "trace-encoding",
R"(Encoding)",
        "traceEncoding",
        fTraceEncoding,
        gTraceOah->fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceEncodingAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceEncodingAtom);

  // divisions

  fTraceDivisions = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceDivisionsAtom =
      oahTwoBooleansAtom::create (
        "tdivs", "trace-divisions",
R"(Divisions)",
        "traceDivisions",
        fTraceDivisions,
        gTraceOah->fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceDivisionsAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceDivisionsAtom);

  // backup

  fTraceBackup = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceBackupAtom =
      oahTwoBooleansAtom::create (
        "tbackup", "trace-backup",
R"(Backup)",
        "traceBackup",
        fTraceBackup,
        gTraceOah->fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceBackupAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceBackupAtom);

  // forward

  fTraceForward = boolOptionsInitialValue;

  S_oahBooleanAtom
    traceForwardAtom =
      oahTwoBooleansAtom::create (
        "tforward", "trace-forward",
R"(Forward)",
        "traceForward",
        fTraceForward,
        gTraceOah->fTracePasses);
  subGroup->
    appendAtomToSubGroup (
      traceForwardAtom);
  musicXMLMultiplexBooleansAtom->
    addBooleanAtom (
      traceForwardAtom);

  // MusicXML tree visiting

  fTraceMusicXMLTreeVisitors = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tmxmltv", "trace-musicxml-tree-visitors",
R"(Write a trace of the MusicXML tree visiting activity to standard error.)",
        "traceMusicXMLTreeVisitors",
        fTraceMusicXMLTreeVisitors));
}
#endif

void mxmlTreeOah::initializeMusicXMLHeaderOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Header",
        "hmxmlh", "help-musicxml-header",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // file name as work title

  fUseFilenameAsWorkTitle = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ufawt", "use-filename-as-work-title",
R"(Use the file name as work title if there is none in the MusicXML data.
Standard input (-) becomes 'Standard input' in that case.)",
        "useFilenameAsWorkTitle",
        fUseFilenameAsWorkTitle));

  // lyricists as poets

  fUseLyricistsAsPoets = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ulap", "use-lyricists-as-poets",
R"(Set the 'poet' to the value of 'lyricist' in the LilyPond code \header.
This is useful because LilyPond doesn't feature a 'lyricist' variable.)",
        "useLyricistsAsPoets",
        fUseLyricistsAsPoets));
}

void mxmlTreeOah::initializeMusicXMLClefsKeysTimesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs, keys, times",
        "hmxmlckt", "help-musicxml-clefs-keys-times",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // replace clef

  subGroup->
    appendAtomToSubGroup (
      msrReplaceClefAtom::create (
        "mrc", "msr-replace-clef",
        replaceSubstringInString (
          replaceSubstringInString (
R"(Raplace clef ORIGINAL_CLEF by NEW_CLEF.
REPLACE_CLEF_SPEC can be:
'ORIGINAL_CLEF = NEW_CLEF'
or
"ORIGINAL_CLEF = NEW_CLEF"
The single or double quotes are used to allow spaces in the clef names
and around the '=' sign, otherwise they can be dispensed with.
The NUMBER clefs available are:
CLEFS_NAMES.
There can be several occurrences of this option.)",
            "NUMBER",
            to_string (gClefKindsMap.size ())),
          "CLEFS_NAMES",
          gIndenter.indentMultiLineString (
            existingClefKindsNames (K_NAMES_LIST_MAX_LENGTH))),
        "REPLACE_CLEF_SPEC",
        "replaceClefMapVariable",
        fReplaceClefMapVariable));

  // the 'ir' prefix

  S_oahPrefix
    shortIgnoreRedundantPrefix =
      oahPrefix::create (
        "ir",
        "ir",
        "'-ir=abc,yz' is equivalent to '-irabc, -iryz'");
  fHandlerUpLink->
    appendPrefixToHandler (shortIgnoreRedundantPrefix);

  // the 'ignore-redundant' prefix

  S_oahPrefix
    longIgnoreRedundantPrefix =
      oahPrefix::create (
        "ignore-redundant",
        "ignore-redundant-",
        "'-ignore-redundant=abc,yz' is equivalent to '-ignore-redundant-abc, -ignore-redundant-yz'");
  fHandlerUpLink->
    appendPrefixToHandler (longIgnoreRedundantPrefix);

  // the 'ignore redundant' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    ignoreRedundantMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Ignore ELEMENTS that are the same as the current one.",
        "ELEMENTS",
        "ELEMENTS",
        shortIgnoreRedundantPrefix,
        longIgnoreRedundantPrefix);

  subGroup->
    appendAtomToSubGroup (
      ignoreRedundantMultiplexBooleansAtom);

  // redundant clefs

  fIgnoreRedundantClefs = boolOptionsInitialValue;

  fIgnoreRedundantClefsAtom =
    oahBooleanAtom::create (
      "irclefs", "ignore-redundant-clefs",
R"(Ignore clefs that are the same as the current one.)",
      "ignoreRedundantClefs",
      fIgnoreRedundantClefs);
  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantClefsAtom);
  ignoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantClefsAtom);

  // redundant keys

  fIgnoreRedundantKeys  = boolOptionsInitialValue;

  fIgnoreRedundantKeysAtom =
    oahBooleanAtom::create (
      "irkeys", "ignore-redundant-keys",
R"(Ignore keys that are the same as the current one.)",
      "ignoreRedundantKeys",
      fIgnoreRedundantKeys);
  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantKeysAtom);
  ignoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantKeysAtom);

  // redundant times

  fIgnoreRedundantTimes = boolOptionsInitialValue;

  fIgnoreRedundantTimesAtom =
    oahBooleanAtom::create (
      "irtimes", "ignore-redundant-times",
R"(Ignore times that are the same as the current one.)",
      "ignoreRedundantTimes",
      fIgnoreRedundantTimes);
  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantTimesAtom);
  ignoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantTimesAtom);
}

void mxmlTreeOah::initializeMusicXMMeasuresOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "hmxmlm", "help-musicxml-measures",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // add empty measures

  subGroup->
    appendAtomToSubGroup (
      oahStringToIntMapAtom::create (
        "aem", "add-empty-measures",
R"###(Add empty mesure according to SPECIFICATION.
SPECIFICATION should be of the form 'MEASURE_NUMBER MEASURES_TO_ADD',
where MEASURE_NUMBER is a string, and MEASURES_TO_ADD is the number
of empty measures to add after measure MEASURE_NUMBER.
MEASURE_NUMBER should be the number of an existing, empty measure,
and MEASURES_TO_ADD should be at least 1, , such as '17 3'.
This comes in handly when MusicXML data obtained by scanning contains
a single empty measure when there were several in the original score.
This option can be used any number of times.)###",
        "SPECIFICATION",
        "addEmptyMeasuresStringToIntMap",
        fAddEmptyMeasuresStringToIntMap));
}

void mxmlTreeOah::initializeMusicXMLWordsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Words",
        "hmxmlw", "help-musicxml-words",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // convert words to dal segno
  subGroup->
    appendAtomToSubGroup (
      msrDalSegnoAtom::create (
        "ds", "dal-segno",
R"(Convert words elements STRING to an MSR 'dal segno' element'.)",
        "STRING",
        "convertWordsToRehearsalMarks",
        fConvertWordsToDalSegno));
}

void mxmlTreeOah::initializeMusicXMLDynamicsAndWedgesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics and wedges",
        "hmxmldaw", "help-musicxml-dynamics-and-wedges",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // dynamics

  fAllDynamicsBelow = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "adb", "all-dynamics-below",
R"(Ignore dynamics placement and set it to 'below'.)",
        "allDynamicsBelow",
        fAllDynamicsBelow));

  // wedges

  fAllWedgesBelow = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "awb", "all-wedges-below",
R"(Ignore wedges placement and set it to 'below'.)",
        "allWedgesBelow",
        fAllWedgesBelow));
}

void mxmlTreeOah::initializeMusicXMLCombinedOptionsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Combined options",
        "hmxmlco", "help-musicxml-combined-options",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // cubase

  fCubase = false;

  // create the 'cubase' combined atom
  S_oahCombinedBooleansAtom
    cubaseCombinedBooleansAtom =
      oahCombinedBooleansAtom::create (
        "cubase", "",
R"(Useful settings for MusicXML data exported from Cubase.)",
        "cubase",
        fCubase);

  subGroup->
    appendAtomToSubGroup (
      cubaseCombinedBooleansAtom);

  // set the '-cubase' option by default
  cubaseCombinedBooleansAtom->
    setCombinedBooleanVariables (fCubase);

  // populate the 'cubase' combined atom
  cubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantClefsAtom);
  cubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantKeysAtom);
  cubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantTimesAtom);
}

void mxmlTreeOah::initializeMusicXMLLoopOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Loop",
        "hmxmllo", "help-musicxml-loop-options",
R"()",
        kElementVisibilityAlways,
        this);

  appendSubGroupToGroup (subGroup);

  // '-loop' is hidden...

  fLoopToMusicXML = boolOptionsInitialValue;

  S_oahBooleanAtom
    loopOptionsBooleanAtom =
      oahBooleanAtom::create (
        "loop", "loop-to-musicxml",
R"(Close the loop, generating a MusicXML file from the MSR.
The file name receives a '_LOOP.xml' suffix. Currently under development.)",
        "loopToMusicXML",
        fLoopToMusicXML);
  loopOptionsBooleanAtom->
    setIsHidden ();

  subGroup->
    appendAtomToSubGroup (
      loopOptionsBooleanAtom);
}

void mxmlTreeOah::initializeMxmlTreeOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeMusicXMLTraceOah (
    boolOptionsInitialValue);
#endif

  // header
  // --------------------------------------
  initializeMusicXMLHeaderOptions (
    boolOptionsInitialValue);

  // clefs, keys, times
  // --------------------------------------
  initializeMusicXMLClefsKeysTimesOptions (
    boolOptionsInitialValue);

  // measures
  // --------------------------------------
  initializeMusicXMMeasuresOptions (
    boolOptionsInitialValue);

  // words
  // --------------------------------------
  initializeMusicXMLWordsOptions (
    boolOptionsInitialValue);

  // dynamics and wedges
  // --------------------------------------
  initializeMusicXMLDynamicsAndWedgesOptions (
    boolOptionsInitialValue);

  // combined options
  // --------------------------------------
  initializeMusicXMLCombinedOptionsOptions (
    boolOptionsInitialValue);

  // loop to do JMI
  // --------------------------------------

  initializeMusicXMLLoopOptions (
    boolOptionsInitialValue);
}

S_mxmlTreeOah mxmlTreeOah::createCloneWithDetailedTrace ()
{
  S_mxmlTreeOah
    clone =
      mxmlTreeOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;


    // header
    // --------------------------------------

  clone->fUseFilenameAsWorkTitle =
    fUseFilenameAsWorkTitle;

  clone->fUseLyricistsAsPoets =
    fUseLyricistsAsPoets;


  // clefs, keys, times
  // --------------------------------------

  clone->fIgnoreRedundantClefs =
    fIgnoreRedundantClefs;
  clone->fIgnoreRedundantKeys =
    fIgnoreRedundantKeys;
  clone->fIgnoreRedundantTimes =
    fIgnoreRedundantTimes;


  // dynamics and wedges
  // --------------------------------------

  clone->fAllDynamicsBelow =
    fAllDynamicsBelow;
  clone->fAllWedgesBelow =
    fAllWedgesBelow;


  // combined options, cubase
  // --------------------------------------

  clone->fCubase =
    fCubase;


  // loop
  // --------------------------------------

  clone->fLoopToMusicXML =
    fLoopToMusicXML;


  // trace
  // --------------------------------------

#ifdef TRACE_OAH
  clone->fTraceMusicXMLTreeVisitors =
    fTraceMusicXMLTreeVisitors;

  clone->fTraceEncoding =
    fTraceEncoding;

  clone->fTraceDivisions =
    fTraceDivisions;

  clone->fTraceBackup =
    fTraceBackup;

  clone->fTraceForward =
    fTraceForward;
#endif
  return clone;
}

//______________________________________________________________________________
void mxmlTreeOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace

    // encoding
    fTraceEncoding = boolOptionsInitialValue;

    // divisions
    fTraceDivisions = boolOptionsInitialValue;

    // backup
    fTraceBackup = boolOptionsInitialValue;

    // forward
    fTraceForward = boolOptionsInitialValue;
#endif
}

//______________________________________________________________________________
void mxmlTreeOah::enforceQuietness ()
{
  fTraceMusicXMLTreeVisitors = false;
}

//______________________________________________________________________________
void mxmlTreeOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void mxmlTreeOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> mxmlTreeOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTreeOah>*
    p =
      dynamic_cast<visitor<S_mxmlTreeOah>*> (v)) {
        S_mxmlTreeOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching mxmlTreeOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mxmlTreeOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> mxmlTreeOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTreeOah>*
    p =
      dynamic_cast<visitor<S_mxmlTreeOah>*> (v)) {
        S_mxmlTreeOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching mxmlTreeOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mxmlTreeOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> mxmlTreeOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void mxmlTreeOah::printMxmlTreeOahValues (int fieldWidth)
{
  gLogOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;


  // header
  // --------------------------------------

  gLogOstream <<
    "Header:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "useFilenameAsWorkTitle" << " : " <<
      booleanAsString (fUseFilenameAsWorkTitle) <<
      endl <<
    setw (fieldWidth) << "useLyricistsAsPoets" << " : " <<
      booleanAsString (fUseLyricistsAsPoets) <<
      endl;

  gIndenter--;

  // clefs, keys, times
  // --------------------------------------

  gLogOstream <<
    "Clefs, keys, times:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "ignoreRedundantClefs" << " : " <<
    booleanAsString (fIgnoreRedundantClefs) <<
    endl <<

    setw (fieldWidth) << "ignoreRedundantKeys" << " : " <<
    booleanAsString (fIgnoreRedundantKeys) <<
    endl <<

    setw (fieldWidth) << "ignoreRedundantTimes" << " : " <<
    booleanAsString (fIgnoreRedundantTimes) <<
    endl;

  gIndenter--;

  // measures
  // --------------------------------------

  gLogOstream <<
    "Measures:" <<
    endl;

  gIndenter++;

//     map<string,int>       fAddEmptyMeasuresStringToIntMap;

/* JMI
  gLogOstream << left <<
    setw (fieldWidth) << "ignoreRedundantClefs" << " : " <<
    booleanAsString (fIgnoreRedundantClefs) <<
    endl <<

    setw (fieldWidth) << "ignoreRedundantKeys" << " : " <<
    booleanAsString (fIgnoreRedundantKeys) <<
    endl <<

    setw (fieldWidth) << "ignoreRedundantTimes" << " : " <<
    booleanAsString (fIgnoreRedundantTimes) <<
    endl;
*/
  gIndenter--;

  // dynamics and wedges
  // --------------------------------------

  gLogOstream <<
    "Dynamics and wedges:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "allDynamicsBelow" << " : " <<
    booleanAsString (fAllDynamicsBelow) <<
    endl <<
    setw (fieldWidth) << "allWedgesBelow" << " : " <<
    booleanAsString (fAllWedgesBelow) <<
    endl;

  gIndenter--;

  // combined options, cubase
  // --------------------------------------

  gLogOstream <<
    "Cubase:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "cubase" << " : " <<
    booleanAsString (fCubase) <<
    endl;

  gIndenter--;

  // loop
  // --------------------------------------

  gLogOstream <<
    "Loop:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "loopToMusicXML" << " : " <<
    booleanAsString (fLoopToMusicXML) <<
    endl;

  gIndenter--;

  // trace
  // --------------------------------------

  gLogOstream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
#ifdef TRACE_OAH
    setw (fieldWidth) << "traceEncoding" << " : " <<
    booleanAsString (fTraceEncoding) <<
    endl <<

    setw (fieldWidth) << "traceDivisions" << " : " <<
    booleanAsString (fTraceDivisions) <<
    endl <<

    setw (fieldWidth) << "traceBackup" << " : " <<
    booleanAsString (fTraceBackup) <<
    endl <<

    setw (fieldWidth) << "traceForward" << " : " <<
    booleanAsString (fTraceForward) <<
    endl <<
#endif

    setw (fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
    booleanAsString (fTraceMusicXMLTreeVisitors) <<
    endl;

  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_mxmlTreeOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMxmlTreeOahHandling (
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

  gMxmlTreeOahUserChoices = mxmlTreeOah::create (
    handler);
  assert(gMxmlTreeOahUserChoices != 0);

  gMxmlTreeOah =
    gMxmlTreeOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gMxmlTreeOahWithDetailedTrace =
    gMxmlTreeOah->
      createCloneWithDetailedTrace ();
      */
}


}
