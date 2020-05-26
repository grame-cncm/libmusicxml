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
#include "mxmlTree2MsrOah.h"


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

S_mxmlTree2MsrOah gMxmlTree2MsrOah;
S_mxmlTree2MsrOah gMxmlTree2MsrOahUserChoices;
S_mxmlTree2MsrOah gMxmlTree2MsrOahWithDetailedTrace;

S_mxmlTree2MsrOah mxmlTree2MsrOah::create (
  S_oahHandler handlerUpLink)
{
  mxmlTree2MsrOah* o = new mxmlTree2MsrOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

mxmlTree2MsrOah::mxmlTree2MsrOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Mxmltree2Msr",
    "hmxmlt2msr", "help-mxmltree-to-msr",
R"(These options control the way xmlelement trees are translated to MSR.)",
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
  initializeMxmlTree2Msr (false);
}

mxmlTree2MsrOah::~mxmlTree2MsrOah ()
{}

#ifdef TRACE_OAH
void mxmlTree2MsrOah::initializeTraceOah (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Trace",
        "hmxmlt2ot", "help-mxmltree-to-oah-trace",
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

/* not used as of may 2020 JMI
  // the 'MusicXML' multiplex booleans atom

  S_oahMultiplexBooleansAtom
    musicXMLMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
        "Trace SHORT_NAME/LONG_NAME when converting xmlelement tree to MusicXML data.",
        "SHORT_NAME",
        "LONG_NAME",
        shortTracePrefix,
        longTracePrefix);

  subGroup->
    appendAtomToSubGroup (
      musicXMLMultiplexBooleansAtom);
*/

  // specific trace
  // --------------------------------------
}
#endif

void mxmlTree2MsrOah::initializeHeaderOptions (
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
  // --------------------------------------

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
  // --------------------------------------

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

void mxmlTree2MsrOah::initializePartsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Parts",
      "hm2p", "help-msr-to-lpsr-parts",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // MSR omit part ID
  // --------------------------------------

  fOmitPartIDAtom =
    oahStringSetAtom::create (
      "mopi", "msr-omit-part-id",
R"(Omit part with ID PART_ID.
There can be several occurrences of this option.
All the parts not omitted are kept.
This option is incompatible with '-mkpi, -msr-keep-part-id'.)",
      "PART_ID",
      "partsOmitIDSet",
      fPartsOmitIDSet);

  subGroup->
    appendAtomToSubGroup (
      fOmitPartIDAtom);

  // MSR omit part name
  // --------------------------------------

  fOmitPartNameAtom =
    oahStringSetAtom::create (
      "mopn", "msr-omit-part-name",
R"(Omit part named PART_NAME.
There can be several occurrences of this option.
All the parts not omitted are kept.
This option is incompatible with '-mkpn, -msr-keep-part-name'.)",
      "PART_NAME",
      "partsOmitNameSet",
      fPartsOmitNameSet);

  subGroup->
    appendAtomToSubGroup (
      fOmitPartNameAtom);

  // MSR keep part ID
  // --------------------------------------

  fKeepPartIDAtom =
    oahStringSetAtom::create (
      "mkpi", "msr-keep-part-id",
R"(Keep part with ID PART_ID.
There can be several occurrences of this option.
All the parts not kept are omitted.
This option is incompatible with '-mopi, -msr-omit-part-id'.)",
      "PART_ID",
      "partsKeepIDSet",
      fPartsKeepIDSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepPartIDAtom);

  // MSR keep part name
  // --------------------------------------

  fKeepPartNameAtom =
    oahStringSetAtom::create (
      "mkpn", "msr-keep-part-name",
R"(Keep part named PART_NAME.
There can be several occurrences of this option.
All the parts not kept are omitted.
This option is incompatible with '-mopn, -msr-omit-part-name'.)",
      "PART_NAME",
      "partsKeepNameSet",
      fPartsKeepNameSet);

  subGroup->
    appendAtomToSubGroup (
      fKeepPartNameAtom);
}

