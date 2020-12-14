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

#include "enableExtraOahIfDesired.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "msr.h"
#include "lpsr.h"

#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "mxmlTree2msrOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"
#include "lilypondOah.h"

#include "version.h"

#include "xml2lyRegularOahHandler.h"


using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
//#define ENFORCE_TRACE_OAH

//______________________________________________________________________________
S_xml2lyRegularOahHandler xml2lyRegularOahHandler::create (
  string      executableName,
  string      executableAboutInformation,
  string      handlerHeader,
  S_xml2lyInsiderOahHandler
              insiderOahHandler)
{
  // create the regular handler
  xml2lyRegularOahHandler* o = new
    xml2lyRegularOahHandler (
      executableName,
      executableAboutInformation,
      handlerHeader,
      insiderOahHandler);
  assert (o!=0);

  return o;
}

xml2lyRegularOahHandler::xml2lyRegularOahHandler (
  string      executableName,
  string      executableAboutInformation,
  string      handlerHeader,
  S_xml2lyInsiderOahHandler
              insiderOahHandler)
  : oahRegularOahHandler (
      executableName,
      executableAboutInformation,
      handlerHeader,
      insiderOahHandler)
{
  // this is done only only after the constructor has been executed,
  // because it uses pure virtual methods
  initializeOahRegularOahHandler ();

  // create the regular handler groups
  createRegularHandlerGroups ();

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  // print the options handler initial state
  gLogStream <<
    "xml2lyRegularOahHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    endl;
#endif
#endif
  }

xml2lyRegularOahHandler::~xml2lyRegularOahHandler ()
{}

void xml2lyRegularOahHandler::createRegularHandlerGroups ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating the regular handler groups for \"" << fHandlerHeader << "\"" <<
    endl;
#endif
#endif

  createInformationsRegularGroup ();

  createFilesRegularGroup ();

  createOahRegularGroup ();

  createWarningAndErrorsRegularGroup ();

  createInputRegularGroup ();

  createPartsRegularGroup ();
  createStavesRegularGroup ();
  createVoicesRegularGroup ();

  createTranspositionsRegularGroup ();

  createClefsRegularGroup ();
  createKeysRegularGroup ();
  createTimesRegularGroup ();

  createRepeatsRegularGroup ();

  createMeasuresRegularGroup ();

  createRestsRegularGroup ();
  createNotesRegularGroup ();

  creatBeamsRegularGroup ();

  createArticulationsRegularGroup ();

  createOrnamentsRegularGroup ();

  createGraceNotesRegularGroup ();

  createChordsRegularGroup ();

  createTiesRegularGroup ();
  createSlursRegularGroup ();
  createLigaturesRegularGroup ();

  createDynamicsRegularGroup ();
  createWedgesRegularGroup ();

  createTupletsRegularGroup ();

  createLyricsRegularGroup ();

  createStringsRegularGroup ();

  createTablaturesRegularGroup ();

  createHarmoniesRegularGroup ();

  createFiguredBassesRegularGroup ();

  createBookRegularGroup ();
  createHeaderRegularGroup ();
  createPaperRegularGroup ();
  createLayoutRegularGroup ();

  createOutputRegularGroup ();

  createMidiRegularGroup ();

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "All the regular handler groups for \"" <<
    fHandlerHeader <<
    "\" have been created" <<
    endl;
#endif
#endif
}

void xml2lyRegularOahHandler::createOahRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Options and help group",
        "hoah-group", "help-oah-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options and help",
        "hoah", "help-oah",
        "",
        kElementVisibilityHeaderOnly,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup (K_INSIDER_OPTION_NAME, subGroup);
  registerAtomInRegularSubgroup (K_REGULAR_OPTION_NAME, subGroup);

  registerAtomInRegularSubgroup ("help", subGroup);
  registerAtomInRegularSubgroup ("help-summary", subGroup);

  registerAtomInRegularSubgroup ("help-options-usage", subGroup);

  registerAtomInRegularSubgroup ("name-help", subGroup);
  registerAtomInRegularSubgroup ("apropos-option-name", subGroup);
  registerAtomInRegularSubgroup ("find-string", subGroup);

  registerAtomInRegularSubgroup ("display-options-values", subGroup);

  registerAtomInRegularSubgroup ("show-options-and-arguments", subGroup);

  registerAtomInRegularSubgroup ("display-partgroups", subGroup);

  registerAtomInRegularSubgroup ("display-msr-skeleton", subGroup);
  registerAtomInRegularSubgroup ("display-msr", subGroup);
  registerAtomInRegularSubgroup ("display-lpsr", subGroup);

  registerAtomInRegularSubgroup ("trace-oah", subGroup);
  registerAtomInRegularSubgroup ("trace-oah-details", subGroup);
  registerAtomInRegularSubgroup ("trace-passes", subGroup);

  registerAtomInRegularSubgroup ("trace-msr", subGroup);
}

