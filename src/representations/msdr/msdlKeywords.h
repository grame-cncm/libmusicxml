/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msdlKeywords__
#define __msdlKeywords__

#include <string>
#include <map>

#include "smartpointer.h"


using namespace std;

namespace MusicXML2
{

// the MSDL keywords
//______________________________________________________________________________
enum msdlKeywordKind {
  k_NoMsdlKeywordKind,

	kMsdlKeywordPitches,

	kMsdlKeywordScore,
	kMsdlKeywordPartGroup,
	kMsdlKeywordGroup,
	kMsdlKeywordStaff,
  kMsdlKeywordVoice,
  kMsdlKeywordFragment,

  kMsdlKeywordAnacrusis,

  kMsdlKeywordClef,
  kMsdlKeywordTreble,
  kMsdlKeywordAlto,
  kMsdlKeywordTenor,
  kMsdlKeywordBaryton,
  kMsdlKeywordBasse,

  kMsdlKeywordKey,

  kMsdlKeywordTime,

	kMsdlKeywordDoubleBar,
	kMsdlKeywordFinalBar
};

msdlKeywordKind msdlKeywordKindFromString (std::string theString);

// the MSDL keywords languages
//______________________________________________________________________________
enum msdlKeywordLanguagesKind {
	kEnglish,
	kFrancais
};

// global variables
//______________________________________________________________________________
extern map<string, msdlKeywordLanguagesKind>
  gGlobalKeywordsLanguagesKindsMap;

extern map<msdlKeywordKind, string> gGlobalEnglishKeywordsNamesMap;
extern map<msdlKeywordKind, string> gGlobalFrenchKeywordsNamesMap;

void initializeMsdlKeywordsLanguageKinds ();

void initializeEnglishKeywordsNamesMap ();
void initializeFrancaisKeywordsNamesMap ();

string existingKeywordsLanguageKinds (unsigned int namesListMaxLength);

// initialization
//______________________________________________________________________________
void initializeMSDLKeywords ();


}


#endif


/* JMI
//______________________________________________________________________________
class EXP msdlKeyword : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msdlKeyword> create (
      int         inputLineNumber,
      int         number,
      msdlKeywordKind keywordKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlKeyword (
                            int         inputLineNumber,
                            int         number,
                            msdlKeywordKind keywordKind);

    virtual               ~msdlKeyword ();

  public:

    // set and get
    // ------------------------------------------------------

    msdlKeywordKind       getBeamKind   () const
                              { return fBeamKind; }

    int                   getBeamNumber () const
                              { return fBeamNumber; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fBeamNumber;
    msdlKeywordKind           fBeamKind;
};
typedef SMARTP<msdlKeyword> S_msdlKeyword;
EXP ostream& operator<< (ostream& os, const S_msdlKeyword& elt);
*/
