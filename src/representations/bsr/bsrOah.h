/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrOah___
#define ___bsrOah___

#include "exports.h"

#include "bsrBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
enum bsrFacSimileKind {
  kFacSimileYes, kFacSimileNo
};

string bsrFacSimileKindAsString (
  bsrFacSimileKind facSimileKind);

//______________________________________________________________________________
class EXP bsrFacSimileKindAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrFacSimileKindAtom> create (
      string           shortName,
      string           longName,
      string           description,
      string           valueSpecification,
      string           variableName,
      bsrFacSimileKind bsrFacSimileKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrFacSimileKindAtom (
      string           shortName,
      string           longName,
      string           description,
      string           valueSpecification,
      string           variableName,
      bsrFacSimileKind bsrFacSimileKindVariable);

    virtual ~bsrFacSimileKindAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setOptionsFacSimileKindVariable (
                            bsrFacSimileKind value)
                              { fBsrFacSimileKindVariable = value; }

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

    bsrFacSimileKind      fBsrFacSimileKindVariable;
};
typedef SMARTP<bsrFacSimileKindAtom> S_bsrFacSimileKindAtom;
EXP ostream& operator<< (ostream& os, const S_bsrFacSimileKindAtom& elt);

//______________________________________________________________________________
class EXP bsrTextsLanguageAtom : public oahAtomWithValue
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTextsLanguageAtom> create (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      bsrTextsLanguageKind&
                         bsrTextsLanguageKindVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTextsLanguageAtom (
      string             shortName,
      string             longName,
      string             description,
      string             valueSpecification,
      string             variableName,
      bsrTextsLanguageKind&
                         bsrTextsLanguageKindVariable);

    virtual ~bsrTextsLanguageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBsrTextsLanguageKindVariable (
                            bsrTextsLanguageKind value)
                              {
                                fBsrTextsLanguageKindVariable = value;
                              }

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

    bsrTextsLanguageKind& fBsrTextsLanguageKindVariable;
};
typedef SMARTP<bsrTextsLanguageAtom> S_bsrTextsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_bsrTextsLanguageAtom& elt);

//______________________________________________________________________________
class EXP bsrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBsrOahGroup ();

  public:

    // constructors/destructor
    // ------------------------------------------------------

    bsrOahGroup ();

    virtual ~bsrOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    bool                  setBsrTextsLanguage (
                            string language);

    // display

    void                  setDisplayBsr ()
                              { fDisplayBsr = true; }
    bool                  getDisplayBsr () const
                              { return fDisplayBsr; }

    void                  setDisplayBsrShort ()
                              { fDisplayBsrShort = true; }
    bool                  getDisplayBsrShort () const
                              { return fDisplayBsrShort; }

    bool                  getDisplayBsrDetails () const
                              { return fDisplayBsrDetails; }

    // languages

    bsrTextsLanguageKind  getBsrTextsLanguageKind () const // JMI
                              { return fBsrTextsLanguageKind; }

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED
    // BSR
    void                  setTraceBsr ()
                              { fTraceBsr = true; }
    bool                  getTraceBsr () const
                              { return fTraceBsr; }

    // pages & lines
    void                  setTracePages ()
                              { fTracePages = true; }
    bool                  getTracePages () const
                              { return fTracePages; }

    void                  setTraceLines ()
                              { fTraceLines = true; }
    bool                  getTraceLines () const
                              { return fTraceLines; }

    void                  setTraceSpaces ()
                              { fTraceSpaces = true; }
    bool                  getTraceSpaces () const
                              { return fTraceSpaces; }

    void                  setTraceNumbers ()
                              { fTraceNumbers = true; }
    bool                  getTraceNumbers () const
                              { return fTraceNumbers; }

    // parallels
    void                  setTraceParallels ()
                              { fTraceParallels = true; }
    bool                  getTraceParallels () const
                              { return fTraceParallels; }

    // visitors
    void                  setTraceBsrVisitors ()
                              { fTraceBsrVisitors = true; }
    bool                  getTraceBsrVisitors () const
                              { return fTraceBsrVisitors; }
#endif

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkGroupOptionsConsistency () override;

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeBsrDisplayOptions ();

    void                  initializeBsrLanguagesOptions ();

#ifdef TRACING_IS_ENABLED
    void                  initializeBsrTraceOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printBsrOahHelp ();

    void                  printBsrOahValues (unsigned int fieldWidth);

  private:

    // private fields
    // --------------------------------------

    // display

    bool                  fDisplayBsr;
    bool                  fDisplayBsrShort;
    bool                  fDisplayBsrDetails;

    // languages

    bsrTextsLanguageKind  fBsrTextsLanguageKind;

    // trace
    // --------------------------------------

#ifdef TRACING_IS_ENABLED

    // BSR
    bool                  fTraceBsr;

    // pages & lines
    bool                  fTracePages;

    bool                  fTraceLines;

    bool                  fTraceSpaces;
    bool                  fTraceNumbers;

    // parallels
    bool                  fTraceParallels;

    // visitors
    bool                  fTraceBsrVisitors;
#endif
};
typedef SMARTP<bsrOahGroup> S_bsrOahGroup;
EXP ostream& operator<< (ostream& os, const S_bsrOahGroup& elt);

EXP extern S_bsrOahGroup gGlobalBsrOahGroup;

//______________________________________________________________________________
EXP  S_bsrOahGroup createGlobalBsrOahGroup ();


}


#endif
