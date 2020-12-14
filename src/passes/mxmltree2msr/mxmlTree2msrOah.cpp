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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"

#include "mxmlTree2msrOah.h"


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
                      stringToDalSegnoKindMapVariable)
{
  msrDalSegnoAtom* o = new
    msrDalSegnoAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToDalSegnoKindMapVariable);
  assert (o!=0);
  return o;
}

msrDalSegnoAtom::msrDalSegnoAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<string, msrDalSegno::msrDalSegnoKind>&
                      stringToDalSegnoKindMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToDalSegnoKindMapVariable (
      stringToDalSegnoKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrDalSegnoAtom::~msrDalSegnoAtom ()
{}

void msrDalSegnoAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrDalSegnoAtom'" <<
      endl;
  }
#endif

  // theString contains the dal segno specification

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrDalSegnoAtom'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> theString = \"" << theString << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  map<string, msrDalSegno::msrDalSegnoKind>::iterator
    it =
      fStringToDalSegnoKindMapVariable.find (theString);

  if (it != fStringToDalSegnoKindMapVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Dal segno value \"" << theString << "\" occurs more that once" <<
      "in a '--dal-segno' option";

    oahError (s.str ());
  }

  else {
    fStringToDalSegnoKindMapVariable [theString] = msrDalSegno::kDalSegno;
  }
}

void msrDalSegnoAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrDalSegnoAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_msrDalSegnoAtom>*> (v)) {
        S_msrDalSegnoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrDalSegnoAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrDalSegnoAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrDalSegnoAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrDalSegnoAtom>*
    p =
      dynamic_cast<visitor<S_msrDalSegnoAtom>*> (v)) {
        S_msrDalSegnoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrDalSegnoAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrDalSegnoAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
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

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "fStringToDalSegnoKindMapVariable" << " : " <<
    endl;

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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

  gIndenter--;
}

void msrDalSegnoAtom::printAtomWithValueOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToDalSegnoKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<string, msrDalSegno::msrDalSegnoKind>::const_iterator
      iBegin = fStringToDalSegnoKindMapVariable.begin (),
      iEnd   = fStringToDalSegnoKindMapVariable.end (),
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
                      clefKindToClefKindMapVariable)
{
  msrReplaceClefAtom* o = new
    msrReplaceClefAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      clefKindToClefKindMapVariable);
  assert (o!=0);
  return o;
}

msrReplaceClefAtom::msrReplaceClefAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  map<msrClefKind, msrClefKind>&
                      clefKindToClefKindMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fClefKindToClefKindMapVariable (
      clefKindToClefKindMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

msrReplaceClefAtom::~msrReplaceClefAtom ()
{}

void msrReplaceClefAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'msrReplaceClefAtom'" <<
      endl;
  }
#endif

  // theString contains the replace clef specification
  // decipher it to extract the old and new clef names

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for part transpose string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 3) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      for (unsigned i = 0; i < smSize; ++i) {
        gLogStream <<
          "[" << sm [i] << "] ";
      } // for
      gLogStream << endl;
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
    originalClefName    = sm [1],
    destinationClefName = sm [2];

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> originalClefName = \"" << originalClefName << "\", " <<
      "--> destinationClefName = \"" << destinationClefName << "\"" <<
      endl;
  }
#endif

  // is originalClefName in the replace clef map?
  msrClefKind
    originalClefKind =
       clefKindFromString (originalClefName);

  map<msrClefKind, msrClefKind>::iterator
    it =
      fClefKindToClefKindMapVariable.find (originalClefKind);

  if (it != fClefKindToClefKindMapVariable.end ()) {
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
        clefKindFromString (destinationClefName);

    fClefKindToClefKindMapVariable [originalClefKind] = destinationClefKind;
  }
}

void msrReplaceClefAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrReplaceClefAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrReplaceClefAtom>*
    p =
      dynamic_cast<visitor<S_msrReplaceClefAtom>*> (v)) {
        S_msrReplaceClefAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrReplaceClefAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrReplaceClefAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msrReplaceClefAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrReplaceClefAtom>*
    p =
      dynamic_cast<visitor<S_msrReplaceClefAtom>*> (v)) {
        S_msrReplaceClefAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msrReplaceClefAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msrReplaceClefAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
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

  if (! fClefKindToClefKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
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

  if (! fClefKindToClefKindMapVariable.size ()) {
    s << "none";
  }
  else {
    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
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

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    setw (fieldWidth) <<
    "clefKindToClefKindMapVariable" << " : " <<
    endl;

  if (! fClefKindToClefKindMapVariable.size ()) {
    os << "none";
  }
  else {
    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
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

  gIndenter--;
}

void msrReplaceClefAtom::printAtomWithValueOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fClefKindToClefKindMapVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    gIndenter++;

    map<msrClefKind, msrClefKind>::const_iterator
      iBegin = fClefKindToClefKindMapVariable.begin (),
      iEnd   = fClefKindToClefKindMapVariable.end (),
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

S_mxmlTree2msrOahGroup gGlobalMxmlTree2msrOahGroup;

S_mxmlTree2msrOahGroup mxmlTree2msrOahGroup::create (
  S_oahPrefix shortIgnoreRedundantPrefix,
  S_oahPrefix longIgnoreRedundantPrefix,
  S_oahPrefix shortDelayRestsPrefix,
  S_oahPrefix longDelayRestsPrefix)
{
  mxmlTree2msrOahGroup* o = new mxmlTree2msrOahGroup (
    shortIgnoreRedundantPrefix,
    longIgnoreRedundantPrefix,
    shortDelayRestsPrefix,
    longDelayRestsPrefix);
  assert (o!=0);
  return o;
}

mxmlTree2msrOahGroup::mxmlTree2msrOahGroup (
  S_oahPrefix shortIgnoreRedundantPrefix,
  S_oahPrefix longIgnoreRedundantPrefix,
  S_oahPrefix shortDelayRestsPrefix,
  S_oahPrefix longDelayRestsPrefix)
  : oahGroup (
    "Mxmltree2msr",
    "hmxmlt2msr", "help-mxmlTree-to-msr",
R"(These options control the way xmlelement trees are translated to MSR.)",
    kElementVisibilityWhole)
{
  fShortIgnoreRedundantPrefix = shortIgnoreRedundantPrefix;
  fLongIgnoreRedundantPrefix  = longIgnoreRedundantPrefix;

  fShortDelayRestsPrefix = shortDelayRestsPrefix;
  fLongDelayRestsPrefix  = longDelayRestsPrefix;

  initializeMxmlTree2msr ();
}

mxmlTree2msrOahGroup::~mxmlTree2msrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void mxmlTree2msrOahGroup::initializeMxmlTree2msrTraceOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "mxmlTree2msr Trace",
        "hmxmlt2ot", "help-mxmlTree-to-oah-trace",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);
}
#endif

void mxmlTree2msrOahGroup::initializeHeaderOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Header",
        "hmxmlh", "help-musicxml-header",
R"()",
        kElementVisibilityWhole,
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

void mxmlTree2msrOahGroup::initializePartsOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Parts",
      "hm2p", "help-msr-to-lpsr-parts",
R"()",
    kElementVisibilityWhole,
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

void mxmlTree2msrOahGroup::initializeClefsKeysTimesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs, keys, times",
        "hmxmlckt", "help-musicxml-clefs-keys-times",
R"()",
        kElementVisibilityWhole,
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
            to_string (gGlobalClefKindsMap.size ())),
          regex ("CLEFS_NAMES"),
          gIndenter.indentMultiLineString (
            existingClefKindsNames (K_NAMES_LIST_MAX_LENGTH))),
        "REPLACE_CLEF_SPEC",
        "replaceClefKindToClefKindMapVariable",
        fReplaceClefKindToClefKindMapVariable));

  // the 'ignore redundant' multiplex booleans atom
  // --------------------------------------

  fIgnoreRedundantMultiplexBooleansAtom =
    oahMultiplexBooleansAtom::create (
      "ism", "ignore-same-elements",
      "Ignore ELEMENTS that are the same as the current one.",
      "ELEMENTS",
      "ELEMENTS",
      fShortIgnoreRedundantPrefix,
      fLongIgnoreRedundantPrefix);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantMultiplexBooleansAtom);

  // redundant clefs
  // --------------------------------------

  fIgnoreRedundantClefs = false;

  fIgnoreRedundantClefsAtom =
    oahBooleanAtom::create (
      "irclefs", "ignore-redundant-clefs",
R"(Ignore clefs that are the same as the current one.)",
      "ignoreRedundantClefs",
      fIgnoreRedundantClefs);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantClefsAtom);

  fIgnoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantClefsAtom);

  // redundant keys
  // --------------------------------------

  fIgnoreRedundantKeys  = false;

  fIgnoreRedundantKeysAtom =
    oahBooleanAtom::create (
      "irkeys", "ignore-redundant-keys",
R"(Ignore keys that are the same as the current one.)",
      "ignoreRedundantKeys",
      fIgnoreRedundantKeys);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantKeysAtom);

  fIgnoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantKeysAtom);

  // redundant times
  // --------------------------------------

  fIgnoreRedundantTimes = false;

  fIgnoreRedundantTimesAtom =
    oahBooleanAtom::create (
      "irtimes", "ignore-redundant-times",
R"(Ignore times that are the same as the current one.)",
      "ignoreRedundantTimes",
      fIgnoreRedundantTimes);

  subGroup->
    appendAtomToSubGroup (
      fIgnoreRedundantTimesAtom);

  fIgnoreRedundantMultiplexBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantTimesAtom);
}

