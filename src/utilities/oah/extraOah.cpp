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

#ifdef EXTRA_OAH_IS_ENABLED


#include <iomanip>      // setw, setprecision, ...
#include <regex>

#include "version.h"
#include "utilities.h"

#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

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
  assert (o!=0);
  return o;
}

extraShowAllHarmoniesStructuresAtom::extraShowAllHarmoniesStructuresAtom (
  string shortName,
  string longName,
  string description)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      "valueSpecification JMI",
      "variableName JMI")
{}

extraShowAllHarmoniesStructuresAtom::~extraShowAllHarmoniesStructuresAtom ()
{}

/* JMI
void extraShowAllHarmoniesStructuresAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a extraShowAllHarmoniesStructuresAtom" <<
      endl;
  }
#endif

  printAllHarmoniesStructures (os);
}
*/

void extraShowAllHarmoniesStructuresAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a extraShowAllHarmoniesStructuresAtom" <<
      endl;
  }
#endif

  printAllHarmoniesStructures (os);
}

void extraShowAllHarmoniesStructuresAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesStructuresAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesStructuresAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesStructuresAtom>*> (v)) {
        S_extraShowAllHarmoniesStructuresAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowAllHarmoniesStructuresAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowAllHarmoniesStructuresAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesStructuresAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesStructuresAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesStructuresAtom>*> (v)) {
        S_extraShowAllHarmoniesStructuresAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowAllHarmoniesStructuresAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowAllHarmoniesStructuresAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesStructuresAtom::browseData ()" <<
      endl;
  }
#endif
}

void extraShowAllHarmoniesStructuresAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsShowAllHarmoniesStructuresAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void extraShowAllHarmoniesStructuresAtom::printAllHarmoniesStructures (ostream& os) const
{
  msrHarmonyStructure::printAllHarmoniesStructures (os);
}

void extraShowAllHarmoniesStructuresAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
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
  assert (o!=0);
  return o;
}

extraShowAllHarmoniesContentsAtom::extraShowAllHarmoniesContentsAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowAllHarmoniesContentsVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      extraShowAllHarmoniesContentsVariable)
{
  fElementHelpOnlyKind = kElementHelpOnlyYes;
}

extraShowAllHarmoniesContentsAtom::~extraShowAllHarmoniesContentsAtom ()
{}

void extraShowAllHarmoniesContentsAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraShowAllHarmoniesContentsAtom'" <<
      endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
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
          quarterTonesPitchesLanguageKindAsString (
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
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
    gLogStream,
    semiTonesPitchKind);
}

void extraShowAllHarmoniesContentsAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesContentsAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesContentsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesContentsAtom>*> (v)) {
        S_extraShowAllHarmoniesContentsAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowAllHarmoniesContentsAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowAllHarmoniesContentsAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesContentsAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowAllHarmoniesContentsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowAllHarmoniesContentsAtom>*> (v)) {
        S_extraShowAllHarmoniesContentsAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowAllHarmoniesContentsAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowAllHarmoniesContentsAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowAllHarmoniesContentsAtom::browseData ()" <<
      endl;
  }
#endif
}

void extraShowAllHarmoniesContentsAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowAllHarmoniesContentsAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

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

void extraShowAllHarmoniesContentsAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
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
  assert (o!=0);
  return o;
}

extraShowHarmonyDetailsAtom::extraShowHarmonyDetailsAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowHarmonyDetailsVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      extraShowHarmonyDetailsVariable)
{
  fElementHelpOnlyKind = kElementHelpOnlyYes;
}

extraShowHarmonyDetailsAtom::~extraShowHarmonyDetailsAtom ()
{}

void extraShowHarmonyDetailsAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraShowHarmonyDetailsAtom'" <<
      endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
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

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for Harmony details string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      smSize << " elements: ";
    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        "[" << sm [i] << "] ";
    } // for
    gLogStream << endl;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
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
          quarterTonesPitchesLanguageKindAsString (
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
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
    gLogStream,
    semiTonesPitchKind,
    harmonyKind);
}

void extraShowHarmonyDetailsAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyDetailsAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowHarmonyDetailsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyDetailsAtom>*> (v)) {
        S_extraShowHarmonyDetailsAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowHarmonyDetailsAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowHarmonyDetailsAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyDetailsAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowHarmonyDetailsAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyDetailsAtom>*> (v)) {
        S_extraShowHarmonyDetailsAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowHarmonyDetailsAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowHarmonyDetailsAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyDetailsAtom::browseData ()" <<
      endl;
  }
#endif
}

void extraShowHarmonyDetailsAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowHarmonyDetailsAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void extraShowHarmonyDetailsAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
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
  assert (o!=0);
  return o;
}

