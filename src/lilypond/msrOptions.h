/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrMsrOptions__
#define __msrMsrOptions__

#include "smartpointer.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
/*!
  \brief The MSR code generation options.

  A class is used to avoid passing arguments one by one
  to the various methods that need them.
*/

// trace and display

#define _TRACE_MSR_LONG_NAME_  "traceMsr"
#define _TRACE_MSR_SHORT_NAME_ "tmsr"

#define _TRACE_MSR_VISITORS_LONG_NAME_  "traceMsrVisitors"
#define _TRACE_MSR_VISITORS_SHORT_NAME_ "tmvisits"

#define _DISPLAY_MSR_LONG_NAME_  "displayMsr"
#define _DISPLAY_MSR_SHORT_NAME_ "msr"

#define _DISPLAY_MSR_SUMMARY_LONG_NAME_  "displayMsrSummary"
#define _DISPLAY_MSR_SUMMARY_SHORT_NAME_ "sum"

// languages

#define _MSR_PITCHES_LANGUAGE_LONG_NAME_  "msrPitchesLanguage"
#define _MSR_PITCHES_LANGUAGE_SHORT_NAME_ "mpl"

// parts

#define _PART_NAME_LONG_NAME_  "partName"
#define _PART_NAME_SHORT_NAME_ "part"

// voices

#define _STAFF_RELATIVE_VOICE_NUMBERS_LONG_NAME_  "voicesStaffRelativeNumbers"
#define _STAFF_RELATIVE_VOICE_NUMBERS_SHORT_NAME_ "vsrvn"

#define _SHOW_SILENT_VOICES_LONG_NAME_  "showSilentVoices"
#define _SHOW_SILENT_VOICES_SHORT_NAME_ "ssv"

#define _KEEP_SILENT_VOICES_LONG_NAME_  "keepSilentVoices"
#define _KEEP_SILENT_VOICES_SHORT_NAME_ "ksv"

// notes

#define _DELAY_RESTS_DYNAMICS_LONG_NAME_  "delayRestsDynamics"
#define _DELAY_RESTS_DYNAMICS_SHORT_NAME_ "drdyns"

#define _DELAY_RESTS_WORDS_LONG_NAME_  "delayRestsWords"
#define _DELAY_RESTS_WORDS_SHORT_NAME_ "drwords"

#define _DELAY_RESTS_SLURS_LONG_NAME_  "delayRestsSlurs"
#define _DELAY_RESTS_SLURS_SHORT_NAME_ "drslurs"

#define _DELAY_RESTS_LIGATURES_LONG_NAME_  "delayRestsLigatures"
#define _DELAY_RESTS_LIGATURES_SHORT_NAME_ "drligs"

#define _DELAY_RESTS_WEDGES_LONG_NAME_  "delayRestsWedges"
#define _DELAY_RESTS_WEDGES_SHORT_NAME_ "drwedges"

// lyrics

#define _SHOW_MSR_STANZAS_LONG_NAME_  "showMsrStanzas"
#define _SHOW_MSR_STANZAS_SHORT_NAME_ "sms"

#define _KEEP_MUTE_STANZAS_LONG_NAME_  "keepMuteStanzas"
#define _KEEP_MUTE_STANZAS_SHORT_NAME_ "kms"

// harmonies

#define _SHOW_HARMONY_VOICES_LONG_NAME_  "showHarmonyVoices"
#define _SHOW_HARMONY_VOICES_SHORT_NAME_ "shv"

#define _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_  "keepEmptyHarmoniesVoice"
#define _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_ "kehv"


class EXP msrOptions : public smartable
{
  public:

    static SMARTP<msrOptions> create ();
      
    SMARTP<msrOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrOptions (
                            bool boolOptionsInitialValue);
    
    void                  printMsrOptionsHelp ();

    void                  printMsrOptionsValues (int fieldWidth);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptions();
  
    virtual ~msrOptions();
 
    // set and get
    // ------------------------------------------------------

    bool                  setMsrQuarterTonesPitchesLanguage (
                            string language);    
 
  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceMsr;
    
    bool                  fTraceMsrVisitors;
    
    bool                  fDisplayMsr;
    
    bool                  fDisplayMsrSummary;
    
    // languages
    // --------------------------------------
    
    msrQuarterTonesPitchesLanguage
                          fMsrQuarterTonesPitchesLanguage;
    
    // parts
    // --------------------------------------
    
    map<string, string>   fPartsRenaming;

    // voices
    // --------------------------------------
    
    bool                  fCreateVoicesStaffRelativeNumbers;
    
    bool                  fShowSilentVoices;
    bool                  fKeepSilentVoices;

    // notes
    // --------------------------------------
    
    bool                  fDelayRestsDynamics;
    bool                  fDelayRestsWords; // JMI
    bool                  fDelayRestsSlurs; // JMI
    bool                  fDelayRestsLigatures; // JMI
    bool                  fDelayRestsWedges; // JMI

    // lyrics
    // --------------------------------------
    
    bool                  fShowMsrStanzas;
    bool                  fKeepMuteStanzas;
    
    // harmonies
    // --------------------------------------
    
    bool                  fShowHarmonyVoices;
    bool                  fKeepEmptyHarmonyVoices;
};
typedef SMARTP<msrOptions> S_msrOptions;

extern S_msrOptions gMsrOptions;
extern S_msrOptions gMsrOptionsUserChoices;
extern S_msrOptions gMsrOptionsWithDetailedTrace;


/*! @} */

}


#endif
