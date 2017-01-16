/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2PartsInfoExtractor__
#define __xml2PartsInfoExtractor__

#include <map>
// JMI #include <vector>

#include "xmlPartsInfo.h"

namespace MusicXML2
{

/*!
\addtogroup visitors
@{
*/

/*!
\brief Extracts the part information from MusicXML data
*/
class EXP xml2PartsInfoExtractor : 
  
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
  
    xml2PartsInfoExtractor (
      S_xmlPartsInfoOptions& msrOpts);

    virtual ~xml2PartsInfoExtractor ();

    S_xmlPartsInfo extractPartsInfoFromXMLElementTree (
        const Sxmlelement& xmlTree);

    S_xmlPartsInfo getXmlPartsInfo () const
        { return fXmlPartsInfo; };
    
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
                     
    S_xmlPartsInfoOptions     fxmlPartsInfoOptions;

    // dividing quater notes in MusicXML
    // ------------------------------------------------------
    int                       fCurrentDivisionsPerQuarterNote;

    // the xmlPartsInfo we're building
    // ------------------------------------------------------
    S_xmlPartsInfo            fXmlPartsInfo;

    // geometry handling
    // ------------------------------------------------------
    int                       fMillimeters;
    int                       fTenths;
    bool                      fOnGoingPageLayout;
    
    // part handling
    // ------------------------------------------------------
    string                    fCurrentPartID; // used throughout
    string                    fCurrentPartName;

    map<string, S_msrPart>    fPartsMap;
    
    S_msrPart                 fCurrentPart; // used throughout

    S_msrPart                 createDownToPartIfNeeded (
                                int            inputLineNumber,
                                string         partID);

    // staff handling
    // ------------------------------------------------------
    int                       fCurrentStaffNumber; // used throughout
    msrStaff::msrStaffTypeKind
                              fCurrentStaffTypeKind;
    
    S_msrStaff                createStaffInCurrentPartIfNeeded (
                                int            inputLineNumber,
                                int            staffNumber);
  
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
    // the last lyrics fragment met
    string                    fCurrentText;

    int                       fCurrentLyricsNumber;
    bool                      fCurrentNoteHasLyrics;
    bool                      fCurrentLyricsHasText;

    // harmony handling
    // ------------------------------------------------------
    
};


/*! @} */

}

#endif