void xml2lyRegularOahHandler::createInformationsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Informations group",
        "hinfos-group", "help-informations-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Informations",
        "hinfos", "help-informations",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms from the insider handler

  registerAtomInRegularSubgroup ("version", subGroup);
  registerAtomInRegularSubgroup ("about", subGroup);
  registerAtomInRegularSubgroup ("contact", subGroup);

  registerAtomInRegularSubgroup ("display-cpu-usage", subGroup);
}

void xml2lyRegularOahHandler::createWarningAndErrorsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Warning and errors group",
        "hwae-group", "help-warning-and-errors-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Warning and errors",
        "hwae", "help-warning-and-errors",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("quiet", subGroup);
  registerAtomInRegularSubgroup ("dont-show-errors", subGroup);
  registerAtomInRegularSubgroup ("dont-quit-on-errors", subGroup);
}

void xml2lyRegularOahHandler::createInputRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Input group",
        "hinput-group", "help-input-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Input",
        "hinput", "help-input",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-words", subGroup);
  registerAtomInRegularSubgroup ("convert-tempos-to-rehearsal-marks", subGroup);
  registerAtomInRegularSubgroup ("convert-words-to-rehearsal-marks", subGroup);
  registerAtomInRegularSubgroup ("convert-words-to-tempo", subGroup);

  registerAtomInRegularSubgroup ("dal-segno", subGroup);
}

void xml2lyRegularOahHandler::createFilesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Files group",
        "hfiles-group", "help-files-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Files", "hfiles", "help-files",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("output-file-name", subGroup);
  registerAtomInRegularSubgroup ("auto-output-file-name", subGroup);
}

void xml2lyRegularOahHandler::createPartsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Parts group",
        "hparts-group", "help-parts-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Parts",
        "hparts", "help-parts",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("msr-omit-part-id", subGroup);
  registerAtomInRegularSubgroup ("msr-omit-part-name", subGroup);
  registerAtomInRegularSubgroup ("msr-keep-part-id", subGroup);
  registerAtomInRegularSubgroup ("msr-keep-part-name", subGroup);
  registerAtomInRegularSubgroup ("msr-rename-part", subGroup);
}

void xml2lyRegularOahHandler::createStavesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Staves group",
        "hstaves-group",
        "help-staves-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Staves",
        "hstaves", "help-staves",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("create-voices-staff-relative-numbers", subGroup);

  registerAtomInRegularSubgroup ("ledger-lines-color", subGroup);
}

void xml2lyRegularOahHandler::createVoicesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Voices group",
        "hvoices-group", "help-voices-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices",
        "hvoices", "help-voices",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("create-single-line-staves-as-rythmic", subGroup);
  registerAtomInRegularSubgroup ("connect-arpeggios", subGroup);
}

void xml2lyRegularOahHandler::createTranspositionsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Transpositions group",
        "htransp-group", "help-transpositions-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Transpositions",
        "hetransp", "help-transpositions",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms
// JMI
}

void xml2lyRegularOahHandler::createClefsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Clefs group",
        "hlefs-group", "help-clefs-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs",
        "hclefs", "help-clefs",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("msr-replace-clef", subGroup);
  registerAtomInRegularSubgroup ("ignore-redundant-clefs", subGroup);
  registerAtomInRegularSubgroup ("comment-clef-changes", subGroup);
}

void xml2lyRegularOahHandler::createKeysRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Keys group",
        "hkeys-group", "help-keys-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Keys",
        "hkeys", "help-keys",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("ignore-redundant-keys", subGroup);
}

void xml2lyRegularOahHandler::createTimesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Times group",
        "htimes-group", "help-times-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Times",
        "htimes", "help-times",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("cubase", subGroup);
  registerAtomInRegularSubgroup ("ignore-redundant-times", subGroup);
  registerAtomInRegularSubgroup ("numerical-time", subGroup);
}

void xml2lyRegularOahHandler::createRepeatsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Repeats group",
        "hreps-group", "help-repeats-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Repeats",
        "hreps", "help-repeats",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("create-implicit-initial-repeat-barline", subGroup);
  registerAtomInRegularSubgroup ("keep-repeat-barlines", subGroup);
  registerAtomInRegularSubgroup ("repeat-brackets", subGroup);
  registerAtomInRegularSubgroup ("ignore-repeat-numbers", subGroup);
}

