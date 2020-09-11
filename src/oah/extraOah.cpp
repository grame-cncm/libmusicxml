/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "extraOah.h"

#ifdef EXTRA_OAH


#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "version.h"
#include "utilities.h"

#include "oahOah.h"
#include "generalOah.h"

#include "traceOah.h"

#include "lpsrOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_extraShowAllHarmoniesStructuresAtom extraShowAllHarmoniesStructuresAtom::create (
  string shortName,
  string longName,
  string description)
{
  extraShowAllHarmoniesStructuresAtom* o = new
    extraShowAllHarmoniesStructuresAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

extraShowAllHarmoniesStructuresAtom::extraShowAllHarmoniesStructuresAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

extraShowAllHarmoniesStructuresAtom::~extraShowAllHarmoniesStructuresAtom ()
{}

S_oahValuedAtom extraShowAllHarmoniesStructuresAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a extraShowAllHarmoniesStructuresAtom" <<
      endl;
  }
#endif

  // handle it at once
  printAllHarmoniesStructures (os);

  // no option value is needed
  return nullptr;
}

void extraShowAllHarmoniesStructuresAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowAllHarmoniesStructuresAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesStructuresAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesStructuresAtom>*> (v)) {
        S_extraShowAllHarmoniesStructuresAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraShowAllHarmoniesStructuresAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowAllHarmoniesStructuresAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowAllHarmoniesStructuresAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesStructuresAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesStructuresAtom>*> (v)) {
        S_extraShowAllHarmoniesStructuresAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraShowAllHarmoniesStructuresAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowAllHarmoniesStructuresAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowAllHarmoniesStructuresAtom::browseData ()" <<
      endl;
  }
#endif
}

void extraShowAllHarmoniesStructuresAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsShowAllHarmoniesStructuresAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void extraShowAllHarmoniesStructuresAtom::printAllHarmoniesStructures (ostream& os) const
{
  msrHarmonyStructure::printAllHarmoniesStructures (os);
}

void extraShowAllHarmoniesStructuresAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_extraShowAllHarmoniesStructuresAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_extraShowAllHarmoniesContentsAtom extraShowAllHarmoniesContentsAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowAllHarmoniesContentsVariable)
{
  extraShowAllHarmoniesContentsAtom* o = new
    extraShowAllHarmoniesContentsAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      extraShowAllHarmoniesContentsVariable);
  assert(o!=0);
  return o;
}

extraShowAllHarmoniesContentsAtom::extraShowAllHarmoniesContentsAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowAllHarmoniesContentsVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      extraShowAllHarmoniesContentsVariable)
{}

extraShowAllHarmoniesContentsAtom::~extraShowAllHarmoniesContentsAtom ()
{}

S_oahValuedAtom extraShowAllHarmoniesContentsAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a extraShowAllHarmoniesContentsAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void extraShowAllHarmoniesContentsAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowAllHarmoniesContentsAtom'" <<
      endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowAllHarmoniesContentsAtom'" <<
      ", theString = \"" << theString << "\"" <<
      endl;
  }
#endif

  // fetch the semitones pitch from theString
  msrSemiTonesPitchKind
    semiTonesPitchKind =
      semiTonesPitchKindFromString (
        theString);

  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      {
        stringstream s;

        s <<
          "'" << theString <<
          "' is no diatonic (semitones) pitch" <<
          " in pitch language '" <<
          msrQuarterTonesPitchesLanguageKindAsString (
            globalLpsrOah->
              fLpsrQuarterTonesPitchesLanguageKind) <<
          "'" <<
          endl;

        oahError (s.str ());
      }
      break;

    case kA_Flat_STP:
    case kA_Natural_STP:
    case kA_Sharp_STP:

    case kB_Flat_STP:
    case kB_Natural_STP:
    case kB_Sharp_STP:

    case kC_Flat_STP:
    case kC_Natural_STP:
    case kC_Sharp_STP:

    case kD_Flat_STP:
    case kD_Natural_STP:
    case kD_Sharp_STP:

    case kE_Flat_STP:
    case kE_Natural_STP:
    case kE_Sharp_STP:

    case kF_Flat_STP:
    case kF_Natural_STP:
    case kF_Sharp_STP:

    case kG_Flat_STP:
    case kG_Natural_STP:
    case kG_Sharp_STP:
      break;

    default:
      {
        stringstream s;

        s <<
          "'" << theString <<
          "' is no diatonic (semitones) pitch" <<
          endl;

        oahError (s.str ());
      }
  } // switch

  // print all the harmonies notes
  printAllHarmoniesContents (
    os,
    semiTonesPitchKind);

  // register 'show all harmonies contents' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOahHandlerFoundAHelpOption ();
}

void extraShowAllHarmoniesContentsAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowAllHarmoniesContentsAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesContentsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesContentsAtom>*> (v)) {
        S_extraShowAllHarmoniesContentsAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraShowAllHarmoniesContentsAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowAllHarmoniesContentsAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowAllHarmoniesContentsAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesContentsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesContentsAtom>*> (v)) {
        S_extraShowAllHarmoniesContentsAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraShowAllHarmoniesContentsAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowAllHarmoniesContentsAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowAllHarmoniesContentsAtom::browseData ()" <<
      endl;
  }
#endif
}

void extraShowAllHarmoniesContentsAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowAllHarmoniesContentsAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void extraShowAllHarmoniesContentsAtom::printAllHarmoniesContents (
  ostream&              os,
  msrSemiTonesPitchKind semiTonesPitchKind) const
{
  msrHarmonyContents::printAllHarmoniesContents (
    os,
    semiTonesPitchKind);
}

void extraShowAllHarmoniesContentsAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_extraShowAllHarmoniesContentsAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_extraShowHarmonyDetailsAtom extraShowHarmonyDetailsAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowHarmonyDetailsVariable)
{
  extraShowHarmonyDetailsAtom* o = new
    extraShowHarmonyDetailsAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      extraShowHarmonyDetailsVariable);
  assert(o!=0);
  return o;
}

extraShowHarmonyDetailsAtom::extraShowHarmonyDetailsAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowHarmonyDetailsVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      extraShowHarmonyDetailsVariable)
{}

extraShowHarmonyDetailsAtom::~extraShowHarmonyDetailsAtom ()
{}

S_oahValuedAtom extraShowHarmonyDetailsAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a extraShowHarmonyDetailsAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void extraShowHarmonyDetailsAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowHarmonyDetailsAtom'" <<
      endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowHarmonyDetailsAtom'" <<
      ", theString = \"" << theString << "\"" <<
      endl;
  }
#endif

  // decipher theString with a regular expression
  string regularExpression (
    "[[:space:]]*"
    "([[:alnum:]]+)"
    "[[:space:]]*"
    "([[:alnum:]]+)"
    "[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for Harmony details string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      smSize << " elements: ";
    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        "[" << sm [i] << "] ";
    } // for
    os << endl;
  }
#endif

  if (smSize == 3) { // JMI ???
  }

  else {
    stringstream s;

    s <<
      "-Harmony details argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());
  }

  string
    rootName    = sm [1],
    harmonyName = sm [2];

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "--> rootName = \"" << rootName << "\", " <<
      "--> harmonyName = \"" << harmonyName << "\"" <<
      endl;
  }
#endif

  // fetch the semitones pitch from rootName
  msrSemiTonesPitchKind
    semiTonesPitchKind =
      semiTonesPitchKindFromString (
        rootName);

  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      {
        stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) root pitch" <<
          " in pitch language '" <<
          msrQuarterTonesPitchesLanguageKindAsString (
            globalLpsrOah->
              fLpsrQuarterTonesPitchesLanguageKind) <<
          "'" <<
          endl;

        oahError (s.str ());
      }
      break;

    case kA_Flat_STP:
    case kA_Natural_STP:
    case kA_Sharp_STP:

    case kB_Flat_STP:
    case kB_Natural_STP:
    case kB_Sharp_STP:

    case kC_Flat_STP:
    case kC_Natural_STP:
    case kC_Sharp_STP:

    case kD_Flat_STP:
    case kD_Natural_STP:
    case kD_Sharp_STP:

    case kE_Flat_STP:
    case kE_Natural_STP:
    case kE_Sharp_STP:

    case kF_Flat_STP:
    case kF_Natural_STP:
    case kF_Sharp_STP:

    case kG_Flat_STP:
    case kG_Natural_STP:
    case kG_Sharp_STP:
      break;

    default:
      {
        stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) pitch" <<
          endl;

        oahError (s.str ());
      }
  } // switch

  // fetch the harmony kind from harmonyName
  msrHarmonyKind
    harmonyKind =
      msrHarmonyKindFromString (
        harmonyName);

  // print the Harmony details
  printHarmonyDetails (
    os,
    semiTonesPitchKind,
    harmonyKind);

  // register 'show all harmonies contents' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOahHandlerFoundAHelpOption ();
}

void extraShowHarmonyDetailsAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowHarmonyDetailsAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowHarmonyDetailsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyDetailsAtom>*> (v)) {
        S_extraShowHarmonyDetailsAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraShowHarmonyDetailsAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowHarmonyDetailsAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowHarmonyDetailsAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowHarmonyDetailsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyDetailsAtom>*> (v)) {
        S_extraShowHarmonyDetailsAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraShowHarmonyDetailsAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowHarmonyDetailsAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowHarmonyDetailsAtom::browseData ()" <<
      endl;
  }
#endif
}

void extraShowHarmonyDetailsAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowHarmonyDetailsAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void extraShowHarmonyDetailsAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

//______________________________________________________________________________
S_extraShowHarmonyAnalysisAtom extraShowHarmonyAnalysisAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowHarmonyAnalysisVariable)
{
  extraShowHarmonyAnalysisAtom* o = new
    extraShowHarmonyAnalysisAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      extraShowHarmonyAnalysisVariable);
  assert(o!=0);
  return o;
}

extraShowHarmonyAnalysisAtom::extraShowHarmonyAnalysisAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowHarmonyAnalysisVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      extraShowHarmonyAnalysisVariable)
{}

extraShowHarmonyAnalysisAtom::~extraShowHarmonyAnalysisAtom ()
{}

S_oahValuedAtom extraShowHarmonyAnalysisAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a extraShowHarmonyAnalysisAtom" <<
      endl;
  }
#endif

  // an option value is needed
  return this;
}

void extraShowHarmonyAnalysisAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowHarmonyAnalysisAtom'" <<
      endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "==> oahAtom is of type 'extraShowHarmonyAnalysisAtom'" <<
      ", theString = \"" << theString << "\"" <<
      endl;
  }
#endif

  // decipher theString with a regular expression
  string regularExpression (
    "[[:space:]]*"
    "([[:alnum:]]+)"
    "[[:space:]]+"
    "([[:alnum:]]+)"
    "[[:space:]]+"
    "([[:digit:]]+)"
    "[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "There are " << smSize << " matches" <<
      " for Harmony analysis string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 4) {
#ifdef TRACE_OAH
    if (globalTraceOah->fTraceOah) {
      os <<
        smSize << " elements: ";
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
      "Harmony analysis argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());

/* JMI ??? should work...
    printSubGroupSpecificHelp (
      os,
      showHarmonyAnalysisAtom->
        getSubGroupUpLink ());
*/
  }

  string
    rootName          = sm [1],
    harmonyName       = sm [2],
    inversionAsString = sm [3];

  int
    inversion;

  stringstream s;

  s << inversionAsString;

  s >> inversion;

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    os <<
      "--> rootName = \"" << rootName << "\", " <<
      "--> harmonyName = \"" << harmonyName << "\"" <<
      "--> inversion = " << inversion <<
      endl;
  }
#endif

  // fetch the semitones pitch from rootName
  msrSemiTonesPitchKind
    semiTonesPitchKind =
      semiTonesPitchKindFromString (
        rootName);

  switch (semiTonesPitchKind) {
    case k_NoSemiTonesPitch_STP:
      {
        stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) root pitch" <<
          " in pitch language '" <<
          msrQuarterTonesPitchesLanguageKindAsString (
            globalLpsrOah->
              fLpsrQuarterTonesPitchesLanguageKind) <<
          "'" <<
          endl;

        oahError (s.str ());
      }
      break;

    case kA_Flat_STP:
    case kA_Natural_STP:
    case kA_Sharp_STP:

    case kB_Flat_STP:
    case kB_Natural_STP:
    case kB_Sharp_STP:

    case kC_Flat_STP:
    case kC_Natural_STP:
    case kC_Sharp_STP:

    case kD_Flat_STP:
    case kD_Natural_STP:
    case kD_Sharp_STP:

    case kE_Flat_STP:
    case kE_Natural_STP:
    case kE_Sharp_STP:

    case kF_Flat_STP:
    case kF_Natural_STP:
    case kF_Sharp_STP:

    case kG_Flat_STP:
    case kG_Natural_STP:
    case kG_Sharp_STP:
      break;

    default:
      {
        stringstream s;

        s <<
          "'" << rootName <<
          "' is no diatonic (semitones) pitch" <<
          endl;

        oahError (s.str ());
      }
  } // switch

  // fetch the harmony kind from harmonyName
  msrHarmonyKind
    harmonyKind =
      msrHarmonyKindFromString (
        harmonyName);

  if (harmonyKind == k_NoHarmony) {
    stringstream s;

    s <<
      "'" << harmonyName <<
      "' is no harmony name, valid names are:" <<
      endl <<
      "maj, min,aug, dim, dom, maj7, min7, dim7, aug7, halfdim, minmaj7, maj6, min6, dom9, maj9, min9, dom11, maj11, min11, dom13, maj13, min13, sus2, sus4, neapolitan, italian, french, german, pedal, power, tristan, minmaj9, domsus4, domaug5, dommin9, domaug9dim5, domaug9aug5, domaug11 and maj7aug11" <<
      endl;

    oahError (s.str ());
  }

  // print the Harmony analysis
  printHarmonyAnalysis (
    os,
    semiTonesPitchKind,
    harmonyKind,
    inversion);

  // register 'show all harmonies contents' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOahHandlerFoundAHelpOption ();
}

void extraShowHarmonyAnalysisAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowHarmonyAnalysisAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowHarmonyAnalysisAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyAnalysisAtom>*> (v)) {
        S_extraShowHarmonyAnalysisAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraShowHarmonyAnalysisAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowHarmonyAnalysisAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowHarmonyAnalysisAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowHarmonyAnalysisAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyAnalysisAtom>*> (v)) {
        S_extraShowHarmonyAnalysisAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraShowHarmonyAnalysisAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowHarmonyAnalysisAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraShowHarmonyAnalysisAtom::browseData ()" <<
      endl;
  }
#endif
}

void extraShowHarmonyAnalysisAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowHarmonyAnalysisAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void extraShowHarmonyAnalysisAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

//_______________________________________________________________________________

S_extraOah globalExtraOah;
S_extraOah globalExtraOahUserChoices;
S_extraOah globalExtraOahWithDetailedTrace;

S_extraOah extraOah::create (
  S_oahHandler handlerUpLink)
{
  extraOah* o = new extraOah(
    handlerUpLink);
  assert(o!=0);
  return o;
}

extraOah::extraOah (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "Extra",
    "he", "help-extra",
R"(These extra provide features not related to translation from MusicXML to other formats.
In the text below:
  - ROOT_DIATONIC_PITCH should belong to the names available in
    the selected MSR pitches language, "nederlands" by default;
  - other languages can be chosen with the '-mpl, -msr-pitches-language' option;
  - HARMONY_NAME should be one of:
      MusicXML harmonies:
        "maj", "min", "aug", "dim", "dom",
        "maj7", "min7", "dim7", "aug7", "halfdim", "minmaj7",
        "maj6", "min6", "dom9", "maj9", "min9", "dom11", "maj11", "min11",
        "dom13", "maj13", "min13", "sus2", "sus4",
        "neapolitan", "italian", "french", "german"
      Jazz-specific harmonies:
        "pedal", "power", "tristan", "minmaj9", "domsus4", "domaug5",
        "dommin9", "domaug9dim5", "domaug9aug5", "domaug11", "maj7aug11"
The single or double quotes are used to allow spaces in the names
and around the '=' sign, otherwise they can be dispensed with.)",
    kElementVisibilityWhole,
    handlerUpLink)
{
  // append this extra group to the extra handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeExtraOah (false);
}

extraOah::~extraOah ()
{}

void extraOah::initializeExtraShowAllHarmoniesStructuresOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies structures",
        "hecs", "help-extra-harmonies-structures",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  subGroup->
    appendAtomToSubGroup (
      extraShowAllHarmoniesStructuresAtom::create (
        "scs", "show-harmonies-structures",
R"(Write all known harmonies structures to standard output.)"));
}

void extraOah::initializeExtraShowAllHarmoniesContentsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies contents",
        "hecc", "help-extra-harmonies-contents",
//          regex_replace (
//            regex ("HARMONY_KINDS"),
//            existingHarmonyKindsNames ()
//    HARMONY_KINDS.
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  subGroup->
    appendAtomToSubGroup (
      extraShowAllHarmoniesContentsAtom::create (
        "sacc", "show-all-harmonies-contents",
R"(Write all harmonies contents for the given diatonic (semitones) PITCH,
supplied in the current language to standard output.)",
        "PITCH",
        "diatonic (semitones) pitch",
        fHarmoniesRootAsString));
}

