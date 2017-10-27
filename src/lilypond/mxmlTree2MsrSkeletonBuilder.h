/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __mxmlTree2MsrSkeletonBuilder__
#define __mxmlTree2MsrSkeletonBuilder__


namespace MusicXML2
{

/*!
\addtogroup mxmltree2Msr
@{
*/

/*!
\brief Produces a summary of a MusicXML part.
*/
class EXP mxmlTree2MsrSkeletonBuilder :

  // score

  public visitor<S_score_partwise>,
  
  // parts & part groups
  
  public visitor<S_part_list>,
  
  public visitor<S_part_group>,
  public visitor<S_group_name>,
  public visitor<S_group_name_display>,
  public visitor<S_display_text>,
  public visitor<S_accidental_text>,
  public visitor<S_group_abbreviation>,
  public visitor<S_group_symbol>,
  public visitor<S_group_barline>,

  public visitor<S_score_part>,
  public visitor<S_part_name>,
  public visitor<S_part_abbreviation>,
  public visitor<S_instrument_name>,
  public visitor<S_instrument_abbreviation>,
  
  public visitor<S_part>,

  // staves
  
  public visitor<S_staves>,
  public visitor<S_staff>,
    
  // voices

  public visitor<S_voice>,

  // measures
  
  public visitor<S_measure>,
  
  // notes
  
  public visitor<S_note>,
  
  // lyrics
  
  public visitor<S_lyric>,
  public visitor<S_syllabic>,
  public visitor<S_text>,
  public visitor<S_elision>,
  public visitor<S_extend>,
  
  // harmonies
  
  public visitor<S_harmony>,
  
  // figured bass
  
