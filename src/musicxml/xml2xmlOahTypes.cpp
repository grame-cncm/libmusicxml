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

#include "xml2xmlOahTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2xmlVersionOahAtom xml2xmlVersionOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2xmlVersionOahAtom* o = new
    xml2xmlVersionOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2xmlVersionOahAtom::xml2xmlVersionOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2xmlVersionOahAtom::~xml2xmlVersionOahAtom ()
{}

S_oahValuedAtom xml2xmlVersionOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2xmlVersionOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printVersion (os);

  // no option value is needed
  return nullptr;
}

void xml2xmlVersionOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlVersionOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlVersionOahAtom>*> (v)) {
        S_xml2xmlVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2xmlVersionOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2xmlVersionOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlVersionOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlVersionOahAtom>*> (v)) {
        S_xml2xmlVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2xmlVersionOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2xmlVersionOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlVersionOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2xmlVersionOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2xmlVersionOahAtom:" <<
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

void xml2xmlVersionOahAtom::printVersion (ostream& os) const
{
  os <<
    endl <<
    "This is xml2xml" <<
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

void xml2xmlVersionOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2xmlVersionOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2xmlAboutOahAtom xml2xmlAboutOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2xmlAboutOahAtom* o = new
    xml2xmlAboutOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2xmlAboutOahAtom::xml2xmlAboutOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2xmlAboutOahAtom::~xml2xmlAboutOahAtom ()
{}

S_oahValuedAtom xml2xmlAboutOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2xmlAboutOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printAbout (os);

  // no option value is needed
  return nullptr;
}

void xml2xmlAboutOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlAboutOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlAboutOahAtom>*> (v)) {
        S_xml2xmlAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2xmlAboutOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2xmlAboutOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlAboutOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlAboutOahAtom>*> (v)) {
        S_xml2xmlAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2xmlAboutOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2xmlAboutOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlAboutOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2xmlAboutOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2xmlAboutOahAtom:" <<
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

void xml2xmlAboutOahAtom::printAbout (ostream& os) const
{
  os <<
    endl <<
R"(What xml2xml does:

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

void xml2xmlAboutOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2xmlAboutOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2xmlContactOahAtom xml2xmlContactOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2xmlContactOahAtom* o = new
    xml2xmlContactOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2xmlContactOahAtom::xml2xmlContactOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2xmlContactOahAtom::~xml2xmlContactOahAtom ()
{}

S_oahValuedAtom xml2xmlContactOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2xmlContactOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printContact (os);

  // no option value is needed
  return nullptr;
}

void xml2xmlContactOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlContactOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlContactOahAtom>*> (v)) {
        S_xml2xmlContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2xmlContactOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2xmlContactOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlContactOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2xmlContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2xmlContactOahAtom>*> (v)) {
        S_xml2xmlContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2xmlContactOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2xmlContactOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2xmlContactOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2xmlContactOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2xmlContactOahAtom:" <<
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

void xml2xmlContactOahAtom::printContact (ostream& os) const
{
  os <<
    endl <<
R"(To contact xml2xml maintainers:

    Send a mail to mailto:lilypond-user@gnu.org describing the problem
    and error messages you obtain if relevant.)" <<
    endl;

  // register 'print contact' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOahHandlerFoundAHelpOption ();
}

void xml2xmlContactOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2xmlContactOahAtom& elt)
{
  elt->print (os);
  return os;
}


}
