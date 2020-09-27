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

#include "setTraceOahIfDesired.h"
#include "traceOah.h"

#include "msr.h"
#include "lpsr.h"

#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "mxmlTree2MsrOah.h"
#include "msrOah.h"
#include "msr2LpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2LilypondOah.h"
#include "lilypondOah.h"
#include "msr2MxmlTreeOah.h"    // this is in case the '-loop' option is selected

#include "version.h"

#include "xml2lyRegularOahHandler.h"


using namespace std;

namespace MusicXML2
{
/*
  OAH_TRACE can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
//#define OAH_TRACE

//______________________________________________________________________________
S_xml2lyRegularOahHandler xml2lyRegularOahHandler::create (
  string   executableName,
  string   handlerHeader,
  ostream& os)
{
  // create the regular handler
  xml2lyRegularOahHandler* o = new
    xml2lyRegularOahHandler (
      executableName,
      handlerHeader,
      os);
  assert(o!=0);

  return o;
}

S_xml2lyRegularOahHandler xml2lyRegularOahHandler::createFromInsiderHandler (
  string                    executableName,
  string                    handlerHeader,
  S_xml2lyInsiderOahHandler insiderHandler,
  ostream&                  os)
{
  // create the regular handler
  xml2lyRegularOahHandler* o = new
    xml2lyRegularOahHandler (
      executableName,
      handlerHeader,
      insiderHandler,
      os);
  assert(o!=0);

  return o;
}

xml2lyRegularOahHandler::xml2lyRegularOahHandler (
  string   executableName,
  string   handlerHeader,
  ostream& os)
  : oahHandler (
      executableName,
      handlerHeader,
      executableName + " options values",
      "h", "help",
      "hs","helpSummary",
R"(                      Welcome to xml2ly,
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: xml2ly [options] [MusicXMLFile|-] [options]
)",
R"(
Option '-h, -help' prints the full help,
  while '-hs, -helpSummary' only prints a help summary.)",
    os)
{
  // create the 'insider view' handler
#ifdef TRACE_OAH
#ifdef OAH_TRACE
  gLogOstream <<
    "Creating the insider view handler for \"" << fHandlerHeader << "\"" <<
    endl;
#endif
#endif

  S_xml2lyInsiderOahHandler
    insiderHandler =
      xml2lyInsiderOahHandler::create (
        executableName,
        executableName + " insider handler OAH",
        os);

  initializeXml2lyRegularOahHandler (
    insiderHandler);
}

xml2lyRegularOahHandler::xml2lyRegularOahHandler (
  string                    executableName,
  string                    handlerHeader,
  S_xml2lyInsiderOahHandler insiderHandler,
  ostream&                  os)
  : oahHandler (
      executableName,
      handlerHeader,
      executableName + " options values",
      "h", "help",
      "hs","helpSummary",
R"(                      Welcome to xml2ly,
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: xml2ly [options] [MusicXMLFile|-] [options]
)",
R"(
Option '-h, -help' prints the full help,
  while '-hs, -helpSummary' only prints a help summary.)",
    os)
{
  initializeXml2lyRegularOahHandler (
    insiderHandler);
}

xml2lyRegularOahHandler::~xml2lyRegularOahHandler ()
{}

void xml2lyRegularOahHandler::initializeXml2lyRegularOahHandler (
  S_xml2lyInsiderOahHandler insiderHandler)
{
#ifdef TRACE_OAH
#ifdef OAH_TRACE
      gLogOstream <<
        "Initializing xml2ly regular options handling" <<
        endl;
#endif
#endif

  // get the prefixes from insiderHandler
  fHandlerPrefixesMap =
    insiderHandler->
      getHandlerPrefixesMap ();

  // create the regular handler groups
  createRegularHandlerGroups ();

  // populate the regular handler from the insider view handler
  populateRegularViewHandlerFromInsiderHandler (
    insiderHandler);

  // register regular handler in itself,
  // so that the 'global' help options can be handled
  this->
    registerHandlerInItself ();

#ifdef TRACE_OAH
#ifdef OAH_TRACE
  // print the options handler initial state
  fHandlerLogOstream <<
    "xml2lyRegularOahHandler \"" <<
    fHandlerHeader <<
    "\" has been initialized as:" <<
    endl;

  gIndenter++;

//  this->printShort (fHandlerLogOstream);
  this->printHelp (fHandlerLogOstream); // JMI

  fHandlerLogOstream <<
    endl <<
    endl;

  gIndenter--;
#endif
#endif

  // register options handler in itself,
  // so that the 'global' help options can be handled
  /*
  this->
    registerHandlerInItself ();
*/
/* JMI
  // register options handler names in itself
  this->
    registerHandlerOptionNamesInItself ();
*/

#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
    fHandlerLogOstream <<
      "xml2lyRegularOahHandler help:" <<
      endl;

    this->
      printHelp (
        fHandlerLogOstream);
  }
#endif
}

void xml2lyRegularOahHandler::createRegularHandlerGroups ()
{
#ifdef TRACE_OAH
#ifdef OAH_TRACE
  gLogOstream <<
    "Creating the regular handler groups for \"" << fHandlerHeader << "\"" <<
    endl;
#endif
#endif

  createOahRegularGroup ();

  createInformationsRegularGroup ();

  createWarningAndErrorsRegularGroup ();

  createFilesRegularGroup ();

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

  createHeaderRegularGroup ();
  createPaperRegularGroup ();
  createLayoutRegularGroup ();

  createOutputRegularGroup ();

  createMidiRegularGroup ();
}

