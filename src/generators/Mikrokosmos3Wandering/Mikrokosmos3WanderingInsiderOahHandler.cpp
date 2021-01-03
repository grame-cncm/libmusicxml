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

#include "oahOah.h"

#include "generalOah.h"
#include "extraOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "msr2mxmlTreeOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"

#include "msr2mxmlTreeOah.h"
#include "mxmlTree2msrOah.h"

#include "msr2lpsrOah.h"
#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"
#include "lilypondOah.h"

#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "bsr2brailleOah.h"
#include "brailleOah.h"

#include "version.h"

// JMI #include "Mikrokosmos3WanderingManPageOah.h"

#include "Mikrokosmos3WanderingInsiderOahHandler.h"


using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
string Mikrokosmos3WanderingAboutInformation ()
{
  return
R"(What Mikrokosmos3Wandering does:

    This multi-pass translator basically performs 5 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a first MusicXML tree;
        Pass 2a: converts that MusicXML tree into to
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: converts that tree and the skeleton into a
                 Music Score Representation (MSR);
        Pass 3:  converts the MSR into a second MSR;
        Pass 4:  converts the second MSR into a second MusicXML tree;
        Pass 5:  converts the second MusicXML tree to Guido source code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
S_Mikrokosmos3WanderingInsiderOahHandler Mikrokosmos3WanderingInsiderOahHandler::create (
  string            executableName,
  string            executableAboutInformation,
  string            handlerHeader,
  generatedCodeKind theGeneratedCodeKind)
{
  // create the insider handler
  Mikrokosmos3WanderingInsiderOahHandler* o = new
    Mikrokosmos3WanderingInsiderOahHandler (
      executableName,
      executableAboutInformation,
      handlerHeader,
      theGeneratedCodeKind);
  assert (o!=0);

  return o;
}

Mikrokosmos3WanderingInsiderOahHandler::Mikrokosmos3WanderingInsiderOahHandler (
  string            executableName,
  string            executableAboutInformation,
  string            handlerHeader,
  generatedCodeKind theGeneratedCodeKind)
  : oahHandler (
      executableName,
      executableAboutInformation,
      handlerHeader,
R"(                Welcome to Mikrokosmos3Wandering,
     a generator of Guido, MusicXML, LilyPond or braille music
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: Mikrokosmos3Wandering ([options] | [MusicXMLFile|-])+
)")
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Initializing \"" <<
    fHandlerHeader <<
    "\" regular options handler" <<
    endl;
#endif
#endif

  fGeneratedCodeKind = theGeneratedCodeKind;

  // create the Mikrokosmos3Wandering prefixes
  createTheMikrokosmos3WanderingPrefixes ();

  // create the Mikrokosmos3Wandering option groups
  createTheMikrokosmos3WanderingOptionGroups (executableName);
}

Mikrokosmos3WanderingInsiderOahHandler::~Mikrokosmos3WanderingInsiderOahHandler ()
{}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahHandler::createTheMikrokosmos3WanderingPrefixes ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating the Mikrokosmos3Wandering prefixes in \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahHandler::createTheMikrokosmos3WanderingOptionGroups (
  string executableName)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating the prefixes in \"" <<
    fHandlerHeader <<
    "\" insider option groups" <<
    endl;
#endif
#endif

  // initialize options handling, phase 1
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  // create the trace OAH group
  appendGroupToHandler (
    createGlobalTraceOahGroup ());
