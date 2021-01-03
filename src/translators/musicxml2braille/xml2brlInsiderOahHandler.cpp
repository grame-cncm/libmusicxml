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

#include "enableExtraOahIfDesired.h"
#ifdef EXTRA_OAH_IS_ENABLED
  #include "extraOah.h"
#endif

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "msr.h"
#include "bsr.h"

#include "oahOah.h"

#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"
#include "mxmlTree2msrOah.h"
#include "msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"
#include "bsr2brailleOah.h"
#include "brailleOah.h"

#include "version.h"

#include "xml2brlOahTypes.h"

#include "xml2brlInsiderOahHandler.h"


using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
string xml2brlAboutInformation ()
{
  return
R"(What xml2brl does:

    This multi-pass translator basically performs 6 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML tree into to
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: converts that tree and the skeleton into a
                 Music Score Representation (MSR);
        Pass 3a: converts the MSR into a
                 Braille Score Representation (BSR)
                 containing one Braille page per MusicXML page;
        Pass 3b: converts the BSR into to another BSR
                 with as many Braille pages as needed
                 to fit the line and page lengthes;
        Pass 4:  converts the BSR to Unicode text
                 and writes it to standard output.

    In this preliminary version, pass 3b merely clones the BSR it receives.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)";
}

//______________________________________________________________________________
S_xml2brlInsiderOahHandler xml2brlInsiderOahHandler::create (
  string executableName,
  string executableAboutInformation,
  string handlerHeader)
{
  // create the insider handler
  xml2brlInsiderOahHandler* o = new
    xml2brlInsiderOahHandler (
      executableName,
      executableAboutInformation,
      handlerHeader);
  assert (o!=0);

  return o;
}

xml2brlInsiderOahHandler::xml2brlInsiderOahHandler (
  string executableName,
  string executableAboutInformation,
  string handlerHeader)
  : oahHandler (
      executableName,
      executableAboutInformation,
      handlerHeader,
R"(                         Welcome to xml2brl,
             the MusicXML to braille music translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond
)",
R"(
Usage: xml2brl ([options] | [MusicXMLFile|-])+
)")
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Initializing xml2brl insider options handler \"" <<
    fHandlerHeader <<
    "\"" <<
    endl;
#endif
#endif

  // create the xml2brl prefixes
  createTheXml2brlPrefixes ();

  // create the xml2brl option groups
  createTheXml2brlOptionGroups (executableName);
}

xml2brlInsiderOahHandler::~xml2brlInsiderOahHandler ()
{}

//______________________________________________________________________________
void xml2brlInsiderOahHandler::createTheXml2brlPrefixes ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating the xml2brl prefixes" <<
    endl;
#endif
#endif

  createTheCommonPrefixes ();
}

//______________________________________________________________________________
void xml2brlInsiderOahHandler::createTheXml2brlOptionGroups (
  string executableName)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating the xml2brl insider option groups" <<
    fHandlerHeader <<
    "\"" <<
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
  initializeBSR ();

  // initialize options handling, phase 2
  // ------------------------------------------------------

  // create the xml2brl OAH group
  appendGroupToHandler (
    createGlobalXml2brlOahGroup (
      executableName,
      fHandlerHeader));

  // create the MusicXML OAH group
  appendGroupToHandler (
    createGlobalMusicxmlOahGroup ());

  // create the mxmlTree OAH group
  appendGroupToHandler (
    createGlobalMxmlTreeOahGroup ());

  // create the mxmlTree2msr OAH group
  appendGroupToHandler (
    createGlobalMxmlTree2msrOahGroup ());

  // create the MSR OAH group
  appendGroupToHandler (
    createGlobalMsrOahGroup ());

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

#ifdef EXTRA_OAH_IS_ENABLED
  // create the extra OAH group
  appendGroupToHandler (
    createGlobalExtraOahGroup ());
