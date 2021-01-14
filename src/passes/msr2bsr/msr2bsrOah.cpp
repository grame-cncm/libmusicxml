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

#include "version.h"
#include "utilities.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "messagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"
#include "msr2bsrOah.h"

#include "oahAtomsCollection.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________

S_msr2bsrOahGroup gGlobalMsr2bsrOahGroup;

S_msr2bsrOahGroup msr2bsrOahGroup::create ()
{
  msr2bsrOahGroup* o = new msr2bsrOahGroup ();
  assert (o != nullptr);
  return o;
}

msr2bsrOahGroup::msr2bsrOahGroup ()
  : oahGroup (
    "Msr2bsr",
    "hmsr2bsr", "help-msr-to-bsr",
R"(These options control the way MSR data is translated to BSR.)",
    kElementVisibilityWhole)
{
  initializeMsr2bsrOahGroup ();
}

msr2bsrOahGroup::~msr2bsrOahGroup ()
{}

void msr2bsrOahGroup::initializeMsr2bsrMiscellaneousOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Miscellaneous",
        "hlpm", "help-miscellaneous",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // braille music headings

  fNoBrailleMusicHeadings = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nmh", "no-music-headings",
R"(Don't generate any music headings in the Braille code.)",
        "noBrailleMusicHeadings",
        fNoBrailleMusicHeadings));

  // braille tempos

  fNoTempos = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nt", "no-tempos",
R"(Don't generate any tempos in the Braille code.)",
        "noTempos",
        fNoTempos));

  // braille page numbers

  fNoPageNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "npn", "no-page-numbers",
R"(Don't generate any page numbers in the Braille code.)",
        "noBraillePageNumbers",
        fNoPageNumbers));

  // braille line numbers

  fNoLineNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nln", "no-line-numbers",
R"(Don't generate any line numbers in the Braille code.)",
        "noBrailleLineNumbers",
        fNoLineNumbers));

  // braille measure numbers

  fNoMeasureNumbers = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nmn", "no-measure-numbers",
R"(Don't generate any measure numbers in the Braille code.)",
        "noMeasureNumbers",
        fNoMeasureNumbers));

  // braille lyrics

  fNoBrailleLyrics = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "nl", "no-lyrics",
R"(Don't generate any lyrics in the Braille code.)",
        "noBrailleLyrics",
        fNoBrailleLyrics));

  // braille compile date

  fBrailleCompileDate = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "lpcd", "braille-compile-date",
R"(Generate code to include the compilation date
when Braille creates the score.)",
        "brailleCompileDate",
        fBrailleCompileDate));

  // facsimile

  fFacSimileKind = kFacSimileNo;

  subGroup->
    appendAtomToSubGroup (
      bsrFacSimileKindAtom::create (
        "fs", "facsimile",
R"(Generate facsimile Braille nusic code.
By default, non-facsimile code is generated.)",
        "YES_OR_NO",
        "facSimileKind",
        fFacSimileKind));

  // clefs

  fIncludeClefs = false;

  subGroup->
    appendAtomToSubGroup (
      oahBooleanAtom::create (
        "clefs", "",
R"(Include clefs in BSR. By default, they are not.)",
        "includeClefs",
        fIncludeClefs));
}

void msr2bsrOahGroup::initializeMsr2bsrQuitAfterSomePassesOptions ()
{
  S_oahSubGroup
    subGroup =
      oahSubGroup::create (
        "Quit after some passes",
        "hbquit", "help-bsr-quit",
R"()",
      kElementVisibilityWhole,
      this);

  appendSubGroupToGroup (subGroup);

  // '-quit-3a' is hidden...

  fQuitAfterPass3a = false;

  S_oahBooleanAtom
    quit3aOahBooleanAtom =
      oahBooleanAtom::create (
        "q3a", "quitAfterPass-3a",
R"(Quit after pass 3a, i.e. after conversion
of the MSR to the first BSR score.)",
        "quitAfterPass3a",
        fQuitAfterPass3a);

  subGroup->
    appendAtomToSubGroup (
      quit3aOahBooleanAtom);

  fQuitAfterPass3b = false;

  S_oahBooleanAtom
    quit3bOahBooleanAtom =
      oahBooleanAtom::create (
        "q3b", "quitAfterPass-3b",
R"(Quit after pass 3b, i.e. after conversion
of the first BSR to the second BSR.)",
        "quitAfterPass3b",
        fQuitAfterPass3b);

  subGroup->
    appendAtomToSubGroup (
      quit3bOahBooleanAtom);
}

