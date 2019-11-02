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

#include "basicOah2ManPageGenerator.h"

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
basicOah2ManPageGenerator::basicOah2ManPageGenerator (
//  const S_oahHandler handler,
  indentedOstream&   logOstream,
  ostream&           manPageOutputStream)
    :  fLogOutputStream (
        logOstream),
      fManPageOutputStream (
        manPageOutputStream)
{}

basicOah2ManPageGenerator::~basicOah2ManPageGenerator ()
{}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahHandler& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahHandler" <<
      " \"" << elt->getHandlerHeader () << "\"" <<
      endl;
  }
#endif

  fLogOutputStream <<
    ".TH " <<
    gOahOah->fHandlerExecutableName <<
    " \"" <<
    "1" << // JMI section ???
    "\" \"" <<
    "October 2019" <<
    "\" " <<
    gOahOah->fHandlerExecutableName <<
    "\" \"" <<
    "User Commands" <<
    "\"" <<
    endl <<

    ".SH NAME" <<
    endl <<
    gOahOah->fHandlerExecutableName <<
    " \\- manual page for " <<
    gOahOah->fHandlerExecutableName <<
    endl <<

    ".SH DESCRIPTION" <<
    endl <<
    ".IP" <<
    endl <<
    elt->getHandlerPreamble () <<
    endl <<

    ".SH \"USAGE\"" <<
    endl <<
    ".IP" <<
    endl <<
    elt->getHandlerUsage () <<
    endl;
}

void basicOah2ManPageGenerator::visitEnd (S_oahHandler& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> End visiting oahHandler" <<
      " \"" << elt->getHandlerHeader () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahPrefix& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahPrefix" <<
      " \"" << elt->getPrefixName () << "\"" <<
      endl;
  }
#endif

//  elt->printPrefixHeader (fManPageOutputStream);
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahGroup& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahGroup" <<
      " \"" << elt->getGroupHeader () << "\"" <<
      endl;
  }
#endif

  fLogOutputStream <<
    ".SH " <<
    elt->getGroupHeader () <<
    endl <<
    "(\\fB\\-" <<
    replaceSubstringInString (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getLongName (), "-", "\\-") <<
    "\\fR): " <<
    endl;
}

void basicOah2ManPageGenerator::visitEnd (S_oahGroup& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> End visiting oahGroup" <<
      " \"" << elt->getGroupHeader () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahSubGroup& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahSubGroup" <<
      " \"" << elt->getSubGroupHeader () << "\"" <<
      endl;
  }
#endif

  fLogOutputStream <<
    ".TP" <<
    endl <<
    elt->getSubGroupHeader () <<
    endl <<
    ".BL -tag -width indent" <<
    endl <<
    "(\\fB\\-" <<
    replaceSubstringInString (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    replaceSubstringInString (elt->getLongName (), "-", "\\-") <<
    "\\fR): " <<
    endl;
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting S_oahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

void basicOah2ManPageGenerator::visitEnd (S_oahAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> End visiting S_oahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahAtomWithVariableName& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahAtomWithVariableName" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

}

void basicOah2ManPageGenerator::visitEnd (S_oahAtomWithVariableName& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> End visiting oahAtomWithVariableName" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahAtomSynonym& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahAtomSynonym" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahOptionsUsageAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahOptionsUsageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahOptionsSummaryAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahOptionsSummaryAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahOptionNameHelpAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahOptionNameHelpAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahBooleanAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahBooleanAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fLogOutputStream <<
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

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahTwoBooleansAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahTwoBooleansAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fLogOutputStream <<
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

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahThreeBooleansAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahThreeBooleansAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

  fLogOutputStream <<
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

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahCombinedBooleansAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahCombinedBooleansAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahMultiplexBooleansAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahMultiplexBooleansAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahValuedAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahValuedAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahIntegerAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahIntegerAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahFloatAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahFloatAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahStringAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahStringAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahRationalAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahRationalAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahMonoplexStringAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahMonoplexStringAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahStringWithDefaultValueAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahStringWithDefaultValueAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

void basicOah2ManPageGenerator::visitEnd (S_oahStringWithDefaultValueAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> End visiting oahStringWithDefaultValueAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahNaturalNumbersSetElementAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahNaturalNumbersSetElementAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahNaturalNumbersSetAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahNaturalNumbersSetAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahStringsSetElementAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahStringsSetElementAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahStringsSetAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahStringsSetAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void basicOah2ManPageGenerator::visitStart (S_oahRGBColorAtom& elt)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    fLogOutputStream <<
      endl <<
      ".\\\" --> Start visiting oahRGBColorAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}


} // namespace