#endif

  // create the OAH OAH group
  appendGroupToHandler (
    createGlobalOahOahGroup (
      executableName));

  // create the general OAH group
  appendGroupToHandler (
    createGlobalGeneralOahGroup ());

  // initialize the library
  // ------------------------------------------------------

  initializeMSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the Mikrokosmos3Wandering OAH group
  appendGroupToHandler (
    createGlobalMikrokosmos3WanderingOahGroup ());

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

  // create the msr2msr OAH group
  appendGroupToHandler (
    createGlobalMsr2msrOahGroup ());

  // create the groups needed according to the generated code kind
  switch (fGeneratedCodeKind) {
    case k_NoGeneratedCode:
      // should not occur
      break;

    case kGuido:
      // create the msr2mxmlTree OAH group
      appendGroupToHandler (
        createGlobalMsr2mxmlTreeOahGroup ());

      // create the mxmlTree OAH group
      appendGroupToHandler (
        createGlobalMxmlTreeOahGroup ());
      break;

    case kLilyPond:
      // create the msr2lpsr OAH group
      appendGroupToHandler (
        createGlobalMsr2lpsrOahGroup ());

      // create the LPSR OAH group
      appendGroupToHandler (
        createGlobalLpsrOahGroup ());

      // create the lpsr2lilypond OAH group
      appendGroupToHandler (
        createGlobalLpsr2lilypondOahGroup ());

      // create the LilyPond OAH group
      appendGroupToHandler (
        createGlobalLilypondOahGroup ());
      break;

    case kBrailleMusic:
      // create the msr2bsr OAH group
      appendGroupToHandler (
        createGlobalMsr2bsrOahGroup ());

      // create the BSR OAH group
      appendGroupToHandler (
        createGlobalBsrOahGroup ());

      // create the bsr2braille OAH group
      appendGroupToHandler (
        createGlobalBsr2brailleOahGroup ());

      // create the braille OAH group
      appendGroupToHandler (
        createGlobalBrailleOahGroup ());
      break;

    case kMusicXML:
      // create the msr2mxmlTree OAH group
      appendGroupToHandler (
        createGlobalMsr2mxmlTreeOahGroup ());

      // create the mxmlTree OAH group
      appendGroupToHandler (
        createGlobalMxmlTreeOahGroup ());

      // create the MusicXML OAH group
      appendGroupToHandler (
        createGlobalMusicxmlOahGroup ());
      break;
  } // switch

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalExtraOahGroup ());
#endif
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahHandler::checkOptionsAndArgumentsFromArgcAndArgv () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "checking options and arguments from argc/argv in \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  checkNoInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
string Mikrokosmos3WanderingInsiderOahHandler::fetchOutputFileNameFromTheOptions () const
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Fetching the output file name from the options in OAH handler \"" <<
      fHandlerHeader <<
      "\"" <<
      endl;
  }
#endif

  string result;

  S_oahStringAtom
    outputFileNameStringAtom =
      gGlobalMikrokosmos3WanderingInsiderOahGroup->
        getOutputFileNameStringAtom ();

  S_oahBooleanAtom
    autoOutputFileNameAtom =
      gGlobalMikrokosmos3WanderingInsiderOahGroup->
        getAutoOutputFileNameAtom ();

  bool
    outputFileNameHasBeenSet =
      outputFileNameStringAtom->
        getVariableHasBeenSet ();

  bool
    autoOutputFileNameHasBeenSet =
      autoOutputFileNameAtom->
        getVariableHasBeenSet ();

  if (outputFileNameHasBeenSet) {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has been chosen
      // '-aofn, -auto-output-file-name' has been chosen
      stringstream s;

      s <<
        "options' " <<
        outputFileNameStringAtom->fetchNames () <<
        "' and '" <<
        autoOutputFileNameAtom->fetchNames () <<
        "' cannot be chosen simultaneously" <<
        "\")";

      oahError (s.str ());
    }
    else {
      // '-o, -output-file-name' has been chosen
      // '-aofn, -auto-output-file-name' has NOT been chosen
      result =
        outputFileNameStringAtom->
          getStringVariable ();
    }
  }

  else {
    if (autoOutputFileNameHasBeenSet) {
      // '-o, -output-file-name' has NOT been chosen
      // '-aofn, -auto-output-file-name' has been chosen
      string
        inputSourceName =
          gGlobalOahOahGroup->getInputSourceName ();

      // determine output file base name
      if (inputSourceName == "-") {
        result = "stdin";
      }

      else {
        // determine output file name,
        result =
          baseName (inputSourceName);

        size_t
          posInString =
            result.rfind ('.');

        // remove file extension
        if (posInString != string::npos) {
          result.replace (
            posInString,
            result.size () - posInString,
            "");
        }
      }

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        gLogStream <<
          "Mikrokosmos3WanderingInsiderOahHandler::fetchOutputFileNameFromTheOptions(): result 1 = \"" <<
          result <<
          "\"" <<
          endl;
      }
#endif

      // append the file extension to the output file name
       result += ".gmn";

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        gLogStream <<
          "Mikrokosmos3WanderingInsiderOahHandler::fetchOutputFileNameFromTheOptions(): result 2 = " <<
          result <<
          "\"" <<
          endl;
      }