void xml2lyRegularOahHandler::createMeasuresRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Measures group",
        "hmeas-group", "help-measures-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "hmeas", "help-measures",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("add-empty-measures", subGroup);
  registerAtomInRegularSubgroup ("replicate-empty-measure", subGroup);
}

void xml2lyRegularOahHandler::createRestsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Rests group",
        "hrests-group", "help-rests-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Rests",
        "hrests", "help-rests",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("delay-rests-slashes", subGroup);

  registerAtomInRegularSubgroup ("compress-full-measure-rests", subGroup);
  registerAtomInRegularSubgroup ("merge-rests", subGroup);
}

void xml2lyRegularOahHandler::createNotesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Notes group",
        "hnotes-group", "help-notes-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "hnotes", "help-notes",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("msr-pitches-language", subGroup);

  registerAtomInRegularSubgroup ("absolute", subGroup);
  registerAtomInRegularSubgroup ("relative", subGroup);
  registerAtomInRegularSubgroup ("fixed", subGroup);

  registerAtomInRegularSubgroup ("white-note-heads", subGroup);
  registerAtomInRegularSubgroup ("generate-commented-out-variables", subGroup);

  registerAtomInRegularSubgroup ("accidental-style", subGroup);

  registerAtomInRegularSubgroup ("non-print-notes-head-color", subGroup);
}

void xml2lyRegularOahHandler::creatBeamsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Beams group",
        "hbeams-group", "help-beams-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Beams",
        "hbeams", "help-beams",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("no-auto-beaming", subGroup);
  registerAtomInRegularSubgroup ("no-beams", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-beams", subGroup); // JMI
}

void xml2lyRegularOahHandler::createArticulationsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Articulations group",
        "harts-group", "help-articulations-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Articulations",
        "harts", "help-articulations",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-articulations", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-pedals", subGroup);
}

void xml2lyRegularOahHandler::createOrnamentsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ornaments group",
        "horns-group", "help-ornaments-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ornaments",
        "horns", "help-ornaments",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-ornaments", subGroup);

  registerAtomInRegularSubgroup ("delayed-ornaments-fraction", subGroup);
}

void xml2lyRegularOahHandler::createGraceNotesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Grace notes group",
        "hgraces-group", "help-grace-notes-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Grace notes",
        "hgraces", "help-grace-notes",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("slash-all-grace-notes", subGroup);
  registerAtomInRegularSubgroup ("slur-all-grace-notes", subGroup);
  registerAtomInRegularSubgroup ("beam-all-grace-notes", subGroup);
}

void xml2lyRegularOahHandler::createChordsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Chords group",
        "hchords-group", "help-chords-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords",
        "hchords", "help-chords",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("trace-chords", subGroup);

  registerAtomInRegularSubgroup ("lpsr-pitches-language", subGroup);

  registerAtomInRegularSubgroup ("lpsr-chords-language", subGroup);

  registerAtomInRegularSubgroup ("chords-display", subGroup);
}

void xml2lyRegularOahHandler::createTiesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ties group",
        "hties-group", "help-ties-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ties",
        "hties", "help-ties",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-ties", subGroup);
}

void xml2lyRegularOahHandler::createSlursRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Slurs group",
        "hslurs-group", "help-slurs-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Slurs",
        "hslurs", "help-slurs",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-slurs", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-slurs", subGroup);
}

void xml2lyRegularOahHandler::createLigaturesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ligatures group",
        "hligs-group", "help-ligatures-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ligatures",
        "hligs", "help-ligatures",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("delay-rests-ligatures", subGroup);
}

void xml2lyRegularOahHandler::createDynamicsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Dynamics group",
        "hdyns-group", "help-dynamics-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics",
        "hdyns", "help-dynamics",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-dynamics", subGroup);

  registerAtomInRegularSubgroup ("all-dynamics-below", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-dynamics", subGroup);
}

void xml2lyRegularOahHandler::createWedgesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Wedges group",
        "hweds-group", "help-wedges-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Wedges",
        "hweds", "help-wedges",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-wedges", subGroup);

  registerAtomInRegularSubgroup ("all-wedges-below", subGroup);

  registerAtomInRegularSubgroup ("delay-rests-wedges", subGroup);
}

void xml2lyRegularOahHandler::createTupletsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tuplets group",
        "htups-group", "help-tuplets-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tuplets",
        "htups", "help-tuplets",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("indent-tuplets", subGroup);
}

