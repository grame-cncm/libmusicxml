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

#include "Mikrokosmos3WanderingOahTypes.h"
#include "Mikrokosmos3WanderingOah.h"
#include "Mikrokosmos3WanderingInsiderOahHandler.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_generatorOutputKindAtom generatorOutputKindAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  generatorOutputKind& generatorOutputKindVariable)
{
  generatorOutputKindAtom* o = new
    generatorOutputKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      generatorOutputKindVariable);
  assert (o!=0);
  return o;
}

generatorOutputKindAtom::generatorOutputKindAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  generatorOutputKind& generatorOutputKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fGeneratorOutputKindVariable (
      generatorOutputKindVariable)
{}

generatorOutputKindAtom::~generatorOutputKindAtom ()
{}

void generatorOutputKindAtom::applyAtomWithValue (
  const string& theString,
  ostream&      os)
{
  // theString contains the output kind name:
  // is it in the  output kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> handling atom '" << fetchNames () << "; which is of type 'generatorOutputKindAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
  }
#endif

  map<string, generatorOutputKind>::const_iterator
    it =
      gGlobalGeneratorOutputKindsMap.find (
        theString);

  if (it == gGlobalGeneratorOutputKindsMap.end ()) {
    // no, optional values style kind is unknown in the map
    stringstream s;

    s <<
      "generate code kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalGeneratorOutputKindsMap.size () - 1 <<
      " known generate code kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingGeneratorOutputKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  fGeneratorOutputKindVariable =
    (*it).second;
  fVariableHasBeenSet = true;
}

void generatorOutputKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generatorOutputKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_generatorOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_generatorOutputKindAtom>*> (v)) {
        S_generatorOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching generatorOutputKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void generatorOutputKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generatorOutputKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_generatorOutputKindAtom>*
    p =
      dynamic_cast<visitor<S_generatorOutputKindAtom>*> (v)) {
        S_generatorOutputKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching generatorOutputKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void generatorOutputKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> generatorOutputKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string generatorOutputKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    generatorOutputKindAsString (fGeneratorOutputKindVariable);

  return s.str ();
}

string generatorOutputKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    generatorOutputKindAsString (fGeneratorOutputKindVariable);

  return s.str ();
}

void generatorOutputKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "GeneratorOutputKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "brailleUTFKindVariable" << " : \"" <<
    generatorOutputKindAsString (
      fGeneratorOutputKindVariable) <<
      "\"" <<
    endl;

  --gIndenter;
}

void generatorOutputKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    generatorOutputKindAsString (
      fGeneratorOutputKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_generatorOutputKindAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________

S_Mikrokosmos3WanderingOahGroup gGlobalMikrokosmos3WanderingOahGroup;

S_Mikrokosmos3WanderingOahGroup Mikrokosmos3WanderingOahGroup::create ()
{
  Mikrokosmos3WanderingOahGroup* o = new Mikrokosmos3WanderingOahGroup ();
  assert (o!=0);
  return o;
}

Mikrokosmos3WanderingOahGroup::Mikrokosmos3WanderingOahGroup ()
  : oahGroup (
    "Mikrokosmos3Wandering",
    "hbsr2brl", "help-bsr-to-braille",
R"(These options control the way BSR data is translated to braille music.)",
    kElementVisibilityWhole)
{
  fGenerationAPIKind = kMsrFunctionsAPIKind;

  fGeneratorOutputKind = k_NoOutput;

  fUTFKind = kUTF8; // default value

  fBrailleOutputKind = kBrailleOutputAscii; // default value

  fByteOrderingKind = kByteOrderingNone;

  // initialize it
  initializeMikrokosmos3WanderingOahGroup ();
}

Mikrokosmos3WanderingOahGroup::~Mikrokosmos3WanderingOahGroup ()
{}

void Mikrokosmos3WanderingOahGroup::initializeGenerationAPIOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generation API",
        "hga", "help-generation-api",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  const msrGenerationAPIKind
    msrGenerationAPIKindDefaultValue =
      kMsrFunctionsAPIKind; // default value

  fGenerationAPIKindAtom =
    msrGenerationAPIKindAtom::create (
      K_GENERATION_API_KIND_SHORT_NAME, K_GENERATION_API_KIND_LONG_NAME,
      regex_replace (
        regex_replace (
          regex_replace (
  R"(Generate GENERATION_API code to the output.
  The NUMBER generation API kinds available are:
  GENERATION_API_KINDS.
  The default is 'DEFAULT_VALUE'.)",
            regex ("NUMBER"),
            to_string (gGlobalGenerationAPIKindsMap.size ())),
          regex ("GENERATION_API_KINDS"),
          existingGenerationAPIKinds (K_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
        msrGenerationAPIKindAsString (
          msrGenerationAPIKindDefaultValue)),
      "GENERATION_API",
      "msrGenerationAPIKind",
      fGenerationAPIKind);

  subGroup->
    appendAtomToSubGroup (
      fGenerationAPIKindAtom);
}

void Mikrokosmos3WanderingOahGroup::initializeGenerateCodeOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generated output",
        "hgc", "help-generate-output",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  const generatorOutputKind
    generatorOutputKindDefaultValue =
      kLilyPondOutput; // default value

  fGeneratorOutputKindAtom =
    generatorOutputKindAtom::create (
      K_GENERATED_OUTPUT_KIND_SHORT_NAME, K_GENERATED_OUTPUT_KIND_LONG_NAME,
      regex_replace (
        regex_replace (
          regex_replace (
  R"(Generate GENERATED_OUTPUT_KIND code to the output.
  The NUMBER generate code kinds available are:
  GENERATED_OUTPUT_KINDS.
  The default is 'DEFAULT_VALUE'.)",
            regex ("NUMBER"),
            to_string (gGlobalGeneratorOutputKindsMap.size ())),
          regex ("GENERATED_OUTPUT_KINDS"),
          existingGeneratorOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
        regex ("DEFAULT_VALUE"),
        generatorOutputKindAsString (
          generatorOutputKindDefaultValue)),
      "GENERATED_OUTPUT_KIND",
      "generatorOutputKind",
      fGeneratorOutputKind);

  subGroup->
    appendAtomToSubGroup (
      fGeneratorOutputKindAtom);
}

void Mikrokosmos3WanderingOahGroup::initializeBrailleEncodingOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Braille encoding",
        "hbe_mkk", "help-braille-encoding_mkk",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  const bsrBrailleOutputKind
    brailleOutputKindDefaultValue =
      kBrailleOutputAscii; // default value

  fBrailleOutputKindAtom =
    brailleOutputKindAtom::create (
    "bok_mkk", "braille-output-kind_mkk",
    regex_replace (
      regex_replace (
        regex_replace (
R"(Use OUTPUT_KIND to write the generated braille music to the output.
The NUMBER output kinds available are:
BRAILLE_OUTPUT_KINDS.
'utf8d' leads to every line in the braille score to be generated
as a line of cells followed by a line of text showing the contents
for debug purposes.
The default is 'DEFAULT_VALUE'.)",
          regex ("NUMBER"),
          to_string (gGlobalBsrBrailleOutputKindsMap.size ())),
        regex ("BRAILLE_OUTPUT_KINDS"),
        existingBsrBrailleOutputKinds (K_NAMES_LIST_MAX_LENGTH)),
      regex ("DEFAULT_VALUE"),
      brailleOutputKindAsString (
        brailleOutputKindDefaultValue)),
    "OUTPUT_KIND",
    "brailleOutputKind",
    fBrailleOutputKind);

  subGroup->
    appendAtomToSubGroup (
      fBrailleOutputKindAtom);
}

void Mikrokosmos3WanderingOahGroup::initializeBrailleByteOrderingOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Byte ordering",
        "hlpbo", "help-byte-ordering",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // byte ordering

  fByteOrderingKind = kByteOrderingBigEndian; // default value

  fByteOrderingKindAtom =
    brailleByteOrderingKindAtom::create (
    "bom", "byte-ordering-mark",
R"(Generate an initial BOM_ENDIAN byte ordering mark (BOM)
ahead of the Braille nusic code,
which can be one of 'big' or 'small'.
By default, a big endian BOM is generated.)",
    "BOM_ENDIAN",
    "byteOrderingKind",
    fByteOrderingKind);

  subGroup->
    appendAtomToSubGroup (
      fByteOrderingKindAtom);
}

