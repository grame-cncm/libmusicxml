/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef MSVC
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "msrOah.h"
#include "msr2msrOah.h"
#include "musicxmlOah.h"

#include "msr2guidoInsiderOahHandler.h"
#include "msr2guidoRegularOahHandler.h"

#include "musicxml2mxmlTreeInterface.h"
#include "mxmlTree2msrSkeletonBuilderInterface.h"
#include "mxmlTree2msrTranslatorInterface.h"

#include "msr2msrInterface.h"
#include "msr2mxmlTreeInterface.h"
#include "mxmlTree2guidoTranlatorInterface.h"

#include "mxmlTree.h"
#include "msr2guido.h"


using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
EXP xmlErr msrScore2guidoWithHandler (
  S_msrScore   theMsrScore,
  string       passNumber1,
  string       passNumber2,
  string       passNumber3,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->getQuiet ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // convert the first MSR score into a new, second MSR score
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      convertMsrScoreToMsrScore (
        theMsrScore,
        gGlobalMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        passNumber1);
  }
  catch (mxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // convert the second MSR score into a new, second mxmlTree
  // ------------------------------------------------------

  Sxmlelement secondMxmlTree;

  try {
    secondMxmlTree =
      convertMsrScoreToMxmltree (
        secondMsrScore,
        gGlobalMsrOahGroup,
        passNumber2,
        timingItem::kMandatory);
  }
  catch (mxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // generate Guido from the second mxmlTree
  // ------------------------------------------------------

  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    generateGuidoFromMxmlTree (
      secondMxmlTree,
      outputFileName,
      err,
      passNumber3);
  }
  catch (mxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  return kNoErr;
}


}
