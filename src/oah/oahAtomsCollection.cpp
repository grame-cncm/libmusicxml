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
#include <regex>

#include "utilities.h"
#include "messagesHandling.h"

#include "version.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "oahAtomsCollection.h"


namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before "trace-oah", if present, has been applied
*/
//#define ENFORCE_TRACE_OAH

//______________________________________________________________________________
S_oahAtomSynonym oahAtomSynonym::create (
  string    shortName,
  string    longName,
  string    description,
  S_oahAtom originalOahAtom)
{
  oahAtomSynonym* o = new
    oahAtomSynonym (
      shortName,
      longName,
      description,
      originalOahAtom);
  assert (o!=0);
  return o;
}

oahAtomSynonym::oahAtomSynonym (
  string    shortName,
  string    longName,
  string    description,
  S_oahAtom originalOahAtom)
  : oahAtom (
      shortName,
      longName,
      description,
      originalOahAtom->getElementKind ())
{
  // sanity check
  msgAssert (
    originalOahAtom != nullptr,
    "originalOahAtom is null");

  fOriginalOahAtom = originalOahAtom;
}

oahAtomSynonym::~oahAtomSynonym ()
{}

void oahAtomSynonym::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahAtomSynonym" <<
      endl;
  }
#endif

  // JMI ???
}

void oahAtomSynonym::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomSynonym::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomSynonym>*
    p =
      dynamic_cast<visitor<S_oahAtomSynonym>*> (v)) {
        S_oahAtomSynonym elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomSynonym::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAtomSynonym::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomSynonym::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAtomSynonym>*
    p =
      dynamic_cast<visitor<S_oahAtomSynonym>*> (v)) {
        S_oahAtomSynonym elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAtomSynonym::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAtomSynonym::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAtomSynonym::browseData ()" <<
      endl;
  }
#endif

  if (fOriginalOahAtom) {
    // browse the original atom
    oahBrowser<oahAtom> browser (v);
    browser.browse (*fOriginalOahAtom);
  }
}

void oahAtomSynonym::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomSynonym:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAtomSynonym::printShort (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AtomSynonym: ";

  oahElement::printOahElementEssentialsShort (
    os, fieldWidth);

  os <<
    fOriginalOahAtom->fetchNames () <<
    endl;
}

void oahAtomSynonym::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahAtomSynonym& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_oahHelpOnlyAtom oahHelpOnlyAtom::create (
  string shortName,
  string longName,
  string description,
  string executableName)
{
  oahHelpOnlyAtom* o = new
    oahHelpOnlyAtom (
      shortName,
      longName,
      description,
      executableName);
  assert (o!=0);
  return o;
}
*/

oahHelpOnlyAtom::oahHelpOnlyAtom (
  string shortName,
  string longName,
  string description,
  string executableName)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementWithoutValue)
{
  fHelpOnlyAtomExecutableName = executableName;

  fElementHelpOnlyKind = kElementHelpOnlyYes;
}

oahHelpOnlyAtom::~oahHelpOnlyAtom ()
{}

void oahHelpOnlyAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpOnlyAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHelpOnlyAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpOnlyAtom>*> (v)) {
        S_oahHelpOnlyAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpOnlyAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahHelpOnlyAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpOnlyAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHelpOnlyAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpOnlyAtom>*> (v)) {
        S_oahHelpOnlyAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpOnlyAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahHelpOnlyAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpOnlyAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahHelpOnlyAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "HelpOnlyAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahHelpOnlyAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahHelpOnlyAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahOptionsUsageAtom oahOptionsUsageAtom::create (
  string shortName,
  string longName,
  string description,
  string executableName)
{
  oahOptionsUsageAtom* o = new
    oahOptionsUsageAtom (
      shortName,
      longName,
      description,
      executableName);
  assert (o!=0);
  return o;
}

oahOptionsUsageAtom::oahOptionsUsageAtom (
  string shortName,
  string longName,
  string description,
  string executableName)
  : oahHelpOnlyAtom (
      shortName,
      longName,
      description,
      executableName)
{}

oahOptionsUsageAtom::~oahOptionsUsageAtom ()
{}

void oahOptionsUsageAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOptionsUsageAtom" <<
      endl;
  }
#endif

  printOptionsUsage (os);
}

void oahOptionsUsageAtom::printOptionsUsage (ostream& os) const
{
  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  os <<
    fHelpOnlyAtomExecutableName <<
    " options usage:" <<
    endl;

  ++gIndenter;

  os <<
    gIndenter.indentMultiLineString (
      regex_replace (
        regex_replace (
R"(In EXECUTABLE_NAME, '-' as an argument, represents standard input.

Most options have a short and a long name for commodity.
The long name may be empty if the short name is explicit enough.

The options are organized in a group-subgroup-atom hierarchy.
Help can be obtained for groups or subgroups at will,
as well as for any option with the OPTION_NAME_HELP_NAMES option.

A subgroup can be showm as a header only, in which case its description is printed
only when the corresponding short or long names are used.

Both '-' and '--' can be used to introduce options,
even though the help facility only shows them with '-'.

There some prefixes to allow for shortcuts,
such as '-t=voices,meas' for '-tvoices, -tmeas'.

The options can be placed in any order,
provided the values immediately follow the atoms that need them.

Using options that attempt to create files, such as '-o, -output-file-name',
leads to an error if the environment is read-only access,
as is the case of https://libmusicxml.grame.fr .)",
          regex ("OPTION_NAME_HELP_NAMES"),
          gGlobalOahOahGroup->
            getOptionNameHelpAtom ()->
              fetchNamesBetweenQuotes ()
          ),
        regex ("EXECUTABLE_NAME"),
        fHelpOnlyAtomExecutableName
        )
      ) <<
    endl;

  --gIndenter;

  gIndenter.setIndent (saveIndent);
}

void oahOptionsUsageAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionsUsageAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionsUsageAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsUsageAtom>*> (v)) {
        S_oahOptionsUsageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOptionsUsageAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOptionsUsageAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionsUsageAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionsUsageAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionsUsageAtom>*> (v)) {
        S_oahOptionsUsageAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOptionsUsageAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOptionsUsageAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionsUsageAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahOptionsUsageAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsUsageAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahOptionsUsageAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahOptionsUsageAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahHelpAtom oahHelpAtom::create (
  string shortName,
  string longName,
  string description,
  string executableName)
{
  oahHelpAtom* o = new
    oahHelpAtom (
      shortName,
      longName,
      description,
      executableName);
  assert (o!=0);
  return o;
}

oahHelpAtom::oahHelpAtom (
  string shortName,
  string longName,
  string description,
  string executableName)
  : oahHelpOnlyAtom (
      shortName,
      longName,
      description,
      executableName)
{}

oahHelpAtom::~oahHelpAtom ()
{}

void oahHelpAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahHelpAtom" <<
      endl;
  }
#endif

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  fetchAtomHandlerUpLink ()->
    printHelp (os);

  gIndenter.setIndent (saveIndent);
}

void oahHelpAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpAtom>*> (v)) {
        S_oahHelpAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahHelpAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpAtom>*> (v)) {
        S_oahHelpAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahHelpAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahHelpAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahHelpAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahHelpAtom::printOptionsSummary (ostream& os) const
{
  os <<
    gGlobalOahOahGroup->getHandlerExecutableName () <<
    endl;
}

void oahHelpAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahHelpAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahHelpSummaryAtom oahHelpSummaryAtom::create (
  string shortName,
  string longName,
  string description,
  string executableName)
{
  oahHelpSummaryAtom* o = new
    oahHelpSummaryAtom (
      shortName,
      longName,
      description,
      executableName);
  assert (o!=0);
  return o;
}

oahHelpSummaryAtom::oahHelpSummaryAtom (
  string shortName,
  string longName,
  string description,
  string executableName)
  : oahHelpOnlyAtom (
      shortName,
      longName,
      description,
      executableName)
{}

oahHelpSummaryAtom::~oahHelpSummaryAtom ()
{}

void oahHelpSummaryAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahHelpSummaryAtom" <<
      endl;
  }
#endif

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  fetchAtomHandlerUpLink ()->
    printOptionsSummary (os);

  gIndenter.setIndent (saveIndent);
}

void oahHelpSummaryAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpSummaryAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHelpSummaryAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpSummaryAtom>*> (v)) {
        S_oahHelpSummaryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpSummaryAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahHelpSummaryAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpSummaryAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahHelpSummaryAtom>*
    p =
      dynamic_cast<visitor<S_oahHelpSummaryAtom>*> (v)) {
        S_oahHelpSummaryAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahHelpSummaryAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahHelpSummaryAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahHelpSummaryAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahHelpSummaryAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsSummaryAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahHelpSummaryAtom::printOptionsSummary (ostream& os) const
{
  os <<
    gGlobalOahOahGroup->getHandlerExecutableName () <<
    endl;
}

void oahHelpSummaryAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahHelpSummaryAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahAboutAtom oahAboutAtom::create (
  string shortName,
  string longName,
  string description,
  string executableName)
{
  oahAboutAtom* o = new
    oahAboutAtom (
      shortName,
      longName,
      description,
      executableName);
  assert (o!=0);
  return o;
}

oahAboutAtom::oahAboutAtom (
  string shortName,
  string longName,
  string description,
  string executableName)
  : oahHelpOnlyAtom (
      shortName,
      longName,
      description,
      executableName)
{}

oahAboutAtom::~oahAboutAtom ()
{}

void oahAboutAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahAboutAtom" <<
      endl;
  }
#endif

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  printAbout (os);

  gIndenter.setIndent (saveIndent);
}

void oahAboutAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAboutAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAboutAtom>*
    p =
      dynamic_cast<visitor<S_oahAboutAtom>*> (v)) {
        S_oahAboutAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAboutAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAboutAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAboutAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAboutAtom>*
    p =
      dynamic_cast<visitor<S_oahAboutAtom>*> (v)) {
        S_oahAboutAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAboutAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAboutAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAboutAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahAboutAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahAboutAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahAboutAtom::printAbout (ostream& os) const
{
  os <<
    fetchAtomHandlerUpLink ()->
      getHandlerExecutableAboutInformation () <<
    endl;
}

void oahAboutAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahAboutAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahVersionAtom oahVersionAtom::create (
  string shortName,
  string longName,
  string description,
  string executableName)
{
  oahVersionAtom* o = new
    oahVersionAtom (
      shortName,
      longName,
      description,
      executableName);
  assert (o!=0);
  return o;
}

oahVersionAtom::oahVersionAtom (
  string shortName,
  string longName,
  string description,
  string executableName)
  : oahHelpOnlyAtom (
      shortName,
      longName,
      description,
      executableName)
{}

oahVersionAtom::~oahVersionAtom ()
{}

void oahVersionAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahVersionAtom" <<
      endl;
  }
#endif

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  printVersion (os);

  gIndenter.setIndent (saveIndent);
}

void oahVersionAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahVersionAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahVersionAtom>*> (v)) {
        S_oahVersionAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahVersionAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahVersionAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahVersionAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahVersionAtom>*
    p =
      dynamic_cast<visitor<S_oahVersionAtom>*> (v)) {
        S_oahVersionAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahVersionAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahVersionAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahVersionAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahVersionAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahVersionAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahVersionAtom::printVersion (ostream& os) const
{
  os <<
    "This is " <<
    fHelpOnlyAtomExecutableName <<
    " version " <<
    currentVersionNumber () <<
    endl << endl;

  // print versions history
  printVersionsHistory (
    fHelpOnlyAtomExecutableName,
    os);
}

void oahVersionAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahVersionAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahContactAtom oahContactAtom::create (
  string shortName,
  string longName,
  string description,
  string executableName)
{
  oahContactAtom* o = new
    oahContactAtom (
      shortName,
      longName,
      description,
      executableName);
  assert (o!=0);
  return o;
}

oahContactAtom::oahContactAtom (
  string shortName,
  string longName,
  string description,
  string executableName)
  : oahHelpOnlyAtom (
      shortName,
      longName,
      description,
      executableName)
{}


oahContactAtom::~oahContactAtom ()
{}

void oahContactAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> option '" << fetchNames () << "' is a oahContactAtom" <<
      endl;
  }
#endif

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  printContact (os);

  gIndenter.setIndent (saveIndent);
}

void oahContactAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahContactAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahContactAtom>*
    p =
      dynamic_cast<visitor<S_oahContactAtom>*> (v)) {
        S_oahContactAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahContactAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahContactAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahContactAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahContactAtom>*
    p =
      dynamic_cast<visitor<S_oahContactAtom>*> (v)) {
        S_oahContactAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahContactAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahContactAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahContactAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahContactAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "oahContactAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  --gIndenter;
}

void oahContactAtom::printContact (ostream& os) const
{
  os <<
    gIndenter.indentMultiLineString (
      regex_replace (
  R"(To contact the maintainers of EXECUTABLE_NAME:
    Send a mail to mailto:lilypond-user@gnu.org describing the problem
    and error messages you obtain if relevant.)",
        regex ("EXECUTABLE_NAME"),
        fHelpOnlyAtomExecutableName)) <<
    endl;
}

void oahContactAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahContactAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahBooleanAtom oahBooleanAtom::create (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable)
{
  oahBooleanAtom* o = new
    oahBooleanAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable);
  assert (o!=0);
  return o;
}

oahBooleanAtom::oahBooleanAtom (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementWithoutValue),
    fBooleanVariable (
      booleanVariable)
{
  fVariableName = variableName;
  fVariableHasBeenSet = false;
}

oahBooleanAtom::~oahBooleanAtom ()
{}

void oahBooleanAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahBooleanAtom" <<
      endl;
  }
#endif

  setBooleanVariable (true);
}

void oahBooleanAtom::setBooleanVariable (bool value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    if (fLongName == "trace-oah") { // JMI
      // get the options handler
      S_oahHandler
        handler =
          fetchAtomHandlerUpLink (); // JMI

      gLogStream <<
        "Setting option '" <<
        fetchNames () <<
        "' boolean variable to '" <<
        booleanAsString (value) <<
        "'" <<
        endl;
    }
  }
#endif

  fBooleanVariable = value;
  fVariableHasBeenSet = true;
}

void oahBooleanAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahBooleanAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahBooleanAtom>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtom>*> (v)) {
        S_oahBooleanAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahBooleanAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahBooleanAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahBooleanAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahBooleanAtom>*
    p =
      dynamic_cast<visitor<S_oahBooleanAtom>*> (v)) {
        S_oahBooleanAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahBooleanAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahBooleanAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahBooleanAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahBooleanAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "BooleanAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "booleanVariable" << " : " <<
    booleanAsString (fBooleanVariable) <<
    endl;

  --gIndenter;
}

void oahBooleanAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    booleanAsString (fBooleanVariable);
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahBooleanAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahTwoBooleansAtom oahTwoBooleansAtom::create (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable,
  bool&  booleanSecondaryVariable)
{
  oahTwoBooleansAtom* o = new
    oahTwoBooleansAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable,
      booleanSecondaryVariable);
  assert (o!=0);
  return o;
}

oahTwoBooleansAtom::oahTwoBooleansAtom (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable,
  bool&  booleanSecondaryVariable)
  : oahBooleanAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable),
    fBooleanSecondaryVariable (
      booleanSecondaryVariable)
{}

oahTwoBooleansAtom::~oahTwoBooleansAtom ()
{}

void oahTwoBooleansAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahTwoBooleansAtom" <<
      endl;
  }
#endif

  setTwoBooleansVariables (true);
}

void oahTwoBooleansAtom::setTwoBooleansVariables (bool value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' two booleans variables to '" <<
      booleanAsString (value) <<
      "'" <<
      endl;
  }
#endif

  fBooleanVariable = value;
  fBooleanSecondaryVariable = value;
  fVariableHasBeenSet = true;
}

void oahTwoBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoBooleansAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtom>*> (v)) {
        S_oahTwoBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoBooleansAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahTwoBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoBooleansAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoBooleansAtom>*> (v)) {
        S_oahTwoBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoBooleansAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahTwoBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoBooleansAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahTwoBooleansAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "TwoBooleansAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    setw (fieldWidth) <<
    "fDescription" << " : " <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "booleanVariable" << " : " <<
    booleanAsString (
      fBooleanVariable) <<
    endl <<
    setw (fieldWidth) <<
    "booleanSecondaryVariable" << " : " <<
    booleanAsString (
      fBooleanSecondaryVariable) <<
    endl;

  --gIndenter;
}

void oahTwoBooleansAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    booleanAsString (fBooleanVariable);
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahTwoBooleansAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahThreeBooleansAtom oahThreeBooleansAtom::create (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable,
  bool&  booleanSecondaryVariable,
  bool&  booleanTertiaryVariable)
{
  oahThreeBooleansAtom* o = new
    oahThreeBooleansAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable,
      booleanSecondaryVariable,
      booleanTertiaryVariable);
  assert (o!=0);
  return o;
}

oahThreeBooleansAtom::oahThreeBooleansAtom (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable,
  bool&  booleanSecondaryVariable,
  bool&  booleanTertiaryVariable)
  : oahBooleanAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable),
    fBooleanSecondaryVariable (
      booleanSecondaryVariable),
    fBooleanTertiaryVariable (
      booleanTertiaryVariable)
{}

oahThreeBooleansAtom::~oahThreeBooleansAtom ()
{}

void oahThreeBooleansAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahThreeBooleansAtom" <<
      endl;
  }
#endif

  setThreeBooleansVariables (true);
}

void oahThreeBooleansAtom::setThreeBooleansVariables (bool value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' three booleans variables to '" <<
      booleanAsString (value) <<
      "'" <<
      endl;
  }
#endif

  fBooleanVariable = value;
  fBooleanSecondaryVariable = value;
  fBooleanTertiaryVariable = value;
  fVariableHasBeenSet = true;
}

void oahThreeBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahThreeBooleansAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahThreeBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahThreeBooleansAtom>*> (v)) {
        S_oahThreeBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahThreeBooleansAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahThreeBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahThreeBooleansAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahThreeBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahThreeBooleansAtom>*> (v)) {
        S_oahThreeBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahThreeBooleansAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahThreeBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahThreeBooleansAtom::browseData ()" <<
      endl;
  }
#endif
}

void oahThreeBooleansAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "ThreeBooleansAtom:" <<
    endl;

  ++gIndenter;

  oahElement::printOahElementEssentials (
    os, fieldWidth);

  os <<
    setw (fieldWidth) <<
    "fDescription" << " : " <<
    gIndenter.indentMultiLineString (
      fDescription) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "booleanVariable" << " : " <<
    booleanAsString (
      fBooleanVariable) <<
    endl <<
    setw (fieldWidth) <<
    "booleanSecondaryVariable" << " : " <<
    booleanAsString (
      fBooleanSecondaryVariable) <<
    endl <<
    setw (fieldWidth) <<
    "booleanTertiaryVariable" << " : " <<
    booleanAsString (
      fBooleanTertiaryVariable) <<
    endl;

  --gIndenter;
}

void oahThreeBooleansAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    booleanAsString (
      fBooleanVariable) <<
    endl;
  if (fBooleanVariable) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
}

ostream& operator<< (ostream& os, const S_oahThreeBooleansAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahCombinedBooleansAtom oahCombinedBooleansAtom::create (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable)
{
  oahCombinedBooleansAtom* o = new
    oahCombinedBooleansAtom (
      shortName,
      longName,
      description,
      variableName,
      booleanVariable);
  assert (o!=0);
  return o;
}

oahCombinedBooleansAtom::oahCombinedBooleansAtom (
  string shortName,
  string longName,
  string description,
  string variableName,
  bool&  booleanVariable)
  : oahAtomWithVariableName (
      shortName,
      longName,
      description,
      variableName),
    fBooleanVariable (
      booleanVariable)
{}

oahCombinedBooleansAtom::~oahCombinedBooleansAtom ()
{}

void oahCombinedBooleansAtom::addBooleanAtom (
  S_oahBooleanAtom booleanAtom)
{
  // sanity check
  msgAssert (
    booleanAtom != nullptr,
    "booleanAtom is null");

  fBooleanAtomsList.push_back (
    booleanAtom);
}

void oahCombinedBooleansAtom::addBooleanAtomByName (
  string name)
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomHandlerUpLink ();

  // sanity check
  msgAssert (
    handler != nullptr,
    "handler is null");

  // is name known in options map?
  S_oahElement
    element =
      handler->
        fetchNameInNamesToElementsMap (name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    stringstream s;

    s <<
      "Option name \"" << name <<
      "\" is unknown";

    oahError (s.str ());
  }

  else {
    // name is known, let's handle it

    if (
      // boolean atom?
      S_oahBooleanAtom
        atom =
          dynamic_cast<oahBooleanAtom*>(&(*element))
      ) {
      // handle the atom
      fBooleanAtomsList.push_back (atom);
    }

    else {
      stringstream s;

      s <<
        "option name \"" << name << "\" is not that of an atom";

      oahError (s.str ());
    }
  }
}

void oahCombinedBooleansAtom::setCombinedBooleanVariables (bool value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' combined boolean variables to '" <<
      booleanAsString (value) <<
      "'" <<
      endl;
  }
#endif

  // set the combined atoms atom variable
  fBooleanVariable = value;

  // set the value of the atoms in the list
  if (fBooleanAtomsList.size ()) {
    for (
      list<S_oahBooleanAtom>::const_iterator i =
        fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      ++i
    ) {
      S_oahAtom atom = (*i);

      if (
        // boolean atom?
        S_oahBooleanAtom
          booleanAtom =
            dynamic_cast<oahBooleanAtom*>(&(*atom))
        ) {
        // set the boolean variable
        booleanAtom->
          setBooleanVariable (value);
      }
    } // for
  }
}

void oahCombinedBooleansAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahCombinedBooleansAtom" <<
      endl;
  }
#endif

  setCombinedBooleanVariables (true);
}

void oahCombinedBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahCombinedBooleansAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahCombinedBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCombinedBooleansAtom>*> (v)) {
        S_oahCombinedBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahCombinedBooleansAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahCombinedBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahCombinedBooleansAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahCombinedBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahCombinedBooleansAtom>*> (v)) {
        S_oahCombinedBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahCombinedBooleansAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahCombinedBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahCombinedBooleansAtom::browseData ()" <<
      endl;
  }
#endif

  // browse the boolean atoms
  if (fBooleanAtomsList.size ()) {
    for (
      list<S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      ++i
    ) {
      S_oahBooleanAtom booleanAtom = (*i);

      // browse the boolean atom
      oahBrowser<oahBooleanAtom> browser (v);
      browser.browse (*(booleanAtom));
    } // for
  }
}

void oahCombinedBooleansAtom::printCombinedBooleansEssentials (
  ostream& os,
  unsigned int fieldWidth) const
{
  printAtomWithVariableNameEssentials (
    os, fieldWidth);

/* JMI
  os << left <<
    setw (fieldWidth) <<
    "valueSpecification" << " : " <<
    fValueSpecification <<
    endl;
    */
}

void oahCombinedBooleansAtom::printCombinedBooleansEssentialsShort (
  ostream& os,
  unsigned int fieldWidth) const
{

  printAtomWithVariableNameEssentialsShort (
    os, fieldWidth);

/* JMI
  os << left <<
    setw (fieldWidth) <<
    "valueSpecification" << " : " <<
    fValueSpecification <<
    endl;
    */
}

void oahCombinedBooleansAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "CombinedBooleansAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fBooleanAtomsList" << " : ";

  if (! fBooleanAtomsList.size ()) {
    os <<
      "none";
  }

  else {
    os << endl;

    ++gIndenter;

    os << "'";

    list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os << "'";

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

void oahCombinedBooleansAtom::printHelp (ostream& os) const
{
  os <<
    fetchNames () <<
    ":" <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
  }

  os <<
    "This combined option is equivalent to: ";

  if (! fBooleanAtomsList.size ()) {
    os <<
      "none" <<
      endl;
  }

  else {
    os << endl;

    ++gIndenter;

    list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahBooleanAtom booleanAtom = (*i);

      os <<
        booleanAtom-> fetchNames () <<
        ":" <<
        endl;

      ++gIndenter;

      os <<
        gIndenter.indentMultiLineString (
          booleanAtom-> getDescription ()) <<
        endl;

      --gIndenter;

      if (++i == iEnd) break;
//      os << endl;
    } // for

    --gIndenter;
  }

  if (fDescription.size ()) {
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahCombinedBooleansAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    booleanAsString (
      fBooleanVariable) <<
    endl;

  unsigned int fieldWidth =
    valueFieldWidth - gIndenter.getIndent () + 1;

  ++gIndenter; // only now

  if (! fBooleanAtomsList.size ()) {
    os <<
      "none" <<
      endl;
  }

  else {
    list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_oahAtom
        atom = (*i);

      if (
        // boolean atom?
        S_oahBooleanAtom
          booleanAtom =
            dynamic_cast<oahBooleanAtom*>(&(*atom))
        ) {
        // print the boolean value
        booleanAtom->
          printAtomWithValueOptionsValues (
            os, fieldWidth);
      }

      if (++i == iEnd) break;

  // JMI    os << endl;
    } // for
  }

  --gIndenter;

}

ostream& operator<< (ostream& os, const S_oahCombinedBooleansAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahMultiplexBooleansAtom oahMultiplexBooleansAtom::create (
  string      shortName,
  string      longName,
  string      description,
  string      shortSuffixDescriptor,
  string      longSuffixDescriptor,
  S_oahPrefix shortNamesPrefix,
  S_oahPrefix longNamesPrefix)
{
  oahMultiplexBooleansAtom* o = new
    oahMultiplexBooleansAtom (
      shortName,
      longName,
      description,
      shortSuffixDescriptor,
      longSuffixDescriptor,
      shortNamesPrefix,
      longNamesPrefix);
  assert (o!=0);
  return o;
}

oahMultiplexBooleansAtom::oahMultiplexBooleansAtom (
  string      shortName,
  string      longName,
  string      description,
  string      shortSuffixDescriptor,
  string      longSuffixDescriptor,
  S_oahPrefix shortNamesPrefix,
  S_oahPrefix longNamesPrefix)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementWithoutValue)
{
  // sanity checks
  msgAssert (
    shortNamesPrefix != nullptr,
    "shortNamesPrefix is null");
  msgAssert (
    longNamesPrefix != nullptr,
    "fLongNamesPrefix is null");

  fShortSuffixDescriptor = shortSuffixDescriptor;
  fLongSuffixDescriptor  = longSuffixDescriptor;

  fShortNamesPrefix = shortNamesPrefix;
  fLongNamesPrefix  = longNamesPrefix;

  // get prefixes names
  fShortNamesPrefixName =
    fShortNamesPrefix->getPrefixName ();
  fLongNamesPrefixName =
    fLongNamesPrefix->getPrefixName ();
}

oahMultiplexBooleansAtom::~oahMultiplexBooleansAtom ()
{}

void oahMultiplexBooleansAtom::addBooleanAtom (
  S_oahBooleanAtom booleanAtom)
{
  // sanity check
  msgAssert (
    booleanAtom != nullptr,
    "booleanAtom is null");

  // short name consistency check
  {
    string booleanAtomShortName =
      booleanAtom->getShortName ();

    std::size_t found =
      booleanAtomShortName.find (fShortNamesPrefixName);

    if (found == string::npos) {
      stringstream s;

      s <<
        "Option short name \"" <<
        booleanAtomShortName <<
        "\" is different that the short names prefix name \"" <<
        fShortNamesPrefixName <<
        "\"";

      booleanAtom->print (s);

      oahError (s.str ());
    }

    else if (found != 0) {
      stringstream s;

      s <<
        "Option short name \"" <<
        booleanAtomShortName <<
        "\" doesn't start by the short names prefix name \"" <<
        fShortNamesPrefixName <<
        "\"";

      booleanAtom->print (s);

      oahError (s.str ());
    }

    else {
      string booleanAtomShortNameSuffix =
        booleanAtomShortName.substr (
          fShortNamesPrefixName.size ());

      if (booleanAtomShortNameSuffix.size () == 0) {
        stringstream s;

        s <<
          "Option short name \"" <<
          booleanAtomShortName <<
          "\" is nothing more than the short names prefix name \"" <<
          fShortNamesPrefixName <<
          "\"";

        booleanAtom->print (s);

        oahError (s.str ());
      }
      else {
        // register this boolean atom's suffix in the list
        fShortNamesSuffixes.push_back (booleanAtomShortNameSuffix);
      }
    }
  }

  // long name consistency check
  {
    string booleanAtomLongName =
      booleanAtom->getLongName ();

    if (booleanAtomLongName.size ()) {
      std::size_t found =
        booleanAtomLongName.find (fLongNamesPrefixName);

      if (found == string::npos) {
        stringstream s;

        s <<
          "Option long name \"" <<
          booleanAtomLongName <<
          "\" is different that the long names prefix name \"" <<
          fLongNamesPrefixName <<
          "\"";

        booleanAtom->print (s);

        oahError (s.str ());
      }

      else if (found != 0) {
        stringstream s;

        s <<
          "Option long name \"" <<
          booleanAtomLongName <<
          "\" doesn't start by the long names prefix name \"" <<
          fLongNamesPrefixName <<
          "\"";

        booleanAtom->print (s);

        oahError (s.str ());
      }

      else {
        string booleanAtomLongNameSuffix =
          booleanAtomLongName.substr (
            fLongNamesPrefixName.size ());

        if (booleanAtomLongNameSuffix.size () == 0) {
          stringstream s;

          s <<
            "Option long name \"" <<
            booleanAtomLongName <<
            "\" is nothing more than the long names prefix name \"" <<
            fLongNamesPrefixName <<
            "\"";

          booleanAtom->print (s);

          oahError (s.str ());
        }
        else {
          // register this boolean atom's suffix in the list
          fLongNamesSuffixes.push_back (booleanAtomLongNameSuffix);
        }
      }
    }

    else {
      stringstream s;

      s <<
        "Option long name \"" <<
        booleanAtomLongName <<
        "\" is empty, atom \"" <<
        fLongNamesPrefixName <<
        "\" cannot be used in a multiplex booleans atom";

      booleanAtom->print (s);

      oahError (s.str ());
    }
  }

  // append the boolean atom to the list
  fBooleanAtomsList.push_back (
    booleanAtom);

  // make this atom imvisible
  booleanAtom->
    setElementVisibilityKind (
      kElementVisibilityNone);
}

void oahMultiplexBooleansAtom::addBooleanAtomByName (
  string name)
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomHandlerUpLink ();

  // sanity check
  msgAssert (
    handler != nullptr,
    "handler is null");

  // is name known in options map?
  S_oahElement
    element =
      handler->
        fetchNameInNamesToElementsMap (name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    stringstream s;

    s <<
      "Option name \"" << name <<
      "\" is unknown";

    oahError (s.str ());
  }

  else {
    // name is known, let's handle it

    if (
      // boolean atom?
      S_oahBooleanAtom
        atom =
          dynamic_cast<oahBooleanAtom*>(&(*element))
      ) {
      // add the boolean atom
      addBooleanAtom (atom);
    }

    else {
      stringstream s;

      s <<
        "option name \"" << name << "\" is not that of an atom";

      oahError (s.str ());
    }
  }
}

void oahMultiplexBooleansAtom::applyElement (ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahMultiplexBooleansAtom" <<
      endl;
  }
#endif

  // handle it at once JMI ???
}

void oahMultiplexBooleansAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMultiplexBooleansAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMultiplexBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahMultiplexBooleansAtom>*> (v)) {
        S_oahMultiplexBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMultiplexBooleansAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahMultiplexBooleansAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMultiplexBooleansAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMultiplexBooleansAtom>*
    p =
      dynamic_cast<visitor<S_oahMultiplexBooleansAtom>*> (v)) {
        S_oahMultiplexBooleansAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMultiplexBooleansAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahMultiplexBooleansAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMultiplexBooleansAtom::browseData ()" <<
      endl;
  }
#endif

  if (fShortNamesPrefix) {
    // browse the short names prefix
    oahBrowser<oahPrefix> browser (v);
    browser.browse (*(fShortNamesPrefix));
  }

  if (fLongNamesPrefix) {
    // browse the long names prefix
    oahBrowser<oahPrefix> browser (v);
    browser.browse (*(fLongNamesPrefix));
  }

  // browse the boolean atoms
  if (fBooleanAtomsList.size ()) {
    for (
      list<S_oahBooleanAtom>::const_iterator i = fBooleanAtomsList.begin ();
      i != fBooleanAtomsList.end ();
      ++i
    ) {
      S_oahBooleanAtom booleanAtom = (*i);

      // browse the boolean atom
      oahBrowser<oahBooleanAtom> browser (v);
      browser.browse (*(booleanAtom));
    } // for
  }
}

void oahMultiplexBooleansAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MultiplexBooleansAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    "shortSuffixDescriptor" << " : " <<
    fShortSuffixDescriptor <<
    endl <<
    "longSuffixDescriptor" << " : " <<
    fLongSuffixDescriptor <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "shortNamesPrefix" << " : ";
  if (fShortNamesPrefix) {
    os << fShortNamesPrefix;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "longNamesPrefix" << " : ";
  if (fLongNamesPrefix) {
    os << fLongNamesPrefix;
  }
  else {
    os << "null" << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fBooleanAtomsList" << " : ";

  if (! fBooleanAtomsList.size ()) {
    os << "none";
  }
  else {
    os << endl;

    ++gIndenter;

    list<S_oahBooleanAtom>::const_iterator
      iBegin = fBooleanAtomsList.begin (),
      iEnd   = fBooleanAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    --gIndenter;
  }
  os << endl;

  --gIndenter;
}

void oahMultiplexBooleansAtom::printHelp (ostream& os) const
{
  os <<
    "-" << fShortNamesPrefixName << "<" << fShortSuffixDescriptor << ">" <<
    ", " <<
    "-" << fLongNamesPrefixName << "-<" << fLongSuffixDescriptor << ">" <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;

    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }

  os <<
    "The " <<
    fShortNamesSuffixes.size () <<
    " known " << fShortSuffixDescriptor << "s are: ";

  if (fShortNamesSuffixes.size ()) {
    os << endl;
    ++gIndenter;

    list<string>::const_iterator
      iBegin = fShortNamesSuffixes.begin (),
      iEnd   = fShortNamesSuffixes.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string suffix = (*i);

      cumulatedLength += suffix.size ();
      if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) {
        os << endl;
        cumulatedLength = 0;
        break;
      }

      os << suffix;

      if (++i == iEnd) break;

      if (next (i) == iEnd) {
        os << " and ";
      }
      else {
        os << ", ";
      }
    } // for

    os << "." << endl;
    --gIndenter;
  }
  else {
    os <<
      "none" <<
      endl;
  }

  if (fLongSuffixDescriptor != fShortSuffixDescriptor) {
    int longNamesSuffixesCount = 0;

    {
      list<string>::const_iterator
        iBegin = fLongNamesSuffixes.begin (),
        iEnd   = fLongNamesSuffixes.end (),
        i      = iBegin;

      for ( ; ; ) {
        if ((*i).size ()) {
          ++longNamesSuffixesCount;
        }

        if (++i == iEnd) break;
      } // for
    }

    os <<
      "The " <<
      fLongNamesSuffixes.size () <<
 // JMI     " -- " << longNamesSuffixesCount <<
      " known " << fLongSuffixDescriptor << "s are: ";

    if (fLongNamesSuffixes.size ()) {
      os << endl;
      ++gIndenter;

      list<string>::const_iterator
        iBegin = fLongNamesSuffixes.begin (),
        iEnd   = fLongNamesSuffixes.end (),
        i      = iBegin;

      int cumulatedLength = 0;

      for ( ; ; ) {
        string suffix = (*i);

        cumulatedLength += suffix.size ();
        if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) {
          os << endl;
          cumulatedLength = 0;
//        break;
        }

        os << suffix;

        if (++i == iEnd) break;

        if (next (i) == iEnd) {
          os << " and ";
        }
        else {
          os << ", ";
        }
      } // for

      os << "." << endl;
      --gIndenter;
    }
    else {
      os <<
        "none" <<
        endl;
    }
  }

  if (fDescription.size ()) { // ??? JMI
// JMI    g_Indenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahMultiplexBooleansAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the boolean atoms in the list
}

ostream& operator<< (ostream& os, const S_oahMultiplexBooleansAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahIntegerAtom oahIntegerAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  int&   integerVariable)
{
  oahIntegerAtom* o = new
    oahIntegerAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      integerVariable);
  assert (o!=0);
  return o;
}

oahIntegerAtom::oahIntegerAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  int&   integerVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fIntegerVariable (
      integerVariable)
{}

oahIntegerAtom::~oahIntegerAtom ()
{}

void oahIntegerAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  // theString contains the integer value

  // check whether it is well-formed
  string regularExpression (
    "([[:digit:]]+)");

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        "[" << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize) {
    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << theString;
      s >> integerValue;
    }

    setIntegerVariable (integerValue);
  }

  else {
    stringstream s;

    s <<
      "integer value \"" << theString <<
      "\" for option \"" << fetchNames () <<
      "\" is ill-formed";

    oahError (s.str ());
  }
}

void oahIntegerAtom::setIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' integer variable to '" <<
      value <<
      "'" <<
      endl;
  }
#endif

  fIntegerVariable = value;
  fVariableHasBeenSet = true;
}

void oahIntegerAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntegerAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahIntegerAtom>*> (v)) {
        S_oahIntegerAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIntegerAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahIntegerAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntegerAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahIntegerAtom>*> (v)) {
        S_oahIntegerAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIntegerAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahIntegerAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntegerAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahIntegerAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fIntegerVariable;

  return s.str ();
}

string oahIntegerAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fIntegerVariable;

  return s.str ();
}

void oahIntegerAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntegerAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "integerVariable" << " : " <<
    fIntegerVariable <<
    endl;

  --gIndenter;
}

void oahIntegerAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fIntegerVariable;
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahIntegerAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahTwoIntegersAtom oahTwoIntegersAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  int&   integerVariable,
  int&   integerSecondaryVariable)
{
  oahTwoIntegersAtom* o = new
    oahTwoIntegersAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      integerVariable,
      integerSecondaryVariable);
  assert (o!=0);
  return o;
}

oahTwoIntegersAtom::oahTwoIntegersAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  int&   integerVariable,
  int&   integerSecondaryVariable)
  : oahIntegerAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      integerVariable),
    fIntegerSecondaryVariable (
      integerSecondaryVariable)
{}

oahTwoIntegersAtom::~oahTwoIntegersAtom ()
{}

void oahTwoIntegersAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  // theString contains the two integer values

  // check whether it is well-formed
  string regularExpression (
    "([[:digit:]]+)" // integer value
    "[[:space:]]+"
    "([[:digit:]]+)" // integer secondary value
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        "[" << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 3) {
    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << sm [ 1 ];
      s >> integerValue;
    }
    fIntegerVariable = integerValue;

    {
      stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fIntegerSecondaryVariable = integerValue;
  }

  else {
    stringstream s;

    s <<
      "integer value '" << theString <<
      "' for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahTwoIntegersAtom::setIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' two integers variable to '" <<
      value <<
      "'" <<
      endl;
  }
#endif

  fIntegerVariable = value;
  fVariableHasBeenSet = true;
}

void oahTwoIntegersAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoIntegersAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoIntegersAtom>*> (v)) {
        S_oahTwoIntegersAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoIntegersAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahTwoIntegersAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoIntegersAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahTwoIntegersAtom>*> (v)) {
        S_oahTwoIntegersAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahTwoIntegersAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahTwoIntegersAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahTwoIntegersAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahTwoIntegersAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName <<
    " \"" <<
    fIntegerVariable <<
    " " <<
    fIntegerSecondaryVariable <<
    "\"";

  return s.str ();
}

string oahTwoIntegersAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName <<
    " \"" <<
    fIntegerVariable <<
    " " <<
    fIntegerSecondaryVariable <<
    "\"";

  return s.str ();
}

void oahTwoIntegersAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntegerAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "integerVariable" << " : " <<
    fIntegerVariable <<
    endl <<
    setw (fieldWidth) <<
    "integerSecondaryVariable" << " : " <<
    fIntegerSecondaryVariable <<
    endl;

  --gIndenter;
}

void oahTwoIntegersAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    "\"" <<
    fIntegerVariable <<
    " " <<
    fIntegerSecondaryVariable <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahTwoIntegersAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahFloatAtom oahFloatAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  float& floatVariable)
{
  oahFloatAtom* o = new
    oahFloatAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      floatVariable);
  assert (o!=0);
  return o;
}

oahFloatAtom::oahFloatAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  float& floatVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fFloatVariable (
      floatVariable)
{}

oahFloatAtom::~oahFloatAtom ()
{}

void oahFloatAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  // theString contains the float value

  // check whether it is well-formed
  string regularExpression (
    // no sign, a '-' would be handled as an option name JMI   "([+|-]?)"
    "([[:digit:]]+)(.[[:digit:]]*)?"
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for float string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        "[" << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 3) {
    // leave the low level details to the STL...
    float floatValue;
    {
      stringstream s;

      s << sm [ 0 ];
      s >> floatValue;
    }

    setFloatVariable (floatValue);
  }

  else {
    stringstream s;

    s <<
      "float value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahFloatAtom::setFloatVariable (float value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' float variable to '" <<
      value <<
      "'" <<
      endl;
  }
#endif

  fFloatVariable = value;
  fVariableHasBeenSet = true;
}

void oahFloatAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFloatAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahFloatAtom>*
    p =
      dynamic_cast<visitor<S_oahFloatAtom>*> (v)) {
        S_oahFloatAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFloatAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahFloatAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFloatAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahFloatAtom>*
    p =
      dynamic_cast<visitor<S_oahFloatAtom>*> (v)) {
        S_oahFloatAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFloatAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahFloatAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFloatAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahFloatAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fFloatVariable;

  return s.str ();
}

string oahFloatAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fFloatVariable;

  return s.str ();
}

void oahFloatAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FloatAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "floatVariable" << " : " <<
    fFloatVariable <<
    endl;

  --gIndenter;
}

void oahFloatAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fFloatVariable;
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahFloatAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringAtom oahStringAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable)
{
  oahStringAtom* o = new
    oahStringAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o!=0);
  return o;
}

oahStringAtom::oahStringAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      stringVariable)
{}

oahStringAtom::~oahStringAtom ()
{}

void oahStringAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  setStringVariable (theString);
}

void oahStringAtom::setStringVariable (string value)
  {
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
     value <<
      "\"" <<
      endl;
  }
#endif

  fStringVariable = value;
  fVariableHasBeenSet = true;
}

void oahStringAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAtom>*> (v)) {
        S_oahStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAtom>*> (v)) {
        S_oahStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " '" << fStringVariable << "'";

  return s.str ();
}

string oahStringAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " '" << fStringVariable << "'";

  return s.str ();
}

void oahStringAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl;

  --gIndenter;
}

void oahStringAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    fStringVariable <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahStringAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahMonoplexStringAtom oahMonoplexStringAtom::create (
  string shortName,
  string longName,
  string description,
  string atomNameDescriptor,
  string stringValueDescriptor)
{
  oahMonoplexStringAtom* o = new
    oahMonoplexStringAtom (
      shortName,
      longName,
      description,
      atomNameDescriptor,
      stringValueDescriptor);
  assert (o!=0);
  return o;
}

oahMonoplexStringAtom::oahMonoplexStringAtom (
  string shortName,
  string longName,
  string description,
  string atomNameDescriptor,
  string stringValueDescriptor)
  : oahAtom (
      shortName,
      longName,
      description,
      kElementWithoutValue),
    fAtomNameDescriptor (
      atomNameDescriptor),
    fStringValueDescriptor (
      stringValueDescriptor)
{
  // sanity checks
  msgAssert (
    stringValueDescriptor.size () > 0,
    "stringValueDescriptor is empty");
  msgAssert (
    stringValueDescriptor.size () > 0,
    "stringValueDescriptor is empty");
}