void mxmlTree2msrOahGroup::initializeMeasuresOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "hmxmlm", "help-musicxml-measures",
R"()",
        kElementVisibilityWhole,
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

void mxmlTree2msrOahGroup::initializeNotesOptions ()
{
  S_oahSubGroup subGroup =
    oahSubGroup::create (
      "Notes",
      "hmsrn", "help-msr-notes",
R"()",
    kElementVisibilityWhole,
    this);

  appendSubGroupToGroup (subGroup);

  // omit articulations
  // --------------------------------------

  fOmitArticulations = false;

  S_oahBooleanAtom
    omitArticulationsAtom =
      oahBooleanAtom::create (
        "oarts", "omit-articulations",
R"(Omit articulations in MusicXML data.)",
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
R"(Omit ornaments in MusicXML data.)",
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
R"(Omit '<word/>' in MusicXML data.)",
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
R"(Omit ties in MusicXML data)",
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
R"(Omit dynamics in MusicXML data)",
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
R"(Omit slurs in MusicXML data.)",
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
R"(Omit '<wedge/>' in MusicXML data.)",
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
R"(Omit lyrics in MusicXML data.)",
        "omitLyrics",
        fOmitLyrics));


  // omit harmonies
  // --------------------------------------

  fOmitHarmonies = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "oharms", "omit-harmonies",
R"(Omit harmonies in MusicXML data.)",
        "omitHarmonies",
        fOmitHarmonies));


  // omit figured basses
  // --------------------------------------

  fOmitFiguredBasses = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ofigbasses", "omit-figured-basses",
R"(Omit figured basses in MusicXML data.)",
        "omitFiguredBasses",
        fOmitFiguredBasses));

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

  // the 'delay rests' multiplex booleans atom
  // --------------------------------------

  S_oahMultiplexBooleansAtom
    delayRestsMultiplexBooleansAtom =
      oahMultiplexBooleansAtom::create (
      "dtnn", "delay-to-next-note",
        "Delay SHORT_NAME/LONG_NAME that occur on a rest until the next note.",
        "SHORT_NAME",
        "LONG_NAME",
        fShortDelayRestsPrefix,
        fLongDelayRestsPrefix);

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
}

void mxmlTree2msrOahGroup::initializeWordsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Words",
        "hmxmlw", "help-musicxml-words",
R"(Omit words in MusicXML data.)",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // convert words to dal segno
  // --------------------------------------

  subGroup->
    appendAtomToSubGroup (
      msrDalSegnoAtom::create (
        "ds", "dal-segno",
R"(Convert words elements STRING to a 'dal segno'.)",
        "STRING",
        "convertWordsToRehearsalMarks",
        fConvertWordsToDalSegno));
}

void mxmlTree2msrOahGroup::initializeDynamicsAndWedgesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics and wedges",
        "hmxmldaw", "help-musicxml-dynamics-and-wedges",
R"()",
        kElementVisibilityWhole,
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

