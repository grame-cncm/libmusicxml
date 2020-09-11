/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "version.h"
#include "utilities.h"

#include "utilities.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "xml2lyOahTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2lyVersionOahAtom xml2lyVersionOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2lyVersionOahAtom* o = new
    xml2lyVersionOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2lyVersionOahAtom::xml2lyVersionOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2lyVersionOahAtom::~xml2lyVersionOahAtom ()
{}

S_oahValuedAtom xml2lyVersionOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2lyVersionOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printVersion (os);

  // no option value is needed
  return nullptr;
}

void xml2lyVersionOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyVersionOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyVersionOahAtom>*> (v)) {
        S_xml2lyVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyVersionOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyVersionOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyVersionOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyVersionOahAtom>*> (v)) {
        S_xml2lyVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyVersionOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyVersionOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyVersionOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2lyVersionOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2lyVersionOahAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void xml2lyVersionOahAtom::printVersion (ostream& os) const
{
  os <<
    endl <<
    "This is xml2ly" <<
    " version " << currentVersionNumber () <<
    endl <<
    endl;

  // print versions history
  printVersionsHistory (os);

  // register 'print version' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOahHandlerFoundAHelpOption ();
}

void xml2lyVersionOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2lyVersionOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2lyAboutOahAtom xml2lyAboutOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2lyAboutOahAtom* o = new
    xml2lyAboutOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2lyAboutOahAtom::xml2lyAboutOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2lyAboutOahAtom::~xml2lyAboutOahAtom ()
{}

S_oahValuedAtom xml2lyAboutOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2lyAboutOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printAbout (os);

  // no option value is needed
  return nullptr;
}

void xml2lyAboutOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyAboutOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyAboutOahAtom>*> (v)) {
        S_xml2lyAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyAboutOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyAboutOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyAboutOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyAboutOahAtom>*> (v)) {
        S_xml2lyAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyAboutOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyAboutOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyAboutOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2lyAboutOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2lyAboutOahAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void xml2lyAboutOahAtom::printAbout (ostream& os) const
{
  os <<
    endl <<
R"(What xml2ly does:

    This multi-pass translator basically performs 5 passes:
        Pass 1:  reads the contents of MusicXMLFile or stdin ('-')
                 and converts it to a MusicXML tree;
        Pass 2a: converts that MusicXML tree into to
                 a Music Score Representation (MSR) skeleton;
        Pass 2b: converts that tree and the skeleton into a
                 Music Score Representation (MSR);
        Pass 3:  converts the MSR into a
                 LilyPond Score Representation (LPSR);
        Pass 4:  converts the LPSR to LilyPond source code
                 and writes it to standard output.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    The activity log and warning/error messages go to standard error.)" <<
    endl;

  // register 'print about' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOahHandlerFoundAHelpOption ();
}

void xml2lyAboutOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2lyAboutOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2lyContactOahAtom xml2lyContactOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2lyContactOahAtom* o = new
    xml2lyContactOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2lyContactOahAtom::xml2lyContactOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2lyContactOahAtom::~xml2lyContactOahAtom ()
{}

S_oahValuedAtom xml2lyContactOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2lyContactOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printContact (os);

  // no option value is needed
  return nullptr;
}

void xml2lyContactOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyContactOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyContactOahAtom>*> (v)) {
        S_xml2lyContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyContactOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyContactOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyContactOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyContactOahAtom>*> (v)) {
        S_xml2lyContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (globalOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyContactOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyContactOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (globalOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyContactOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2lyContactOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2lyContactOahAtom:" <<
    endl;

  gIndenter++;

  oahElement::printOptionEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void xml2lyContactOahAtom::printContact (ostream& os) const
{
  os <<
    endl <<
R"(To contact xml2ly maintainers:

    Send a mail to mailto:lilypond-user@gnu.org describing the problem
    and error messages you obtain if relevant.)" <<
    endl;

  // register 'print contact' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOahHandlerFoundAHelpOption ();
}

void xml2lyContactOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2lyContactOahAtom& elt)
{
  elt->print (os);
  return os;
}


}
