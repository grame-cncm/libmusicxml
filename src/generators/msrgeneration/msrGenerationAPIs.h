/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrGenerationAPIs___
#define ___msrGenerationAPIs___

//#include <ostream>

//#include <map>

//#include "exports.h"
//#include "smartpointer.h"
//#include "basevisitor.h"

#include "messagesHandling.h"

#include "oahBasicTypes.h"


using namespace std;
namespace MusicXML2
{

//______________________________________________________________________________
enum msrGenerationAPIKind {
  kMsrRegularAPIKind,
  kMsrStringsAPIKind
};

EXP extern string msrGenerationAPIKindAsString (
  msrGenerationAPIKind generationAPIKind);

EXP extern msrGenerationAPIKind msrGenerationAPIKindFromString (
  const string& theString);

extern map<string, msrGenerationAPIKind>
  gGlobalGenerationAPIKindsMap;

string existingGenerationAPIKinds (unsigned int namesListMaxLength);

void initializeGenerationAPIKindsMap ();

//______________________________________________________________________________
class EXP msrGenerationAPIKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrGenerationAPIKindAtom> create (
      const string&         shortName,
      const string&         longName,
      const string&         description,
      const string&         valueSpecification,
      const string&         variableName,
      msrGenerationAPIKind& generationAPIKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrGenerationAPIKindAtom (
      const string&         shortName,
      const string&         longName,
      const string&         description,
      const string&         valueSpecification,
      const string&         variableName,
      msrGenerationAPIKind& generationAPIKindVariable);

    virtual ~msrGenerationAPIKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            const string& theString,
                            ostream&      os) override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrGenerationAPIKind& fGenerationAPIKindVariable;
};
typedef SMARTP<msrGenerationAPIKindAtom> S_msrGenerationAPIKindAtom;
EXP ostream& operator<< (ostream& os, const S_msrGenerationAPIKindAtom& elt);

//______________________________________________________________________________
void initializeMsrGenerationAPI ();


}


#endif