void mxmlTree2MsrOah::initializeClefsKeysTimesOptions (
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
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      msrReplaceClefAtom::create (
        "mrc", "msr-replace-clef",
        regex_replace (
          regex_replace (
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
            regex ("NUMBER"),
            to_string (gClefKindsMap.size ())),
          regex ("CLEFS_NAMES"),
          gIndenter.indentMultiLineString (
            existingClefKindsNames (K_NAMES_LIST_MAX_LENGTH))),
        "REPLACE_CLEF_SPEC",
        "replaceClefMapVariable",
        fReplaceClefMapVariable));

  // the 'ir' prefix
  // --------------------------------------

  S_oahPrefix
    shortIgnoreRedundantPrefix =
      oahPrefix::create (
        "ir",
        "ir",
        "'-ir=abc,yz' is equivalent to '-irabc, -iryz'");
  fHandlerUpLink->
    appendPrefixToHandler (shortIgnoreRedundantPrefix);

  // the 'ignore-redundant' prefix
  // --------------------------------------

  S_oahPrefix
    longIgnoreRedundantPrefix =
      oahPrefix::create (
        "ignore-redundant",
        "ignore-redundant-",
        "'-ignore-redundant=abc,yz' is equivalent to '-ignore-redundant-abc, -ignore-redundant-yz'");
  fHandlerUpLink->
    appendPrefixToHandler (longIgnoreRedundantPrefix);

  // the 'ignore redundant' multiplex booleans atom
  // --------------------------------------

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
  // --------------------------------------

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
  // --------------------------------------

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
  // --------------------------------------

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

void mxmlTree2MsrOah::initializeMeasuresOptions (
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
  // --------------------------------------

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

void mxmlTree2MsrOah::initializeNotesOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Notes",
      "hmsrn", "help-msr-notes",
R"()",
    kElementVisibilityAlways,
    this);

  appendSubGroupToGroup (subGroup);

  // omit articulations
  // --------------------------------------

  fOmitArticulations = false;

  S_oahBooleanAtom
    omitArticulationsAtom =
      oahBooleanAtom::create (
        "oarts", "omit-articulations",
R"('<word/>' in MusicXML, '<!' in LilyPond)",
        "omitArticulations",
        fOmitArticulations);
  subGroup->
    appendAtomToSubGroup (
      omitArticulationsAtom);

  // omit ornaments
  // --------------------------------------

  fOmitOrnaments = false;

  S_oahBooleanAtom
    omitOrnamentsAtom =
      oahBooleanAtom::create (
        "oorns", "omit-ornaments",
R"('<trill-mark/>' in MusicXML, '<!' in LilyPond)",
        "omitOrnaments",
        fOmitOrnaments);
  subGroup->
    appendAtomToSubGroup (
      omitOrnamentsAtom);

  // omit words
  // --------------------------------------

  fOmitWords = false;

  S_oahBooleanAtom
    omitWordsAtom =
      oahBooleanAtom::create (
        "owords", "omit-words",
R"('<word/>' in MusicXML, '<!' in LilyPond)",
        "omitWords",
        fOmitWords);
  subGroup->
    appendAtomToSubGroup (
      omitWordsAtom);

  // omit ties
  // --------------------------------------

  fOmitTies = false;

  S_oahBooleanAtom
    omitTiesAtom =
      oahBooleanAtom::create (
        "oties", "omit-ties",
R"()",
        "omitTies",
        fOmitTies);
  subGroup->
    appendAtomToSubGroup (
      omitTiesAtom);

  // omit dynamics
  // --------------------------------------

  fOmitDynamics = false;

  S_oahBooleanAtom
    omitDynamicsAtom =
      oahBooleanAtom::create (
        "odyns", "omit-dynamics",
R"()",
        "omitDynamics",
        fOmitDynamics);
  subGroup->
    appendAtomToSubGroup (
      omitDynamicsAtom);

  // omit slurs
  // --------------------------------------

  fOmitSlurs = false;

  S_oahBooleanAtom
    omitSlursAtom =
      oahBooleanAtom::create (
        "oslurs", "omit-slurs",
R"()",
        "omitSlurs",
        fOmitSlurs);
  subGroup->
    appendAtomToSubGroup (
      omitSlursAtom);

  // omit wedges
  // --------------------------------------

  fOmitWedges = false;

  S_oahBooleanAtom
    omitWedgesAtom =
      oahBooleanAtom::create (
        "owedges", "omit-wedges",
R"('<wedge/>' in MusicXML, '<!' in LilyPond)",
        "omitWedges",
        fOmitWedges);
  subGroup->
    appendAtomToSubGroup (
      omitWedgesAtom);

  // omit lyrics
  // --------------------------------------

  fOmitLyrics = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "olyrics", "omit-lyrics",