oahMonoplexStringAtom::~oahMonoplexStringAtom ()
{}

void oahMonoplexStringAtom::addStringAtom (
  S_oahStringAtom stringAtom)
{
  // sanity check
  msgAssert (
    stringAtom != nullptr,
    "stringAtom is null");

  // atom short name consistency check
  {
    string stringAtomShortName =
      stringAtom->getShortName ();

    if (stringAtomShortName.size () == 0) {
      stringstream s;

      s <<
        "Option short name \"" << stringAtomShortName <<
        "\" is empty";

      stringAtom->print (s);

      oahError (s.str ());
    }
    else {
      // register this string atom's suffix in the list
      fAtomNamesList.push_back (stringAtomShortName);
    }
  }

  // atom long name consistency check
  {
    string stringAtomLongName =
      stringAtom->getLongName ();

    if (stringAtomLongName.size () != 0) {
      stringstream s;

      s <<
        "Option long name \"" << stringAtomLongName <<
        "\" is not empty";

      stringAtom->print (s);

      oahError (s.str ());
    }
  }

  // append the string atom to the list
  fStringAtomsList.push_back (
    stringAtom);

  // make this atom imvisible
  stringAtom->
    setElementVisibilityKind (
      kElementVisibilityNone);
}

void oahMonoplexStringAtom::addStringAtomByName (
  string name)
{
  // get the options handler
  S_oahHandler
    handler =
      fetchAtomHandlerUpLink ();

  // sanity check
  msgAssert (
    handler != nullptr,
    "handler is null");

  // is name known in options map?
  S_oahElement
    element =
      handler->
        fetchNameInNamesToElementsMap (name);

  if (! element) {
    // no, name is unknown in the map
    handler->
      printOptionsSummary ();

    stringstream s;

    s <<
      "Option name \"" << name <<
      "\" is unknown";

    oahError (s.str ());
  }

  else {
    // name is known, let's handle it

    if (
      // string atom?
      S_oahStringAtom
        atom =
          dynamic_cast<oahStringAtom*>(&(*element))
      ) {
      // add the string atom
      addStringAtom (atom);
    }

    else {
      stringstream s;

      s <<
        "option name \"" << name << "\" is not that of an atom";

      oahError (s.str ());
    }
  }
}

void oahMonoplexStringAtom::applyElement (ostream& os) // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Applying monoplex string atom '" <<
      fetchNames () <<
      "' which is a oahMonoplexStringAtom" <<
      endl;
  }
#endif

  // handle it at once JMI ???
}

void oahMonoplexStringAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMonoplexStringAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMonoplexStringAtom>*
    p =
      dynamic_cast<visitor<S_oahMonoplexStringAtom>*> (v)) {
        S_oahMonoplexStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMonoplexStringAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahMonoplexStringAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMonoplexStringAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMonoplexStringAtom>*
    p =
      dynamic_cast<visitor<S_oahMonoplexStringAtom>*> (v)) {
        S_oahMonoplexStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMonoplexStringAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahMonoplexStringAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMonoplexStringAtom::browseData ()" <<
      endl;
  }
#endif

  // browse the string atoms
  if (fStringAtomsList.size ()) {
    for (
      list<S_oahStringAtom>::const_iterator i = fStringAtomsList.begin ();
      i != fStringAtomsList.end ();
      ++i
    ) {
      S_oahStringAtom stringAtom = (*i);

      // browse the string atom
      oahBrowser<oahStringAtom> browser (v);
      browser.browse (*(stringAtom));
    } // for
  }
}

void oahMonoplexStringAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MonoplexStringAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    os, fieldWidth);

  os << left <<
    "atomNameDescriptor" << " : " <<
    fAtomNameDescriptor <<
    endl <<
    "stringValueDescriptor" << " : " <<
    fStringValueDescriptor <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fStringAtomsList" << " : ";

  if (! fStringAtomsList.size ()) {
    os << "none";
  }

  else {
    os << endl;

    ++gIndenter;

    os << "'";

    list<S_oahStringAtom>::const_iterator
      iBegin = fStringAtomsList.begin (),
      iEnd   = fStringAtomsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os << "'";

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

void oahMonoplexStringAtom::printHelp (ostream& os) const
{
  os <<
    "-" << fAtomNameDescriptor << " " << fStringValueDescriptor <<
    endl;

  if (fDescription.size ()) {
    // indent a bit more for readability
    gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);

    os <<
      gIndenter.indentMultiLineString (
        fDescription) <<
      endl;
  }

  os <<
    "The " <<
    fAtomNamesList.size () <<
    " known " << fAtomNameDescriptor << "s are: ";

  if (! fAtomNamesList.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    list<string>::const_iterator
      iBegin = fAtomNamesList.begin (),
      iEnd   = fAtomNamesList.end (),
      i      = iBegin;

    int cumulatedLength = 0;

    for ( ; ; ) {
      string suffix = (*i);

      cumulatedLength += suffix.size ();
      if (cumulatedLength >= K_NAMES_LIST_MAX_LENGTH) {
        os << endl;
        cumulatedLength = 0;
        break;
      }

      os << suffix;

      if (++i == iEnd) break;
      if (next (i) == iEnd) {
        os << " and ";
      }
      else {
        os << ", ";
      }
    } // for

    os << "." << endl;
    --gIndenter;
  }

  if (fDescription.size ()) {
    gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
  }
}

void oahMonoplexStringAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to do, these options values will be printed
  // by the string atoms in the list
}

ostream& operator<< (ostream& os, const S_oahMonoplexStringAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringWithDefaultValueAtom oahStringWithDefaultValueAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable,
  string  defaultStringValue)
{
  oahStringWithDefaultValueAtom* o = new
    oahStringWithDefaultValueAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultStringValue);
  assert (o!=0);
  return o;
}

oahStringWithDefaultValueAtom::oahStringWithDefaultValueAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable,
  string  defaultStringValue)
  : oahStringAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable),
    fDefaultStringValue (
      defaultStringValue)
{
  this->setElementKind (kElementWithOptionalValue);
}

oahStringWithDefaultValueAtom::~oahStringWithDefaultValueAtom ()
{}

void oahStringWithDefaultValueAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahStringWithDefaultValueAtom" <<
      endl;
  }
#endif

  oahStringAtom::applyAtomWithValue (
    theString,
    os);
}

void oahStringWithDefaultValueAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringWithDefaultValueAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringWithDefaultValueAtom>*
    p =
      dynamic_cast<visitor<S_oahStringWithDefaultValueAtom>*> (v)) {
        S_oahStringWithDefaultValueAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringWithDefaultValueAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringWithDefaultValueAtom::setStringVariable (string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
     value <<
      "\"" <<
      endl;
  }
#endif

  oahStringAtom::setStringVariable (value);
  fVariableHasBeenSet = true;
}

void oahStringWithDefaultValueAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringWithDefaultValueAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringWithDefaultValueAtom>*
    p =
      dynamic_cast<visitor<S_oahStringWithDefaultValueAtom>*> (v)) {
        S_oahStringWithDefaultValueAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringWithDefaultValueAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringWithDefaultValueAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringWithDefaultValueAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringWithDefaultValueAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fStringVariable;

  return s.str ();
}

string oahStringWithDefaultValueAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fStringVariable;

  return s.str ();
}

void oahStringWithDefaultValueAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringWithDefaultValueAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "defaultStringValue" << " : " <<
    "\"" << fDefaultStringValue << "\"" <<
    endl;

  --gIndenter;
}

void oahStringWithDefaultValueAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    fStringVariable <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahStringWithDefaultValueAtom& elt)
{
  os <<
    "StringWithDefaultValueAtom:" <<
    endl;
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahRationalAtom oahRationalAtom::create (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  rational& rationalVariable)
{
  oahRationalAtom* o = new
    oahRationalAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      rationalVariable);
  assert (o!=0);
  return o;
}

oahRationalAtom::oahRationalAtom (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  rational& rationalVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fRationalVariable (
      rationalVariable)
{}

oahRationalAtom::~oahRationalAtom ()
{}

void oahRationalAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  // theString contains the fraction:
  // decipher it to extract numerator and denominator values

  string regularExpression (
    "[[:space:]]*([[:digit:]]+)[[:space:]]*" // numerator
    "/"
    "[[:space:]]*([[:digit:]]+)[[:space:]]*" // denominator
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for rational string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        "[" << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 3) { // JMI ???
    int
      numerator,
      denominator;

    {
      stringstream s;
      s << sm [1];
      s >> numerator;
    }
    {
      stringstream s;
      s << sm [2];
      s >> denominator;
    }

    rational
      rationalValue =
        rational (numerator, denominator);

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      gLogStream <<
        "rationalValue = " <<
        rationalValue <<
        endl;
    }
#endif

    setRationalVariable (rationalValue);
  }

  else {
    stringstream s;

    s <<
      "rational atom value \"" << theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }
}

void oahRationalAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRationalAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRationalAtom>*
    p =
      dynamic_cast<visitor<S_oahRationalAtom>*> (v)) {
        S_oahRationalAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahRationalAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahRationalAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRationalAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRationalAtom>*
    p =
      dynamic_cast<visitor<S_oahRationalAtom>*> (v)) {
        S_oahRationalAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahRationalAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahRationalAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRationalAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahRationalAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fRationalVariable;

  return s.str ();
}

string oahRationalAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fRationalVariable;

  return s.str ();
}

void oahRationalAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "RationalAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "rationalVariable" << " : " <<
    fRationalVariable <<
    endl;

  --gIndenter;
}

void oahRationalAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fRationalVariable;
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahRationalAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahNaturalNumbersSetAtom oahNaturalNumbersSetAtom::create (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& naturalNumbersSetVariable)
{
  oahNaturalNumbersSetAtom* o = new
    oahNaturalNumbersSetAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      naturalNumbersSetVariable);
  assert (o!=0);
  return o;
}

oahNaturalNumbersSetAtom::oahNaturalNumbersSetAtom (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& naturalNumbersSetVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fNaturalNumbersSetVariable (
      naturalNumbersSetVariable)
{}

oahNaturalNumbersSetAtom::~oahNaturalNumbersSetAtom ()
{}

void oahNaturalNumbersSetAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  fNaturalNumbersSetVariable =
    decipherNaturalNumbersSetSpecification (
      theString,
      false); // 'true' to debug it
}

void oahNaturalNumbersSetAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahNaturalNumbersSetAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahNaturalNumbersSetAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetAtom>*> (v)) {
        S_oahNaturalNumbersSetAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahNaturalNumbersSetAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahNaturalNumbersSetAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahNaturalNumbersSetAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahNaturalNumbersSetAtom>*
    p =
      dynamic_cast<visitor<S_oahNaturalNumbersSetAtom>*> (v)) {
        S_oahNaturalNumbersSetAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahNaturalNumbersSetAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahNaturalNumbersSetAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahNaturalNumbersSetAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahNaturalNumbersSetAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "[";

  set<int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

string oahNaturalNumbersSetAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "[";

  set<int>::const_iterator
    iBegin = fNaturalNumbersSetVariable.begin (),
    iEnd   = fNaturalNumbersSetVariable.end (),
    i      = iBegin;

  for ( ; ; ) {
    s << (*i);
    if (++i == iEnd) break;
    s << " ";
  } // for

  s <<
    "]";

  return s.str ();
}

void oahNaturalNumbersSetAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "NaturalNumbersSetAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "naturalNumbersSetVariable" << " : " <<
    endl;

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
      "'";
  }

  os << endl;

  --gIndenter;
}

void oahNaturalNumbersSetAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fNaturalNumbersSetVariable.size ()) {
    os <<
      "none";
  }

  else {
    os <<
      "'";

    set<int>::const_iterator
      iBegin = fNaturalNumbersSetVariable.begin (),
      iEnd   = fNaturalNumbersSetVariable.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << " ";
    } // for

    os <<
        "'";

    if (fVariableHasBeenSet) {
      os <<
        ", variableHasBeenSet: " <<
        booleanAsString (fVariableHasBeenSet);
    }
  }
}

ostream& operator<< (ostream& os, const S_oahNaturalNumbersSetAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahRGBColorAtom oahRGBColorAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  msrRGBColor& RGBColorVariable)
{
  oahRGBColorAtom* o = new
    oahRGBColorAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      RGBColorVariable);
  assert (o!=0);
  return o;
}

oahRGBColorAtom::oahRGBColorAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  msrRGBColor& RGBColorVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fRGBColorVariable (
      RGBColorVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahRGBColorAtom::~oahRGBColorAtom ()
{}

void oahRGBColorAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  msrRGBColor theRGBColor (theString);

  setRGBColorVariable (theRGBColor);
}

void oahRGBColorAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRGBColorAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRGBColorAtom>*
    p =
      dynamic_cast<visitor<S_oahRGBColorAtom>*> (v)) {
        S_oahRGBColorAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahRGBColorAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahRGBColorAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRGBColorAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahRGBColorAtom>*
    p =
      dynamic_cast<visitor<S_oahRGBColorAtom>*> (v)) {
        S_oahRGBColorAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahRGBColorAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahRGBColorAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahRGBColorAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahRGBColorAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "[" <<
    fRGBColorVariable.asString () <<
    "]";

  return s.str ();
}

string oahRGBColorAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "[" <<
    fRGBColorVariable.asString () <<
    "]";

  return s.str ();
}

void oahRGBColorAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "RGBColorAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "RGBColorVariable" << " : " <<
    fRGBColorVariable.asString () <<
    endl;

  --gIndenter;
}

void oahRGBColorAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fRGBColorVariable.asString ();
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahRGBColorAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahIntSetAtom oahIntSetAtom::create (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& intSetVariable)
{
  oahIntSetAtom* o = new
    oahIntSetAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      intSetVariable);
  assert (o!=0);
  return o;
}

oahIntSetAtom::oahIntSetAtom (
  string    shortName,
  string    longName,
  string    description,
  string    valueSpecification,
  string    variableName,
  set<int>& intSetVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fIntSetVariable (
      intSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahIntSetAtom::~oahIntSetAtom ()
{}

void oahIntSetAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  // theString contains the bar number specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling atom '" <<
      fetchNames () <<
      "' of type 'oahIntSetAtom'" <<
      " with value \"" << theString << "\"" <<
      endl;
  }
#endif

  string regularExpression (
    "([[:digit:]]+)"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for reset measure number string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 2) {
    stringstream s;

    s <<
      "-boxAroundBarNumber argument \"" << theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  int lilypondMeasureNumber;
  {
    stringstream s;
    s << sm [1];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      endl;
  }
#endif

  fIntSetVariable.insert (lilypondMeasureNumber);
}

void oahIntSetAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntSetAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahIntSetAtom>*
    p =
      dynamic_cast<visitor<S_oahIntSetAtom>*> (v)) {
        S_oahIntSetAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIntSetAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahIntSetAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntSetAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahIntSetAtom>*
    p =
      dynamic_cast<visitor<S_oahIntSetAtom>*> (v)) {
        S_oahIntSetAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahIntSetAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahIntSetAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahIntSetAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahIntSetAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fIntSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string oahIntSetAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fIntSetVariable.size ()) {
    s << "empty";
  }
  else {
    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahIntSetAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "IntSetAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "intSetVariable" << " : '" <<
    endl;

  if (! fIntSetVariable.size ()) {
    os << "empty";
  }
  else {
    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;

  --gIndenter;
}

void oahIntSetAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fIntSetVariable.size ()) {
    os <<
      "empty" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    set<int>::const_iterator
      iBegin = fIntSetVariable.begin (),
      iEnd   = fIntSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i) << endl;
      if (++i == iEnd) break;
    } // for

    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet) <<
      endl;

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_oahIntSetAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringSetAtom oahStringSetAtom::create (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
{
  oahStringSetAtom* o = new
    oahStringSetAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringSetVariable);
  assert (o!=0);
  return o;
}

oahStringSetAtom::oahStringSetAtom (
  string       shortName,
  string       longName,
  string       description,
  string       valueSpecification,
  string       variableName,
  set<string>& stringSetVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringSetVariable (
      stringSetVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringSetAtom::~oahStringSetAtom ()
{}

void oahStringSetAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringSetAtom'" <<
      endl;
  }
#endif

  // theString contains the name of the part to be omitted

  string partName = theString;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "--> partName = \"" << partName << "\", " <<
      endl;
  }
#endif

  // is this part name in the part renaming map?
  set<string>::iterator
    it =
      fStringSetVariable.find (partName);

  if (it != fStringSetVariable.end ()) {
    // yes, issue error message
    stringstream s;

    s <<
      "Part \"" << partName << "\" occurs more that once" <<
      "in the '--msr-omit-part' option";

    oahError (s.str ());
  }

  else {
    fStringSetVariable.insert (partName);
  }
}

void oahStringSetAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringSetAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringSetAtom>*
    p =
      dynamic_cast<visitor<S_oahStringSetAtom>*> (v)) {
        S_oahStringSetAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringSetAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringSetAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringSetAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringSetAtom>*
    p =
      dynamic_cast<visitor<S_oahStringSetAtom>*> (v)) {
        S_oahStringSetAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringSetAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringSetAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringSetAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringSetAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string oahStringSetAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringSetVariable.size ()) {
    s << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahStringSetAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringSetAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "stringSetVariable" << " : " <<
    endl;

  if (! fStringSetVariable.size ()) {
    os << "none";
  }
  else {
    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  os << endl;

  --gIndenter;
}

void oahStringSetAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringSetVariable.size ()) {
    os <<
      "none" <<
      endl;
  }
  else {
    os << endl;

    ++gIndenter;

    set<string>::const_iterator
      iBegin = fStringSetVariable.begin (),
      iEnd   = fStringSetVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" << (*i) << "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet) <<
      endl;

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_oahStringSetAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringToIntMapAtom oahStringToIntMapAtom::create (
  string            shortName,
  string            longName,
  string            description,
  string            valueSpecification,
  string            variableName,
  map<string, int>& stringToIntMapVariable)
{
  oahStringToIntMapAtom* o = new
    oahStringToIntMapAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringToIntMapVariable);
  assert (o!=0);
  return o;
}

oahStringToIntMapAtom::oahStringToIntMapAtom (
  string            shortName,
  string            longName,
  string            description,
  string            valueSpecification,
  string            variableName,
  map<string, int>& stringToIntMapVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringToIntMapVariable (
      stringToIntMapVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringToIntMapAtom::~oahStringToIntMapAtom ()
{}

void oahStringToIntMapAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToIntMapAtom'" <<
      endl;
  }
#endif

  // theString contains the string int map specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahStringToIntMapAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:w:]]+)"     // string
    "[[:space:]]*"
//    "=" JMI
//    "[[:space:]]*"
    "([[:digit:]]+)" // int
    "[[:space:]]*"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for reset measure number string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-resetMeasureElementMeasureNumber argument \"" <<
      theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string musicXMLMeasureNumber = sm [1];

  int lilypondMeasureNumber;
  {
    stringstream s;
    s << sm [2];
    s >> lilypondMeasureNumber;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "musicXMLMeasureNumber = " <<
      musicXMLMeasureNumber <<
      endl <<
      "lilypondMeasureNumber = " <<
      lilypondMeasureNumber <<
      endl;
  }
#endif

  fStringToIntMapVariable [musicXMLMeasureNumber] = lilypondMeasureNumber;
}

void oahStringToIntMapAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToIntMapAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringToIntMapAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToIntMapAtom>*> (v)) {
        S_oahStringToIntMapAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringToIntMapAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringToIntMapAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToIntMapAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringToIntMapAtom>*
    p =
      dynamic_cast<visitor<S_oahStringToIntMapAtom>*> (v)) {
        S_oahStringToIntMapAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringToIntMapAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringToIntMapAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringToIntMapAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringToIntMapAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " ";

  if (! fStringToIntMapVariable.size ()) {
    s << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

string oahStringToIntMapAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " ";

  if (! fStringToIntMapVariable.size ()) {
    s << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i).first << "=" << (*i).second;
      if (++i == iEnd) break;
      s << ",";
    } // for
  }

  return s.str ();
}

void oahStringToIntMapAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionsResetMeasureElementMeasureNumberAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "stringSetVariable" << " : '" <<
    endl;

  if (! fStringToIntMapVariable.size ()) {
    os << "empty";
  }
  else {
    map<string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  os << endl;

  --gIndenter;
}

void oahStringToIntMapAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : ";

  if (! fStringToIntMapVariable.size ()) {
    os <<
      "empty" <<
      endl;
  }
  else {
    os << endl;
    ++gIndenter;

    map<string, int>::const_iterator
      iBegin = fStringToIntMapVariable.begin (),
      iEnd   = fStringToIntMapVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        "\"" <<
        (*i).first <<
        "\" --> \"" <<
        (*i).second <<
        "\"" <<
        endl;
      if (++i == iEnd) break;
    } // for

    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet) <<
      endl;

    --gIndenter;
  }
}

ostream& operator<< (ostream& os, const S_oahStringToIntMapAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringAndIntegerAtom oahStringAndIntegerAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  stringVariableName,
  string& stringVariable,
  string  integerVariableName,
  int&    integerVariable)
{
  oahStringAndIntegerAtom* o = new
    oahStringAndIntegerAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      stringVariableName,
      stringVariable,
      integerVariableName,
      integerVariable);
  assert (o!=0);
  return o;
}

