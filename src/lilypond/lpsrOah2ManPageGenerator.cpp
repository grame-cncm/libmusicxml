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

#include "lpsrOah2ManPageGenerator.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "version.h"

#include "oahOah.h"

using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
lpsrOah2ManPageGenerator::lpsrOah2ManPageGenerator (
  const S_oahHandler handler,
  indentedOstream&   logOstream,
  ostream&           manPageOutputStream)
    : oah2ManPageGenerator (
        handler,
        logOstream,
        manPageOutputStream)
{}

lpsrOah2ManPageGenerator::~lpsrOah2ManPageGenerator ()
{}

//________________________________________________________________________
void lpsrOah2ManPageGenerator::visitStart (S_lpsrScoreOutputKindAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting lpsrScoreOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fOah2ManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsrOah2ManPageGenerator::visitEnd (S_lpsrScoreOutputKindAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting lpsrScoreOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsrOah2ManPageGenerator::visitStart (S_lpsrPitchesLanguageAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting lpsrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fOah2ManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsrOah2ManPageGenerator::visitEnd (S_lpsrPitchesLanguageAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting lpsrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsrOah2ManPageGenerator::visitStart (S_lpsrChordsLanguageAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting lpsrChordsLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fOah2ManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsrOah2ManPageGenerator::visitEnd (S_lpsrChordsLanguageAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting lpsrChordsLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsrOah2ManPageGenerator::visitStart (S_lpsrTransposeAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting lpsrTransposeAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fOah2ManPageOutputStream <<
    ".HP" <<
    endl <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
}

void lpsrOah2ManPageGenerator::visitEnd (S_lpsrTransposeAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting lpsrTransposeAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
