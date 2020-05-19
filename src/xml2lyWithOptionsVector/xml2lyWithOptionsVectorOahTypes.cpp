/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "xml2lyWithOptionsVectorOahTypes.h"

#include "version.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_xml2lyWithOptionsVectorVersionOahAtom xml2lyWithOptionsVectorVersionOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2lyWithOptionsVectorVersionOahAtom* o = new
    xml2lyWithOptionsVectorVersionOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2lyWithOptionsVectorVersionOahAtom::xml2lyWithOptionsVectorVersionOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2lyWithOptionsVectorVersionOahAtom::~xml2lyWithOptionsVectorVersionOahAtom ()
{}

S_oahValuedAtom xml2lyWithOptionsVectorVersionOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2lyWithOptionsVectorVersionOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printVersion (os);

  // no option value is needed
  return nullptr;
}

void xml2lyWithOptionsVectorVersionOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorVersionOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorVersionOahAtom>*> (v)) {
        S_xml2lyWithOptionsVectorVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorVersionOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyWithOptionsVectorVersionOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorVersionOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorVersionOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorVersionOahAtom>*> (v)) {
        S_xml2lyWithOptionsVectorVersionOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorVersionOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyWithOptionsVectorVersionOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorVersionOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2lyWithOptionsVectorVersionOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2lyWithOptionsVectorVersionOahAtom:" <<
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

void xml2lyWithOptionsVectorVersionOahAtom::printVersion (ostream& os) const
{
  os <<
    endl <<
    "This is xml2lyWithOptionsVector" <<
    " version " << currentVersionNumber () <<
    endl <<
    endl;

  // print versions history
  printVersionsHistory (os);

  // register 'print version' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOptionsHandlerFoundAHelpOption ();
}

void xml2lyWithOptionsVectorVersionOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorVersionOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2lyWithOptionsVectorAboutOahAtom xml2lyWithOptionsVectorAboutOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2lyWithOptionsVectorAboutOahAtom* o = new
    xml2lyWithOptionsVectorAboutOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2lyWithOptionsVectorAboutOahAtom::xml2lyWithOptionsVectorAboutOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2lyWithOptionsVectorAboutOahAtom::~xml2lyWithOptionsVectorAboutOahAtom ()
{}

S_oahValuedAtom xml2lyWithOptionsVectorAboutOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2lyWithOptionsVectorAboutOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printAbout (os);

  // no option value is needed
  return nullptr;
}

void xml2lyWithOptionsVectorAboutOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorAboutOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorAboutOahAtom>*> (v)) {
        S_xml2lyWithOptionsVectorAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorAboutOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyWithOptionsVectorAboutOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorAboutOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorAboutOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorAboutOahAtom>*> (v)) {
        S_xml2lyWithOptionsVectorAboutOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorAboutOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyWithOptionsVectorAboutOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorAboutOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2lyWithOptionsVectorAboutOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2lyWithOptionsVectorAboutOahAtom:" <<
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

void xml2lyWithOptionsVectorAboutOahAtom::printAbout (ostream& os) const
{
  os <<
    endl <<
R"(What xml2lyWithOptionsVector does:

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
        setOptionsHandlerFoundAHelpOption ();
}

void xml2lyWithOptionsVectorAboutOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorAboutOahAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_xml2lyWithOptionsVectorContactOahAtom xml2lyWithOptionsVectorContactOahAtom::create (
  string shortName,
  string longName,
  string description)
{
  xml2lyWithOptionsVectorContactOahAtom* o = new
    xml2lyWithOptionsVectorContactOahAtom (
      shortName,
      longName,
      description);
  assert(o!=0);
  return o;
}

xml2lyWithOptionsVectorContactOahAtom::xml2lyWithOptionsVectorContactOahAtom (
  string shortName,
  string longName,
  string description)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementValueExpectedNo)
{}

xml2lyWithOptionsVectorContactOahAtom::~xml2lyWithOptionsVectorContactOahAtom ()
{}

S_oahValuedAtom xml2lyWithOptionsVectorContactOahAtom::handleOptionUnderName (
  string   optionName,
  ostream& os)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOah) {
    gLogOstream <<
      "==> option '" << optionName << "' is a xml2lyWithOptionsVectorContactOahAtom" <<
      endl;
  }
#endif

  // handle it at once
  printContact (os);

  // no option value is needed
  return nullptr;
}

void xml2lyWithOptionsVectorContactOahAtom::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorContactOahAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorContactOahAtom>*> (v)) {
        S_xml2lyWithOptionsVectorContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorContactOahAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void xml2lyWithOptionsVectorContactOahAtom::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorContactOahAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_xml2lyWithOptionsVectorContactOahAtom>*
    p =
      dynamic_cast<visitor<S_xml2lyWithOptionsVectorContactOahAtom>*> (v)) {
        S_xml2lyWithOptionsVectorContactOahAtom elem = this;

#ifdef TRACE_OAH
        if (gOahOah->fTraceOahVisitors) {
          gLogOstream <<
            ".\\\" ==> Launching xml2lyWithOptionsVectorContactOahAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void xml2lyWithOptionsVectorContactOahAtom::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gOahOah->fTraceOahVisitors) {
    gLogOstream <<
      ".\\\" ==> xml2lyWithOptionsVectorContactOahAtom::browseData ()" <<
      endl;
  }
#endif
}

void xml2lyWithOptionsVectorContactOahAtom::print (ostream& os) const
{
  const int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "xml2lyWithOptionsVectorContactOahAtom:" <<
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

void xml2lyWithOptionsVectorContactOahAtom::printContact (ostream& os) const
{
  os <<
    endl <<
R"(To contact xml2lyWithOptionsVector maintainers:

    Send a mail to mailto:lilypond-user@gnu.org describing the problem
    and error messages you obtain if relevant.)" <<
    endl;

  // register 'print contact' action in options groups's options handler upLink
  fSubGroupUpLink->
    getGroupUpLink ()->
      getHandlerUpLink ()->
        setOptionsHandlerFoundAHelpOption ();
}

void xml2lyWithOptionsVectorContactOahAtom::printAtomOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorContactOahAtom& elt)
{
  elt->print (os);
  return os;
}


}
