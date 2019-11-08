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

#include "msrOah2ManPageGenerator.h"

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
msrOah2ManPageGenerator::msrOah2ManPageGenerator (
  indentedOstream& logOstream,
  ostream&         manPageOutputStream)
    : fMsrOah2ManPageLogOstream (
        logOstream),
      fMsrOah2ManPageOutputStream (
        manPageOutputStream)
{}

msrOah2ManPageGenerator::~msrOah2ManPageGenerator ()
{}

//________________________________________________________________________
void msrOah2ManPageGenerator::visitStart (S_msrRenamePartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting msrRenamePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fMsrOah2ManPageOutputStream <<
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

void msrOah2ManPageGenerator::visitEnd (S_msrRenamePartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting msrRenamePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2ManPageGenerator::visitStart (S_msrTransposePartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting msrTransposePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fMsrOah2ManPageOutputStream <<
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

void msrOah2ManPageGenerator::visitEnd (S_msrTransposePartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting msrTransposePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2ManPageGenerator::visitStart (S_msrOmitPartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting msrOmitPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fMsrOah2ManPageOutputStream <<
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

void msrOah2ManPageGenerator::visitEnd (S_msrOmitPartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting msrOmitPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2ManPageGenerator::visitStart (S_msrKeepPartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting msrKeepPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fMsrOah2ManPageOutputStream <<
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

void msrOah2ManPageGenerator::visitEnd (S_msrKeepPartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting msrKeepPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2ManPageGenerator::visitStart (S_msrPitchesLanguageAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting msrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fMsrOah2ManPageOutputStream <<
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

void msrOah2ManPageGenerator::visitEnd (S_msrPitchesLanguageAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fMsrOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting msrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