R"(Don't create lyrics in the MSR.)",
        "omitLyrics",
        fOmitLyrics));


  // omit harmonies
  // --------------------------------------

  fOmitHarmonies = boolOptionsInitialValue;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "oharms", "omit-harmonies",
R"(Don't create harmonies in the MSR.)",
        "omitHarmonies",
        fOmitHarmonies));


  // omit figured basses
  // --------------------------------------

  fOmitFiguredBasses = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ofigbasses", "omit-figured-basses",
R"(Don't create figured basses in the MSR.)",
        "omitFiguredBasses",
        fOmitFiguredBasses));
}

void mxmlTree2MsrOah::initializeWordsOptions (
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
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      msrDalSegnoAtom::create (
        "ds", "dal-segno",
R"(Convert words elements STRING to an MSR 'dal segno' element'.)",
        "STRING",
        "convertWordsToRehearsalMarks",
        fConvertWordsToDalSegno));
}

void mxmlTree2MsrOah::initializeDynamicsAndWedgesOptions (
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
  // --------------------------------------

  fAllDynamicsBelow = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "adb", "all-dynamics-below",
R"(Ignore dynamics placement and set it to 'below'.)",
        "allDynamicsBelow",
        fAllDynamicsBelow));

  // wedges
  // --------------------------------------

  fAllWedgesBelow = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "awb", "all-wedges-below",
R"(Ignore wedges placement and set it to 'below'.)",
        "allWedgesBelow",
        fAllWedgesBelow));
}

void mxmlTree2MsrOah::initializeCombinedOptionsOptions (
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
  // --------------------------------------

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

void mxmlTree2MsrOah::initializeMxmlTree2Msr (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // trace
  // --------------------------------------
  initializeTraceOah (
    boolOptionsInitialValue);
#endif

  // header
  // --------------------------------------
  initializeHeaderOptions (
    boolOptionsInitialValue);

  // parts
  // --------------------------------------
  initializePartsOptions (
    boolOptionsInitialValue);

  // clefs, keys, times
  // --------------------------------------
  initializeClefsKeysTimesOptions (
    boolOptionsInitialValue);

  // measures
  // --------------------------------------
  initializeMeasuresOptions (
    boolOptionsInitialValue);

  // notes
  // --------------------------------------
  initializeNotesOptions (
    boolOptionsInitialValue);

  // dynamics and wedges
  // --------------------------------------
  initializeDynamicsAndWedgesOptions (
    boolOptionsInitialValue);

  // words
  // --------------------------------------
  initializeWordsOptions (
    boolOptionsInitialValue);

  // combined options
  // --------------------------------------
  initializeCombinedOptionsOptions (
    boolOptionsInitialValue);
}

S_mxmlTree2MsrOah mxmlTree2MsrOah::createCloneWithDetailedTrace ()
{
  S_mxmlTree2MsrOah
    clone =
      mxmlTree2MsrOah::create (0);
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

  // parts
  // --------------------------------------

  clone->fPartsOmitIDSet =
    fPartsOmitIDSet;
  clone->fPartsKeepIDSet =
    fPartsKeepIDSet;

  clone->fPartsOmitNameSet =
    fPartsOmitNameSet;
  clone->fPartsKeepNameSet =
    fPartsKeepNameSet;

  // clefs, keys, times
  // --------------------------------------

  clone->fIgnoreRedundantClefs =
    fIgnoreRedundantClefs;
  clone->fIgnoreRedundantKeys =
    fIgnoreRedundantKeys;
  clone->fIgnoreRedundantTimes =
    fIgnoreRedundantTimes;

  // articulations
  // --------------------------------------

  clone->fOmitArticulations =
    fOmitArticulations;

  // ornaments
  // --------------------------------------

  clone->fOmitOrnaments =
    fOmitOrnaments;

  // words
  // --------------------------------------

  clone->fOmitWords =
    fOmitWords;

  // ties
  // --------------------------------------

  clone->fOmitTies =
    fOmitTies;

  // dynamics
  // --------------------------------------

  clone->fOmitDynamics =
    fOmitDynamics;

  // slurs
  // --------------------------------------

  clone->fOmitSlurs =
    fOmitSlurs;

  // wedges
  // --------------------------------------

  clone->fOmitWedges =
    fOmitWedges;

  // dynamics and wedges
  // --------------------------------------

  clone->fAllDynamicsBelow =
    fAllDynamicsBelow;
  clone->fAllWedgesBelow =
    fAllWedgesBelow;

  // lyrics
  // --------------------------------------

  clone->fOmitLyrics =
    fOmitLyrics;

  // harmonies
  // --------------------------------------

  clone->fOmitHarmonies =
    fOmitHarmonies;

  // figured bass
  // --------------------------------------

  clone->fOmitFiguredBasses =
    fOmitFiguredBasses;

  // combined options, cubase
  // --------------------------------------

  clone->fCubase =
    fCubase;

  // trace
  // --------------------------------------

#ifdef TRACE_OAH
  // specific trace
  // --------------------------------------
#endif

  return clone;
}

//______________________________________________________________________________
void mxmlTree2MsrOah::setAllMusicXMLTraceOah (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OAH
  // specific trace
  // JMI
#endif
}

//______________________________________________________________________________
void mxmlTree2MsrOah::enforceQuietness ()
{
  // specific trace
  // --------------------------------------
}

//______________________________________________________________________________
void mxmlTree2MsrOah::checkOptionsConsistency ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "Checking the consistency of mxmlTree2MsrOah group \"" <<
      fGroupHeader <<
      "\"" <<
      endl;
  }
