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

#include "xml2lyOah2ManPageGenerator.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahOah.h"

using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
xml2lyOah2ManPageGenerator::xml2lyOah2ManPageGenerator (
  const S_oahHandler handler,
  S_manPageOah&      manPageOpts,
  indentedOstream&   logOstream,
  ostream&            manPageOutputStream)
    : basicOah2ManPageGenerator (
        handler,
        manPageOpts,
        logOstream,
        manPageOutputStream)
{
  fManPageOutputStream <<
    "Constructing xml2lyOah2ManPageGenerator" <<
    endl;
}

xml2lyOah2ManPageGenerator::~xml2lyOah2ManPageGenerator ()
{}

//________________________________________________________________________
void xml2lyOah2ManPageGenerator::generateManPageFromOahHandler ()
{
  fManPageOutputStream <<
    "Generating the man page from xml2ly OAH handler \"" <<
    fVisitedOahHandler->getHandlerHeader () <<
    "\"" <<
    endl;

  if (fVisitedOahHandler) {
    // browse the visited oahHandler
    oahBrowser<oahHandler> browser (this);
    browser.browse (*fVisitedOahHandler);
  }
}

//________________________________________________________________________
void xml2lyOah2ManPageGenerator::visitStart (S_msrRenamePartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting msrRenamePartAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void xml2lyOah2ManPageGenerator::visitStart (S_msrTransposePartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting msrTransposePartAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void xml2lyOah2ManPageGenerator::visitStart (S_msrOmitPartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting msrOmitPartAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void xml2lyOah2ManPageGenerator::visitStart (S_msrKeepPartAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting msrKeepPartAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void xml2lyOah2ManPageGenerator::visitStart (S_msrPitchesLanguageAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting msrPitchesLanguageAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