void extraOah::initializeExtraShowHarmonyDetailsOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmony details",
        "hecd", "help-extra-harmony-details",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  subGroup->
    appendAtomToSubGroup (
      extraShowHarmonyDetailsAtom::create (
        "scd", "show-harmony-details",
        regex_replace (
R"(Write the details of the harmony for the given diatonic (semitones) pitch
in the current language and the given harmony to standard output.
HARMONY_SPEC can be:
'ROOT_DIATONIC_PITCH HARMONY_NAME'
or
"ROOT_DIATONIC_PITCH = HARMONY_NAME"
Using double quotes allows for shell variables substitutions, as in:
HARMONY="maj7"
EXECUTABLE -show-harmony-details "bes ${HARMONY}")",
         regex ("EXECUTABLE"),
          globalOahOah->fHandlerExecutableName),
        "HARMONY_SPEC",
        "diatonic (semitones) pitch",
        fHarmoniesRootAsString));
}

void extraOah::initializeExtraShowHarmonyAnalysisOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmony analysis",
        "heca", "help-extra-harmony-analysis",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  subGroup->
    appendAtomToSubGroup (
      extraShowHarmonyAnalysisAtom::create (
        "sca", "show-harmony-analysis", // -sca "c dommin9 0"
        regex_replace (
R"(Write an analysis of the harmony for the given diatonic (semitones) pitch
in the current language and the given harmony to standard output.
HARMONY_SPEC can be:
'ROOT_DIATONIC_PITCH HARMONY_NAME INVERSION'
or
"ROOT_DIATONIC_PITCH = HARMONY_NAME INVERSION"
Using double quotes allows for shell variables substitutions, as in:
HARMONY="maj7"
INVERSION=2
EXECUTABLE -show-harmony-analysis "bes ${HARMONY} ${INVERSION}")",
          regex ("EXECUTABLE"),
          globalOahOah->fHandlerExecutableName),
        "HARMONY_SPEC",
        "diatonic (semitones) pitch",
        fHarmoniesRootAsString));
}

void extraOah::initializeExtraOah (
  bool boolOptionsInitialValue)
{
  // show all harmonies structures
  // --------------------------------------
  initializeExtraShowAllHarmoniesStructuresOptions (
    boolOptionsInitialValue);

  // show all harmonies contents
  // --------------------------------------
  initializeExtraShowAllHarmoniesContentsOptions (
    boolOptionsInitialValue);

  // show harmony details
  // --------------------------------------
  initializeExtraShowHarmonyDetailsOptions (
    boolOptionsInitialValue);

  // show harmony analysis
  // --------------------------------------
  initializeExtraShowHarmonyAnalysisOptions (
    boolOptionsInitialValue);
}

S_extraOah extraOah::createCloneWithDetailedTrace ()
{
  S_extraOah
    clone =
      extraOah::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the extra handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;


  // harmony intervals
  // --------------------------------------

  // JMI

  // harmony notes
  // --------------------------------------

  // JMI

  return clone;
}

//______________________________________________________________________________
void extraOah::enforceQuietness ()
{
}

//______________________________________________________________________________
void extraOah::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void extraOah::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraOah::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraOah>*
    p =
      dynamic_cast<visitor<S_extraOah>*> (v)) {
        S_extraOah elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraOah::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraOah::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraOah::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraOah>*
    p =
      dynamic_cast<visitor<S_extraOah>*> (v)) {
        S_extraOah elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching extraOah::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraOah::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> extraOah::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void extraOah::printExtraOahValues (int fieldWidth)
{
  gLogOstream <<
    "The extra extra are:" << // JMI
    endl;

  gIndenter++;

  // harmony intervals
  // --------------------------------------

  // harmony notes
  // --------------------------------------

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_extraOah& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeExtraOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah && ! globalGeneralOah->fQuiet) {
    gLogOstream <<
      "Initializing extra extra handling" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    // create the options variables
    // ------------------------------------------------------

    globalExtraOahUserChoices = extraOah::create (
      handler);
    assert(globalExtraOahUserChoices != 0);

    globalExtraOah =
      globalExtraOahUserChoices;

    pThisMethodHasBeenRun = true;
  }

  // prepare for measure detailed trace
  // ------------------------------------------------------

/* JMI
  globalExtraOahWithDetailedTrace =
    globalExtraOah->
      createCloneWithDetailedTrace ();
      */
}


}

#endif