#endif

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
    // print the options handler initial state
    gLogStream <<
      "xml2brlInsiderOahHandler has been initialized as:" <<
      endl;

    ++gIndenter;

    print (gLogStream);

    gLogStream <<
      endl;

    --gIndenter;
#endif
#endif
}

//______________________________________________________________________________
void xml2brlInsiderOahHandler::checkOptionsAndArgumentsFromArgcAndArgv () const
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

  checkSingleInputSourceInArgumentsVector ();
}

//______________________________________________________________________________
string xml2brlInsiderOahHandler::fetchOutputFileNameFromTheOptions () const
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
      gGlobalXml2brlInsiderOahGroup->
        getOutputFileNameStringAtom ();

  S_oahBooleanAtom
    autoOutputFileNameAtom =
      gGlobalXml2brlInsiderOahGroup->
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
          "xml2brlInsiderOahHandler::fetchOutputFileNameFromTheOptions(): result 1 = \"" <<
          result <<
          "\"" <<
          endl;
      }
#endif

      // should encoding be used by the output file name?
      bsrBrailleOutputKind
        brailleOutputKind =
          gGlobalBsr2brailleOahGroup->
            getBrailleOutputKind ();

      if (gGlobalBsr2brailleOahGroup->getUseEncodingInFileName ()) {
        switch (brailleOutputKind) {
          case kBrailleOutputAscii:
            result += "_ASCII";
            break;

          case kBrailleOutputUTF8:
            result += "_UTF8";
              /* JMI
            switch (gGlobalBsr2brailleOahGroup->getByteOrderingKind ()) {
              case kByteOrderingNone:
                break;
              case kByteOrderingBigEndian:
                result += "_BE";
                break;
              case kByteOrderingSmallEndian:
                // should not occur JMI
                break;
            } // switch
            */
            break;

          case kBrailleOutputUTF8Debug:
            result += "_UTF8Debug";
            break;

          case kBrailleOutputUTF16:
            result += "_UTF16";
            switch (gGlobalBsr2brailleOahGroup->getByteOrderingKind ()) {
              case kByteOrderingNone:
                break;

              case kByteOrderingBigEndian:
                result += "_BE";
                break;

              case kByteOrderingSmallEndian:
                result += "_SE";
                break;
            } // switch
            break;
        } // switch
      }

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        gLogStream <<
          "xml2brlInsiderOahHandler::fetchOutputFileNameFromTheOptions(): result 2 = " <<
          result <<
          "\"" <<
          endl;
      }
#endif

      // append the file extension to the output file name
      switch (brailleOutputKind) {
        case kBrailleOutputAscii:
         result += ".brf";
          break;

        case kBrailleOutputUTF8:
        case kBrailleOutputUTF16:
          result += ".brf";
          break;

        case kBrailleOutputUTF8Debug:
          result += ".brf"; // since braille cells and regular text are present
          break;
      } // switch

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        gLogStream <<
          "xml2brlInsiderOahHandler::fetchOutputFileNameFromTheOptions(): result 3 = " <<
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
void xml2brlInsiderOahHandler::checkHandlerOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2brlInsiderOahHandler::enforceHandlerQuietness ()
{
#ifdef TRACING_IS_ENABLED
  gGlobalTraceOahGroup->
    enforceGroupQuietness ();
#endif

  gGlobalXml2brlInsiderOahGroup->
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

  gGlobalMsr2bsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBsrOahGroup->
    enforceGroupQuietness ();

  gGlobalBsr2brailleOahGroup->
    enforceGroupQuietness ();

  gGlobalBrailleOahGroup->
    enforceGroupQuietness ();

#ifdef EXTRA_OAH_IS_ENABLED
  gGlobalExtraOahGroup->
    enforceGroupQuietness ();
#endif
}