#endif

  // JMI and if mixed ID and name options are used?

  if (fPartsOmitIDSet.size () > 0 && fPartsKeepIDSet.size () > 0) {
    stringstream s;

    s <<
      "options '" <<
      fOmitPartIDAtom->fetchNames () <<
      "' and '" <<
      fKeepPartIDAtom->fetchNames () <<
      "' are incompatible";

    oahError (s.str ());
  }

  if (fPartsOmitNameSet.size () > 0 && fPartsKeepNameSet.size () > 0) {
    stringstream s;

    s <<
      "options '" <<
      fOmitPartNameAtom->fetchNames () <<
      "' and '" <<
      fKeepPartNameAtom->fetchNames () <<
      "' are incompatible";

    oahError (s.str ());
  }
}

//______________________________________________________________________________
void mxmlTree2MsrOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> mxmlTree2MsrOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTree2MsrOah>*
    p =
      dynamic_cast<visitor<S_mxmlTree2MsrOah>*> (v)) {
        S_mxmlTree2MsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching mxmlTree2MsrOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mxmlTree2MsrOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> mxmlTree2MsrOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTree2MsrOah>*
    p =
      dynamic_cast<visitor<S_mxmlTree2MsrOah>*> (v)) {
        S_mxmlTree2MsrOah elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching mxmlTree2MsrOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mxmlTree2MsrOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> mxmlTree2MsrOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void mxmlTree2MsrOah::printMxmlTree2MsrValues (int valueFieldWidth)
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
    setw (valueFieldWidth) << "useFilenameAsWorkTitle" << " : " <<
      booleanAsString (fUseFilenameAsWorkTitle) <<
      endl <<
    setw (valueFieldWidth) << "useLyricistsAsPoets" << " : " <<
      booleanAsString (fUseLyricistsAsPoets) <<
      endl;

  gIndenter--;

  // parts
  // --------------------------------------

  // parts
  // --------------------------------------

  // parts omitted IDs

  gLogOstream << left <<
    setw (valueFieldWidth) << "parts omitted IDs" << " : ";

  if (! fPartsOmitIDSet.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsOmitIDSet.begin ();
      i != fPartsOmitIDSet.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept IDs

  gLogOstream << left <<
    setw (valueFieldWidth) << "parts kept IDs" << " : ";

  if (! fPartsKeepIDSet.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsKeepIDSet.begin ();
      i != fPartsKeepIDSet.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts omitted names

  gLogOstream << left <<
    setw (valueFieldWidth) << "parts omitted names" << " : ";

  if (! fPartsOmitNameSet.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsOmitNameSet.begin ();
      i != fPartsOmitNameSet.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept names

  gLogOstream << left <<
    setw (valueFieldWidth) << "parts kept names" << " : ";

  if (! fPartsKeepNameSet.size ()) {
    gLogOstream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsKeepNameSet.begin ();
      i != fPartsKeepNameSet.end ();
      i++
  ) {
        gLogOstream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  gLogOstream << endl;

  gIndenter--;

  // clefs, keys, times
  // --------------------------------------

  gLogOstream <<
    "Clefs, keys, times:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "ignoreRedundantClefs" << " : " <<
    booleanAsString (fIgnoreRedundantClefs) <<
    endl <<

    setw (valueFieldWidth) << "ignoreRedundantKeys" << " : " <<
    booleanAsString (fIgnoreRedundantKeys) <<
    endl <<

    setw (valueFieldWidth) << "ignoreRedundantTimes" << " : " <<
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
    setw (valueFieldWidth) << "ignoreRedundantClefs" << " : " <<
    booleanAsString (fIgnoreRedundantClefs) <<
    endl <<

    setw (valueFieldWidth) << "ignoreRedundantKeys" << " : " <<
    booleanAsString (fIgnoreRedundantKeys) <<
    endl <<

    setw (valueFieldWidth) << "ignoreRedundantTimes" << " : " <<
    booleanAsString (fIgnoreRedundantTimes) <<
    endl;
*/
  gIndenter--;

  // articulations
  // --------------------------------------

  gLogOstream <<
     "Articulations:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "omitArticulations" << " : " <<
    booleanAsString (fOmitArticulations) <<
    endl;

  gIndenter--;

  // ornaments
  // --------------------------------------

  gLogOstream <<
     "Ornaments:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "omitOrnaments" << " : " <<
    booleanAsString (fOmitOrnaments) <<
    endl;

  gIndenter--;

  // words
  // --------------------------------------

  gLogOstream <<
     "Words:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "omitWords" << " : " <<
    booleanAsString (fOmitWords) <<
    endl;

  gIndenter--;

  // ties
  // --------------------------------------

  gLogOstream <<
     "Ties:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "omitTies" << " : " <<
    booleanAsString (fOmitTies) <<
    endl;

  gIndenter--;

  // dynamics
  // --------------------------------------

  gLogOstream <<
     "Dynamics:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "omitDynamics" << " : " <<
    booleanAsString (fOmitDynamics) <<
    endl;

  gIndenter--;

  // slurs
  // --------------------------------------

  gLogOstream <<
     "Slurs:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "mitSlurs" << " : " <<
    booleanAsString (fOmitSlurs) <<
    endl;

  gIndenter--;

  // wedges
  // --------------------------------------

  gLogOstream <<
     "Wedges:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "omitWedges" << " : " <<
    booleanAsString (fOmitWedges) <<
    endl;

  gIndenter--;

  // lyrics
  // --------------------------------------

  gLogOstream <<
    "Lyrics:" <<
    endl;

  gIndenter++;

  gLogOstream <<
    setw (valueFieldWidth) << "omitLyrics" << " : " <<
    booleanAsString (fOmitLyrics) <<
    endl;

  gIndenter--;

  // harmonies
  // --------------------------------------

  gLogOstream <<
    "Harmonies:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "omitHarmonies" << " : " <<
    booleanAsString (fOmitHarmonies) <<
    endl;

  gIndenter--;

  // figured bass
  // --------------------------------------

  gLogOstream <<
    "Figured bass:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "omitFiguredBasses" << " : " <<
    booleanAsString (fOmitFiguredBasses) <<
    endl;

  gIndenter--;

  // dynamics and wedges
  // --------------------------------------

  gLogOstream <<
    "Dynamics and wedges:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (valueFieldWidth) << "allDynamicsBelow" << " : " <<
    booleanAsString (fAllDynamicsBelow) <<
    endl <<
    setw (valueFieldWidth) << "allWedgesBelow" << " : " <<
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
    setw (valueFieldWidth) << "cubase" << " : " <<
    booleanAsString (fCubase) <<
    endl;

  gIndenter--;

#ifdef TRACE_OAH
  // trace
  // --------------------------------------

  gLogOstream <<
    "Trace:" <<
    endl;

  gIndenter++;

  // specific trace
  // --------------------------------------

  gIndenter--;
#endif

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_mxmlTree2MsrOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMxmlTree2MsrOahHandling (
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

  gMxmlTree2MsrOahUserChoices = mxmlTree2MsrOah::create (
    handler);
  assert(gMxmlTree2MsrOahUserChoices != 0);

  gMxmlTree2MsrOah =
    gMxmlTree2MsrOahUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  gMxmlTree2MsrOahWithDetailedTrace =
    gMxmlTree2MsrOah->
      createCloneWithDetailedTrace ();
      */
}


}
