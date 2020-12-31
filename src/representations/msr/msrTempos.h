/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrTempos___
#define ___msrTempos___

#include <list>

#include "msrSegnosAndCodas.h"

#include "msrBasicTypes.h"

#include "msrBeams.h"
#include "msrWords.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrTempoNote : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempoNote> create (
      int      inputLineNumber,
      rational tempoNoteWholeNotes,
      bool     tempoNoteBelongsToATuplet);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTempoNote (
      int      inputLineNumber,
      rational tempoNoteWholeNotes,
      bool     tempoNoteBelongsToATuplet);

    virtual ~msrTempoNote ();

  public:

    // set and get
    // ------------------------------------------------------

    rational              getTempoNoteWholeNotes () const
                              { return fTempoNoteWholeNotes; }

    const list<S_msrBeam>&
                          getTempoNoteBeams () const
                              { return fTempoNoteBeams; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendBeamToTempoNote (S_msrBeam beam);

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

    rational              fTempoNoteWholeNotes;

    list<S_msrBeam>       fTempoNoteBeams;

    bool                  fTempoNoteBelongsToATuplet;
};
typedef SMARTP<msrTempoNote> S_msrTempoNote;
EXP ostream& operator<< (ostream& os, const S_msrTempoNote& elt);

//______________________________________________________________________________
class EXP msrTempoTuplet : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrTempoTupletTypeKind {
      kTempoTupletTypeNone,
      kTempoTupletTypeStart, kTempoTupletTypeStop
    };

    static string tempoTupletTypeKindAsString (
      msrTempoTupletTypeKind tempoTupletTypeKind);

    enum msrTempoTupletBracketKind {
      kTempoTupletBracketYes, kTempoTupletBracketNo
    };

    static string tempoTupletBracketKindAsString (
      msrTempoTupletBracketKind tempoTupletBracketKind);

    enum msrTempoTupletShowNumberKind {
      kTempoTupletShowNumberActual,
      kTempoTupletShowNumberBoth,
      kTempoTupletShowNumberNone
    };

    static string tempoTupletShowNumberKindAsString (
      msrTempoTupletShowNumberKind tempoTupletShowNumberKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempoTuplet> create (
      int                          inputLineNumber,
      int                          tempoTupletNumber,
      msrTempoTupletBracketKind    tempoTupletBracketKind,
      msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
      msrTupletFactor              tempoTupletFactor,
      rational                     memberNotesDisplayWholeNotes);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTempoTuplet (
      int                          inputLineNumber,
      int                          tempoTupletNumber,
      msrTempoTupletBracketKind    tempoTupletBracketKind,
      msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
      msrTupletFactor              tempoTupletFactor,
      rational                     memberNotesDisplayWholeNotes);

    virtual ~msrTempoTuplet ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getTempoTupletNumber () const
                              { return fTempoTupletNumber; }

    msrTempoTupletBracketKind  getTempoTupletBracketKind () const
                              { return fTempoTupletBracketKind; }

    msrTempoTupletShowNumberKind
                          getTempoTupletShowNumberKind () const
                              { return fTempoTupletShowNumberKind; }

    const msrTupletFactor&
                          getTempoTupletFactor () const
                              { return fTempoTupletFactor; }

    rational              getMemberNotesDisplayWholeNotes () const
                              { return fMemberNotesDisplayWholeNotes; }

    const list<S_msrElement>&
                          getTempoTupletElements () const
                              { return fTempoTupletElements; }

    rational              getTempoTupletDisplayWholeNotes () const
                              { return fTempoTupletDisplayWholeNotes; }

  public:

    // public services
    // ------------------------------------------------------

    void                  addTempoNoteToTempoTuplet (S_msrTempoNote tempoNote);
    /*
    void                  addTempoTupletToTempoTuplet (S_msrTempoTuplet tempoTuplet);

    void                  addTempoTupletToTempoTupletClone (S_msrTempoTuplet tempoTuplet);

    void                  removeFirstNoteFromTempoTuplet (
                            int            inputLineNumber,
                            S_msrTempoNote tempoNote);
    */

 // JMI   void                  applyDisplayFactorToTempoTupletMembers ();

    /*
    void                  unapplySoundingFactorToTempoTupletMembers (
                            int containingTempoTupletActualNotes,
                            int containingTempoTupletNormalNotes);
*/

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

    int                   fTempoTupletNumber;

    msrTempoTupletBracketKind
                          fTempoTupletBracketKind;

    msrTempoTupletShowNumberKind
                          fTempoTupletShowNumberKind;

    msrTupletFactor       fTempoTupletFactor;

    rational              fMemberNotesDisplayWholeNotes;

    rational              fTempoTupletDisplayWholeNotes;

    list<S_msrElement>    fTempoTupletElements;
};
typedef SMARTP<msrTempoTuplet> S_msrTempoTuplet;
EXP ostream& operator<< (ostream& os, const S_msrTempoTuplet& elt);

