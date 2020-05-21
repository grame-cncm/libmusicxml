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

#include "xml2brlOah2ManPageGenerator.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "version.h"

using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
S_xml2brlOah2ManPageGenerator xml2brlOah2ManPageGenerator::create (
  const S_oahHandler handler,
  ostream&           logOstream,
  ostream&           manPageOutputStream)
{
  xml2brlOah2ManPageGenerator* o = new
    xml2brlOah2ManPageGenerator (
      handler,
      logOstream,
      manPageOutputStream);
  assert(o!=0);
  return o;
}

xml2brlOah2ManPageGenerator::xml2brlOah2ManPageGenerator (
  const S_oahHandler handler,
  ostream&           logOstream,
  ostream&           manPageOutputStream)
    : oah2ManPageGenerator (
        handler,
        logOstream,
        manPageOutputStream),
      basicOah2ManPageGenerator (
        handler,
        logOstream,
        manPageOutputStream),
      msrOah2ManPageGenerator (
        handler,
        logOstream,
        manPageOutputStream),
      bsrOah2ManPageGenerator (
        handler,
        logOstream,
        manPageOutputStream),
      brailleOah2ManPageGenerator (
        handler,
        logOstream,
        manPageOutputStream),
      extraOah2ManPageGenerator (
        handler,
        logOstream,
        manPageOutputStream),
      oahOah2ManPageGenerator (
        handler,
        logOstream,
        manPageOutputStream)
{}

xml2brlOah2ManPageGenerator::~xml2brlOah2ManPageGenerator ()
{}

//________________________________________________________________________
void xml2brlOah2ManPageGenerator::visitTheHandler ()
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Running xml2brlOah2ManPageGenerator::visitTheHandler()" <<
      endl;
  }
#endif

  if (fVisitedOahHandler) {
    // browse the visited oahHandler
    oahBrowser<oahHandler> browser (this);
    browser.browse (*fVisitedOahHandler);
  }
  else {
    // visited handler is not well handled by this visitor
    stringstream s;

    s <<
      "fVisitedOahHandler is null, cannot generate man page data";

    oahError (s.str ());
  }
}

//________________________________________________________________________
void xml2brlOah2ManPageGenerator::visitStart (S_xml2brlVersionOahAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting xml2brlVersionOahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fOah2ManPageOutputStream <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    elt->fetchNamesBetweenParentheses () <<
    endl <<

    ".IP" <<
    endl <<
    elt->getDescription () <<
    endl;
    */
}

void xml2brlOah2ManPageGenerator::visitEnd (S_xml2brlVersionOahAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting xml2brlVersionOahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void xml2brlOah2ManPageGenerator::visitStart (S_xml2brlAboutOahAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> Start visiting xml2brlAboutOahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fOah2ManPageOutputStream <<
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

void xml2brlOah2ManPageGenerator::visitEnd (S_xml2brlAboutOahAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting xml2brlAboutOahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void xml2brlOah2ManPageGenerator::visitStart (S_xml2brlContactOahAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
     endl <<
      ".\\\" --> Start visiting xml2brlContactOahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
/*
  fOah2ManPageOutputStream <<
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

void xml2brlOah2ManPageGenerator::visitEnd (S_xml2brlContactOahAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fOah2ManPageLogOstream <<
      endl <<
      ".\\\" --> End visiting xml2brlContactOahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