void xml2lyRegularOahHandler::createStringsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Strings group",
        "hstrings-group", "help-strings-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Strings",
        "hstrings", "", "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("roman-string-numbers", subGroup);
  registerAtomInRegularSubgroup ("avoid-open-strings", subGroup);
}

void xml2lyRegularOahHandler::createTablaturesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tablatures group",
        "htabs-group", "help-tablatures-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tablatures",
        "htabs", "help-tablatures",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("tabFullNotation", subGroup);
  registerAtomInRegularSubgroup ("modernTab", subGroup);
}

void xml2lyRegularOahHandler::createHarmoniesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Harmonies group",
        "hharms-group", "help-harmonies-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies",
        "hharms", "help-harmonies",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-harmonies", subGroup);

  registerAtomInRegularSubgroup ("show-harmony-voices", subGroup);

#ifdef EXTRA_OAH_IS_ENABLED
  registerAtomInRegularSubgroup ("show-harmonies-structures", subGroup);
  registerAtomInRegularSubgroup ("show-all-harmonies-contents", subGroup);
  registerAtomInRegularSubgroup ("show-harmony-details", subGroup);
  registerAtomInRegularSubgroup ("show-harmony-analysis", subGroup);
#endif
}

void xml2lyRegularOahHandler::createFiguredBassesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Figured basses group",
        "hfigbass-group", "help-figured-basses-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Figured basses",
        "hfigbass", "help-figured-basses",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-figured-basses", subGroup);

  registerAtomInRegularSubgroup ("show-figured-bass-voices", subGroup);
}

void xml2lyRegularOahHandler::createLyricsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Lyrics group",
        "hlyrics-group", "help-lyrics-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Lyrics",
        "hlyrics", "help-lyrics",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("omit-lyrics", subGroup);
  registerAtomInRegularSubgroup ("add-words-from-the-lyrics", subGroup);

  registerAtomInRegularSubgroup ("no-lilypond-lyrics", subGroup);
  registerAtomInRegularSubgroup ("lyrics-durations", subGroup);
  registerAtomInRegularSubgroup ("add-stanzas-numbers-to-lyrics", subGroup);
}

void xml2lyRegularOahHandler::createBookRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Book group",
        "hbook-group", "help-book-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Book",
        "hbook", "help-book",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("no-book-block", subGroup);
  registerAtomInRegularSubgroup ("no-header-block", subGroup);
  registerAtomInRegularSubgroup ("no-paper-block", subGroup);
  registerAtomInRegularSubgroup ("no-bar-number-checks", subGroup);
}

void xml2lyRegularOahHandler::createHeaderRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Header group",
        "hheader-group", "help-header-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Header",
        "hheader", "help-header",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("rights", subGroup);

  registerAtomInRegularSubgroup ("composer", subGroup);
  registerAtomInRegularSubgroup ("arranger", subGroup);
  registerAtomInRegularSubgroup ("lyricist", subGroup);
  registerAtomInRegularSubgroup ("poet", subGroup);

  registerAtomInRegularSubgroup ("software", subGroup);

  registerAtomInRegularSubgroup ("dedication", subGroup);
  registerAtomInRegularSubgroup ("piece", subGroup);
  registerAtomInRegularSubgroup ("opus", subGroup);

  registerAtomInRegularSubgroup ("title", subGroup);
  registerAtomInRegularSubgroup ("subtitle", subGroup);
  registerAtomInRegularSubgroup ("subsubtitle", subGroup);

  registerAtomInRegularSubgroup ("instrument", subGroup);

  registerAtomInRegularSubgroup ("meter", subGroup);

  registerAtomInRegularSubgroup ("copyright", subGroup);

  registerAtomInRegularSubgroup ("tagline", subGroup);

  registerAtomInRegularSubgroup ("use-lyricists-as-poets", subGroup);

  registerAtomInRegularSubgroup ("use-filename-as-work-title", subGroup);
}