void mxmlTree2msrOahGroup::initializeCombinedOptionsOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Combined options",
        "hmxmltco", "help-mxmlTree-combined-options",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // cubase
  // --------------------------------------

  fCubase = false;

  // create the 'cubase' combined atom
  fCubaseCombinedBooleansAtom =
    oahCombinedBooleansAtom::create (
      "cubase", "",
R"(Useful settings for MusicXML data exported from Cubase.)",
      "cubase",
      fCubase);

  subGroup->
    appendAtomToSubGroup (
      fCubaseCombinedBooleansAtom);

  // set the '-cubase' option by default
  fCubaseCombinedBooleansAtom->
    setCombinedBooleanVariables (fCubase);

  // populate the 'cubase' combined atom
  fCubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantClefsAtom);

  fCubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantKeysAtom);

  fCubaseCombinedBooleansAtom->
    addBooleanAtom (
      fIgnoreRedundantTimesAtom);
}

void mxmlTree2msrOahGroup::initializeMxmlTree2msr ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMxmlTree2msrTraceOptions ();
#endif

  // header
  // --------------------------------------
  initializeHeaderOptions ();

  // parts
  // --------------------------------------
  initializePartsOptions ();

  // clefs, keys, times
  // --------------------------------------
  initializeClefsKeysTimesOptions ();

  // measures
  // --------------------------------------
  initializeMeasuresOptions ();

  // notes
  // --------------------------------------
  initializeNotesOptions ();

  // dynamics and wedges
  // --------------------------------------
  initializeDynamicsAndWedgesOptions ();

  // words
  // --------------------------------------
  initializeWordsOptions ();

  // combined options
  // --------------------------------------
  initializeCombinedOptionsOptions ();
}

//______________________________________________________________________________
void mxmlTree2msrOahGroup::enforceGroupQuietness ()
{
  // specific trace JMI
  // --------------------------------------
}

