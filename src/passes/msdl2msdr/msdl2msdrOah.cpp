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

#include "waeMessagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"

#include "msdlOah.h"

#include "msdl2msdrOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________

S_msdl2msdrOahGroup gGlobalMsdl2msdrOahGroup;

S_msdl2msdrOahGroup msdl2msdrOahGroup::create ()
{
  msdl2msdrOahGroup* o = new msdl2msdrOahGroup ();
  assert (o != nullptr);
  return o;
}

msdl2msdrOahGroup::msdl2msdrOahGroup ()
  : oahGroup (
    "Msdl2msdr",
    "hmsdl2msdr", "help-msdl-to-msdr",
R"(These options control the way xmlelement are translated to MSDR.)",
    kElementVisibilityWhole)
{
  initializeMsdl2msdrGroup ();
}

msdl2msdrOahGroup::~msdl2msdrOahGroup ()
{}

#ifdef TRACING_IS_ENABLED
void msdl2msdrOahGroup::initializeMsdl2msdrTraceOah ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "msdl2msdl Trace",
        "hmsdl2msdlt", "help-msdl-to-msdl-trace",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // MSDL tokens

  fTraceTokens = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "ttoks", "trace-tokens",
R"(Write a trace of the MSDL tokens handling activity to standard error.)",
        "traceTokens",
        fTraceTokens));

  // MSDL tokens details

  fTraceTokensDetails = false;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "ttoksd", "trace-tokens-details",
R"(Write a trace of the MSDL tokens handling activity with more details to standard error.)",
        "traceTokensDetails",
        fTraceTokensDetails,
        fTraceTokens));

  // MSDL syntax

  fTraceSyntax = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tsynt", "trace-syntax",
R"(Write a trace of the MSDL syntax analysis activity to standard error.)",
        "traceSyntax",
        fTraceSyntax));

  // MSDL syntax details

  fTraceSyntaxDetails = false;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "tsyntd", "trace-syntax-details",
R"(Write a trace of the MSDL syntax analysis activity with more details to standard error.)",
        "traceSyntaxDetails",
        fTraceSyntaxDetails,
        fTraceSyntax));

  // MSDL syntax error recovery

  fTraceSyntaxErrorRecovery = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "tsynter", "trace-syntax-error-recovery",
R"(Write a trace of the MSDL syntax error recovery activity to standard error.)",
        "traceSyntaxErrorRecovery",
        fTraceSyntaxErrorRecovery));

  // MSDL syntax error recovery details

  fTraceSyntaxErrorRecoveryDetails = false;

  subGroup->
    appendAtomToSubGroup (
      oahTwoBooleansAtom::create (
        "tsynterd", "trace-syntax-error-recovery-details",
R"(Write a trace of the MSDL syntax error recovery activity with more details to standard error.)",
        "traceSyntaxErrorRecoveryDetails",
        fTraceSyntaxErrorRecoveryDetails,
        fTraceSyntaxErrorRecovery));
}
#endif

void msdl2msdrOahGroup::initializeMsdlLanguagesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Languages",
        "hmsdll", "help-msdl-languages",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  if (! setMsdlQuarterTonesPitchesLanguage ("english")) {
    stringstream s;

    s <<
      "INTERNAL INITIALIZATION ERROR: "
      "MSDR pitches default language 'english' is unknown" <<
      endl <<
      "The " <<
      gGlobalQuarterTonesPitchesLanguageKindsMap.size () <<
      " known MSDL pitches languages are:" <<
      endl;

    ++gIndenter;

    s <<
      existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  // MSDL user language

  const msdlUserLanguageKind
    msdlUserLanguageKindDefaultValue =
      kUserLanguageEnglish; // MSDL default

  fMsdlUserLanguageKind =
    msdlUserLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlUserLanguageAtom::create (
        "mul", "msdl-user-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to interact with the user.
The NUMBER MSDL user languages available are:
USER_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsdlUserLanguageKindsMap.size ())),
            regex ("USER_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingMsdlUserLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msdlUserLanguageKindAsString (
            msdlUserLanguageKindDefaultValue)),
        "LANGUAGE",
        "msdlUserLanguageKind",
        fMsdlUserLanguageKind));

  // MSDL pitches language

  const msrQuarterTonesPitchesLanguageKind
    msrQuarterTonesPitchesLanguageKindDefaultValue =
      fMsdlQuarterTonesPitchesLanguageKind;

  fMsdlQuarterTonesPitchesLanguageKind =
    msrQuarterTonesPitchesLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlPitchesLanguageAtom::create (
        "msdlpl", "msdl-pitches-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to input note pitches.
The NUMBER MSDL pitches languages available are:
PITCHES_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalQuarterTonesPitchesLanguageKindsMap.size ())),
            regex ("PITCHES_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingQuarterTonesPitchesLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msrQuarterTonesPitchesLanguageKindAsString (
            msrQuarterTonesPitchesLanguageKindDefaultValue)),
        "LANGUAGE",
        "msdlPitchesLanguage",
        fMsdlQuarterTonesPitchesLanguageKind));

  // MSDL keywords input language

  const msdlKeywordsLanguageKind
    msdlKeywordsInputLanguageKindDefaultValue =
      msdlKeywordsLanguageKind::kKeywordsEnglish; // MSDL default

  fMsdlKeywordsInputLanguageKind =
    msdlKeywordsInputLanguageKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlKeywordsLanguageAtom::create (
        "mkil", "msdl-keywords-input-language",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use LANGUAGE to input keyword names.
The NUMBER MSDL keywords input languages available are:
KEYWORDS_LANGUAGES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
            regex ("KEYWORDS_LANGUAGES"),
            gIndenter.indentMultiLineString (
              existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msdlKeywordsLanguageKindAsString (
            msdlKeywordsInputLanguageKindDefaultValue)),
        "LANGUAGE",
        "msdlKeywordsInputLanguageKind",
        fMsdlKeywordsInputLanguageKind));
}

