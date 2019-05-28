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

#include "optionsBasicTypes.h"

namespace MusicXML2
{

//______________________________________________________________________________
enum bsrFacSimileKind {
    kFacSimileYes, kFacSimileNo };

string facSimileKindAsString (
  bsrFacSimileKind facSimileKind);

//______________________________________________________________________________
class optionsFacSimileKindItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsFacSimileKindItem> create (
      string           optionsItemShortName,
      string           optionsItemLongName,
      string           optionsItemDescription,
      string           optionsValueSpecification,
      string           optionsFacSimileKindItemVariableDisplayName,
      bsrFacSimileKind optionsFacSimileKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsFacSimileKindItem (
      string           optionsItemShortName,
      string           optionsItemLongName,
      string           optionsItemDescription,
      string           optionsValueSpecification,
      string           optionsFacSimileKindItemVariableDisplayName,
      bsrFacSimileKind optionsFacSimileKindItemVariable);

    virtual ~optionsFacSimileKindItem ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsFacSimileKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsFacSimileKindItemVariableDisplayName;
                              }

    void                  setOptionsFacSimileKindItemVariable (
                            bsrFacSimileKind value)
                              {
                                fOptionsFacSimileKindItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string                fOptionsFacSimileKindItemVariableDisplayName;
    bsrFacSimileKind      fOptionsFacSimileKindItemVariable;
};
typedef SMARTP<optionsFacSimileKindItem> S_optionsFacSimileKindItem;
EXP ostream& operator<< (ostream& os, const S_optionsFacSimileKindItem& elt);

//______________________________________________________________________________
class optionsBsrTextsLanguageItem : public optionsValuedItem
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsBsrTextsLanguageItem> create (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsBsrTextsLanguageKindItemVariableDisplayName,
      bsrTextsLanguageKind&
                         optionsBsrTextsLanguageKindItemVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsBsrTextsLanguageItem (
      string             optionsItemShortName,
      string             optionsItemLongName,
      string             optionsItemDescription,
      string             optionsValueSpecification,
      string             optionsBsrTextsLanguageKindItemVariableDisplayName,
      bsrTextsLanguageKind&
                         optionsBsrTextsLanguageKindItemVariable);

    virtual ~optionsBsrTextsLanguageItem ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionsBsrTextsLanguageKindItemVariableDisplayName () const
                              {
                                return
                                  fOptionsBsrTextsLanguageKindItemVariableDisplayName;
                              }

    void                  setBsrTextsLanguageKindItemVariableValue (
                            bsrTextsLanguageKind value)
                              {
                                fOptionsBsrTextsLanguageKindItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string                fOptionsBsrTextsLanguageKindItemVariableDisplayName;
    bsrTextsLanguageKind&
                          fOptionsBsrTextsLanguageKindItemVariable;
};
typedef SMARTP<optionsBsrTextsLanguageItem> S_optionsBsrTextsLanguageItem;
EXP ostream& operator<< (ostream& os, const S_optionsBsrTextsLanguageItem& elt);

//______________________________________________________________________________
class bsrOptions : public optionsGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrOptions> create (
      S_optionsHandler optionsHandler);

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
      S_optionsHandler optionsHandler);

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

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);

    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_optionsItem item,
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
  S_optionsHandler optionsHandler);


}


#endif