#endif
    }

    else {
      // '-o, -output-file-name' has NOT been chosen
      // '-aofn, -auto-output-file-name' has NOT been chosen
      // nothing to do
    }
  }

  return result;
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTraceOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalMikrokosmos3WanderingInsiderOahGroup->
    enforceGroupQuietness ();

  gGlobalGeneralOahGroup->
    enforceGroupQuietness ();

  gGlobalMusicxmlOahGroup->
    enforceGroupQuietness ();

  gGlobalMxmlTreeOahGroup->
    enforceGroupQuietness ();

  gGlobalMxmlTree2msrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsrOahGroup->
    enforceGroupQuietness ();

  gGlobalMsr2mxmlTreeOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::checkGroupOptionsConsistency ()
{
/* JMI

  if (inputSourceName.size () > 0 && inputSourceName == outputFileName) {
    stringstream s;

    s <<
      "\"" << inputSourceName << "\" is both the input and output file name";;

    oahError (s.str ());
  }




  if (! fOutputFileName.size ()) {
    stringstream s;

    s <<
      "Mikrokosmos3WanderingInsiderOahGroup: a MusicXML output file name must be chosen with '-o, -output-file-name";

    oahError (s.str ());
  }

  else if (fOutputFileName == gGlobalOahOahGroup->getInputSourceName ()) {
    stringstream s;

    s <<
      "\"" << fOutputFileName << "\" is both the input and output file name";

    oahError (s.str ());
  }
  */
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_Mikrokosmos3WanderingInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingInsiderOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching Mikrokosmos3WanderingInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void Mikrokosmos3WanderingInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_Mikrokosmos3WanderingInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_Mikrokosmos3WanderingInsiderOahGroup>*> (v)) {
        S_Mikrokosmos3WanderingInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching Mikrokosmos3WanderingInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void Mikrokosmos3WanderingInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> Mikrokosmos3WanderingInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "Mikrokosmos3WanderingInsiderOahHandler:" <<
    endl;

  ++gIndenter;

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

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingInsiderOahHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_Mikrokosmos3WanderingInsiderOahGroup gGlobalMikrokosmos3WanderingInsiderOahGroup;

S_Mikrokosmos3WanderingInsiderOahGroup Mikrokosmos3WanderingInsiderOahGroup::create ()
{
  Mikrokosmos3WanderingInsiderOahGroup* o = new Mikrokosmos3WanderingInsiderOahGroup ();
  assert (o!=0);

  return o;
}

Mikrokosmos3WanderingInsiderOahGroup::Mikrokosmos3WanderingInsiderOahGroup ()
  : oahGroup (
    "Mikrokosmos3Wandering",
    "hx2x", "help-Mikrokosmos3Wandering",
R"(Options that are used by Mikrokosmos3Wandering are grouped here.)",
    kElementVisibilityWhole)
{
  initializeMikrokosmos3WanderingInsiderOahGroup ();
}

Mikrokosmos3WanderingInsiderOahGroup::~Mikrokosmos3WanderingInsiderOahGroup ()
{}

//_______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::initializeMikrokosmos3WanderingInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream << left <<
    "Initializing \"" <<
    fGroupHeader <<
    "\" group" <<
    endl;
#endif
#endif

  // Guido
  // --------------------------------------

  createInsiderGuidoSubGroup ();

  // output
  // --------------------------------------

  createInsiderOutputSubGroup ();

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

//_______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::createInsiderGuidoSubGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream << left <<
    "Creating insider output subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
#endif
#endif

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Guido",
        "xguido", "help-guido",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // generate comments

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-comments", "",
  R"(Generate comments in the Guido output.)",
        "generateComments",
        fGenerateComments));

  // generate stem

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-stem", "",
  R"(Generate stem in the Guido output.)",
        "generateStem",
        fGenerateStem));

  // generate bars

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "generate-bars", "",
  R"(Generate barlines in the Guido output.)",
        "generateBars",
        fGenerateBars));
}

//_______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::createInsiderOutputSubGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream << left <<
    "Creating insider output subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