oahStringAndIntegerAtom::oahStringAndIntegerAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  stringVariableName,
  string& stringVariable,
  string  integerVariableName,
  int&    integerVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      stringVariableName),
    fStringVariableName (
      stringVariableName),
    fStringVariable (
      stringVariable),
    fIntegerVariableName (
      integerVariableName),
    fIntegerVariable (
      integerVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringAndIntegerAtom::~oahStringAndIntegerAtom ()
{}

void oahStringAndIntegerAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  // theString contains the string and integer values

  // check whether it is well-formed
  string regularExpression (
    "(.+)"           // string
    "[[:space:]]+"
    "([[:digit:]]+)" // int
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        "[" << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 3) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fIntegerVariable = integerValue;
  }

  else {
    stringstream s;

    s <<
      "integer value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahStringAndIntegerAtom::setIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' integer variable to '" <<
      value <<
      "'" <<
      endl;
  }
#endif

  fIntegerVariable = value;
  fVariableHasBeenSet = true;
}

void oahStringAndIntegerAtom::setStringVariable (string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to \"" <<
      value <<
      "\"" <<
      endl;
  }
#endif

  fStringVariable = value;
  fVariableHasBeenSet = true;
}

void oahStringAndIntegerAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndIntegerAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAndIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndIntegerAtom>*> (v)) {
        S_oahStringAndIntegerAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAndIntegerAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringAndIntegerAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndIntegerAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAndIntegerAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndIntegerAtom>*> (v)) {
        S_oahStringAndIntegerAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAndIntegerAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringAndIntegerAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndIntegerAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringAndIntegerAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return s.str ();
}

string oahStringAndIntegerAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fIntegerVariable;

  return s.str ();
}

void oahStringAndIntegerAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAndIntegerAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "stringVariableName" << " : " <<
    "\"" << fStringVariableName << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "integerVariableName" << " : " <<
    fIntegerVariableName <<
    endl <<
    setw (fieldWidth) <<
    "integerVariable" << " : " <<
    fIntegerVariable <<
    endl;

  --gIndenter;
}

void oahStringAndIntegerAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fStringVariableName <<
    " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<

    setw (valueFieldWidth) <<
    fIntegerVariableName <<
    " : " <<
    "\"" << fIntegerVariable << "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahStringAndIntegerAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahStringAndTwoIntegersAtom oahStringAndTwoIntegersAtom::create (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable,
  int&    primaryIntegerVariable,
  int&    secondaryIntegerVariable)
{
  oahStringAndTwoIntegersAtom* o = new
    oahStringAndTwoIntegersAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      primaryIntegerVariable,
      secondaryIntegerVariable);
  assert (o!=0);
  return o;
}

oahStringAndTwoIntegersAtom::oahStringAndTwoIntegersAtom (
  string  shortName,
  string  longName,
  string  description,
  string  valueSpecification,
  string  variableName,
  string& stringVariable,
  int&    primaryIntegerVariable,
  int&    secondaryIntegerVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fStringVariable (
      stringVariable),
    fPrimaryIntegerVariable (
      primaryIntegerVariable),
    fSecondaryIntegerVariable (
      secondaryIntegerVariable)
{
  fMultipleOccurrencesAllowed = true;
}

oahStringAndTwoIntegersAtom::~oahStringAndTwoIntegersAtom ()
{}

void oahStringAndTwoIntegersAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
  // theString contains the string and integer values

  // check whether it is well-formed
  string regularExpression (
    "([[:alpha:]]+)" // string
    "[[:space:]]+"
    "([[:digit:]]+)" // integer value
    "[[:space:]]+"
    "([[:digit:]]+)" // secondary integer value
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        "[" << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 4) {
    fStringVariable = sm [ 1 ];

    // leave the low level details to the STL...
    int integerValue;
    {
      stringstream s;
      s << sm [ 2 ];
      s >> integerValue;
    }
    fPrimaryIntegerVariable = integerValue; // JMI

    {
      stringstream s;
      s << sm [ 3 ];
      s >> integerValue;
    }
    fSecondaryIntegerVariable = integerValue;
  }

  else {
    stringstream s;

    s <<
      "integer value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahStringAndTwoIntegersAtom::setStringVariable (string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' string variable to '" <<
      value <<
      "'" <<
      endl;
  }
#endif

  fStringVariable = value;
  fVariableHasBeenSet = true;
}

void oahStringAndTwoIntegersAtom::setPrimaryIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' primary integer variable to '" <<
      value <<
      "'" <<
      endl;
  }
#endif

  fPrimaryIntegerVariable = value;
  fVariableHasBeenSet = true;
}

void oahStringAndTwoIntegersAtom::setSecondaryIntegerVariable (int value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Setting option '" <<
      fetchNames () <<
      "' secondary integer variable to '" <<
      value <<
      "'" <<
      endl;
  }
#endif

  fSecondaryIntegerVariable = value;
  fVariableHasBeenSet = true;
}

void oahStringAndTwoIntegersAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAndTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndTwoIntegersAtom>*> (v)) {
        S_oahStringAndTwoIntegersAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAndTwoIntegersAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahStringAndTwoIntegersAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahStringAndTwoIntegersAtom>*
    p =
      dynamic_cast<visitor<S_oahStringAndTwoIntegersAtom>*> (v)) {
        S_oahStringAndTwoIntegersAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahStringAndTwoIntegersAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahStringAndTwoIntegersAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahStringAndTwoIntegersAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahStringAndTwoIntegersAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fPrimaryIntegerVariable <<
    " " <<
    fSecondaryIntegerVariable;

  return s.str ();
}

string oahStringAndTwoIntegersAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    "\"" <<
    fStringVariable <<
    "\" " <<
    fPrimaryIntegerVariable <<
    " " <<
    fSecondaryIntegerVariable;

  return s.str ();
}

void oahStringAndTwoIntegersAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "StringAndIntegerAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    setw (fieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    "primaryIntegerVariable" << " : " <<
    fPrimaryIntegerVariable <<
    "secondaryIntegerVariable" << " : " <<
    fSecondaryIntegerVariable <<
    endl;

  --gIndenter;
}

void oahStringAndTwoIntegersAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<
    setw (valueFieldWidth) <<
    "stringVariable" << " : " <<
    "\"" << fStringVariable << "\"" <<
    endl <<
    setw (valueFieldWidth) <<
    "primaryIntegerVariable" <<
    " : " <<
    fPrimaryIntegerVariable <<
    endl <<
    setw (valueFieldWidth) <<
    "secondaryIntegerVariable" <<
    " : " <<
    fSecondaryIntegerVariable;
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahStringAndTwoIntegersAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahLengthUnitKindAtom oahLengthUnitKindAtom::create (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrLengthUnitKind& lengthUnitKindVariable)
{
  oahLengthUnitKindAtom* o = new
    oahLengthUnitKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lengthUnitKindVariable);
  assert (o!=0);
  return o;
}

oahLengthUnitKindAtom::oahLengthUnitKindAtom (
  string             shortName,
  string             longName,
  string             description,
  string             valueSpecification,
  string             variableName,
  msrLengthUnitKind& lengthUnitKindVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLengthUnitKindVariable (
      lengthUnitKindVariable)
{}

oahLengthUnitKindAtom::~oahLengthUnitKindAtom ()
{}

void oahLengthUnitKindAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthUnitKindAtom'" <<
      endl;
  }
#endif

  map<string, msrLengthUnitKind>::const_iterator
    it =
      gGlobalMsrLengthUnitKindsMap.find (
        theString);

  if (it == gGlobalMsrLengthUnitKindsMap.end ()) {
    // no, score output kind is unknown in the map

    stringstream s;

    s <<
      "LPSR score output kind \"" << theString <<
      "\" is unknown" <<
      endl <<
      "The " <<
      gGlobalMsrLengthUnitKindsMap.size () <<
      " known LPSR score output kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsrLengthUnitKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  setLengthUnitKindVariable (
    (*it).second);
}

void oahLengthUnitKindAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthUnitKindAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahLengthUnitKindAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthUnitKindAtom>*> (v)) {
        S_oahLengthUnitKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLengthUnitKindAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahLengthUnitKindAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthUnitKindAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahLengthUnitKindAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthUnitKindAtom>*> (v)) {
        S_oahLengthUnitKindAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLengthUnitKindAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahLengthUnitKindAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthUnitKindAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahLengthUnitKindAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return s.str ();
}

string oahLengthUnitKindAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    msrLengthUnitKindAsString (fLengthUnitKindVariable);

  return s.str ();
}

void oahLengthUnitKindAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "LengthUnitKindAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "optionsLengthUnitKindVariable" << " : \"" <<
    msrLengthUnitKindAsString (
      fLengthUnitKindVariable) <<
    "\"" <<
    endl;

  --gIndenter;
}

void oahLengthUnitKindAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    msrLengthUnitKindAsString (
      fLengthUnitKindVariable) <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahLengthUnitKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahLengthAtom oahLengthAtom::create (
  string     shortName,
  string     longName,
  string     description,
  string     valueSpecification,
  string     variableName,
  msrLength& lengthVariable)
{
  oahLengthAtom* o = new
    oahLengthAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      lengthVariable);
  assert (o!=0);
  return o;
}

oahLengthAtom::oahLengthAtom (
  string     shortName,
  string     longName,
  string     description,
  string     valueSpecification,
  string     variableName,
  msrLength& lengthVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fLengthVariable (
      lengthVariable)
{}

oahLengthAtom::~oahLengthAtom ()
{}

void oahLengthAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthAtom'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahLengthAtom'" <<
      endl;
  }
#endif

  // theString contains the score output kind:

  // check whether it is well-formed
   // no sign, a '-' would be handled as an option name JMI   "([+|-]?)"
  string regularExpression (
    "([[:digit:]]+)"     // integer part
    "(.[[:digit:]]*)"    // decimal part
    "?([[:alpha:]]{2,})" // length unit
    );

  regex e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for integer string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\"" <<
      endl;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        "[" << sm [i] << "] ";
    } // for

    gLogStream << endl;
  }
#endif

  if (smSize == 4) {
    // leave the low level details to the STL...
    float floatValue;
    {
      stringstream s;
      // concatenate the integer and decimal parts
      s << sm [ 1 ] << sm [ 2 ];
      s >> floatValue;
    }

    string lengthUnitName = sm [ 3 ];

    // is lengthUnitName known in the the length unit names map?
    map<string, msrLengthUnitKind>::const_iterator
      it =
        gGlobalMsrLengthUnitKindsMap.find (
          lengthUnitName);

    if (it == gGlobalMsrLengthUnitKindsMap.end ()) {
      // no, length unit name is unknown in the map

      stringstream s;

      s <<
        "length unit name \"" << lengthUnitName <<
        "\" is unknown" <<
        endl <<
        "The " <<
        gGlobalMsrLengthUnitKindsMap.size () <<
        " known length unit names are:" <<
        endl;

      ++gIndenter;

      s <<
        existingMsrLengthUnitKinds (K_NAMES_LIST_MAX_LENGTH);

      --gIndenter;

      oahError (s.str ());
    }

    setLengthVariable (
      msrLength (
        (*it).second,
        floatValue));
  }

  else {
    stringstream s;

    s <<
      "length value \"" << theString <<
      "\" for option '" << fetchNames () <<
      "' is ill-formed";

    oahError (s.str ());
  }
}

void oahLengthAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahLengthAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthAtom>*> (v)) {
        S_oahLengthAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLengthAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahLengthAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahLengthAtom>*
    p =
      dynamic_cast<visitor<S_oahLengthAtom>*> (v)) {
        S_oahLengthAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahLengthAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahLengthAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahLengthAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahLengthAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    fLengthVariable.asString ();

  return s.str ();
}

string oahLengthAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    fLengthVariable.asString ();

  return s.str ();
}

void oahLengthAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "LengthAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "optionsLengthVariable" << " : " <<
    fLengthVariable.asString () <<
    endl;

  --gIndenter;
}

void oahLengthAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : " <<
    fLengthVariable.asString ();
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahLengthAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahMidiTempoAtom oahMidiTempoAtom::create (
  string        shortName,
  string        longName,
  string        description,
  string        valueSpecification,
  string        variableName,
  msrMidiTempo& midiTempoVariable)
{
  oahMidiTempoAtom* o = new
    oahMidiTempoAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      midiTempoVariable);
  assert (o!=0);
  return o;
}

oahMidiTempoAtom::oahMidiTempoAtom (
  string        shortName,
  string        longName,
  string        description,
  string        valueSpecification,
  string        variableName,
  msrMidiTempo& midiTempoVariable)
  : oahAtomWithValue (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fMidiTempoVariable (
      midiTempoVariable)
{}

oahMidiTempoAtom::~oahMidiTempoAtom ()
{}

void oahMidiTempoAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahMidiTempoAtom'" <<
      endl;
  }
#endif

  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "==> oahAtom is of type 'oahMidiTempoAtom'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)" // midiTempoDuration
    "[[:space:]]*"
    "="
    "[[:space:]]*"
    "([[:digit:]]+)"     // midiTempoPerSecond
    "[[:space:]]*"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for MIDI tempo string \"" << theString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-midiTempo argument \"" << theString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string midiTempoDuration = sm [1];

  int midiTempoPerSecond;
  {
    stringstream s;
    s << sm [2];
    s >> midiTempoPerSecond;
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "midiTempoDuration  = " <<
      midiTempoDuration <<
      endl <<
      "midiTempoPerSecond = " <<
      midiTempoPerSecond <<
      endl;

  msrMidiTempo
    theMidiTempo (
      0, // inputLineNumber
      midiTempoDuration,
      midiTempoPerSecond);

  setMidiTempoVariable (
    theMidiTempo);
  }
#endif
}

void oahMidiTempoAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMidiTempoAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMidiTempoAtom>*
    p =
      dynamic_cast<visitor<S_oahMidiTempoAtom>*> (v)) {
        S_oahMidiTempoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMidiTempoAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahMidiTempoAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMidiTempoAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahMidiTempoAtom>*
    p =
      dynamic_cast<visitor<S_oahMidiTempoAtom>*> (v)) {
        S_oahMidiTempoAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahMidiTempoAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahMidiTempoAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahMidiTempoAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahMidiTempoAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " <<
    fMidiTempoVariable.asString ();

  return s.str ();
}

string oahMidiTempoAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " <<
    fMidiTempoVariable.asString ();

  return s.str ();
}

void oahMidiTempoAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "MidiTempoAtom:" <<
    endl;

  ++gIndenter;

  printAtomWithValueEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "midiTempoVariable" << " : \"" <<
    fMidiTempoVariable.asString () <<
    "\"" <<
    endl;

  --gIndenter;
}

void oahMidiTempoAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    fMidiTempoVariable.asString () <<
    "\"";
  if (fVariableHasBeenSet) {
    os <<
      ", variableHasBeenSet: " <<
      booleanAsString (fVariableHasBeenSet);
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_oahMidiTempoAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahOptionNameHelpAtom oahOptionNameHelpAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  string& stringVariable,
  string  defaultOptionName)
{
  oahOptionNameHelpAtom* o = new
    oahOptionNameHelpAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultOptionName);
  assert (o!=0);
  return o;
}

oahOptionNameHelpAtom::oahOptionNameHelpAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  string& stringVariable,
  string  defaultOptionName)
  : oahStringWithDefaultValueAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable,
      defaultOptionName)
{
  this->setElementKind (kElementWithOptionalValue);

  fElementHelpOnlyKind = kElementHelpOnlyYes;

  this->setMultipleOccurrencesAllowed ();
}

oahOptionNameHelpAtom::~oahOptionNameHelpAtom ()
{}

void oahOptionNameHelpAtom::applyAtomWithValueDefaultValue (ostream& os)
{
  // delegate this to the handler
  fetchAtomHandlerUpLink ()->
    printNameIntrospectiveHelp (
      os,
      fDefaultStringValue);
}

void oahOptionNameHelpAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahOptionNameHelpAtom" <<
      endl;
  }
#endif

  // delegate this to the handler
  fetchAtomHandlerUpLink ()->
    printNameIntrospectiveHelp (
      os,
      theString);
}

void oahOptionNameHelpAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionNameHelpAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionNameHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionNameHelpAtom>*> (v)) {
        S_oahOptionNameHelpAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOptionNameHelpAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahOptionNameHelpAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionNameHelpAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahOptionNameHelpAtom>*
    p =
      dynamic_cast<visitor<S_oahOptionNameHelpAtom>*> (v)) {
        S_oahOptionNameHelpAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahOptionNameHelpAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahOptionNameHelpAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahOptionNameHelpAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahOptionNameHelpAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fVariableName;

  return s.str ();
}

string oahOptionNameHelpAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fVariableName;

  return s.str ();
}

void oahOptionNameHelpAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "OptionNameHelpAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahOptionNameHelpAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahOptionNameHelpAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahAProposOptionNameAtom oahAProposOptionNameAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  string& stringVariable)
{
  oahAProposOptionNameAtom* o = new
    oahAProposOptionNameAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o!=0);
  return o;
}

oahAProposOptionNameAtom::oahAProposOptionNameAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  string& stringVariable)
  : oahStringAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable)
{
  this->setElementKind (kElementWithMandatoryValue);

  fElementHelpOnlyKind = kElementHelpOnlyYes;

  this->setMultipleOccurrencesAllowed ();
}

oahAProposOptionNameAtom::~oahAProposOptionNameAtom ()
{}

void oahAProposOptionNameAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahAProposOptionNameAtom" <<
      endl;
  }
#endif

  // delegate this to the handler
  fetchAtomHandlerUpLink ()->
    printNameIntrospectiveHelp (
      os,
      theString);
}

void oahAProposOptionNameAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAProposOptionNameAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAProposOptionNameAtom>*
    p =
      dynamic_cast<visitor<S_oahAProposOptionNameAtom>*> (v)) {
        S_oahAProposOptionNameAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAProposOptionNameAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahAProposOptionNameAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAProposOptionNameAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahAProposOptionNameAtom>*
    p =
      dynamic_cast<visitor<S_oahAProposOptionNameAtom>*> (v)) {
        S_oahAProposOptionNameAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahAProposOptionNameAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahAProposOptionNameAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahAProposOptionNameAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahAProposOptionNameAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fVariableName;

  return s.str ();
}

string oahAProposOptionNameAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fVariableName;

  return s.str ();
}

void oahAProposOptionNameAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "AProposOptionNameAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahAProposOptionNameAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahAProposOptionNameAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_oahFindStringAtom oahFindStringAtom::create (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  string& stringVariable)
{
  oahFindStringAtom* o = new
    oahFindStringAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable);
  assert (o!=0);
  return o;
}

oahFindStringAtom::oahFindStringAtom (
  string shortName,
  string longName,
  string description,
  string valueSpecification,
  string variableName,
  string& stringVariable)
  : oahStringAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      stringVariable)
{
  this->setElementKind (kElementWithMandatoryValue);

  fElementHelpOnlyKind = kElementHelpOnlyYes;

  this->setMultipleOccurrencesAllowed ();
}

oahFindStringAtom::~oahFindStringAtom ()
{}

void oahFindStringAtom::applyAtomWithValue (
  string   theString,
  ostream& os)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "Handling option name '" <<
      fetchNames () <<
      "' which is a oahFindStringAtom" <<
      endl;
  }
#endif

  // a strings list to collect the results
  list<string> foundStringsList;

  // delegate this to the handler
  fetchAtomHandlerUpLink ()->
    findStringInHandler (
      stringToLowerCase (theString),
      foundStringsList,
      os);

  //  print the found strings
  unsigned int foundStringsListSize =
    foundStringsList.size ();

  int saveIndent = gIndenter.getIndent ();

  gIndenter.resetToZero ();

  os <<
    singularOrPlural (
      foundStringsListSize, "occurrence", "occurrences") <<
    " of string \"" <<
    theString <<
    "\" " <<
    singularOrPluralWithoutNumber (
      foundStringsListSize, "has", "have") <<
    " been found";

  if (foundStringsListSize) {
    os <<
      ":" <<
      endl;

    ++gIndenter;

    list<string>::const_iterator
      iBegin = foundStringsList.begin (),
      iEnd   = foundStringsList.end (),
      i      = iBegin;

    int counter = 0;

    for ( ; ; ) {
      string theString = (*i);

      ++counter;

      os << right <<
        setw (2) << counter << ": " <<
        endl;

      // indent a bit more for readability
//      gIndenter.increment (K_OAH_ELEMENTS_INDENTER_OFFSET);
      ++gIndenter;

      os <<
        gIndenter.indentMultiLineString (theString) <<
        endl;

//      gIndenter.decrement (K_OAH_ELEMENTS_INDENTER_OFFSET);
      --gIndenter;

      if (++i == iEnd) break;
// JMI      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os << endl;
  }

  gIndenter.setIndent (saveIndent);
}

void oahFindStringAtom::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFindStringAtom::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_oahFindStringAtom>*
    p =
      dynamic_cast<visitor<S_oahFindStringAtom>*> (v)) {
        S_oahFindStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFindStringAtom::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void oahFindStringAtom::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFindStringAtom::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_oahFindStringAtom>*
    p =
      dynamic_cast<visitor<S_oahFindStringAtom>*> (v)) {
        S_oahFindStringAtom elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching oahFindStringAtom::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void oahFindStringAtom::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> oahFindStringAtom::browseData ()" <<
      endl;
  }
#endif
}

string oahFindStringAtom::asShortNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fShortName << " " << fVariableName;

  return s.str ();
}

string oahFindStringAtom::asActualLongNamedOptionString () const
{
  stringstream s;

  s <<
    "-" << fLongName << " " << fVariableName;

  return s.str ();
}

void oahFindStringAtom::print (ostream& os) const
{
  const unsigned int fieldWidth = K_OAH_FIELD_WIDTH;

  os <<
    "FindStringAtom:" <<
    endl;

  ++gIndenter;

  printOahElementEssentials (
    gLogStream, fieldWidth);

  --gIndenter;
}

void oahFindStringAtom::printAtomWithValueOptionsValues (
  ostream&     os,
  unsigned int valueFieldWidth) const
{
  // nothing to print here
}

ostream& operator<< (ostream& os, const S_oahFindStringAtom& elt)
{
  elt->print (os);
  return os;
}


}
