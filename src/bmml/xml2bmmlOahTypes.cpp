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

#include "xml2bmmlOahTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2bmmlVersionOahAtom xml2bmmlVersionOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2bmmlVersionOahAtom* o = new
    xml2bmmlVersionOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2bmmlVersionOahAtom::xml2bmmlVersionOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2bmmlVersionOahAtom::~xml2bmmlVersionOahAtom ()
{}

S_oahValuedAtom xml2bmmlVersionOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2bmmlVersionOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printVersion (os);

  // no option value is needed
  return nullptr;
}

void xml2bmmlVersionOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlVersionOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2bmmlVersionOahAtom>*> (v)) {
        S_xml2bmmlVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlVersionOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2bmmlVersionOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlVersionOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2bmmlVersionOahAtom>*> (v)) {
        S_xml2bmmlVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlVersionOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2bmmlVersionOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlVersionOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2bmmlVersionOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2bmmlVersionOahAtom:" <<
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

void xml2bmmlVersionOahAtom::printVersion (ostream& os) const
{
  os <<
    endl <<
    "This is xml2bmml" <<
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

void xml2bmmlVersionOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2bmmlVersionOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2bmmlAboutOahAtom xml2bmmlAboutOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2bmmlAboutOahAtom* o = new
    xml2bmmlAboutOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2bmmlAboutOahAtom::xml2bmmlAboutOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2bmmlAboutOahAtom::~xml2bmmlAboutOahAtom ()
{}

S_oahValuedAtom xml2bmmlAboutOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2bmmlAboutOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printAbout (os);

  // no option value is needed
  return nullptr;
}

void xml2bmmlAboutOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlAboutOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2bmmlAboutOahAtom>*> (v)) {
        S_xml2bmmlAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlAboutOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2bmmlAboutOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlAboutOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2bmmlAboutOahAtom>*> (v)) {
        S_xml2bmmlAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlAboutOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2bmmlAboutOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlAboutOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2bmmlAboutOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2bmmlAboutOahAtom:" <<
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

void xml2bmmlAboutOahAtom::printAbout (ostream& os) const
{
  os <<
    endl <<
R"(What xml2bmml does:

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

void xml2bmmlAboutOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2bmmlAboutOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2bmmlContactOahAtom xml2bmmlContactOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2bmmlContactOahAtom* o = new
    xml2bmmlContactOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2bmmlContactOahAtom::xml2bmmlContactOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2bmmlContactOahAtom::~xml2bmmlContactOahAtom ()
{}

S_oahValuedAtom xml2bmmlContactOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2bmmlContactOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printContact (os);

  // no option value is needed
  return nullptr;
}

void xml2bmmlContactOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlContactOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2bmmlContactOahAtom>*> (v)) {
        S_xml2bmmlContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlContactOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2bmmlContactOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlContactOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2bmmlContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2bmmlContactOahAtom>*> (v)) {
        S_xml2bmmlContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2bmmlContactOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2bmmlContactOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2bmmlContactOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2bmmlContactOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2bmmlContactOahAtom:" <<
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

void xml2bmmlContactOahAtom::printContact (ostream& os) const
{
  os <<
    endl <<
R"(To contact xml2bmml maintainers:

    Send a mail to mailto:lilypond-user@gnu.org describing the problem
    and error messages you obtain if relevant.)" <<
    endl;

  // register 'print contact' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOahHandlerFoundAHelpOption ();
}

void xml2bmmlContactOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2bmmlContactOahAtom& elt)
{
  elt->print (os);
  return os;
}


}
