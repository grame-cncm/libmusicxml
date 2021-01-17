/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___generatorsBasicTypes___
#define ___generatorsBasicTypes___

#include <map>

#include "messagesHandling.h"

#include "oahBasicTypes.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
#define K_GENERATED_OUTPUT_KIND_SHORT_NAME    "gen"
#define K_GENERATED_OUTPUT_KIND_LONG_NAME     "generate"

#define K_GENERATED_OUTPUT_KIND_GUIDO_NAME    "guido"
#define K_GENERATED_OUTPUT_KIND_LIlYPOND_NAME "lilypond"
#define K_GENERATED_OUTPUT_KIND_BRAILLE_NAME  "braille"
#define K_GENERATED_OUTPUT_KIND_MUSICXML_NAME "musicxml"
#define K_GENERATED_OUTPUT_KIND_MIDI_NAME     "midi"

//______________________________________________________________________________
enum generatorOutputKind {
  k_NoOutput,
  kGuidoOutput, kLilyPondOutput, kBrailleOutput, kMusicXMLOutput, kMidiOutput
};

EXP extern string generatorOutputKindAsString (
  generatorOutputKind generatorOutputKind);

EXP extern generatorOutputKind generatorOutputKindFromString (
  const string& theString);

extern map<string, generatorOutputKind>
  gGlobalGeneratorOutputKindsMap;

string existingGeneratorOutputKinds (unsigned int namesListMaxLength);

void initializeGeneratorOutputKindsMap ();

//______________________________________________________________________________
class EXP generatorOutputKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

                          static SMARTP<generatorOutputKindAtom> create (
                            string               shortName,
                            string               longName,
                            string               description,
                            string               valueSpecification,
                            string               variableName,
                            generatorOutputKind& generatorOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          generatorOutputKindAtom (
                            string               shortName,
                            string               longName,
                            string               description,
                            string               valueSpecification,
                            string               variableName,
                            generatorOutputKind& generatorOutputKindVariable);

    virtual               ~generatorOutputKindAtom ();

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

    generatorOutputKind&    fGeneratorOutputKindVariable;
};
typedef SMARTP<generatorOutputKindAtom> S_generatorOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_generatorOutputKindAtom& elt);


}


#endif
