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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "xml2meiOahTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2meiVersionOahAtom xml2meiVersionOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2meiVersionOahAtom* o = new
    xml2meiVersionOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2meiVersionOahAtom::xml2meiVersionOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2meiVersionOahAtom::~xml2meiVersionOahAtom ()
{}

S_oahValuedAtom xml2meiVersionOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2meiVersionOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printVersion (os);

  // no option value is needed
  return nullptr;
}

void xml2meiVersionOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiVersionOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2meiVersionOahAtom>*> (v)) {
        S_xml2meiVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiVersionOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2meiVersionOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiVersionOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2meiVersionOahAtom>*> (v)) {
        S_xml2meiVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiVersionOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2meiVersionOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiVersionOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2meiVersionOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2meiVersionOahAtom:" <<
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

void xml2meiVersionOahAtom::printVersion (ostream& os) const
{
  os <<
    endl <<
    "This is xml2mei" <<
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

void xml2meiVersionOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2meiVersionOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2meiAboutOahAtom xml2meiAboutOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2meiAboutOahAtom* o = new
    xml2meiAboutOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2meiAboutOahAtom::xml2meiAboutOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2meiAboutOahAtom::~xml2meiAboutOahAtom ()
{}

S_oahValuedAtom xml2meiAboutOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2meiAboutOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printAbout (os);

  // no option value is needed
  return nullptr;
}

void xml2meiAboutOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiAboutOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2meiAboutOahAtom>*> (v)) {
        S_xml2meiAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiAboutOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2meiAboutOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiAboutOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2meiAboutOahAtom>*> (v)) {
        S_xml2meiAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiAboutOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2meiAboutOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiAboutOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2meiAboutOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2meiAboutOahAtom:" <<
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

void xml2meiAboutOahAtom::printAbout (ostream& os) const
{
  os <<
    endl <<
R"(What xml2mei does:

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

void xml2meiAboutOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2meiAboutOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2meiContactOahAtom xml2meiContactOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2meiContactOahAtom* o = new
    xml2meiContactOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2meiContactOahAtom::xml2meiContactOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2meiContactOahAtom::~xml2meiContactOahAtom ()
{}

S_oahValuedAtom xml2meiContactOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2meiContactOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printContact (os);

  // no option value is needed
  return nullptr;
}

void xml2meiContactOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiContactOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2meiContactOahAtom>*> (v)) {
        S_xml2meiContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiContactOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2meiContactOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiContactOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2meiContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2meiContactOahAtom>*> (v)) {
        S_xml2meiContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2meiContactOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2meiContactOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2meiContactOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2meiContactOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2meiContactOahAtom:" <<
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

void xml2meiContactOahAtom::printContact (ostream& os) const
{
  os <<
    endl <<
R"(To contact xml2mei maintainers:

    Send a mail to mailto:lilypond-user@gnu.org describing the problem
    and error messages you obtain if relevant.)" <<
    endl;

  // register 'print contact' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOahHandlerFoundAHelpOption ();
}

void xml2meiContactOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2meiContactOahAtom& elt)
{
  elt->print (os);
  return os;
}


}
