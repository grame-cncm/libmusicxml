/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lilypondOptions__
#define __lilypondOptions__

#include <set>

#include "optionsHandling.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class EXP lilypondOptions : public msrOptionsGroup
{
  public:

    // data types
    // ------------------------------------------------------

    enum scoreNotationKind {
      kWesternNotation, kJianpuNotation, kABCNotation };

    static string scoreNotationKindAsString (
      scoreNotationKind notationKind);

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondOptions> create ();
    
    SMARTP<lilypondOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilypondOptions (
                            bool boolOptionsInitialValue);
    
    void                  printLilypondOptionsHelp ();

    void                  printLilypondOptionsValues (int fieldWidth);
    
  public:
  
    // constructors/destructor
    // ------------------------------------------------------

    lilypondOptions();
    
    virtual ~lilypondOptions();

    // set and get
    // ------------------------------------------------------

    bool                  setAccidentalStyle (
                            string accidentalStyle);    
 
  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkLilypondOptionsConsistency () const;

  public:

    // time
    // --------------------------------------
    
    bool                  fNumericalTime;

    // notes
    // --------------------------------------
    
    bool                  fAbsoluteOctaves;
    
    bool                  fAllDurations;
    
    bool                  fStems;
    bool                  fNoAutoBeaming;
    
    bool                  fRomanStringNumbers;
    bool                  fAvoidOpenStrings;
    
    string                fAccidentalStyle;
    
    bool                  fCompressMultiMeasureRests;

    bool                  fNoteInputLineNumbers;

    // bars
    // --------------------------------------
    
    bool                  fShowAllBarNumbers;
    
    // line breaks
    // --------------------------------------
    
    bool                  fDontKeepLineBreaks;
    
    bool                  fBreakLinesAtIncompleteRightMeasures;
    
    int                   fSeparatorLineEveryNMeasures;

    // page breaks
    // --------------------------------------
    
    bool                  fDontKeepPageBreaks;
    
    // staves
    // --------------------------------------
    
    bool                  fModernTab;

    // tuplets
    // --------------------------------------
    
    bool                  fTupletsOnALine;

    // repeats
    // --------------------------------------
    
    bool                  fRepeatBrackets;

    // ornaments
    // --------------------------------------

    rational              fDelayedOrnamentsFraction;

    // fonts
    // --------------------------------------

    bool                  fJazzFonts;

    // code generation
    // --------------------------------------

    bool                  fComments;

    bool                  fGlobal;
    
    bool                  fDisplayMusic;

    bool                  fNoLilypondCode;

    bool                  fNoLilypondLyrics;

    bool                  fLilypondCompileDate;

    // score notation
    // --------------------------------------

 // JMI   scoreNotationKind     fScoreNotationKind;
    bool                  fJianpu;
    
    // midi
    // --------------------------------------

    pair<string, int>     fMidiTempo;
    
    bool                  fNoMidi;


// JMI ???

    bool                  fKeepStaffSize; // JMI

    bool                  fSilentVoices; // JMI

  private:

    set<string>           fLilypondAccidentalStyles;
};
typedef SMARTP<lilypondOptions> S_lilypondOptions;
EXP ostream& operator<< (ostream& os, const S_lilypondOptions& elt);

extern S_lilypondOptions gLilypondOptions;
extern S_lilypondOptions gLilypondOptionsUserChoices;
extern S_lilypondOptions gLilypondOptionsWithDetailedTrace;


/*! @} */

}


#endif
