/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___multiGeneratorsBasicTypes___
#define ___multiGeneratorsBasicTypes___

#include <map>

//#include "waeMessagesHandling.h"

#include "oahBasicTypes.h"

#include "oahAtomsCollection.h"


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
enum class multiGeneratorOutputKind {
  k_NoOutput,
  kGuidoOutput, kLilyPondOutput, kBrailleOutput, kMusicXMLOutput,
  kMidiOutput
};

EXP extern string multiGeneratorOutputKindAsString (
  multiGeneratorOutputKind generatorOutputKind);

EXP extern multiGeneratorOutputKind multiGeneratorOutputKindFromString (
  const string& theString);

extern map<string, multiGeneratorOutputKind>
  gGlobalMultiGeneratorOutputKindsMap;

string existingMultiGeneratorOutputKinds (unsigned int namesListMaxLength);

void initializeGeneratorOutputKindsMap ();

//______________________________________________________________________________
class EXP multiGeneratorOutputKindAtom : public oahAtomStoringAValueInAVariable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<multiGeneratorOutputKindAtom> create (
                            string               shortName,
                            string               longName,
                            string               description,
                            string               valueSpecification,
                            string               variableName,
                            multiGeneratorOutputKind& multiGeneratorOutputKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          multiGeneratorOutputKindAtom (
                            string               shortName,
                            string               longName,
                            string               description,
                            string               valueSpecification,
                            string               variableName,
                            multiGeneratorOutputKind& multiGeneratorOutputKindVariable);

    virtual               ~multiGeneratorOutputKindAtom ();

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

    void                  printAtomWithVariableOptionsValues (
                            ostream&     os,
                            unsigned int valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    multiGeneratorOutputKind&
                          fGeneratorOutputKindVariable;
};
typedef SMARTP<multiGeneratorOutputKindAtom> S_multiGeneratorOutputKindAtom;
EXP ostream& operator<< (ostream& os, const S_multiGeneratorOutputKindAtom& elt);

//______________________________________________________________________________
class EXP multiGeneratorOahGroup : public oahGroup
{
  public:

    static SMARTP<multiGeneratorOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMultiGeneratorOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    multiGeneratorOahGroup ();

    virtual               ~multiGeneratorOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    multiGeneratorOutputKind
                          getGeneratorOutputKind () const
                              { return fGeneratorOutputKind; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeGenerateCodeOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMultiGeneratorOahHelp ();

    void                  printMultiGeneratorOahValues (unsigned int fieldWidth);

  private:

    multiGeneratorOutputKind
                          fGeneratorOutputKind;

    S_multiGeneratorOutputKindAtom
                          fGeneratorOutputKindAtom;
};
typedef SMARTP<multiGeneratorOahGroup> S_multiGeneratorOahGroup;
EXP ostream& operator<< (ostream& os, const S_multiGeneratorOahGroup& elt);

EXP extern S_multiGeneratorOahGroup gGlobalMultiGeneratorOahGroup;

//______________________________________________________________________________
EXP S_multiGeneratorOahGroup createGlobalMultiGeneratorOahGroup ();


}


#endif
