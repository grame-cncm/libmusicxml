/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2msrOah___
#define ___msr2msrOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"

#include "lpsrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrOmitPartAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrOmitPartAtom> create (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOmitPartAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

    virtual ~msrOmitPartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            string  partName)
                              { fStringSetVariable.insert (partName); }

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<string>&          fStringSetVariable;
};
typedef SMARTP<msrOmitPartAtom> S_msrOmitPartAtom;
EXP ostream& operator<< (ostream& os, const S_msrOmitPartAtom& elt);

//______________________________________________________________________________
class EXP msrKeepPartAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrKeepPartAtom> create (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrKeepPartAtom (
      string       shortName,
      string       longName,
      string       description,
      string       valueSpecification,
      string       variableName,
      set<string>& stringSetVariable);

    virtual ~msrKeepPartAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringSetVariable (
                            string  partName)
                              { fStringSetVariable.insert (partName); }

    const set<string>&    getStringSetVariable ()
                              { return fStringSetVariable; }

  public:

    // public services
    // ------------------------------------------------------

    void                  applyAtomWithValue (
                            string   theString,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const override;
    string                asActualLongNamedOptionString () const override;

    void                  print (ostream& os) const override;

    void                  printAtomWithValueOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // private fields
    // ------------------------------------------------------

    set<string>&          fStringSetVariable;
};
typedef SMARTP<msrKeepPartAtom> S_msrKeepPartAtom;
EXP ostream& operator<< (ostream& os, const S_msrKeepPartAtom& elt);

//______________________________________________________________________________
class EXP msr2msrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2msrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2msrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2msrOahGroup ();

    virtual ~msr2msrOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // compress
    bool                  getCompressFullMeasureRests () const
                              { return fCompressFullMeasureRests; }
    bool                  getMergeRests () const
                              { return fMergeRests; }

    // barlines
    bool                  getCreateImplicitInitialRepeatBarline () const
                              { return fCreateImplicitInitialRepeatBarline; }

    // convert words to tempo
    bool                  getConvertWordsToTempo () const
                              { return fConvertWordsToTempo; }
    // add words from the lyrics
    bool                  getAddWordsFromTheLyrics () const
                              { return fAddWordsFromTheLyrics; }

    // convert words to rehearsal marks
    bool                  getConvertWordsToRehearsalMarks () const
                              { return fConvertWordsToRehearsalMarks; }

    // convert tempos to rehearsal marks
    bool                  getConvertTemposToRehearsalMarks () const
                              { return fConvertTemposToRehearsalMarks; }
  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializMsr2msrTraceOptions ();
#endif

    void                  initializeCompressOptions ();

    void                  initializeRepeatsOptions ();

    void                  initializeNotesOptions ();

    void                  initializeLyricsOptions ();

    void                  initializeTemposOptions ();

    void                  initializeWordsOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2msrOahHelp ();

    void                  printMsr2msrOahValues (int valueFieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // compress
    bool                  fCompressFullMeasureRests;
    bool                  fMergeRests;

    // repeats
    bool                  fCreateImplicitInitialRepeatBarline;

    // rehearsal marks
    bool                  fConvertTemposToRehearsalMarks;

    // convert words to tempo
    bool                  fConvertWordsToTempo;
    // add words from the lyrics
    bool                  fAddWordsFromTheLyrics;

    // convert words to rehearsal marks
    bool                  fConvertWordsToRehearsalMarks;
};
typedef SMARTP<msr2msrOahGroup> S_msr2msrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2msrOahGroup& elt);

EXP extern S_msr2msrOahGroup gGlobalMsr2msrOahGroup;

//______________________________________________________________________________
EXP S_msr2msrOahGroup createGlobalMsr2msrOahGroup ();


}


#endif
