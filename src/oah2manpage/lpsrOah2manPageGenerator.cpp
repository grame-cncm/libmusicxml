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

#include "lpsrOah2manPageGenerator.h"

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
lpsrOah2manPageGenerator::lpsrOah2manPageGenerator (
  const S_oahHandler handler,
  ostream&           manPageOutputStream)
    : oah2manPageGenerator (
        handler,
        manPageOutputStream)
{}

lpsrOah2manPageGenerator::~lpsrOah2manPageGenerator ()
{}

/*
//________________________________________________________________________
void lpsrOah2manPageGenerator::visitStart (S_lpsrScoreOutputKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lpsrScoreOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/ *
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
    * /
}

void lpsrOah2manPageGenerator::visitEnd (S_lpsrScoreOutputKindAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lpsrScoreOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}
*/

//________________________________________________________________________
void lpsrOah2manPageGenerator::visitStart (S_lpsrPitchesLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lpsrPitchesLanguageAtom" <<
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

void lpsrOah2manPageGenerator::visitEnd (S_lpsrPitchesLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lpsrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsrOah2manPageGenerator::visitStart (S_lpsrChordsLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lpsrChordsLanguageAtom" <<
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

void lpsrOah2manPageGenerator::visitEnd (S_lpsrChordsLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lpsrChordsLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsrOah2manPageGenerator::visitStart (S_lpsrTransposeAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> Start visiting lpsrTransposeAtom" <<
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

void lpsrOah2manPageGenerator::visitEnd (S_lpsrTransposeAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      endl <<
      ".\\\" --> End visiting lpsrTransposeAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