void msdl2msdrOahGroup::initializeMsdlWholeInputScansOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MSDL whole input scans",
        "hmwis", "help-msdl-whole-input-scans",
R"()",
        kElementVisibilityWhole,
        this);

  appendSubGroupToGroup (subGroup);

  // perform lexical analysis only

  fLexicalAnalysisOnly = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lex", "lex-only",
R"(Perform lexical analysis only of the whole input at once,
without syntactic nor semantic analysis. No code is generated.)",
        "lexicalAnalysisOnly",
        fLexicalAnalysisOnly));

  // ignore separator tokens

  fIgnoreSeparatorTokens = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "istoks", "ignore-separator-tokens",
R"(Ignore separator tokens such as space, tab and end of line
when performing lexical analysis of the whole input at once.)",
        "ignoreSeparatorTokens",
        fIgnoreSeparatorTokens));

  // MSDL keywords translation language

  fMsdlKeywordsTranslationLanguageKind = msdlKeywordsLanguageKind::k_NoKeywordsLanguage;

  subGroup->
    appendAtomToSubGroup (
      msdlKeywordsLanguageAtom::create (
        "mktl", "msdl-keywords-translation-language",
        regex_replace (
          regex_replace (
R"(Use LANGUAGE to output translated keyword names.
This option causes the translated MSDL text to be written to standard output,
followed by a quit.
The NUMBER MSDL keywords translation languages available are:
KEYWORDS_LANGUAGES.
The default is not to translate the keywords.)",
            regex ("NUMBER"),
            to_string (gGlobalMsdlKeywordsLanguageKindsMap.size ())),
          regex ("KEYWORDS_LANGUAGES"),
          gIndenter.indentMultiLineString (
            existingMsdlKeywordsLanguageKinds (K_NAMES_LIST_MAX_LENGTH))),
        "LANGUAGE",
        "msdlKeywordsTranslationLanguageKind",
        fMsdlKeywordsTranslationLanguageKind));

  // MSDL comments type

  const msdlCommentsTypeKind
    msdlCommentsTypeKindDefaultValue =
      kCommentsTypePercent; // MSDL default

  fMsdlCommentsTypeTranslationKind =
    msdlCommentsTypeKindDefaultValue;

  subGroup->
    appendAtomToSubGroup (
      msdlCommentsTypeAtom::create (
        "mct", "msdl-comments-types",
        regex_replace (
          regex_replace (
            regex_replace (
R"(Use TYPE for the translated comments.
The NUMBER MSDL comments types available are:
COMMENTS_TYPES.
The default is 'DEFAULT_VALUE'.)",
              regex ("NUMBER"),
              to_string (gGlobalMsdlCommentsTypeKindsMap.size ())),
            regex ("COMMENTS_TYPES"),
            gIndenter.indentMultiLineString (
              existingMsdlCommentsTypeKinds (K_NAMES_LIST_MAX_LENGTH))),
          regex ("DEFAULT_VALUE"),
          msdlCommentsTypeKindAsString (
            msdlCommentsTypeKindDefaultValue)),
        "TYPE",
        "msdlCommentsTypeTranslationKind",
        fMsdlCommentsTypeTranslationKind));
}

