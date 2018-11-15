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

#include "bsrBasicTypes.h"
#include "optionsBasicTypes.h"
#include "exports.h"

namespace MusicXML2 
{

//______________________________________________________________________________
class bsrOptions : public optionsGroup
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrByteOrderingKind {
        kByteOrderingBigEndian, kByteOrderingSmallEndian };
        
    static string byteOrderingKindAsString (
      bsrByteOrderingKind byteOrderingKind);

    enum bsrFacSimileKind {
        kFacSimileYes, kFacSimileNo };
        
    static string facSimileKindAsString (
      bsrFacSimileKind facSimileKind);

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

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // services
    // ------------------------------------------------------

    virtual S_optionsItem handleOptionsItem (
                            ostream&      os,
                            S_optionsItem item);
                            
    virtual void          handleOptionsItemValue (
                            ostream&      os,
                            S_optionsItem item,
                            string        theString);

  public:

    // print
    // ------------------------------------------------------

    void                  printBsrOptionsHelp ();

    void                  printBsrOptionsValues (int fieldWidth);
    
  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceBsr;
    
    bool                  fTraceBsrVisitors;
    
    bool                  fDisplayBsr;
    bool                  fDisplayBsrDetails;
    
    // lyrics vs words
    // --------------------------------------

    bool                  fAddWordsFromTheLyrics;

    // languages
    // --------------------------------------
/*
    msrQuarterTonesPitchesLanguageKind
                          fBsrQuarterTonesPitchesLanguageKind;
    
    bsrChordsLanguageKind
                          fBsrChordsLanguageKind;
                          */
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