#endif
#endif

  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Output file",
        "hx2xof", "help-Mikrokosmos3Wandering-output-file",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // output filename

  fOutputFileNameStringAtom =
    oahStringAtom::create (
      "o", "output-file-name",
R"(Write Guido code to file FILENAME instead of standard output.)",
      "FILENAME",
      "outputFileName",
      fOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fOutputFileNameStringAtom);

  // auto output filename

  fAutoOutputFileName = false;

  fAutoOutputFileNameAtom =
    oahBooleanAtom::create (
      "aofn", "auto-output-file-name",
R"(This option can only be used when reading from a file.
Write Guido code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'gmn'
or adding '.gmn' if none is present.)",
      "autoOutputFileName",
      fAutoOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fAutoOutputFileNameAtom);
}

//_______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::createInsiderQuitSubGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream << left <<
    "Creating insider quit subgroup in \"" <<
    fGroupHeader <<
    "\"" <<
    endl;
#endif
#endif

  S_oahSubGroup
    quitAfterSomePassesSubGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "hm2xquit", "help-msr2gmn-quit",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (quitAfterSomePassesSubGroup);

  // quit after pass 2a

  fQuitAfterPass2a = false;

  S_oahBooleanAtom
    quit2aOahBooleanAtom =
      oahBooleanAtom::create (
        "qap2a", "quitAfterPass-after-pass2a",
R"(Quit after pass 2a, i.e. after conversion
of the MusicXML tree to an MSR skeleton.)",
        "quitAfterPass2a",
        fQuitAfterPass2a);

  quitAfterSomePassesSubGroup->
    appendAtomToSubGroup (
      quit2aOahBooleanAtom);

  // quit after pass 2b

  fQuitAfterPass2b = false;

  S_oahBooleanAtom
    quit2bOahBooleanAtom =
      oahBooleanAtom::create (
        "qap2b", "quitAfterPass-after-pass2b",
R"(Quit after pass 2b, i.e. after conversion
of the MusicXML tree to MSR.)",
        "quitAfterPass2b",
        fQuitAfterPass2b);

  quitAfterSomePassesSubGroup->
    appendAtomToSubGroup (
      quit2bOahBooleanAtom);
}

//______________________________________________________________________________
void Mikrokosmos3WanderingInsiderOahGroup::printMikrokosmos3WanderingInsiderOahGroupValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The Mikrokosmos3Wandering options are:" <<
    endl;

  ++gIndenter;

  // generated code kind
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Code kind:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) <<
    "generatedCodeKind" << " : " <<
    generatedCodeKindAsString (fGeneratedCodeKind) <<
    endl;

  --gIndenter;

  // Guido
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Guido:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) <<
    "generateComments" << " : " << booleanAsString (fGenerateComments) <<
    endl <<
    setw (fieldWidth) <<
    "generateStem" << " : " << booleanAsString (fGenerateStem) <<
    endl <<
    setw (fieldWidth) <<
    "generateBars" << " : " << booleanAsString (fGenerateBars) <<
    endl;

  --gIndenter;

  // output file
  // --------------------------------------

  gLogStream << left <<
    setw (fieldWidth) << "Output file:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "outputFileName" << " : \"" <<
    fOutputFileName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "autoOutputFileName" << " : \"" <<
    booleanAsString (fAutoOutputFileName) <<
    "\"" <<
    endl;

  --gIndenter;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "quitAfterPass2a" << " : " <<
    booleanAsString (fQuitAfterPass2a) <<
    endl <<
    setw (fieldWidth) << "quitAfterPass2b" << " : " <<
    booleanAsString (fQuitAfterPass2b) <<
    endl;

  --gIndenter;

  --gIndenter;
}

//______________________________________________________________________________
S_Mikrokosmos3WanderingInsiderOahGroup createGlobalMikrokosmos3WanderingInsiderOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global \"Mikrokosmos3Wandering\" insider OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMikrokosmos3WanderingInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalMikrokosmos3WanderingInsiderOahGroup =
      Mikrokosmos3WanderingInsiderOahGroup::create ();
    assert (gGlobalMikrokosmos3WanderingInsiderOahGroup != 0);

    // append versions information to list
    // ------------------------------------------------------

    appendVersionToVersionInfoList (
      "Initial",
      "December 2020",
      "First draft version");
  }

  // return the global OAH group
  return gGlobalMikrokosmos3WanderingInsiderOahGroup;
}


}
