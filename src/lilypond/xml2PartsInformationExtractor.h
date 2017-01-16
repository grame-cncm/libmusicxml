/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2PartsInformationExtractor__
#define __xml2PartsInformationExtractor__

#include <map>
// JMI #include <vector>

#include "xml2PartsInformation.h"

namespace MusicXML2
{

/*!
\addtogroup visitors
@{
*/

/*!
\brief Extracts the part information from MusicXML data
*/
class EXP xml2PartsInformationExtractor : 
  
  public visitor<S_millimeters>,
  public visitor<S_tenths>,
  public visitor<S_scaling>,
  
  public visitor<S_part_list>,
  
  public visitor<S_part>,
  public visitor<S_divisions>,

  public visitor<S_staves>,
  
  public visitor<S_staff>,
  
  public visitor<S_voice>,
  
  public visitor<S_direction>,
  public visitor<S_direction_type>,
  public visitor<S_words>,
  public visitor<S_octave_shift>,
  
  public visitor<S_lyric>,
  public visitor<S_syllabic>,
  public visitor<S_text>,
  public visitor<S_elision>,
  
  public visitor<S_harmony>,
  
  public visitor<S_measure>
    
{
  public:
  
    xml2PartsInformationExtractor (
      S_msrOptions& msrOpts);

    virtual ~xml2PartsInformationExtractor ();

    S_xmlPartsInformation extractPartsInformationFromXMLElementTree (
        const Sxmlelement& xmlTree);

    S_msrScore getMSRScore () const
        { return fMsrScore; };
    
  protected:
   
    virtual void visitStart ( S_millimeters& elt);
    virtual void visitStart ( S_tenths& elt);
    virtual void visitEnd   ( S_scaling& elt);

    virtual void visitStart ( S_part_list& elt);
    virtual void visitEnd   ( S_part_list& elt);
        
    virtual void visitStart ( S_part& elt);
    virtual void visitEnd   ( S_part& elt);
    virtual void visitStart ( S_divisions& elt);
  
    virtual void visitStart ( S_staves& elt);
    
    virtual void visitStart ( S_staff& elt);
    
    virtual void visitStart ( S_voice& elt );
    
    virtual void visitStart ( S_direction& elt );
    virtual void visitEnd   ( S_direction& elt );
    virtual void visitStart ( S_direction_type& elt );
    virtual void visitEnd   ( S_direction_type& elt );
    virtual void visitStart ( S_words& elt );
    virtual void visitStart ( S_octave_shift& elt );
    
    virtual void visitStart ( S_lyric& elt);
    virtual void visitEnd   ( S_lyric& elt);
    virtual void visitStart ( S_syllabic& elt);
    virtual void visitEnd   ( S_text& elt );
    virtual void visitEnd   ( S_elision& elt );

    virtual void visitStart ( S_harmony& elt);
    virtual void visitEnd   ( S_harmony& elt);

    virtual void visitStart ( S_measure& elt);
    virtual void visitEnd   ( S_measure& elt);
    
    virtual void visitStart ( S_print& elt);
    
    virtual void visitStart ( S_barline& elt);

  private:
                     
    S_msrOptions              fMsrOptions;

    // dividing quater notes in MusicXML
    // ------------------------------------------------------
    int                       fCurrentDivisionsPerQuarterNote;

    
    // description of the current MusicXML note
    msrNoteData               fNoteData;

    // the MSR score we're building
    // ------------------------------------------------------
    S_msrScore                fMsrScore;

    // geometry handling
    // ------------------------------------------------------
    int                       fMillimeters;
    int                       fTenths;
    bool                      fOnGoingPageLayout;
    
    // part handling
    // ------------------------------------------------------
    string                    fCurrentPartID; // used throughout
    string                    fCurrentPartName;
    string                    fCurrentPartAbbreviation;
    string                    fCurrentPartInstrumentName;

    map<string, S_msrPart>    fPartsMap;
    
    S_msrPart                 fCurrentPart; // used throughout

    S_msrPart                 createDownToPartIfNeeded (
                                int            inputLineNumber,
                                S_msrPartgroup partgroup,
                                string         partID);

    // staff handling
    // ------------------------------------------------------
    int                       fCurrentStaffNumber; // used throughout
    msrStaff::msrStaffTypeKind
                              fCurrentStaffTypeKind;
    
    S_msrStaff                createStaffInCurrentPartIfNeeded (
                                int            inputLineNumber,
                                int            staffNumber);

    int                       fStaffDetailsStaffNumber;
    int                       fCurrentStaffTuningLinesNumber;
    
    int                       fCurrentStaffTuningLine;
    char                      fCurrentStaffTuningStep;
    int                       fCurrentStaffTuningOctave;
    float                     fCurrentStaffTuningAlter;
    
    int                       fCurrentStaffDetailsCapo;
    int                       fCurrentStaffDetailsStaffSize;
  
    // voice handling
    // ------------------------------------------------------
    int                       fCurrentVoiceNumber; // used throughout

    S_msrVoice                registerVoiceInStaffInCurrentPartIfNeeded (
                                int            inputLineNumber,
                                int            staffNumber,
                                int            voiceNumber);
    
    // lyrics handling
    // ------------------------------------------------------
    // the last sysllabic spec met (single, begin, middle or end)
    string                    fCurrentSyllabic;
    msrLyricschunk::msrLyricschunkKind
                              fCurrentLyricschunkKind;
    msrLyricschunk::msrLyricschunkKind
                              fFirstLyricschunkInSlurKind;
    // the last lyrics fragment met
    string                    fCurrentText;
    bool                      fCurrentElision;
    
    int                       fCurrentLyricsNumber;
    bool                      fCurrentNoteHasLyrics;
    bool                      fCurrentLyricsHasText;

    list<S_msrLyricschunk>    fCurrentNoteLyricchunks;

    void                      handleLyrics (S_msrNote newNote);

    // harmony handling
    // ------------------------------------------------------
    char                      fCurrentHarmonyRootStep;
    float                     fCurrentHarmonyRootAlter;
    msrHarmony::msrHarmonyKind
                              fCurrentHarmonyKind;
    string                    fCurrentHarmonyKindText;
    char                      fCurrentHarmonyBassStep;
    float                     fCurrentHarmonyBassAlter;

    int                       fCurrentHarmonyDegreeValue;
    float                     fCurrentHarmonyDegreeAlter;
    
};


/*! @} */

}

#endif