//______________________________________________________________________________
class EXP msrTempoRelationshipElements : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrTempoRelationshipElementsKind {
      kTempoRelationshipElementsLeft,
      kTempoRelationshipElementsRight
    };

    static string tempoRelationshipElementsKindAsString (
      msrTempoRelationshipElementsKind tempoRelationshipElementsKind);

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempoRelationshipElements> create (
      int      inputLineNumber,
      msrTempoRelationshipElementsKind
               tempoRelationshipElementsKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTempoRelationshipElements (
      int      inputLineNumber,
      msrTempoRelationshipElementsKind
               tempoRelationshipElementsKind);

    virtual ~msrTempoRelationshipElements ();

  public:

    // set and get
    // ------------------------------------------------------

    rational              getTempoRelationshipElementsKind () const
                              { return fTempoRelationshipElementsKind; }

    const list<S_msrElement>&
                          getTempoRelationshipElementsList () const
                              { return fTempoRelationshipElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    string                asString () const override;

    void                  addElementToTempoRelationshipElements (
                            S_msrElement element);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    msrTempoRelationshipElementsKind
                          fTempoRelationshipElementsKind;

    list<S_msrElement>    fTempoRelationshipElementsList;
};
typedef SMARTP<msrTempoRelationshipElements> S_msrTempoRelationshipElements;
EXP ostream& operator<< (ostream& os, const S_msrTempoRelationshipElements& elt);

//______________________________________________________________________________
class EXP msrTempo : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrTempoKind {
      k_NoTempoKind,
      kTempoBeatUnitsWordsOnly,
      kTempoBeatUnitsPerMinute,
      kTempoBeatUnitsEquivalence,
      kTempoNotesRelationShip
    };

    static string tempoKindAsString (
      msrTempoKind tempoKind);

    enum msrTempoParenthesizedKind {
      kTempoParenthesizedYes, kTempoParenthesizedNo
    };

    static string tempoParenthesizedKindAsString (
      msrTempoParenthesizedKind tempoParenthesizedKind);

    enum msrTempoRelationKind {
      kTempoRelationNone, kTempoRelationEquals
    };

    static string tempoRelationKindAsString (
      msrTempoRelationKind tempoRelationKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempo> create (
      int               inputLineNumber,
      S_msrWords        tempoWords);

    static SMARTP<msrTempo> create (
      int               inputLineNumber,
      msrDottedDuration tempoBeatUnit,
      string            tempoPerMinute,
      msrTempoParenthesizedKind
                        tempoParenthesizedKind,
      msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> create (
      int               inputLineNumber,
      msrDottedDuration tempoBeatUnit,
      msrDottedDuration tempoEquivalentBeatUnit,
      msrTempoParenthesizedKind
                        tempoParenthesizedKind,
      msrPlacementKind  tempoPlacementKind);

    static SMARTP<msrTempo> create (
      int               inputLineNumber,
      S_msrTempoRelationshipElements
                        tempoRelationLeftElements,
      S_msrTempoRelationshipElements
                        tempoRelationRightElements,
      msrTempoParenthesizedKind
                        tempoParenthesizedKind,
      msrPlacementKind  tempoPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTempo (
      int               inputLineNumber,
      S_msrWords        tempoWords);

    msrTempo (
      int               inputLineNumber,
      msrDottedDuration tempoBeatUnit,
      string            tempoPerMinute,
      msrTempoParenthesizedKind
                        tempoParenthesizedKind,
      msrPlacementKind  tempoPlacementKind);

    msrTempo (
      int               inputLineNumber,
      msrDottedDuration tempoBeatUnit,
      msrDottedDuration tempoEquivalentBeatUnit,
      msrTempoParenthesizedKind
                        tempoParenthesizedKind,
      msrPlacementKind  tempoPlacementKind);

    msrTempo (
      int               inputLineNumber,
      S_msrTempoRelationshipElements
                        tempoRelationLeftElements,
      S_msrTempoRelationshipElements
                        tempoRelationRightElements,
      msrTempoParenthesizedKind
                        tempoParenthesizedKind,
      msrPlacementKind  tempoPlacementKind);

    virtual ~msrTempo ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTempoKind          getTempoKind () const
                              { return fTempoKind; }

    const list<S_msrWords>&
                          getTempoWordsList () const
                              { return fTempoWordsList; }

    msrDottedDuration     getTempoBeatUnit () const
                              { return fTempoBeatUnit; }

    string                getTempoPerMinute () const
                              { return fTempoPerMinute; }

    msrDottedDuration     getTempoEquivalentBeatUnit () const
                              { return fTempoEquivalentBeatUnit; }

    S_msrTempoRelationshipElements
                          getTempoRelationLeftElements () const
                              { return fTempoRelationLeftElements; }
    msrTempoRelationKind  getTempoRelationKind () const
                              { return fTempoRelationKind; }
    S_msrTempoRelationshipElements
                          getTempoRelationRightElements () const
                              { return fTempoRelationRightElements; }

    msrTempoParenthesizedKind
                          getTempoParenthesizedKind () const
                              { return fTempoParenthesizedKind; }

    msrPlacementKind      getTempoPlacementKind () const
                              { return fTempoPlacementKind; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendWordsToTempo (S_msrWords tempoWords)
                              {
                                fTempoWordsList.push_back (tempoWords);
                              }

    string                tempoWordsListAsString (string separator) const;

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

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrTempoKind          fTempoKind;

    list<S_msrWords>      fTempoWordsList;

    msrDottedDuration     fTempoBeatUnit;

    string                fTempoPerMinute; // '90' or '132-156' for example
    msrDottedDuration     fTempoEquivalentBeatUnit;

    S_msrTempoRelationshipElements
                          fTempoRelationLeftElements;
    msrTempoRelationKind  fTempoRelationKind;
    S_msrTempoRelationshipElements
                          fTempoRelationRightElements;

    msrTempoParenthesizedKind
                          fTempoParenthesizedKind;

    msrPlacementKind      fTempoPlacementKind;
};
typedef SMARTP<msrTempo> S_msrTempo;
EXP ostream& operator<< (ostream& os, const S_msrTempo& elt);


} // namespace MusicXML2


#endif