void xml2lyRegularOahHandler::createPaperRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Paper group",
        "h-group", "help-paper-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Paper",
        "hpaper", "help-paper",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("paper-height", subGroup);
  registerAtomInRegularSubgroup ("paper-width", subGroup);

  registerAtomInRegularSubgroup ("left-margin", subGroup);
  registerAtomInRegularSubgroup ("right-margin", subGroup);
  registerAtomInRegularSubgroup ("top-margin", subGroup);
  registerAtomInRegularSubgroup ("bottom-margin", subGroup);

  registerAtomInRegularSubgroup ("ragged-bottom", subGroup);
  registerAtomInRegularSubgroup ("ragged-last", subGroup);
  registerAtomInRegularSubgroup ("ragged-last-bottom", subGroup);
  registerAtomInRegularSubgroup ("ragged-right", subGroup);

  registerAtomInRegularSubgroup ("horizontal-shift", subGroup);
  registerAtomInRegularSubgroup ("indent", subGroup);
  registerAtomInRegularSubgroup ("short-indent", subGroup);

  registerAtomInRegularSubgroup ("markup-system-spacing-padding", subGroup);

  registerAtomInRegularSubgroup ("page-count", subGroup);
  registerAtomInRegularSubgroup ("system-count", subGroup);
}

void xml2lyRegularOahHandler::createLayoutRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Layout group",
        "hlayout-group", "help-layout-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Layout",
        "hlayout", "help-layout",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("global-staff-size", subGroup);
  registerAtomInRegularSubgroup ("keep-staff-size", subGroup);
}

void xml2lyRegularOahHandler::createOutputRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ouptut group",
        "houptut-group", "help-ouptut-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ouptut",
        "houptut", "help-ouptut",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("xml2ly-infos", subGroup);

  registerAtomInRegularSubgroup ("no-lilypond-code", subGroup);

  registerAtomInRegularSubgroup ("draft-mode", subGroup);
  registerAtomInRegularSubgroup ("point-and-clic-off", subGroup);

  registerAtomInRegularSubgroup ("lilypond-version", subGroup);
  registerAtomInRegularSubgroup ("lilypond-compile-date", subGroup);
  registerAtomInRegularSubgroup ("lilypond-comments", subGroup);

  registerAtomInRegularSubgroup ("no-set-global-staff-size", subGroup);

  registerAtomInRegularSubgroup ("msr2lpsr-score-output-kind", subGroup);

  registerAtomInRegularSubgroup ("global", subGroup);

  registerAtomInRegularSubgroup ("ambitus", subGroup);

  registerAtomInRegularSubgroup ("input-line-numbers", subGroup);

  registerAtomInRegularSubgroup ("generate-moments-in-measures", subGroup);
  registerAtomInRegularSubgroup ("generate-positions-in-measures", subGroup);

  registerAtomInRegularSubgroup ("all-bar-numbers", subGroup);
  registerAtomInRegularSubgroup ("all-measure-numbers", subGroup);
  registerAtomInRegularSubgroup ("generate-box-around-bar-number", subGroup);
  registerAtomInRegularSubgroup ("reset-measure-number", subGroup);
  registerAtomInRegularSubgroup ("original-measure-numbers", subGroup);

  registerAtomInRegularSubgroup ("separator-line-every-n-measures", subGroup);

  registerAtomInRegularSubgroup ("ignore-musicxml-line-breaks", subGroup);
  registerAtomInRegularSubgroup ("suppress-musicxml-line-breaks", subGroup);

  registerAtomInRegularSubgroup ("ignore-musixcml-page-breaks", subGroup);
  registerAtomInRegularSubgroup ("suppress-musixcml-page-breaks", subGroup);

  registerAtomInRegularSubgroup ("break-lines-at-incomplete-right-measures", subGroup);
  registerAtomInRegularSubgroup ("custos", subGroup);

  registerAtomInRegularSubgroup ("length-unit", subGroup);

  registerAtomInRegularSubgroup ("all-durations", subGroup);

  registerAtomInRegularSubgroup ("jazz-fonts", subGroup);

  registerAtomInRegularSubgroup ("jianpu", subGroup);

  registerAtomInRegularSubgroup ("minimal", subGroup);
}

void xml2lyRegularOahHandler::createMidiRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "MIDI group",
        "hmidi-group", "help-midi-group",
        "",
        kElementVisibilityWhole);
  appendGroupToRegulalHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MIDI",
        "hmidi", "help-midi",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  registerAtomInRegularSubgroup ("no-midi", subGroup);
  registerAtomInRegularSubgroup ("midi-tempo", subGroup);
}

//______________________________________________________________________________
void xml2lyRegularOahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2lyRegularOahHandler '" << fHandlerHeader << "':" <<
    endl;

  gIndenter++;

  printHandlerEssentials (
    os, fieldWidth);
  os << endl;

  os <<
    "Options groups (" <<
    singularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fHandlerGroupsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the options group
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;

  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2lyRegularOahHandler& elt)
{
  elt->print (os);
  return os;
}


}