void msdl2msdrOahGroup::initializeGenerateCodeOptions ()
{
/* JMI
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Generated output",
        "hgc", "help-generate-output",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // generator output kind

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
  The NUMBER generated output kinds available are:
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

  // generator output macros

  S_oahMacroAtom
    guidoMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_GUIDO_NAME, "",
        "Generate Guido output");
  guidoMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, "guido");
  subGroup->
    appendAtomToSubGroup (
      guidoMacroAtom);


  S_oahMacroAtom
    lilypondMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME, "",
        "Generate LilyPond output");
  lilypondMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, "lilypond");
  subGroup->
    appendAtomToSubGroup (
      lilypondMacroAtom);

  S_oahMacroAtom
    brailleMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_BRAILLE_NAME, "",
        "Generate braille music output");
  brailleMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, K_GENERATED_OUTPUT_KIND_BRAILLE_NAME);
  subGroup->
    appendAtomToSubGroup (
      brailleMacroAtom);

  S_oahMacroAtom
    musicxmlMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_MUSICXML_NAME, "",
        "Generate MusicXML output");
  musicxmlMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, "musicxml");
  subGroup->
    appendAtomToSubGroup (
      musicxmlMacroAtom);

/ * JMI
  S_oahMacroAtom
    midiMacroAtom =
      oahMacroAtom::create (
        K_GENERATED_OUTPUT_KIND_MIDI_NAME, "",
        "Generate MIDI output");
  midiMacroAtom->
    appendAtomStringPairToMacro (
      fGeneratorOutputKindAtom, "midi");
  subGroup->
    appendAtomToSubGroup (
      midiMacroAtom);
      */
}

void msdl2msdrOahGroup::initializeMsdl2msdrGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
  initializeMsdl2msdrTraceOah ();
#endif

  initializeMsdlLanguagesOptions ();

  initializeMsdlWholeInputScansOptions ();

  initializeGenerateCodeOptions ();
}

//______________________________________________________________________________
bool msdl2msdrOahGroup::setMsdlQuarterTonesPitchesLanguage (string language)
{
  // is language in the pitches languages map?
  map<string, msrQuarterTonesPitchesLanguageKind>::const_iterator
    it =
      gGlobalQuarterTonesPitchesLanguageKindsMap.find (language);

  if (it == gGlobalQuarterTonesPitchesLanguageKindsMap.end ()) {
    // no, language is unknown in the map
    return false;
  }

  fMsdlQuarterTonesPitchesLanguageKind = (*it).second;

  return true;
}

//______________________________________________________________________________
void msdl2msdrOahGroup::enforceGroupQuietness ()
{
#ifdef TRACING_IS_ENABLED
// JMI
#endif
}

//______________________________________________________________________________
void msdl2msdrOahGroup::checkGroupOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void msdl2msdrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2msdrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2msdrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2msdrOahGroup>*> (v)) {
        S_msdl2msdrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2msdrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msdl2msdrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2msdrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msdl2msdrOahGroup>*
    p =
      dynamic_cast<visitor<S_msdl2msdrOahGroup>*> (v)) {
        S_msdl2msdrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msdl2msdrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msdl2msdrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msdl2msdrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

//______________________________________________________________________________
void msdl2msdrOahGroup::printMsdl2msdrValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The msdl2msdr options are:" <<
    endl;

  ++gIndenter;

#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------

  gLogStream <<
    "Trace:" <<
    endl;

  ++gIndenter;

  // MSDL tokens
  // --------------------------------------

  gLogStream <<
    "MSDL tokens:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "traceTokens" << " : " <<
      booleanAsString (fTraceTokens) <<
      endl <<
    setw (fieldWidth) << "traceTokens" << " : " <<
      booleanAsString (fTraceTokens) <<
      endl <<

    setw (fieldWidth) << "traceSyntax" << " : " <<
      booleanAsString (fTraceSyntax) <<
      endl <<
    setw (fieldWidth) << "traceSyntaxDetails" << " : " <<
      booleanAsString (fTraceSyntaxDetails) <<
      endl;

  --gIndenter;
#endif

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdl2msdrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msdl2msdrOahGroup createGlobalMsdl2msdrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Creating global msdl2msdr OAH group" <<
      endl;
  }
#endif

  // protect library against multiple initializations
  if (! gGlobalMsdl2msdrOahGroup) {
    // create the global OAH group
    gGlobalMsdl2msdrOahGroup =
      msdl2msdrOahGroup::create ();
    assert (gGlobalMsdl2msdrOahGroup != 0);
  }

  // return global OAH group
  return gGlobalMsdl2msdrOahGroup;
}


}