//______________________________________________________________________________
void mxmlTree2msrOahGroup::checkGroupOptionsConsistency ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Checking the consistency of mxmlTree2msrOahGroup group \"" <<
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
void mxmlTree2msrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxmlTree2msrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTree2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxmlTree2msrOahGroup>*> (v)) {
        S_mxmlTree2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxmlTree2msrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void mxmlTree2msrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxmlTree2msrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_mxmlTree2msrOahGroup>*
    p =
      dynamic_cast<visitor<S_mxmlTree2msrOahGroup>*> (v)) {
        S_mxmlTree2msrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching mxmlTree2msrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void mxmlTree2msrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> mxmlTree2msrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void mxmlTree2msrOahGroup::printMxmlTree2msrValues (int valueFieldWidth)
{
  gLogStream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  // header
  // --------------------------------------

  gLogStream <<
    "Header:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "useFilenameAsWorkTitle" << " : " <<
      booleanAsString (fUseFilenameAsWorkTitle) <<
      endl <<
    setw (valueFieldWidth) << "useLyricistsAsPoets" << " : " <<
      booleanAsString (fUseLyricistsAsPoets) <<
      endl;

  gIndenter--;

  // parts
  // --------------------------------------

  gLogStream <<
    "Parts:" <<
    endl;

  gIndenter++;

  // parts omitted IDs

  gLogStream << left <<
    setw (valueFieldWidth) << "parts omitted IDs" << " : ";

  if (! fPartsOmitIDSet.size ()) {
    gLogStream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsOmitIDSet.begin ();
      i != fPartsOmitIDSet.end ();
      i++
  ) {
        gLogStream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept IDs

  gLogStream << left <<
    setw (valueFieldWidth) << "parts kept IDs" << " : ";

  if (! fPartsKeepIDSet.size ()) {
    gLogStream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsKeepIDSet.begin ();
      i != fPartsKeepIDSet.end ();
      i++
  ) {
        gLogStream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts omitted names

  gLogStream << left <<
    setw (valueFieldWidth) << "parts omitted names" << " : ";

  if (! fPartsOmitNameSet.size ()) {
    gLogStream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsOmitNameSet.begin ();
      i != fPartsOmitNameSet.end ();
      i++
  ) {
        gLogStream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  // parts kept names

  gLogStream << left <<
    setw (valueFieldWidth) << "parts kept names" << " : ";

  if (! fPartsKeepNameSet.size ()) {
    gLogStream <<
      "none";
  }
  else {
    for (
      set<string> ::const_iterator i =
        fPartsKeepNameSet.begin ();
      i != fPartsKeepNameSet.end ();
      i++
  ) {
        gLogStream <<
          "\"" << (*i) << "\" ";
    } // for
  }

  gLogStream << endl;

  gIndenter--;

  // clefs, keys, times
  // --------------------------------------

  gLogStream <<
    "Clefs, keys, times:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
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

  gLogStream <<
    "Measures:" <<
    endl;

  gIndenter++;

//     map<string,int>       fAddEmptyMeasuresStringToIntMap;

/* JMI
  gLogStream << left <<
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

  // notes
  // --------------------------------------

  gLogStream <<
     "Notes:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
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

  // articulations
  // --------------------------------------

  gLogStream <<
     "Articulations:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "omitArticulations" << " : " <<
    booleanAsString (fOmitArticulations) <<
    endl;

  gIndenter--;

  // ornaments
  // --------------------------------------

  gLogStream <<
     "Ornaments:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "omitOrnaments" << " : " <<
    booleanAsString (fOmitOrnaments) <<
    endl;

  gIndenter--;

  // words
  // --------------------------------------

  gLogStream <<
     "Words:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "omitWords" << " : " <<
    booleanAsString (fOmitWords) <<
    endl;

  gIndenter--;

  // ties
  // --------------------------------------

  gLogStream <<
     "Ties:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "omitTies" << " : " <<
    booleanAsString (fOmitTies) <<
    endl;

  gIndenter--;

  // dynamics
  // --------------------------------------

  gLogStream <<
     "Dynamics:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "omitDynamics" << " : " <<
    booleanAsString (fOmitDynamics) <<
    endl;

  gIndenter--;

  // slurs
  // --------------------------------------

  gLogStream <<
     "Slurs:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "mitSlurs" << " : " <<
    booleanAsString (fOmitSlurs) <<
    endl;

  gIndenter--;

  // wedges
  // --------------------------------------

  gLogStream <<
     "Wedges:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "omitWedges" << " : " <<
    booleanAsString (fOmitWedges) <<
    endl;

  gIndenter--;

  // lyrics
  // --------------------------------------

  gLogStream <<
    "Lyrics:" <<
    endl;

  gIndenter++;

  gLogStream <<
    setw (valueFieldWidth) << "omitLyrics" << " : " <<
    booleanAsString (fOmitLyrics) <<
    endl;

  gIndenter--;

  // harmonies
  // --------------------------------------

  gLogStream <<
    "Harmonies:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "omitHarmonies" << " : " <<
    booleanAsString (fOmitHarmonies) <<
    endl;

  gIndenter--;

  // figured bass
  // --------------------------------------

  gLogStream <<
    "Figured bass:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "omitFiguredBasses" << " : " <<
    booleanAsString (fOmitFiguredBasses) <<
    endl;

  gIndenter--;

  // dynamics and wedges
  // --------------------------------------

  gLogStream <<
    "Dynamics and wedges:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "allDynamicsBelow" << " : " <<
    booleanAsString (fAllDynamicsBelow) <<
    endl <<
    setw (valueFieldWidth) << "allWedgesBelow" << " : " <<
    booleanAsString (fAllWedgesBelow) <<
    endl;

  gIndenter--;

  // combined options, cubase
  // --------------------------------------

  gLogStream <<
    "Cubase:" <<
    endl;

  gIndenter++;

  gLogStream << left <<
    setw (valueFieldWidth) << "cubase" << " : " <<
    booleanAsString (fCubase) <<
    endl;

  gIndenter--;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------

  gLogStream <<
    "Trace:" <<
    endl;

  gIndenter++;

  // specific trace
  // --------------------------------------

  gIndenter--;
#endif

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_mxmlTree2msrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_mxmlTree2msrOahGroup createGlobalMxmlTree2msrOahGroup (
  S_oahPrefix shortIgnoreRedundantPrefix,
  S_oahPrefix longIgnoreRedundantPrefix,
  S_oahPrefix shortDelayRestsPrefix,
  S_oahPrefix longDelayRestsPrefix)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global xmlTree2msr OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMxmlTree2msrOahGroup) {
    // create the global options group
    gGlobalMxmlTree2msrOahGroup =
      mxmlTree2msrOahGroup::create (
        shortIgnoreRedundantPrefix,
        longIgnoreRedundantPrefix,
        shortDelayRestsPrefix,
        longDelayRestsPrefix);
    assert (gGlobalMxmlTree2msrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMxmlTree2msrOahGroup;
}


}
