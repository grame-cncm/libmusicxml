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
#include <cmath>
#include <string>

#include <regex>

#include "brailleOah2manPageGenerator.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "version.h"

#include "oahOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
brailleOah2manPageGenerator::brailleOah2manPageGenerator (
  const S_oahHandler handler,
  ostream&           manPageOutputStream)
    : oah2manPageGenerator (
        handler,
        manPageOutputStream)
{}

brailleOah2manPageGenerator::~brailleOah2manPageGenerator ()
{}

//________________________________________________________________________
void brailleOah2manPageGenerator::visitStart (S_brailleOutputKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting brailleOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void brailleOah2manPageGenerator::visitEnd (S_brailleOutputKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting brailleOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void brailleOah2manPageGenerator::visitStart (S_brailleUTFKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting brailleUTFKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void brailleOah2manPageGenerator::visitEnd (S_brailleUTFKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting brailleUTFKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void brailleOah2manPageGenerator::visitStart (S_brailleByteOrderingKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting brailleByteOrderingKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), regex ("-"), "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void brailleOah2manPageGenerator::visitEnd (S_brailleByteOrderingKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting brailleByteOrderingKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