  public visitor<S_figured_bass>

{
  public:
  
    // constructors/destructor
    // ------------------------------------------------------

    mxmlTree2MsrSkeletonBuilder (
      indentedOstream& ios);

    virtual ~mxmlTree2MsrSkeletonBuilder ();

    // set and get
    // ------------------------------------------------------

    S_msrScore            getMsrScore () const
                              { return fMsrScore; };


    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    void browseMxmlTree (
        const Sxmlelement& xmlTree);
  
  protected:

    // score
  
    virtual void visitStart ( S_score_partwise& elt);
    virtual void visitEnd   ( S_score_partwise& elt);
    
  
    // parts & part groups
    // ------------------------------------------------------

    virtual void visitStart ( S_part_list& elt);
    virtual void visitEnd   ( S_part_list& elt);
    
    virtual void visitStart ( S_part_group& elt);
    virtual void visitEnd   ( S_part_group& elt);
    virtual void visitStart ( S_group_name& elt);
    virtual void visitStart ( S_group_name_display& elt);
    virtual void visitStart ( S_display_text& elt);
    virtual void visitStart ( S_accidental_text& elt);
    virtual void visitStart ( S_group_abbreviation& elt);
    virtual void visitStart ( S_group_symbol& elt);
    virtual void visitStart ( S_group_barline& elt);
    
    virtual void visitStart ( S_score_part& elt);
    virtual void visitEnd   ( S_score_part& elt);
    virtual void visitStart ( S_part_name& elt);
    virtual void visitStart ( S_part_abbreviation& elt);
    virtual void visitStart ( S_instrument_name& elt);
    virtual void visitStart ( S_instrument_abbreviation& elt);
    
    virtual void visitStart ( S_part& elt);
    virtual void visitEnd   ( S_part& elt);
    
    // staves
    // ------------------------------------------------------

    virtual void visitStart ( S_staves& elt);
    
    virtual void visitStart ( S_staff& elt);
    
    // voices
    // ------------------------------------------------------

    virtual void visitStart ( S_voice& elt );
    
    // measures
    // ------------------------------------------------------

    virtual void visitStart ( S_measure& elt);
    virtual void visitEnd   ( S_measure& elt);
    
    // notes
    // ------------------------------------------------------

    virtual void visitStart ( S_note& elt);
    virtual void visitEnd   ( S_note& elt);
    
    // lyrics
    // ------------------------------------------------------
    
    virtual void visitStart ( S_lyric& elt);
    virtual void visitEnd   ( S_lyric& elt);

    // harmonies
    // ------------------------------------------------------
    
    virtual void visitStart ( S_harmony& elt);
    
    // figured bass
    // ------------------------------------------------------
    
    virtual void visitStart ( S_figured_bass& elt);

  private:
                     
    indentedOstream&          fLogOutputStream;

    
    // the MSR score we're building
    // ------------------------------------------------------
    
    S_msrScore                fMsrScore;

    
    // score handling

    int                       fScoreNumberOfMeasures;

    // part groups handling
    // ------------------------------------------------------
    
    /*
      There is no hierarchy implied in part-group elements.
      All that matters is the sequence of part-group elements relative to score-part elements.
      The sequencing of two consecutive part-group elements does not matter.
      It is the default-x attribute that indicates the left-to-right ordering of the group symbols.

      <part-group number="1" type="start">
      <group-name>Trombones</group-name>
      <group-abbreviation>Trb.</group-abbreviation>
      <group-symbol default-x="-12">brace</group-symbol>
      <group-barline>yes</group-barline>
      </part-group>
    */
    
    int                       fCurrentPartGroupNumber;
    msrPartGroup::msrPartGroupTypeKind
                              fCurrentPartGroupTypeKind;
    string                    fCurrentPartGroupName;
    bool                      fOnGoingGroupNameDisplay;
    string                    fCurrentPartGroupDisplayText;
    string                    fCurrentPartGroupAccidentalText;
    string                    fCurrentPartGroupAbbreviation;
    msrPartGroup::msrPartGroupSymbolKind
                              fCurrentPartGroupSymbolKind;
    msrPartGroup::msrPartGroupBarlineKind
                              fCurrentPartGroupBarlineKind;
    int                       fCurrentPartGroupSymbolDefaultX;

    // an implicit part group has to be created
    // if none is specified in the MusicXML data,
    // in which case a part group 'stop' has to be forced later
    S_msrPartGroup            fImplicitPartGroup;
                                    
    void                      createImplicitPartGroup (
                                int inputLineNumber);
                                
    bool                      fCurrentPartUsesImplicitPartGroup;
    
    // part groups numbers can be re-used, they're no identifier
    // we use a map to access them by part group number
    map<int, S_msrPartGroup>  fPartGroupsMap;
    
    S_msrPartGroup            fetchPartGroupInTheMap (
                                int partGroupNumber);

    // MusicXML allows part groups to overlap,
    // we use a list in which part groups are orderd by
    // increasing part group <default-x>
    // (all of them are negative)    
    // the current part group is either null or the front of the list
    
    list<S_msrPartGroup>      fPartGroupsList;
    
    set<S_msrPartGroup>       fStartedPartGroupsSet;

    // handling 'start' and 'stop'
 // JMI   stack<S_msrPartGroup>     fPartGroupsStack;

    void                      showPartGroupsMap (
                                int    inputLineNumber,
                                string context);

    void                      showStartedPartGroupsSet (
                                int    inputLineNumber,
                                string context);

/* JMI                                    
    void                      showPartGroupsStack (
                                int    inputLineNumber,
                                string context);
    */
    
    void                      showPartGroupsList (
                                int    inputLineNumber,
                                string context);
    
    void                      showPartGroupsData (
                                int    inputLineNumber,
                                string context);
    
    void                      handlePartGroupStart (
                                int inputLineNumber);
                                
    void                      handlePartGroupStop (
                                int inputLineNumber);


    // parts handling
    // ------------------------------------------------------
    
    string                    fCurrentPartID; // used throughout
    string                    fCurrentPartName;
    string                    fCurrentPartAbbreviation;
    string                    fCurrentPartInstrumentName;
    string                    fCurrentPartInstrumentAbbreviation;

    map<string, S_msrPart>    fPartsMap;

    int                       fCurrentNoIDPartNumber;
    
    S_msrPart                 fCurrentPart; // used throughout


    // staff handling
    // ------------------------------------------------------
    
    int                       fCurrentStaffNumber; // used throughout
    
    S_msrStaff                createStaffIfNotYetDone (
                                int            inputLineNumber,
                                int            staffNumber);


    // voice handling
    // ------------------------------------------------------
    
    int                       fCurrentVoiceNumber; // used throughout

    S_msrVoice                createVoiceIfNotYetDone (
                                int inputLineNumber,
                                int staffNumber,
                                int voiceNumber);
    

    // measures
    // ------------------------------------------------------
    
    string                    fCurrentMeasureNumber;
    int                       fPartNumberOfMeasures;
    

    // notes handling
    // ------------------------------------------------------

    bool                      fOnGoingNote;

    int                       fCurrentNoteStaffNumber;
    int                       fCurrentNoteVoiceNumber;


    // lyrics handling
    // ------------------------------------------------------
        
    string                    fCurrentStanzaNumber;
    

    // harmonies handling
    // ------------------------------------------------------
        

    // figured bass handling
    // ------------------------------------------------------
    
    };


/*! @} */

}

#endif