void Mikrokosmos3WanderingOahGroup::initializeMikrokosmos3WanderingOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMikrokosmos3WanderingTraceOah ();
#endif

  // generation API
  // --------------------------------------
  initializeGenerationAPIOptions ();

  // generate code
  // --------------------------------------
  initializeGenerateCodeOptions ();

/* JMI
  // UTF encoding
  // --------------------------------------
  initializeBrailleEncodingOptions ();

  // byte ordering
  // --------------------------------------
  initializeBrailleByteOrderingOptions ();

  // braille music file name
  // --------------------------------------
  initializeBrailleMusicFileNameOptions ();

  // page parameters
  // --------------------------------------
  initializeBraillePageParametersOptions ();

  // code generation
  // --------------------------------------
  initializeBrailleCodeGenerationOptions ();
  */
}

//______________________________________________________________________________
void Mikrokosmos3WanderingOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void Mikrokosmos3WanderingOahGroup::checkGroupOptionsConsistency ()
{
  switch (fGeneratorOutputKind) {
    case k_NoOutput:
      {
        stringstream s;

        s <<
          fHandlerUpLink->getHandlerExecutableName () <<
          " needs an generate code option chosen among:" <<
          endl;

        ++gIndenter;

        s <<
          existingGeneratorOutputKinds (K_NAMES_LIST_MAX_LENGTH);

        --gIndenter;

        oahError (s.str ());
      }
      break;
    default:
      ;
  } // switch
}

//______________________________________________________________________________
void Mikrokosmos3WanderingOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_Mikrokosmos3WanderingOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching Mikrokosmos3WanderingOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void Mikrokosmos3WanderingOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_Mikrokosmos3WanderingOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching Mikrokosmos3WanderingOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void Mikrokosmos3WanderingOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void Mikrokosmos3WanderingOahGroup::printMikrokosmos3WanderingOahValues (
  unsigned int fieldWidth)
{
  gLogStream <<
    "The Mikrokosmos3Wandering options are:" <<
    endl;

  ++gIndenter;

  // generation API kind
  // --------------------------------------

  gLogStream <<
    "Generation API:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "gnerationAPIKind" << " : " <<
      msrGenerationAPIKindAsString (fGenerationAPIKind) <<
      endl;

  --gIndenter;

  // generator output kind
  // --------------------------------------

  gLogStream <<
    "Generator output:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "generatorOutputKind" << " : " <<
      generatorOutputKindAsString (fGeneratorOutputKind) <<
      endl;

  --gIndenter;


/* JMI
  // code generation
  // --------------------------------------

  gLogStream <<
    "Braille code generation:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "UTFKind" << " : " <<
      bsrUTFKindAsString (fUTFKind) <<
      endl <<

    setw (fieldWidth) << "byteOrderingKind" << " : " <<
      bsrByteOrderingKindAsString (fByteOrderingKind) <<
      endl <<

    setw (fieldWidth) << "useEncodingInFileName" << " : " <<
      booleanAsString (fUseEncodingInFileName) <<
      endl <<

    setw (fieldWidth) << "cellsPerLine" << " : " <<
      fCellsPerLine <<
      endl <<
    setw (fieldWidth) << "measuresPerLine" << " : " <<
      fMeasuresPerLine <<
      endl <<
    setw (fieldWidth) << "linesPerPage" << " : " <<
      fLinesPerPage <<
      endl <<

    setw (fieldWidth) << "xml2brlInfos" << " : " <<
      booleanAsString (fXml2brlInfos) <<
      endl <<

    setw (fieldWidth) << "noBrailleCode" << " : " <<
      booleanAsString (fNoBrailleCode) <<
      endl;

  --gIndenter;
*/

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_Mikrokosmos3WanderingOahGroup createGlobalMikrokosmos3WanderingOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global Mikrokosmos3Wandering OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMikrokosmos3WanderingOahGroup) {

    // initialize the generation API kinds map
    // ------------------------------------------------------

    initializeMsrGenerationAPI ();


    // initialize the generate code kinds map
    // ------------------------------------------------------

    initializeGeneratorOutputKindsMap ();

    // create the Mikrokosmos3Wandering options
    // ------------------------------------------------------

    gGlobalMikrokosmos3WanderingOahGroup =
      Mikrokosmos3WanderingOahGroup::create ();
    assert (gGlobalMikrokosmos3WanderingOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMikrokosmos3WanderingOahGroup;
}


}
