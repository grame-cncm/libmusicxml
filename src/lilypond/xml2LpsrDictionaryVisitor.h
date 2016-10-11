/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2LpsrDictionaryVisitor__
#define __xml2LpsrDictionaryVisitor__

#include <map>
#include <vector>

#include "lpsr.h"
#include "musicxml2lpsr.h"

namespace MusicXML2
{

/*!
\addtogroup visitors
@{
*/

/*!
\brief Produces a summary of a MusicXML part.

  A part summary consists in 
  - a number of staves
  - a map that associates each stave with the corresponding number of notes
  - a map that associates each stave with the corresponding voices and notes
  - a map that associates each stave with the correcponding lyricss
*/
class EXP xml2LpsrDictionaryVisitor : 

  public visitor<S_divisions>,

/*
  <part-list>
  
    <part-group number="1" type="start">
      <group-symbol default-x="-7">bracket</group-symbol>
      <group-barline>yes</group-barline>
    </part-group>
    
    <score-part id="P1">
      <part-name>Piccolo</part-name>
      <part-abbreviation>Picc.</part-abbreviation>
      <score-instrument id="P1-I18">
        <instrument-name>Picc.</instrument-name>
      </score-instrument>
      <midi-instrument id="P1-I18">
        <midi-channel>1</midi-channel>
        <midi-program>73</midi-program>
      </midi-instrument>
    </score-part>
    
    <part-group number="2" type="start">
      <group-name>1
2</group-name>
      <group-barline>yes</group-barline>
    </part-group>
*/

  public visitor<S_part_list>,
  
  public visitor<S_part_group>,
  public visitor<S_group_symbol>,
  public visitor<S_group_barline>,
  
  public visitor<S_score_part>,
  public visitor<S_part_name>,
  public visitor<S_part_abbreviation>,
  public visitor<S_instrument_name>,
  
  public visitor<S_part>,

  public visitor<S_staves>,
  public visitor<S_staff>,

  public visitor<S_voice>,
  
  public visitor<S_lyric>,

  public visitor<S_measure>,
  public visitor<S_barline>,
  public visitor<S_bar_style>,
  public visitor<S_repeat>,

  public visitor<S_note>
{
  public:
  
    xml2LpsrDictionaryVisitor (
        S_translationSettings& ts);
    virtual ~xml2LpsrDictionaryVisitor ();

    S_lpsrDictionary buildDictionaryFromXMLElementTree (
        const Sxmlelement& xmlTree);

    S_lpsrDictionary getDictionary () const
        { return fDictionary; };
    
  protected:
      
    virtual void visitStart ( S_part_list& elt);
    
    virtual void visitStart ( S_part_group& elt);
    virtual void visitStart ( S_group_symbol& elt);
    virtual void visitStart ( S_group_barline& elt);
    
    virtual void visitStart ( S_score_part& elt);
    virtual void visitStart ( S_part_name& elt);
    virtual void visitStart ( S_part_abbreviation& elt);
    virtual void visitStart ( S_instrument_name& elt);
    
    virtual void visitStart ( S_part& elt);
    
    virtual void visitStart ( S_staves& elt);
    virtual void visitStart ( S_staff& elt);
    
    virtual void visitStart ( S_voice& elt );
    
    virtual void visitStart ( S_lyric& elt);
    
    virtual void visitStart ( S_measure& elt);

    virtual void visitEnd   ( S_note& elt);

  private:

    void         internalError (
                    string message);
                     
    S_translationSettings fTranslationSettings;

    S_lpsrDictionary      fDictionary;

    // the current part group, if any
    // the part group number indicates nested/overlapping groups
    int                   fCurrentPartGroupNumber;
    string                fCurrentPartGroupType;
    S_lpsrPartGroup       fCurrentPartGroup;
    string                fCurrentGroupSymbol;
    string                fCurrentGroupBarline;
    
    // number of staves (from the staves element)
    int                   fStavesNumber;
    
    int                   fCurrentStaffNumber;
    S_lpsrStaff           fCurrentStaff;

    string                fCurrentPartMusicXMLName;
    S_lpsrPart            fCurrentPart;

    int                   fCurrentVoiceNumber;
    S_lpsrVoice           fCurrentVoice;
    
    int                   fCurrentLyricNumber;
    S_lpsrLyrics          fCurrentLyrics;

/*
  <barline location="left">
    <bar-style>heavy-light</bar-style>
    <repeat direction="forward"/>
  </barline>
  *
  *   <barline location="right">
    <bar-style>light-heavy</bar-style>
    <repeat direction="backward"/>
  </barline>
  * 
  */
    int                   fCurrentMeasureNumber;
};


/*! @} */

}

#endif
