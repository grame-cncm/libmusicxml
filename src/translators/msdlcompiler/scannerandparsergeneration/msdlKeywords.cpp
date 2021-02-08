/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

/*
#include <iostream>
#include <sstream>

#include "utilities.h"
*/

#include "msdlKeywords.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "generalOah.h"


using namespace std;

namespace MusicXML2
{

// the MSDL keywords
//______________________________________________________________________________
msdlKeywordKind msdlKeywordKindFromString (std::string theString)
{
  msdlKeywordKind result = k_NoMsdlKeywordKind;

  if      (theString == "pitches") {
    result = kMsdlKeywordPitches;
  }
  else if (theString == "score") {
    result =kMsdlKeywordScore ;
  }

  return result;
}

// notes names languages
// ------------------------------------------------------

map<string, msdlKeywordLanguagesKind>
  gGlobalKeywordsLanguagesKindsMap;

map<msdlKeywordKind, string> gGlobalEnglishKeywordsNamesMap;
map<msdlKeywordKind, string> gGlobalFrenchKeywordsNamesMap;

void initializeMsdlKeywordsLanguageKinds ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
    gGlobalKeywordsLanguagesKindsMap ["english"]  = kEnglish;
    gGlobalKeywordsLanguagesKindsMap ["francais"] = kFrancais;

    initializeEnglishKeywordsNamesMap ();
    initializeFrancaisKeywordsNamesMap ();

    pThisMethodHasBeenRun = true;
  }
}

void initializeEnglishKeywordsNamesMap ()
{
  // english
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordPitches]   = "pitches";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordScore]     = "score";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordGroup]     = "group";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordStaff]     = "staff";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordVoice]     = "voice";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordAnacrusis] = "anacrusis";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordClef]      = "clef";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTreble]    = "soprano";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTenor]     = "tenor";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordBasse]     = "bass";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordKey]       = "key";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTime]      = "time";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordDoubleBar] = "doubleBar";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordFinalBar]  = "finalBar";
}

void initializeFrancaisKeywordsNamesMap ()
{
  // français
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordPitches]   = "hauteurDesNotes";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordScore]     = "partition";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordPartGroup] = "partgroup";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordGroup]     = "groupe";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordStaff]     = "portée";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordVoice]     = "voix";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordFragment]  = "fragment";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordAnacrusis] = "anacrouse";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordClef]      = "clé";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTreble]    = "soprano";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordAlto]      = "alto";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTenor]     = "ténor";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordBaryton]   = "baryton";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordBasse]     = "basse";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordKey]       = "armure";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordTime]      = "métrique";

  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordDoubleBar] = "barreDouble";
  gGlobalEnglishKeywordsNamesMap [kMsdlKeywordFinalBar]  = "barreFinale";
}

//______________________________________________________________________________
void initializeMSDLKeywords ()
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
      gLogStream <<
        "Initializing MSR basic types handling" <<
        endl;
  }
#endif

    // keywords languages handling
    // ------------------------------------------------------

    initializeMsdlKeywordsLanguageKinds ();

    pThisMethodHasBeenRun = true;
  }
}


}



/* JMI
//______________________________________________________________________________
S_msdlKeyword msdlKeyword::create (
  int         inputLineNumber,
  int         number,
  msdlKeywordKind keywordKind)
{
  msdlKeyword* o =
    new msdlKeyword (
      inputLineNumber, number, keywordKind);
  assert (o != nullptr);
  return o;
}

msdlKeyword::msdlKeyword (
  int         inputLineNumber,
  int         number,
  msdlKeywordKind keywordKind)
    : msrElement (inputLineNumber)
{
  fBeamNumber = number;
  fBeamKind   = keywordKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Creating keyword '" <<
      this->asString () <<
      "'" <<
      endl;
  }
#endif
}

msdlKeyword::~msdlKeyword ()
{}

void msdlKeyword::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdlKeyword::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msdlKeyword>*
    p =
      dynamic_cast<visitor<S_msdlKeyword>*> (v)) {
        S_msdlKeyword elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdlKeyword::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msdlKeyword::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdlKeyword::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msdlKeyword>*
    p =
      dynamic_cast<visitor<S_msdlKeyword>*> (v)) {
        S_msdlKeyword elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdlKeyword::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msdlKeyword::browseData (basevisitor* v)
{}

string msdlKeyword::asString () const
{
  stringstream s;

  s <<
    "[Beam " <<
    msdlKeywordKindAsString (fBeamKind) <<
    ", number: " << fBeamNumber <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msdlKeyword::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlKeyword& elt)
{
  elt->print (os);
  return os;
}
*/
