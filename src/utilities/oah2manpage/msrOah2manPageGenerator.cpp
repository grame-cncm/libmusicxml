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

#include "msrOah2manPageGenerator.h"

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
msrOah2manPageGenerator::msrOah2manPageGenerator (
  const S_oahHandler handler,
  ostream&           manPageOutputStream)
    : oah2manPageGenerator (
        handler,
        manPageOutputStream)
{}

msrOah2manPageGenerator::~msrOah2manPageGenerator ()
{}

//________________________________________________________________________
void msrOah2manPageGenerator::visitStart (S_msrRenamePartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting msrRenamePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
    */
}

void msrOah2manPageGenerator::visitEnd (S_msrRenamePartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting msrRenamePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2manPageGenerator::visitStart (S_msrOmitPartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting msrOmitPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
    */
}

void msrOah2manPageGenerator::visitEnd (S_msrOmitPartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting msrOmitPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2manPageGenerator::visitStart (S_msrKeepPartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting msrKeepPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
    */
}

void msrOah2manPageGenerator::visitEnd (S_msrKeepPartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting msrKeepPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2manPageGenerator::visitStart (S_msrPitchesLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting msrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
    */
}

void msrOah2manPageGenerator::visitEnd (S_msrPitchesLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting msrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