void xml2lyRegularOahHandler::createOahRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Options and help group",
        "hoah-grp",
        "help-oah-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Options and help",
        "hoah",
        "help-oah",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap [fShortName] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["insider"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["regular"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["help-options-usage"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["option-name-help"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["display-options-values"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["show-options-and-arguments"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["trace-oah"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["trace-passes"] = subGroup;
}

void xml2lyRegularOahHandler::createInformationsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Informations group",
        "hinfos-grp",
        "help-informations-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Informations",
        "hinfos",
        "help-informations",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["version"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["about"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["contact"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["display-cpu-usage"] = subGroup;
}

void xml2lyRegularOahHandler::createWarningAndErrorsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Warning and errors group",
        "hwae-grp",
        "help-warning-and-errors-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Warning and errors",
        "hwae",
        "help-warning-and-errors",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["quiet"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["dont-show-errors"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["dont-exit-on-errors"] = subGroup;
}

void xml2lyRegularOahHandler::createInputRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Input group",
        "hinput-grp",
        "help-input-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);
//  this-> // JMI
//    registerElementInHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Input",
        "hinput",
        "help-input",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-words"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["convert-tempos-to-rehearsal-marks"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["convert-words-to-rehearsal-marks"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["convert-words-to-tempo"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["cubase"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["dal-segno"] = subGroup;
}

void xml2lyRegularOahHandler::createFilesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Files group",
        "hfiles-grp",
        "help-files-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Files",
        "hfiles",
        "help-files",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["output-file-name"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["auto-output-file-name"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["loop-back-to-musicxml"] = subGroup;
}

void xml2lyRegularOahHandler::createPartsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Parts group",
        "hparts-grp",
        "help-parts-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Parts",
        "hparts",
        "help-parts",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["msr-omit-part-id"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["msr-omit-part-name"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["msr-keep-part-id"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["msr-keep-part-name"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["msr-rename-part"] = subGroup;
}

void xml2lyRegularOahHandler::createStavesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Staves group",
        "hstaves-grp",
        "help-staves-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Staves",
        "hstaves",
        "help-staves",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["create-voices-staff-relative-numbers"] = subGroup;
}

void xml2lyRegularOahHandler::createVoicesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Voices group",
        "hvoices-grp",
        "help-voices-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Voices",
        "hvoices",
        "help-voices",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["create-single-line-staves-as-rythmic"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["connect-arpeggios"] = subGroup;
}

void xml2lyRegularOahHandler::createTranspositionsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Transpositions group",
        "htransp-grp",
        "help-transpositions-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Transpositions",
        "hetransp",
        "help-transpositions",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["lpsr-transpose"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["lilypond-transpose-part"] = subGroup;
}

void xml2lyRegularOahHandler::createClefsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Clefs group",
        "hlefs-grp",
        "help-clefs-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Clefs",
        "hclefs",
        "help-clefs",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["msr-replace-clef"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["ignore-redundant-clefs"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["comment-clef-changes"] = subGroup;
}

void xml2lyRegularOahHandler::createKeysRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Keys group",
        "hkeys-grp",
        "help-keys-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Keys",
        "hkeys",
        "help-keys",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

//  fAtomNamesToRegularSubGroupsMap ["msr-replace-keys"] = subGroup; JMI
  fAtomNamesToRegularSubGroupsMap ["ignore-redundant-keys"] = subGroup;
}

void xml2lyRegularOahHandler::createTimesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Times group",
        "htimes-grp",
        "help-times-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Times",
        "htimes",
        "help-times",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["ignore-redundant-times"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["numerical-time"] = subGroup;
}

void xml2lyRegularOahHandler::createRepeatsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Repeats group",
        "hreps-grp",
        "help-repeats-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Repeats",
        "hreps",
        "help-repeats",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["create-implicit-initial-repeat-barline"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["keep-repeat-barlines"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["repeat-brackets"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["ignore-repeat-numbers"] = subGroup;
}

void xml2lyRegularOahHandler::createMeasuresRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Measures group",
        "hmeas-grp",
        "help-measures-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Measures",
        "hmeas",
        "help-measures",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["add-empty-measures"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["replicate-empty-measure"] = subGroup;
}

void xml2lyRegularOahHandler::createRestsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Rests group",
        "hrests-grp",
        "help-rests-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Rests",
        "hrests",
        "help-rests",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["delay-rests"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["delay-rests-slashes"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["compress-full-measure-rests"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["merge-rests"] = subGroup;
}

void xml2lyRegularOahHandler::createNotesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Notes group",
        "hnotes-grp",
        "help-notes-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Notes",
        "hnotes",
        "help-notes",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["msr-pitches-language"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["absolute"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["relative"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["fixed"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["white-note-heads"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["accidental-style"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["no-auto-beaming"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["no-beams"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["stems"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["delay-rests-beams"] = subGroup; // JMI
}

void xml2lyRegularOahHandler::creatBeamsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Beams group",
        "hbeams-grp",
        "help-beams-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Beams",
        "hbeams",
        "help-beams",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["no-auto-beaming"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["no-beams"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["delay-rests-beams"] = subGroup; // JMI
}

void xml2lyRegularOahHandler::createArticulationsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Articulations group",
        "harts-grp",
        "help-articulations-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Articulations",
        "harts",
        "help-articulations",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-articulations"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["delay-rests-pedals"] = subGroup;
}

void xml2lyRegularOahHandler::createOrnamentsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ornaments group",
        "horns-grp",
        "help-ornaments-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ornaments",
        "horns",
        "help-ornaments",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-ornaments"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["delayed-ornaments-fraction"] = subGroup;
}

void xml2lyRegularOahHandler::createGraceNotesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Grace notes group",
        "hgraces-grp",
        "help-grace-notes-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Grace notes",
        "hgraces",
        "help-grace-notes",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["slash-all-grace-notes"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["slur-all-grace-notes"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["beam-all-grace-notes"] = subGroup;
}

void xml2lyRegularOahHandler::createChordsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Chords group",
        "hchords-grp",
        "help-chords-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Chords",
        "hchords",
        "help-chords",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["trace-chords"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["add-words-from-the-chords"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["lpsr-pitches-language"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["lpsr-chords-language"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["chords-display"] = subGroup;
}

void xml2lyRegularOahHandler::createTiesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ties group",
        "hties-grp",
        "help-ties-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ties",
        "hties",
        "help-ties",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-ties"] = subGroup;
}

void xml2lyRegularOahHandler::createSlursRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Slurs group",
        "hslurs-grp",
        "help-slurs-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Slurs",
        "hslurs",
        "help-slurs",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-slurs"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["delay-slurs"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["delay-rests-slurs"] = subGroup;
}

void xml2lyRegularOahHandler::createLigaturesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ligatures group",
        "hligs-grp",
        "help-ligatures-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ligatures",
        "hligs",
        "help-ligatures",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["delay-rests-ligatures"] = subGroup;
}

void xml2lyRegularOahHandler::createDynamicsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Dynamics group",
        "hdyns-grp",
        "help-dynamics-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Dynamics",
        "hdyns",
        "help-dynamics",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-dynamics"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["all-dynamics-below"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["delay-dynamics"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["delay-rests-dynamics"] = subGroup;
}

void xml2lyRegularOahHandler::createWedgesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Wedges group",
        "hweds-grp",
        "help-wedges-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Wedges",
        "hweds",
        "help-wedges",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-wedges"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["all-wedges-below"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["delay-rests-wedges"] = subGroup;
}

void xml2lyRegularOahHandler::createTupletsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tuplets group",
        "htups-grp",
        "help-tuplets-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tuplets",
        "htups",
        "help-tuplets",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["indent-tuplets"] = subGroup;
//  fAtomNamesToRegularSubGroupsMap ["add-words-from-the-tuplets"] = subGroup; JMI

  fAtomNamesToRegularSubGroupsMap ["lpsr-tuplets-language"] = subGroup;
}

void xml2lyRegularOahHandler::createStringsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Strings group",
        "hstrings-grp",
        "help-strings-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Strings",
        "hstrings",
        "",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["roman-string-numbers"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["avoid-open-strings"] = subGroup;
}

void xml2lyRegularOahHandler::createTablaturesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Tablatures group",
        "htabs-grp",
        "help-tablatures-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Tablatures",
        "htabs",
        "help-tablatures",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["tabFullNotation"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["modernTab"] = subGroup;
}

void xml2lyRegularOahHandler::createHarmoniesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Harmonies group",
        "hharms-grp",
        "help-harmonies-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Harmonies",
        "hharms",
        "help-harmonies",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-harmonies"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["show-harmony-voices"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["harmonies-display"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["jazz-harmonies-display"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["show-harmonies-structures"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["show-all-harmonies-contents"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["show-harmony-details"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["show-harmony-analysis"] = subGroup;
}

void xml2lyRegularOahHandler::createFiguredBassesRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Figured basses group",
        "hfigbass-grp",
        "help-figured-basses-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Figured basses",
        "hfigbass",
        "help-figured-basses",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-figured-basses"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["show-figured-bass-voices"] = subGroup;
}

void xml2lyRegularOahHandler::createLyricsRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Lyrics group",
        "hlyrics-grp",
        "help-lyrics-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Lyrics",
        "hlyrics",
        "help-lyrics",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["omit-lyrics"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["add-words-from-the-lyrics"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["no-lilypond-lyrics"] =subGroup;
  fAtomNamesToRegularSubGroupsMap ["lyrics-durations"] =subGroup;
  fAtomNamesToRegularSubGroupsMap ["add-stanzas-numbers-to-lyrics"] =subGroup;
  fAtomNamesToRegularSubGroupsMap ["lyrics-durations"] =subGroup;
}

void xml2lyRegularOahHandler::createHeaderRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Header group",
        "hheader-grp",
        "help-header-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Header",
        "hheader",
        "help-header",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["rights"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["composer"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["arranger"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["lyricist"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["poet"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["software"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["dedication"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["piece"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["opus"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["title"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["subtitle"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["subsubtitle"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["instrument"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["meter"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["copyright"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["tagline"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["use-lyricists-as-poets"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["use-filename-as-work-title"] = subGroup;
}

void xml2lyRegularOahHandler::createPaperRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Paper group",
        "h-grp",
        "help-paper-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Paper",
        "hpaper",
        "help-paper",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["paper-height"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["paper-width"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["ragged-bottom"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["ragged-last"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["ragged-last-bottom"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["ragged-right"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["horizontal-shift"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["indent"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["short-indent"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["markup-system-spacing.padding"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["page-count"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["system-count"] = subGroup;
}

void xml2lyRegularOahHandler::createLayoutRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Layout group",
        "hlayout-grp",
        "help-layout-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Layout",
        "hlayout",
        "help-layout",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["global-staff-size"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["keep-staff-size"] = subGroup;
}

void xml2lyRegularOahHandler::createOutputRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "Ouptut group",
        "houptut-grp",
        "help-ouptut-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Ouptut",
        "houptut",
        "help-ouptut",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["xml2ly-infos"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["no-lilypond-code"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["draft-mode"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["point-and-clic-off"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["lilypond-version"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["lilypond-compile-date"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["lilypond-comments"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["msr2lpsr-score-output-kind"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["lpsr-Languages-language"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["global"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["ambitus"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["input-line-numbers"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["positions-in-measures"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["all-bar-numbers"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["all-measure-numbers"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["generate-box-around-bar-number"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["reset-measure-number"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["original-measure-numbers"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["separator-line-every-n-measures"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["ignore-musicxml-line-breaks"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["ignore-musixcml-page-breaks"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["break-lines-at-incomplete-right-measures"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["custos"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["length-unit"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["all-durations"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["ledger-lines-color"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["jazz-fonts"] = subGroup;

  fAtomNamesToRegularSubGroupsMap ["jianpu"] = subGroup;
}

void xml2lyRegularOahHandler::createMidiRegularGroup ()
{
  // group

  S_oahGroup
    group =
      oahGroup::create (
        "MIDI group",
        "hmidi-grp",
        "help-midi-grp",
        "",
        kElementVisibilityWhole,
        this);
  group->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (group);

  // subgroup

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "MIDI",
        "hmidi",
        "help-midi",
        "",
        kElementVisibilityWhole,
        group);
  group->
    appendSubGroupToGroup (subGroup);

  // atoms

  fAtomNamesToRegularSubGroupsMap ["no-midi"] = subGroup;
  fAtomNamesToRegularSubGroupsMap ["midi-tempo"] = subGroup;
}

void xml2lyRegularOahHandler::populateRegularViewHandlerFromInsiderHandler (
  S_xml2lyInsiderOahHandler insiderHandler)
{
/*
  The relative order of atoms found in the 'insider' view
  is retained in the 'regular' view
*/

#ifdef TRACE_OAH
#ifdef OAH_TRACE
  gLogOstream <<
    "Populating the regular view handler " <<
    fHandlerHeader <<
    "\" from insider view handler \"" <<
    insiderHandler->getHandlerHeader () <<
    "\"" <<
    endl;
#endif
#endif

  // create the 'put aside' group
  fPutAsideInRegularViewGroup =
    oahGroup::create (
      "Put aside group",
      "hput-aside-group",
      "",
      "",
      kElementVisibilityNone,
      this);
  fPutAsideInRegularViewGroup->
    setGroupHeaderIsToBeWritten (false);
  appendGroupToHandler (fPutAsideInRegularViewGroup);

  // create the 'put aside' subgroup
  fPutAsideInRegularViewSubGroup =
    oahSubGroup::create (
      "Put aside",
      "hput-aside",
      "",
      "",
      kElementVisibilityWhole,
      fPutAsideInRegularViewGroup);
  fPutAsideInRegularViewGroup->
    appendSubGroupToGroup (fPutAsideInRegularViewSubGroup);

  // get the insider OAH handler groups list
  const list<S_oahGroup>
    insiderHandlerGroupsList =
      insiderHandler->
        getHandlerGroupsList ();

  // handle it
  if (insiderHandlerGroupsList.size ()) {
    list<S_oahGroup>::const_iterator
      iBegin = insiderHandlerGroupsList.begin (),
      iEnd   = insiderHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // handle the group
      S_oahGroup group = (*i);

      const list<S_oahSubGroup>
        groupSubGroupsList =
          group->
            getSubGroupsList ();

      if (groupSubGroupsList.size ()) {
        list<S_oahSubGroup>::const_iterator
          iBegin = groupSubGroupsList.begin (),
          iEnd   = groupSubGroupsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_oahSubGroup subGroup = (*i);

          // handle the subgroup
          handleSubGroupMapping (subGroup);

          if (++i == iEnd) break;
        } // for
      }

      if (++i == iEnd) break;
    } // for
  }

  // have all the mappings been used?
  checkMappingsUse ();

#ifdef TRACE_OAH
#ifdef OAH_TRACE
  print (gLogOstream);

  gLogOstream <<
    endl << endl <<
    "*******************************************************************" <<
    endl << endl;

/ * JMI
  print (gLogOstream);

  this->
    print (
      gLogOstream);

  gLogOstream <<
    endl << endl <<
    "*******************************************************************" <<
    endl << endl;
* /
#endif
#endif
}

void xml2lyRegularOahHandler::checkMappingsUse ()
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    // print the known prefixes
    printKnownPrefixes (gLogOstream);
  }
#endif

  // check subgroups mapping
  int subGroupNamesToRegularGroupsMapSize =
    fSubGroupNamesToRegularGroupsMap.size ();

  if (subGroupNamesToRegularGroupsMapSize) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      stringstream s;

      s <<
        "The following " <<
        singularOrPlural (
          subGroupNamesToRegularGroupsMapSize, "subgroup name", "subgroup names") <<
        " could not be mapped to regular groups: ";

      map<string, S_oahGroup>::const_iterator
        iBegin = fSubGroupNamesToRegularGroupsMap.begin (),
        iEnd   = fSubGroupNamesToRegularGroupsMap.end (),
        i      = iBegin;
      for ( ; ; ) {
        // handle the name
        s <<
           "\"" << (*i).first << "\"";

        if (++i == iEnd) break;
        s << ", ";
      } // for

      gLogOstream << s.str () << endl;
  //    oahError (s.str ());
    }
#endif
  }

  // check atoms mapping
  int atomNamesToRegularGroupsMapSize =
    fAtomNamesToRegularSubGroupsMap.size ();

  if (atomNamesToRegularGroupsMapSize) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      stringstream s;

      s <<
        "The following " <<
        singularOrPlural (
          atomNamesToRegularGroupsMapSize, "atom name", "atom names") <<
        " could not be mapped to regular subgroups: ";

      map<string, S_oahSubGroup>::const_iterator
        iBegin = fAtomNamesToRegularSubGroupsMap.begin (),
        iEnd   = fAtomNamesToRegularSubGroupsMap.end (),
        i      = iBegin;
      for ( ; ; ) {
        // handle the name
        s <<
           "\"" << (*i).first << "\"";

        if (++i == iEnd) break;
        s << ", ";
      } // for

      gLogOstream << s.str () << endl;
    //    oahError (s.str ());
    }
#endif
  }

  // display the atoms that have been put aside
  int putAsideInRegularViewAtomNamesListSize =
    fPutAsideInRegularViewAtomNamesList.size ();

  if (putAsideInRegularViewAtomNamesListSize) {
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      stringstream s;

      // build the list of atom names to be output
      list<string> atomNamesToBeOutputList;

      for (
        list<string>::const_iterator i = fPutAsideInRegularViewAtomNamesList.begin ();
        i != fPutAsideInRegularViewAtomNamesList.end ();
        i++
      ) {
        // handle the atom name
        string atomName = (*i);

#ifdef OAH_TRACE
        bool
          shouldTheAtomBeOutput =
            false
              ? true
              : atomName.size ()
                  &&
                atomName.find ("trace") == string::npos
                  &&
                atomName.find ("help") == string::npos
                  &&
                atomName.find ("display") == string::npos
                  &&
                atomName.find ("_") == string::npos;

        if (shouldTheAtomBeOutput) {
          atomNamesToBeOutputList.push_back (atomName);
        }
#endif
      } // for

      // output the retained atom names if any
      int atomNamesToBeOutputListSize =
        atomNamesToBeOutputList.size ();

      if (atomNamesToBeOutputListSize) {
        s <<
          "The following non-trace nor non-display " <<
          singularOrPlural (
            atomNamesToBeOutputListSize, "atom name", "atom names") <<
          " have been left out of the mapping to regular subgroups:" <<
          endl;

        // sort the list for legibility
        atomNamesToBeOutputList.sort ();

        list<string>::const_iterator
          iBegin = atomNamesToBeOutputList.begin (),
          iEnd   = atomNamesToBeOutputList.end (),
          i      = iBegin;
        for ( ; ; ) {
          // handle the name
          string theString = (*i);

          s <<
             "\"" << theString << "\"";

          if (++i == iEnd) break;
          s << ", ";
        } // for

        gLogOstream << s.str () << endl;
      }
    }
#endif
  }
}

void xml2lyRegularOahHandler::handleSubGroupMapping (S_oahSubGroup subGroup)
{
  string subGroupShortName = subGroup->getShortName ();
  string subGroupLongName  = subGroup->getLongName ();

  string
    subGroupNameToUse =
      subGroupLongName.size ()
        ? subGroupLongName
        : subGroupShortName;

 // gLogOstream << "subGroupNameToUse = " << subGroupNameToUse << endl;

  // is nameToUse known in fSubGroupNamesToRegularGroupsMap?
  map<string, S_oahGroup>::const_iterator
    it =
      fSubGroupNamesToRegularGroupsMap.find (
        subGroupNameToUse);

  if (it != fSubGroupNamesToRegularGroupsMap.end ()) {
    // subGroupNameToUse is known in the map
    S_oahGroup group = (*it).second;

    // append subgroup to regular group
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
      gLogOstream <<
        "+++ appending subgroup \"" <<
        subGroupNameToUse <<
        "\" to group \"" <<
        group->getGroupHeader () <<
        "\"" <<
        endl;
    }
#endif

    group->
      appendSubGroupToGroup (subGroup);

    // remove subGroupNameToUse from the map
    it = fSubGroupNamesToRegularGroupsMap.erase (it);
  }

  else {
    // subGroupNameToUse is not known in the map

    // are there atoms from this subgroup in fAtomNamesToRegularSubGroupsMap?
    const list<S_oahAtom>&
      subGroupAtomsList =
        subGroup->
          getAtomsList ();

    if (subGroupAtomsList.size ()) {
      list<S_oahAtom>::const_iterator
        iBegin = subGroupAtomsList.begin (),
        iEnd   = subGroupAtomsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_oahAtom atom = (*i);

        // handle the atom
        handleAtomMapping (atom);

        if (++i == iEnd) break;
      } // for
    }
  }
}

void xml2lyRegularOahHandler::handleAtomMapping (S_oahAtom atom)
{
  string atomShortName = atom->getShortName ();
  string atomLongName  = atom->getLongName ();

  string
    atomNameToUse =
      atomLongName.size ()
        ? atomLongName
        : atomShortName;

  // gLogOstream << "atomNameToUse = " << atomNameToUse << endl;

  // is atomNameToUse known in fAtomNamesToRegularSubGroupsMap?
  map<string, S_oahSubGroup>::const_iterator
    it =
      fAtomNamesToRegularSubGroupsMap.find (
        atomNameToUse);

  if (it != fAtomNamesToRegularSubGroupsMap.end ()) {
    // atomNameToUse is known in the map
    S_oahSubGroup subGroup = (*it).second;

    // append atom to regular subgroup
#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
      gLogOstream <<
        "+++ appending atom \"" <<
        atomNameToUse <<
        "\" to subgroup and registering it in the 'regular' handler \"" <<
        subGroup->getSubGroupHeader () <<
        "\"" <<
        endl;
    }
#endif

    subGroup->
      appendAtomToSubGroup (atom);

    atom->
      registerAtomInHandler (this);

    // remove atomNameToUse from the map
    it = fAtomNamesToRegularSubGroupsMap.erase (it);
  }
  else {
    // atomNameToUse is not known in the map
    // place it in the 'put aside' regular subgroup

#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
      gLogOstream <<
        "+++ appending atom \"" <<
        atomNameToUse <<
        "\" to subgroup \"" <<
        fPutAsideInRegularViewSubGroup->getSubGroupHeader () <<
        "\"" <<
        endl;
    }
#endif

    fPutAsideInRegularViewSubGroup->
      appendAtomToSubGroup (atom);
    fPutAsideInRegularViewAtomNamesList.push_back (
      atomNameToUse);
  }
}

void xml2lyRegularOahHandler::checkOptionsAndArguments ()
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOahDetails ()) {
    gOutputOstream <<
      "xml2lyRegularOahHandler::checkOptionsAndArguments() " <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    unsigned int argumentsNumber =
      fHandlerArgumentsVector.size ();

#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
      if (argumentsNumber > 0) {
        fHandlerLogOstream <<
          singularOrPluralWithoutNumber (
            argumentsNumber, "There is", "There are") <<
          " " <<
          argumentsNumber <<
          " " <<
          singularOrPluralWithoutNumber (
            argumentsNumber, "argument", "arguments") <<
          " in handlerArgumentsVector for " <<
          gGlobalOahOahGroup->fHandlerExecutableName <<
          ":" <<
          endl;

        gIndenter++;

        for (unsigned int i = 0; i < argumentsNumber; i++) {
          fHandlerLogOstream <<
            i << " : " << fHandlerArgumentsVector [i] <<
              endl;
        } // for

        fHandlerLogOstream << endl;

        gIndenter--;
      }
      else {
        fHandlerLogOstream <<
          "There are no arguments to " <<
          gGlobalOahOahGroup->fHandlerExecutableName <<
          endl;
      }
    }
#endif

    // input source name
    // ------------------------------------------------------

    switch (argumentsNumber) {
      case 0:
        {
          if (! fOahHandlerFoundAHelpOption) {
            string message =
              "Input file name or '-' for standard input expected";

            fHandlerLogOstream <<
              endl <<
              message <<
              endl <<
              endl;

            throw msrOahException (message);
          }
        }
        break;

      case 1:
        // register intput file name
        gGlobalOahOahGroup->fInputSourceName =
          fHandlerArgumentsVector [0];
        break;

      default:
        fHandlerLogOstream <<
          endl <<
          "Several input file names supplied, only the first one, \"" <<
          fHandlerArgumentsVector [0] <<
          "\", will be translated" <<
          endl <<
          endl;

        // register intput file name
        gGlobalOahOahGroup->fInputSourceName =
          fHandlerArgumentsVector [0];
        break;
    } //  switch

    // build potential output file name
    // ------------------------------------------------------

    string
      inputSourceName =
        gGlobalOahOahGroup->fInputSourceName;

    string potentialOutputFileName;

    if (inputSourceName != "-") {
      // determine potential output file name,
      // may be set differently by '--o, --outputFileName' option
      potentialOutputFileName =
        baseName (inputSourceName);

      // set '.ly' suffix
      size_t
        posInString =
          potentialOutputFileName.rfind ('.');

      if (posInString != string::npos) {
        potentialOutputFileName.replace (
          posInString,
          potentialOutputFileName.size () - posInString,
          ".ly");
      }
    }

    // check auto output file option usage
    // ------------------------------------------------------

    S_oahStringAtom
      outputFileNameStringAtom =
        gGlobalXml2lyInsiderOahGroup->
          getOutputFileNameStringAtom ();

    S_oahBooleanAtom
      autoOutputFileNameAtom =
        gGlobalXml2lyInsiderOahGroup->
          getAutoOutputFileNameAtom ();

#ifdef TRACE_OAH
    if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
      // print the options handler initial state
      fHandlerLogOstream <<
        "xml2lyRegularOahHandler::checkOptionsAndArguments(): " <<
        endl;

      gIndenter++;

      fHandlerLogOstream <<
        "outputFileNameStringAtom:" <<
        endl;

      gIndenter++;
      fHandlerLogOstream <<
        outputFileNameStringAtom <<
        endl;
      gIndenter--;

      fHandlerLogOstream <<
        "autoOutputFileNameAtom:" <<
          endl;

      gIndenter++;
      fHandlerLogOstream <<
        autoOutputFileNameAtom <<
        endl;
      gIndenter--;

      gIndenter--;
    }
#endif

    if (autoOutputFileNameAtom->getVariableHasBeenSet ()) {
      // '-aofn, -auto-output-file-name' has been chosen
      // ---------------------------------------------------

      if (inputSourceName == "-") {
        stringstream s;

        s <<
          "option '-aofn, -auto-output-file-name'" <<
          " cannot be used when reading from standard input";

        oahError (s.str ());
      }

      else {
        if (outputFileNameStringAtom->getVariableHasBeenSet ()) {
          // '-aofn, -auto-output-file-name' has been chosen
          // '-o, -output-file-name' has been chosen

#ifdef TRACE_OAH
          if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
            fHandlerLogOstream <<
              "'-aofn, -auto-output-file-name' has been chosen" <<
              endl <<
              "'-o, -output-file-name' has been chosen" <<
              endl;
          }
#endif

          stringstream s;

          s <<
            "options '-aofn, -auto-output-file-name' and '-o, -output-file-name'"  <<
            endl <<
            "cannot be chosen simultaneously" <<
            " (lilyPondOutputFileName: \"" <<
            outputFileNameStringAtom->getStringVariable () <<
            "\")";

          oahError (s.str ());
        }

        else {
          // '-aofn, -auto-output-file-name' has been chosen
          // '-o, -output-file-name' has NOT been chosen

#ifdef TRACE_OAH
          if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
            fHandlerLogOstream <<
              "'-aofn, -auto-output-file-name' has been chosen" <<
              endl <<
              "'-o, -output-file-name' has NOT been chosen" <<
              endl <<
              "====> output goes to \"" <<
              potentialOutputFileName <<
              "\"" <<
              endl;
          }
#endif

          outputFileNameStringAtom->
            setStringVariable (
              potentialOutputFileName);
        }
      }
    }

    else {
      // '-aofn, -auto-output-file-name' has NOT been chosen
      // ---------------------------------------------------

      if (outputFileNameStringAtom->getVariableHasBeenSet ()) {
        // '-aofn, -auto-output-file-name' has NOT been chosen
        // '-o, -output-file-name' has been chosen

#ifdef TRACE_OAH
        if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
          fHandlerLogOstream <<
            "'-aofn, -auto-output-file-name' has NOT been chosen" <<
            endl <<
            "'-o, -output-file-name' has been chosen" <<
            endl <<
            "====> output goes to \"" <<
            outputFileNameStringAtom->getStringVariable () <<
            "\"" <<
            endl;
        }
#endif
      }

      else {
        // '-aofn, -auto-output-file-name' has NOT been chosen
        // '-o, -output-file-name' has NOT been chosen

#ifdef TRACE_OAH
        if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
          fHandlerLogOstream <<
            "'-aofn, -auto-output-file-name' has NOT been chosen" <<
            endl <<
            "'-o, -output-file-name' has NOT been chosen" <<
            endl <<
            "====> output goes to standard output" <<
            endl;
        }
#endif
      }
    }

// JMI    pThisMethodHasBeenRun = true;
  }
}

//______________________________________________________________________________
void xml2lyRegularOahHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2lyRegularOahHandler::enforceHandlerQuietness ()
{
#ifdef TRACE_OAH
  gGlobalGeneralOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalGeneralOahGroup->
    enforceGroupQuietness ();

  gGlobalMxmlTreeOah->
    enforceGroupQuietness ();

  gGlobalMsrOah->
    enforceGroupQuietness ();

  gGlobalLpsrOah->
    enforceGroupQuietness ();

  gGlobalLpsr2LilypondOah->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH
  gGlobalExtraOah->
    enforceGroupQuietness ();
#endif

  gGlobalXml2lyInsiderOahGroup->
    enforceGroupQuietness ();
}

//______________________________________________________________________________
void xml2lyRegularOahHandler::print (ostream& os) const
{
  const int fieldWidth = 27;

  os <<
    "xml2lyRegularOahHandler '" << fHandlerHeader << "':" <<
    endl;

  gIndenter++;

  os << left <<
    setw (fieldWidth) <<
    "fShortName" << " : " << fShortName <<
    endl <<
    setw (fieldWidth) <<
    "fLongName" << " : " << fLongName <<
    endl <<
    setw (fieldWidth) <<
    "fDescription" << " : " << fDescription <<
    endl <<
    endl;

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

if (false) { // JMI
  // print the known options
  os <<
    "xml2lyRegularOahHandler known options +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" <<
    endl <<
    "xml2lyRegularOahHandler known options +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" <<
    endl;
  printKnownOptions (os);
}

  gIndenter--;

  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2lyRegularOahHandler& elt)
{
  elt->print (os);
  return os;
}

/* JMI
//______________________________________________________________________________
void xml2lyRegularOahGroup::enforceGroupQuietness ()
{}

void xml2lyRegularOahGroup::checkGroupOptionsConsistency ()
{}

void xml2lyRegularOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyRegularOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyRegularOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2lyRegularOahGroup>*> (v)) {
        S_xml2lyRegularOahGroup elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyRegularOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyRegularOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyRegularOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyRegularOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2lyRegularOahGroup>*> (v)) {
        S_xml2lyRegularOahGroup elem = this;

#ifdef TRACE_OAH
        if (gGlobalOahOahGroup->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyRegularOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyRegularOahGroup::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalOahOahGroup->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyRegularOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//_______________________________________________________________________________
// JMI S_xml2lyRegularOahGroup gGlobalXml2lyRegularOahGroup;

S_xml2lyRegularOahGroup xml2lyRegularOahGroup::create (
  S_oahHandler handlerUpLink)
{
  xml2lyRegularOahGroup* o = new xml2lyRegularOahGroup (
    handlerUpLink);
  assert(o!=0);

  return o;
}

xml2lyRegularOahGroup::xml2lyRegularOahGroup (
  S_oahHandler handlerUpLink)
  : oahGroup (
    "xml2ly",
    "hxl", "help-xml2ly",
R"(Options that are used by xml2ly are grouped here.)",
    kElementVisibilityWhole,
    handlerUpLink)
{
  // append this options group to the options handler
  // if relevant
  if (handlerUpLink) {
    handlerUpLink->
      prependGroupToHandler (this);
  }

  // initialize it
  initializeXml2lyRegularOahGroup ();
}

xml2lyRegularOahGroup::~xml2lyRegularOahGroup ()
{}

void xml2lyRegularOahGroup::initializeXml2lyRegularOahGroup ()
{
  // version
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Version",
          "hxv", "help-xml2ly-version",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // version

    subGroup->
      appendAtomToSubGroup (
        xml2lyVersionOahAtom::create (
          "v", "version",
R"(Display xml2ly's version number and history.)"));
  }

  // OAH view
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "OAH view",
          "ov", "help-oah-view",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // the 'insider' option

    subGroup->
      appendAtomToSubGroup (
        xml2lyInsiderOahAtom::create (
          "insider", "",
R"(Use the 'insider' view for the options and help,
in which the options are grouped as they are used by the various
internal representations and translation passes.
This unleashes the full set of display and trace options.)"));

    // the 'regular' option

    subGroup->
      appendAtomToSubGroup (
        xml2lyRegularOahAtom::create (
          "regular", "",
R"(Use the 'regular' view for the options and help,
in which the options are grouped by topics
such a slurs, tuplets and figured bass.
This is the default.)"));
  }

  // about
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "About",
          "hxa", "help-xml2ly-about",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // about

    subGroup->
      appendAtomToSubGroup (
        xml2lyAboutOahAtom::create (
          "a", "about",
R"(Display information about xml2ly.)"));
  }

  // contact
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Contact",
          "hxc", "help-xml2ly-contact",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // contact

    subGroup->
      appendAtomToSubGroup (
        xml2lyContactOahAtom::create (
          "c", "contact",
R"(Display information about how to contacct xml2ly maintainers.)"));
  }

  // output file
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Output file",
          "hxof", "help-xml2ly-output-file",
R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // output filename

    fOutputFileNameStringAtom =
      oahStringAtom::create (
        "o", "output-file-name",
R"(Write LilyPond code to file FILENAME instead of standard output.)",
        "FILENAME",
        "lilyPondOutputFileName",
        fLilyPondOutputFileName);

    subGroup->
      appendAtomToSubGroup (
        fOutputFileNameStringAtom);

    // auto output filename

    fAutoOutputFileName = false;

    fAutoOutputFileNameAtom =
      oahBooleanAtom::create (
        "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write LilyPond code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'ly'
or adding '.ly' if none is present.)",
        "autoOutputFileName",
        fAutoOutputFileName);

    subGroup->
      appendAtomToSubGroup (
        fAutoOutputFileNameAtom);
  }

  // loop back to MusicXML
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Loop",
          "hxml2lylo", "help-xml2ly-loopback-options",
  R"()",
          kElementVisibilityWhole,
          this);

    appendSubGroupToGroup (subGroup);

    // loop

    fLoopBackToMusicXML = false;

    S_oahBooleanAtom
      loopOptionsBooleanAtom =
        oahBooleanAtom::create (
          "loop", "loop-back-to-musicxml",
  R"(Close the loop, generating a MusicXML file from the MSR.
  The file name receives a '_LOOP.xml' suffix.
  This is equivalent to using xml2xml)",
          "loopBackToMusicXML",
          fLoopBackToMusicXML);
    // make this atom imvisible
    loopOptionsBooleanAtom->
      setElementVisibilityKind (
        kElementVisibilityNone);

    subGroup->
      appendAtomToSubGroup (
        loopOptionsBooleanAtom);
  }

  // exit after some passes
  // --------------------------------------

  {
    S_oahSubGroup
      subGroup =
        oahSubGroup::create (
          "Exit after some passes",
          "hmexit", "help-msr-exit",
  R"()",
        kElementVisibilityWhole,
        this);

    appendSubGroupToGroup (subGroup);

    // exit after pass 2a

    fExit2a = false;

    S_oahBooleanAtom
      exit2aOahBooleanAtom =
        oahBooleanAtom::create (
          "e2a", "exit-2a",
  R"(Exit after pass 2a, i.e. after conversion
  of the MusicXML tree to an MSR skeleton.)",
          "exit2a",
          fExit2a);

    subGroup->
      appendAtomToSubGroup (
        exit2aOahBooleanAtom);

    // exit after pass 2b

    fExit2b = false;

    S_oahBooleanAtom
      exit2bOahBooleanAtom =
        oahBooleanAtom::create (
          "e2b", "exit-2b",
  R"(Exit after pass 2b, i.e. after conversion
  of the MusicXML tree to MSR.)",
          "exit2b",
          fExit2b);

    subGroup->
      appendAtomToSubGroup (
        exit2bOahBooleanAtom);
  }
}

//______________________________________________________________________________
void xml2lyRegularOahGroup::printXml2lyRegularOahGroupValues (int fieldWidth)
{
  gLogOstream <<
    "The xml2ly options are:" <<
    endl;

  gIndenter++;

  // output file
  // --------------------------------------

  gLogOstream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "lilyPondOutputFileName" << " : \"" <<
    fLilyPondOutputFileName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "autoOutputFileName" << " : \"" <<
    booleanAsString (fAutoOutputFileName) <<
    "\"" <<
    endl;

  gIndenter--;

  // loop back to MusicXML
  // --------------------------------------

  gLogOstream <<
    "Loop:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "loopToMusicXML" << " : " <<
    booleanAsString (fLoopBackToMusicXML) <<
    endl;

  gIndenter--;

  // exit after some passes
  // --------------------------------------

  gLogOstream <<
    "Exit after some passes:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "exit2a" << " : " <<
    booleanAsString (fExit2a) <<
    endl <<
    setw (fieldWidth) << "exit2b" << " : " <<
    booleanAsString (fExit2b) <<
    endl;

  gIndenter--;

  gIndenter--;
}

//______________________________________________________________________________
void initializeXml2lyRegularOahHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->fQuiet) {
    gLogOstream <<
      "Initializing xml2ly options handling" <<
      endl;
  }
#endif

  // enlist versions information
  // ------------------------------------------------------

  enlistVersion (
    "Initial", "early 2016",
    "Start as xml2lilypond, a clone of xml2guido");

  enlistVersion (
    musicxml2lilypondVersionStr (), "October 2018",
    "First draft version");

  // create the options variables
  // ------------------------------------------------------

  gGlobalXml2lyRegularOahGroup = xml2lyRegularOahGroup::create (
    handler);
  assert (gGlobalXml2lyRegularOahGroup != 0);
}
*/

}
