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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahBasicTypes.h"

namespace MusicXML2
{

//______________________________________________________________________________
enum bsrFacSimileKind {
    kFacSimileYes, kFacSimileNo };

string bsrFacSimileKindAsString (
  bsrFacSimileKind facSimileKind);

//______________________________________________________________________________
class bsrFacSimileKindAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bsrFacSimileKind      fBsrFacSimileKindVariable;
};
typedef SMARTP<bsrFacSimileKindAtom> S_bsrFacSimileKindAtom;
EXP ostream& operator<< (ostream& os, const S_bsrFacSimileKindAtom& elt);

//______________________________________________________________________________
class bsrTextsLanguageAtom : public oahValuedAtom
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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bsrTextsLanguageKind& fBsrTextsLanguageKindVariable;
};
typedef SMARTP<bsrTextsLanguageAtom> S_bsrTextsLanguageAtom;
EXP ostream& operator<< (ostream& os, const S_bsrTextsLanguageAtom& elt);

//______________________________________________________________________________
class bsrOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<bsrOah>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBsrOah (
                            bool boolOptionsInitialValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    bsrOah (
      S_oahHandler handlerUpLink);

    virtual ~bsrOah ();

  public:

    // set and get
    // ------------------------------------------------------

    bool                  setBsrTextsLanguage (
                            string language);

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeBsrDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBsrLanguagesOptions (
                            bool boolOptionsInitialValue);

#ifdef TRACE_OAH
    void                  initializeBsrTraceOah (
                            bool boolOptionsInitialValue);
#endif

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printBsrOahHelp ();

    void                  printBsrOahValues (int fieldWidth);

  public:

    // fileds
    // --------------------------------------

    // display

    bool                  fDisplayBsr;
    bool                  fDisplayBsrDetails;

    // languages

    bsrTextsLanguageKind  fBsrTextsLanguageKind;

    // trace
    // --------------------------------------

#ifdef TRACE_OAH
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
typedef SMARTP<bsrOah> S_bsrOah;
EXP ostream& operator<< (ostream& os, const S_bsrOah& elt);

EXP extern S_bsrOah gBsrOah;
EXP extern S_bsrOah gBsrOahUserChoices;
EXP extern S_bsrOah gBsrOahWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeBsrOahHandling (
  S_oahHandler handler);


}


#endif