//______________________________________________________________________________
void xml2brlInsiderOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void xml2brlInsiderOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void xml2brlInsiderOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlInsiderOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlInsiderOahGroup>*> (v)) {
        S_xml2brlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlInsiderOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2brlInsiderOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlInsiderOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2brlInsiderOahGroup>*
    p =
      dynamic_cast<visitor<S_xml2brlInsiderOahGroup>*> (v)) {
        S_xml2brlInsiderOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching xml2brlInsiderOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2brlInsiderOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> xml2brlInsiderOahGroup::browseData ()" <<
      endl;
  }
#endif

  oahGroup::browseData (v);
}

//______________________________________________________________________________
void xml2brlInsiderOahHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "xml2brlInsiderOahHandler:" <<
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

ostream& operator<< (ostream& os, const S_xml2brlInsiderOahHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_xml2brlInsiderOahGroup gGlobalXml2brlInsiderOahGroup;

S_xml2brlInsiderOahGroup xml2brlInsiderOahGroup::create (
  string executableName,
  string handlerHeader)
{
  xml2brlInsiderOahGroup* o = new xml2brlInsiderOahGroup (
    executableName,
    handlerHeader);
  assert (o!=0);

  return o;
}

xml2brlInsiderOahGroup::xml2brlInsiderOahGroup (
  string executableName,
  string handlerHeader)
  : oahGroup (
    executableName,
    "hx2b", "help-xml2brl",
R"(Options that are used by xml2brl are grouped here.)",
    kElementVisibilityWhole)
{
  initializeXml2brlInsiderOahGroup ();
}

xml2brlInsiderOahGroup::~xml2brlInsiderOahGroup ()
{}

//_______________________________________________________________________________
void xml2brlInsiderOahGroup::initializeXml2brlInsiderOahGroup ()
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

  // output
  // --------------------------------------

  createInsiderOutputSubGroup ();

  // quit after some passes
  // --------------------------------------

  createInsiderQuitSubGroup ();
}

//_______________________________________________________________________________
void xml2brlInsiderOahGroup::createInsiderOutputSubGroup ()
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
        "hx2bof", "help-xml2brl-output-file",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // output filename

  fOutputFileNameStringAtom =
    oahStringAtom::create (
      "o", "output-file-name",
R"(Write braille music to file FILENAME instead of standard output.)",
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
Write MusicXML code to a file in the current working directory.
The file name is derived from that of the input file,
replacing any suffix after the the '.' by 'xml'
or adding '.xml' if none is present.)",
      "autoOutputFileName",
      fAutoOutputFileName);

  subGroup->
    appendAtomToSubGroup (
      fAutoOutputFileNameAtom);
}

//_______________________________________________________________________________
void xml2brlInsiderOahGroup::createInsiderQuitSubGroup ()
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
        "hm2bquit", "help-msr2brl-quit",
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
void xml2brlInsiderOahGroup::printXml2brlInsiderOahGroupValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The xml2brl options are:" <<
    endl;

  ++gIndenter;

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
S_xml2brlInsiderOahGroup createGlobalXml2brlOahGroup (
  string executableName,
  string handlerHeader)
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global \"xml2brl\" OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalXml2brlInsiderOahGroup) {

    // create the global OAH group
    // ------------------------------------------------------

    gGlobalXml2brlInsiderOahGroup =
      xml2brlInsiderOahGroup::create (
        executableName,
        handlerHeader);
    assert (gGlobalXml2brlInsiderOahGroup != 0);

    // append versions information to list
    // ------------------------------------------------------

    appendVersionToVersionInfoList (
      "Initial",
      "october 2018",
      "Derived from xml2ly, with an embryonic BSR");

    appendVersionToVersionInfoList (
      "0.02",
      "September 2019",
      "Development freeze due to lack of interest");

    appendVersionToVersionInfoList (
      "0.03",
      "Nobember 2020",
      "OAH finalization");
  }

  // return the global OAH group
  return gGlobalXml2brlInsiderOahGroup;
}


}