void msr2bsrOahGroup::initializeMsr2bsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
  // trace
  // --------------------------------------
// JMI  initializeMsr2bsrTraceOahGroup ();
#endif

  // miscellaneous
  // --------------------------------------
  initializeMsr2bsrMiscellaneousOptions ();

  // quit after some passes
  // --------------------------------------
  initializeMsr2bsrQuitAfterSomePassesOptions ();
}

//______________________________________________________________________________
void msr2bsrOahGroup::enforceGroupQuietness ()
{}

//______________________________________________________________________________
void msr2bsrOahGroup::checkGroupOptionsConsistency ()
{}

//______________________________________________________________________________
void msr2bsrOahGroup::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2bsrOahGroup::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2bsrOahGroup>*> (v)) {
        S_msr2bsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2bsrOahGroup::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msr2bsrOahGroup::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2bsrOahGroup::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msr2bsrOahGroup>*
    p =
      dynamic_cast<visitor<S_msr2bsrOahGroup>*> (v)) {
        S_msr2bsrOahGroup elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
          gLogStream <<
            ".\\\" ==> Launching msr2bsrOahGroup::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void msr2bsrOahGroup::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTraceOahVisitors ()) {
    gLogStream <<
      ".\\\" ==> msr2bsrOahGroup::browseData ()" <<
      endl;
  }
#endif
}

void msr2bsrOahGroup::printMsr2bsrOahValues (unsigned int fieldWidth)
{
  gLogStream <<
    "The Msr2bsr options are:" <<
    endl;

  ++gIndenter;

  // miscellaneous
  // --------------------------------------

  gLogStream <<
    "Miscellaneous:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "noBrailleMusicHeadings" << " : " <<
      booleanAsString (fNoBrailleMusicHeadings) <<
      endl <<

    setw (fieldWidth) << "noTempos" << " : " <<
      booleanAsString (fNoTempos) <<
      endl <<

    setw (fieldWidth) << "noBraillePageNumbers" << " : " <<
      booleanAsString (fNoPageNumbers) <<
      endl <<
    setw (fieldWidth) << "noBrailleLineNumbers" << " : " <<
      booleanAsString (fNoLineNumbers) <<
      endl <<
    setw (fieldWidth) << "noMeasureNumbers" << " : " <<
      booleanAsString (fNoMeasureNumbers) <<
      endl <<

    setw (fieldWidth) << "noBrailleLyrics" << " : " <<
      booleanAsString (fNoBrailleLyrics) <<
      endl <<

    setw (fieldWidth) << "brailleCompileDate" << " : " <<
      booleanAsString (fBrailleCompileDate) <<
      endl <<

    setw (fieldWidth) << "facSimileKind" << " : " <<
      bsrFacSimileKindAsString (fFacSimileKind) <<
      endl <<

    setw (fieldWidth) << "includeClefs" << " : " <<
      booleanAsString (fIncludeClefs) <<
      endl;

  --gIndenter;

  // quit after some passes
  // --------------------------------------

  gLogStream <<
    "Quit after some passes:" <<
    endl;

  ++gIndenter;

  gLogStream << left <<
    setw (fieldWidth) << "quitAfterPass3a" << " : " <<
    booleanAsString (fQuitAfterPass3a) <<
    endl <<
    setw (fieldWidth) << "quitAfterPass3b" << " : " <<
    booleanAsString (fQuitAfterPass3b) <<
    endl;

  --gIndenter;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msr2bsrOahGroup& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msr2bsrOahGroup createGlobalMsr2bsrOahGroup ()
{
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "Creating global msr2bsr OAH group" <<
    endl;
#endif
#endif

  // protect library against multiple initializations
  if (! gGlobalMsr2bsrOahGroup) {
    // create the global options group
    gGlobalMsr2bsrOahGroup =
      msr2bsrOahGroup::create ();
    assert (gGlobalMsr2bsrOahGroup != 0);
  }

  // return the global OAH group
  return gGlobalMsr2bsrOahGroup;
}


}
