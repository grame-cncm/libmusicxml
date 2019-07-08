/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___extraOptions___
#define ___extraOptions___


#include "setExtraOptionsIfDesired.h"

#ifdef EXTRA_OPTIONS


#include "oahBasicTypes.h"

#include "exports.h"


namespace MusicXML2
{

//______________________________________________________________________________
class optionsShowAllChordsStructuresItem : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowAllChordsStructuresItem> create (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowAllChordsStructuresItem (
      string oahAtomShortName,
      string oahAtomLongName,
      string oahAtomDescription);

    virtual ~optionsShowAllChordsStructuresItem ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAllChordsStructures (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<optionsShowAllChordsStructuresItem> S_optionsShowAllChordsStructuresItem;
EXP ostream& operator<< (ostream& os, const S_optionsShowAllChordsStructuresItem& elt);

//______________________________________________________________________________
class optionsShowAllChordsContentsItem : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowAllChordsContentsItem> create (
      string  oahAtomShortName,
      string  oahAtomLongName,
      string  oahAtomDescription,
      string  optionsValueSpecification,
      string  oahStringAtomVariableDisplayName,
      string& oahStringAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowAllChordsContentsItem (
      string  oahAtomShortName,
      string  oahAtomLongName,
      string  oahAtomDescription,
      string  optionsValueSpecification,
      string  oahStringAtomVariableDisplayName,
      string& oahStringAtomVariable);

    virtual ~optionsShowAllChordsContentsItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowAllChordsContentsItemVariableValue (
                            string value)
                              {
                                fOptionsShowAllChordsContentsItemVariable = value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAllChordsContents (
                            ostream&              os,
                            msrSemiTonesPitchKind semiTonesPitchKind) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    string&               fOptionsShowAllChordsContentsItemVariable;
};
typedef SMARTP<optionsShowAllChordsContentsItem> S_optionsShowAllChordsContentsItem;
EXP ostream& operator<< (ostream& os, const S_optionsShowAllChordsContentsItem& elt);

//______________________________________________________________________________
class optionsShowChordDetailsItem : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowChordDetailsItem> create (
      string  oahAtomShortName,
      string  oahAtomLongName,
      string  oahAtomDescription,
      string  optionsValueSpecification,
      string  oahStringAtomVariableDisplayName,
      string& oahStringAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowChordDetailsItem (
      string  oahAtomShortName,
      string  oahAtomLongName,
      string  oahAtomDescription,
      string  optionsValueSpecification,
      string  oahStringAtomVariableDisplayName,
      string& oahStringAtomVariable);

    virtual ~optionsShowChordDetailsItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowChordDetailsItemVariableValue (
                            string value)
                              {
                                fOptionsShowChordDetailsItemVariable = value;
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

    string&               fOptionsShowChordDetailsItemVariable;
};
typedef SMARTP<optionsShowChordDetailsItem> S_optionsShowChordDetailsItem;
EXP ostream& operator<< (ostream& os, const S_optionsShowChordDetailsItem& elt);

//______________________________________________________________________________
class optionsShowChordAnalysisItem : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<optionsShowChordAnalysisItem> create (
      string  oahAtomShortName,
      string  oahAtomLongName,
      string  oahAtomDescription,
      string  optionsValueSpecification,
      string  oahStringAtomVariableDisplayName,
      string& oahStringAtomVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    optionsShowChordAnalysisItem (
      string  oahAtomShortName,
      string  oahAtomLongName,
      string  oahAtomDescription,
      string  optionsValueSpecification,
      string  oahStringAtomVariableDisplayName,
      string& oahStringAtomVariable);

    virtual ~optionsShowChordAnalysisItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setShowChordAnalysisItemVariableValue (
                            string value)
                              {
                                fOptionsShowChordAnalysisItemVariable = value;
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

    string&               fOptionsShowChordAnalysisItemVariable;
};
typedef SMARTP<optionsShowChordAnalysisItem> S_optionsShowChordAnalysisItem;
EXP ostream& operator<< (ostream& os, const S_optionsShowChordAnalysisItem& elt);

//______________________________________________________________________________
class extraOptions : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<extraOptions> create (
      S_oahHandler oahHandler);

    SMARTP<extraOptions>        createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeExtraOptions (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    extraOptions (
      S_oahHandler oahHandler);

    virtual ~extraOptions ();

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
                          handleOptionsItem (
                            ostream&      os,
                            S_oahAtom item);

    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_oahAtom item,
                            string        theString);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeExtraShowAllChordsStructuresOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExtraShowAllChordsContentsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExtraShowChordDetailsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExtraShowChordAnalysisOptions (
                            bool boolOptionsInitialValue);

    virtual void          handleOptionsItemShowAllChordsContentsValue (
                            ostream&                           os,
                            S_optionsShowAllChordsContentsItem showAllChordsContentsItem,
                            string                             theString);

    virtual void          handleOptionsShowChordDetailsItemValue (
                            ostream&                      os,
                            S_optionsShowChordDetailsItem showChordDetailsItem,
                            string                        theString);

    virtual void          handleOptionshowChordAnalysisItemValue (
                            ostream&                       os,
                            S_optionsShowChordAnalysisItem showChordAnalysisItem,
                            string                         theString);

  public:

    // print
    // ------------------------------------------------------

    void                  printExtraOptionsHelp ();

    void                  printExtraOptionsValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    string                fChordsRootAsString; // JMI
};
typedef SMARTP<extraOptions> S_extraOptions;
EXP ostream& operator<< (ostream& os, const S_extraOptions& elt);

EXP extern S_extraOptions gExtraOptions;
EXP extern S_extraOptions gExtraOptionsUserChoices;
EXP extern S_extraOptions gExtraOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeExtraOptionsHandling (
  S_oahHandler oahHandler);


}


#endif


#endif