extraShowHarmonyAnalysisAtom::extraShowHarmonyAnalysisAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& extraShowHarmonyAnalysisVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      extraShowHarmonyAnalysisVariable)
{
  fElementHelpOnlyKind = kElementHelpOnlyYes;
}

extraShowHarmonyAnalysisAtom::~extraShowHarmonyAnalysisAtom ()
{}

void extraShowHarmonyAnalysisAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'extraShowHarmonyAnalysisAtom'" <<
      endl;
  }
#endif

  // theString contains the pitch name in the current language
  // is it in the accidental styles map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
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

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for Harmony analysis string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
  }
#endif

  if (smSize == 4) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        smSize << " elements: ";
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
      "Harmony analysis argument '" << theString <<
      "' is ill-formed";

    oahError (s.str ());

/* JMI ??? should work...
    printSubGroupSpecificHelp (
      gLogStream,
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
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
          quarterTonesPitchesLanguageKindAsString (
            gGlobalLpsrOahGroup->
              getLpsrQuarterTonesPitchesLanguageKind ()) <<
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
    gLogStream,
    semiTonesPitchKind,
    harmonyKind,
    inversion);
}

void extraShowHarmonyAnalysisAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyAnalysisAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowHarmonyAnalysisAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyAnalysisAtom>*> (v)) {
        S_extraShowHarmonyAnalysisAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowHarmonyAnalysisAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraShowHarmonyAnalysisAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyAnalysisAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraShowHarmonyAnalysisAtom>*
    p =
      dynamic_cast<visitor<S_extraShowHarmonyAnalysisAtom>*> (v)) {
        S_extraShowHarmonyAnalysisAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraShowHarmonyAnalysisAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraShowHarmonyAnalysisAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraShowHarmonyAnalysisAtom::browseData ()" <<
      endl;
  }
#endif
}

void extraShowHarmonyAnalysisAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "extraShowHarmonyAnalysisAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  gIndenter--;
}

void extraShowHarmonyAnalysisAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

//_______________________________________________________________________________

S_extraOahGroup gGlobalExtraOahGroup;

S_extraOahGroup extraOahGroup::create ()
{
  extraOahGroup* o = new extraOahGroup ();
  assert (o!=0);
  return o;
}

extraOahGroup::extraOahGroup ()
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
    kElementVisibilityWhole)
{
  initializeExtraOahGroup ();
}

extraOahGroup::~extraOahGroup ()
{}

void extraOahGroup::initializeExtraShowAllHarmoniesStructuresOptions ()
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

void extraOahGroup::initializeExtraShowAllHarmoniesContentsOptions ()
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

void extraOahGroup::initializeExtraShowHarmonyDetailsOptions ()
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
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        "HARMONY_SPEC",
        "diatonic (semitones) pitch",
        fHarmoniesRootAsString));
}

void extraOahGroup::initializeExtraShowHarmonyAnalysisOptions ()
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
          gGlobalOahOahGroup->getHandlerExecutableName ()),
        "HARMONY_SPEC",
        "diatonic (semitones) pitch",
        fHarmoniesRootAsString));
}

void extraOahGroup::initializeExtraOahGroup ()
{
  // show all harmonies structures
  // --------------------------------------
  initializeExtraShowAllHarmoniesStructuresOptions ();

  // show all harmonies contents
  // --------------------------------------
  initializeExtraShowAllHarmoniesContentsOptions ();

  // show harmony details
  // --------------------------------------
  initializeExtraShowHarmonyDetailsOptions ();

  // show harmony analysis
  // --------------------------------------
  initializeExtraShowHarmonyAnalysisOptions ();
}

//______________________________________________________________________________
void extraOahGroup::enforceGroupQuietness ()
{
}

//______________________________________________________________________________
void extraOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void extraOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_extraOahGroup>*
    p =
      dynamic_cast<visitor<S_extraOahGroup>*> (v)) {
        S_extraOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void extraOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_extraOahGroup>*
    p =
      dynamic_cast<visitor<S_extraOahGroup>*> (v)) {
        S_extraOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching extraOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void extraOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> extraOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void extraOahGroup::printExtraOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The extra extra are:" << // JMI
    endl;

  gIndenter++;

  // harmony intervals
  // --------------------------------------

  // harmony notes
  // --------------------------------------

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_extraOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_extraOahGroup createGlobalExtraOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global extra OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalExtraOahGroup) {
    // create the global OAH group
    gGlobalExtraOahGroup =
      extraOahGroup::create ();
    assert (gGlobalExtraOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalExtraOahGroup;
}


}


#endif
