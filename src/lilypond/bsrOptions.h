/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrOptions___
#define ___bsrOptions___

#include "exports.h"

#include "bsrBasicTypes.h"

#include "oahBasicTypes.h"

namespace MusicXML2
{

//______________________________________________________________________________
enum bsrFacSimileKind {
    kFacSimileYes, kFacSimileNo };

string facSimileKindAsString (
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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string           theString,
                            indentedOstream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
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

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string           theString,
                            indentedOstream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
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
class bsrOptions : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrOptions> create (
      S_oahHandler handler);

    SMARTP<bsrOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBsrOptions (
                            bool boolOptionsInitialValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    bsrOptions (
      S_oahHandler handler);

    virtual ~bsrOptions ();

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

    virtual S_oahValuedAtom
                          handleAtom (
                            ostream&  os,
                            S_oahAtom atom);

    virtual void          handleValuedAtomValue (
                            ostream&      os,
                            S_oahAtom atom,
                            string        theString);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeBsrDisplayOptions (
                            bool boolOptionsInitialValue);

    void                  initializeBsrLanguagesOptions (
                            bool boolOptionsInitialValue);


    void                  initializeBsrMiscellaneousOptions (
                            bool boolOptionsInitialValue);


    void                  initializeBsrExitAfterSomePassesOptions (
                            bool boolOptionsInitialValue);

#ifdef TRACE_OPTIONS
    void                  initializeBsrTraceOptions (
                            bool boolOptionsInitialValue);
#endif

  public:

    // print
    // ------------------------------------------------------

    void                  printBsrOptionsHelp ();

    void                  printBsrOptionsValues (int fieldWidth);

  public:

    // display
    // --------------------------------------

    bool                  fDisplayBsr;
    bool                  fDisplayBsrDetails;

    // languages
    // --------------------------------------

    bsrTextsLanguageKind  fBsrTextsLanguageKind;

    // miscellaneous
    // --------------------------------------

    bool                  fNoBrailleLyrics;

    bool                  fBrailleCompileDate;

    bsrFacSimileKind      fFacSimileKind;

    bool                  fIncludeClefs;

    // exit after some passes
    // --------------------------------------

    bool                  fExit3a;
    bool                  fExit3b;

    // trace
    // --------------------------------------

#ifdef TRACE_OPTIONS
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
typedef SMARTP<bsrOptions> S_bsrOptions;
EXP ostream& operator<< (ostream& os, const S_bsrOptions& elt);

EXP extern S_bsrOptions gBsrOptions;
EXP extern S_bsrOptions gBsrOptionsUserChoices;
EXP extern S_bsrOptions gBsrOptionsWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeBsrOptionsHandling (
  S_oahHandler handler);


}


#endif
