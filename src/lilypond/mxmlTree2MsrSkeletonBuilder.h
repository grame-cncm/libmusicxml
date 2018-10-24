/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___mxmlTree2MsrSkeletonBuilder___
#define ___mxmlTree2MsrSkeletonBuilder___


namespace MusicXML2
{

//________________________________________________________________________
struct mxmlPartGroupDescr : public smartable
{
/*
 * positions represent the order in which the parts appear in <part-list />
*/
 
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxmlPartGroupDescr> create (
      int            startInputLineNumber,
      int            partGroupNumber,
      S_msrPartGroup partGroup,
      int            startPosition);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    mxmlPartGroupDescr (
      int            startInputLineNumber,
      int            partGroupNumber,
      S_msrPartGroup partGroup,
      int            startPosition);

    virtual ~mxmlPartGroupDescr ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getStartInputLineNumber () const
                              { return fStartInputLineNumber; }
    
    int                   getStopInputLineNumber () const
                              { return fStopInputLineNumber; }
    
    int                   getPartGroupNumber () const
                              { return fPartGroupNumber; }
    
    S_msrPartGroup        getPartGroup () const
                              { return fPartGroup; }
    
    int                   getStartPosition () const
                              { return fStartPosition; }
    
    void                  setStopPosition (
                            int stopInputLineNumber,
                            int stopPosition)
                              {
                                fStopInputLineNumber = stopInputLineNumber;
                                fStopPosition = stopPosition;
                              }
    
    int                   getStopPosition () const
                              { return fStopPosition; }
    
    // services
    // ------------------------------------------------------

    string                partGroupDescrAsString () const;

    string                getPartGroupCombinedName () const
                              {
                               return
                                fPartGroup->
                                  getPartGroupCombinedName ();
                              }

    // print
    // ------------------------------------------------------
    
    virtual void          print (ostream& os) const;
    
  private:
     
    // fields
    // ------------------------------------------------------

    int                   fStartInputLineNumber;
    int                   fStopInputLineNumber;
    
    int                   fPartGroupNumber; // may be reused later
    
    S_msrPartGroup        fPartGroup;
    
    int                   fStartPosition;
    int                   fStopPosition;
};
typedef SMARTP<mxmlPartGroupDescr> S_mxmlPartGroupDescr;
EXP ostream& operator<< (ostream& os, const S_mxmlPartGroupDescr& elt);

//________________________________________________________________________
class mxmlTree2MsrSkeletonBuilder :

  public visitor<S_encoding>,

  // score partwise

  public visitor<S_score_partwise>,
  
  // rights
  
  public visitor<S_work_number>,
  public visitor<S_work_title>,
  public visitor<S_movement_number>,
  public visitor<S_movement_title>,
  public visitor<S_creator>,
  public visitor<S_rights>,
  public visitor<S_software>,
  public visitor<S_encoding_date>,
  public visitor<S_miscellaneous_field>,
  
  // geometry
  
  public visitor<S_millimeters>,
  public visitor<S_tenths>,
  public visitor<S_scaling>,
 
  public visitor<S_system_margins>,
  public visitor<S_system_distance>,
  public visitor<S_top_system_distance>,
  public visitor<S_system_dividers>,
  
  // layout
  
  public visitor<S_page_layout>,
  public visitor<S_page_height>,
  public visitor<S_page_width>,
  public visitor<S_left_margin>,
  public visitor<S_right_margin>,
  public visitor<S_top_margin>,
  public visitor<S_bottom_margin>,

  // credits
  
  public visitor<S_credit>,
  public visitor<S_credit_words>,
  
  // parts & part groups
  
  public visitor<S_part_list>,
  
  public visitor<S_display_text>,

  public visitor<S_part_group>,
  
  public visitor<S_group_name>,
  public visitor<S_group_name_display>,
  
  public visitor<S_accidental_text>,
  public visitor<S_group_abbreviation>,
  public visitor<S_group_symbol>,
  public visitor<S_group_barline>,

  public visitor<S_score_part>,
  
  public visitor<S_part_name>,
  public visitor<S_part_name_display>,
  
  public visitor<S_part_abbreviation>,
  public visitor<S_part_abbreviation_display>,
  
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

    virtual void visitStart ( S_encoding& elt);

    // score partwise
  
    virtual void visitStart ( S_score_partwise& elt);
    virtual void visitEnd   ( S_score_partwise& elt);
    
  
    // rights
    // ------------------------------------------------------

    virtual void visitStart ( S_work_number& elt);
    virtual void visitStart ( S_work_title& elt);
    virtual void visitStart ( S_movement_number& elt);
    virtual void visitStart ( S_movement_title& elt);
    virtual void visitStart ( S_creator& elt);
    virtual void visitStart ( S_rights& elt);
    virtual void visitStart ( S_software& elt);
    virtual void visitStart ( S_encoding_date& elt);
    virtual void visitStart ( S_miscellaneous_field& elt);

    // geometry
    // ------------------------------------------------------

    virtual void visitStart ( S_millimeters& elt);
    virtual void visitStart ( S_tenths& elt);
    virtual void visitEnd   ( S_scaling& elt);

    virtual void visitStart ( S_system_margins& elt);
    virtual void visitStart ( S_system_distance& elt);
    virtual void visitStart ( S_top_system_distance& elt);
    virtual void visitStart ( S_system_dividers& elt);

    // layout
    // ------------------------------------------------------

    virtual void visitStart ( S_page_layout& elt);
    virtual void visitEnd   ( S_page_layout& elt);
    virtual void visitStart ( S_page_height& elt);
    virtual void visitStart ( S_page_width& elt);
    virtual void visitStart ( S_left_margin& elt);
    virtual void visitStart ( S_right_margin& elt);
    virtual void visitStart ( S_top_margin& elt);
    virtual void visitStart ( S_bottom_margin& elt);
    
    // credits
    // ------------------------------------------------------

    virtual void visitStart ( S_credit& elt);
    virtual void visitEnd   ( S_credit& elt);
    virtual void visitStart ( S_credit_words& elt);
        
    // parts & part groups
    // ------------------------------------------------------

    virtual void visitStart ( S_part_list& elt);
    virtual void visitEnd   ( S_part_list& elt);
    
    virtual void visitStart ( S_display_text& elt);

    virtual void visitStart ( S_part_group& elt);
    virtual void visitEnd   ( S_part_group& elt);
    
    virtual void visitStart ( S_group_name& elt);
    virtual void visitStart ( S_group_name_display& elt);
    virtual void visitEnd   ( S_group_name_display& elt);
    
    virtual void visitStart ( S_accidental_text& elt);
    virtual void visitStart ( S_group_abbreviation& elt);
    virtual void visitStart ( S_group_symbol& elt);
    virtual void visitStart ( S_group_barline& elt);
    
    virtual void visitStart ( S_score_part& elt);
    virtual void visitEnd   ( S_score_part& elt);
    
    virtual void visitStart ( S_part_name& elt);
    virtual void visitStart ( S_part_name_display& elt);
    virtual void visitEnd   ( S_part_name_display& elt);

    virtual void visitStart ( S_part_abbreviation& elt);
    virtual void visitStart ( S_part_abbreviation_display& elt);
    virtual void visitEnd   ( S_part_abbreviation_display& elt);

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

    // geometry handling
    // ------------------------------------------------------
    
    float                     fCurrentMillimeters;
    int                       fCurrentTenths;
    bool                      fOnGoingPageLayout;

    // credits handling
    // ------------------------------------------------------
    
    S_msrCredit               fCurrentCredit;
    
    // part groups handling
    // ------------------------------------------------------
    
    int                       fCurrentPartGroupNumber;
    msrPartGroup::msrPartGroupTypeKind
                              fCurrentPartGroupTypeKind;
    string                    fCurrentPartGroupName;
    string                    fCurrentPartGroupAbbreviation;
    
    bool                      fOnGoingPartGroupNameDisplay;
    string                    fCurrentPartGroupNameDisplayText;

    string                    fCurrentPartGroupAccidentalText;
    msrPartGroup::msrPartGroupSymbolKind
                              fCurrentPartGroupSymbolKind;
    msrPartGroup::msrPartGroupBarlineKind
                              fCurrentPartGroupBarlineKind;
    int                       fCurrentPartGroupSymbolDefaultX;

    // an implicit part group has to be created to contain everything,
    // since there can be parts out of any explicit part group
    S_mxmlPartGroupDescr      fImplicitPartGroupDescr;
    S_msrPartGroup            fImplicitPartGroup;
                                    
    void                      createImplicitPartGroup ();
                                    
    // part groups numbers can be re-used, they're no identifier
    // we use a map to access them by part group number
    int                       fPartGroupsCounter;
    vector<S_mxmlPartGroupDescr>
                              fPartGroupDescsVector;
    map<int, S_mxmlPartGroupDescr>
                              fAllPartGroupDescrsMap;
    map<int, S_mxmlPartGroupDescr>
                              fStartedPartGroupDescrsMap;

    void                      showPartGroupDescrsVector (
                                int inputLineNumber);
    
    void                      showAllPartGroupDescrsMap (
                                int inputLineNumber);

    void                      showStartedPartGroupDescrsMap (
                                int inputLineNumber);

    S_mxmlPartGroupDescr      fetchStartedPartGroupDescr (
                                int partGroupNumber);
    
    list<S_mxmlPartGroupDescr>
                              fPartGroupsDescrStack;

    void                      showPartGroupsStack (
                                int inputLineNumber);

    S_mxmlPartGroupDescr      fetchPartGroupDescrStackTop ();

    void                      registerPartGroupDescrAsStarted (
                                int                  inputLineNumber,
                                S_mxmlPartGroupDescr partGroupDescr);
        
    void                      registerPartGroupDescrAsStopped (
                                int                  inputLineNumber,
                                S_mxmlPartGroupDescr partGroupDescr);
        
    void                      showPartGroupsData (
                                int    inputLineNumber,
                                string context);
    
    void                      handlePartGroupStart (
                                int inputLineNumber);
                                
    void                      handlePartGroupStop (
                                int inputLineNumber);

    vector<list<S_mxmlPartGroupDescr> >
                              fPositionStartingPartGroupDescrsVector;
    vector<list<S_mxmlPartGroupDescr> >
                              fPositionStoppingPartGroupDescrsVector;

    void                      showPositionStartingPartGroupDescrsVector (
                                int inputLineNumber);
                                
    void                      showPositionStoppingPartGroupDescrsVector (
                                int inputLineNumber);
                                
    void                      insertPartGroupDescInStartingList (
                                int                  inputLineNumber,
                                S_mxmlPartGroupDescr partGroupDescr,
                                list<S_mxmlPartGroupDescr>&
                                                     startingPartGroupDescrsList);
  
    void                      insertPartGroupDescInStoppingList (
                                int                  inputLineNumber,
                                S_mxmlPartGroupDescr partGroupDescr,
                                list<S_mxmlPartGroupDescr>&
                                                     stoppingPartGroupDescrsList);
  /* JMI
    void                      registerPartGroupDescrAsStartingAtCurrentPosition (
                                int                  inputLineNumber,
                                S_mxmlPartGroupDescr partGroupDescr);
   */
          
    void                      registerPartGroupDescrAsStoppingAtCurrentPosition (
                                int                  inputLineNumber,
                                S_mxmlPartGroupDescr partGroupDescr);
                                
    void                      registerPart (
                                int       inputLineNumber,
                                int       partPosition,
                                S_msrPart part);

    void                      doNestPartGroupDescrInItsContainer (
                                int                  inputLineNumber,
                                S_mxmlPartGroupDescr partGroupDescrToBeStopped,
                                S_mxmlPartGroupDescr containingPartGroupDescr);
                                
    void                      doPartGroupsNestingAndPartsAllocation (
                                int inputLineNumber);



    // parts handling
    // ------------------------------------------------------
    
    string                    fCurrentPartID; // used throughout

    string                    fCurrentPartName;
    string                    fCurrentPartNameDisplayText;
    bool                      fOnGoingPartNameDisplay;

    
    string                    fCurrentPartAbbreviation;
    string                    fCurrentPartAbbreviationDisplayText;
    bool                      fOnGoingPartAbbreviationDisplay;

    string                    fCurrentPartInstrumentName;
    string                    fCurrentPartInstrumentAbbreviation;

    map<string, S_msrPart>    fPartsMap;

    int                       fCurrentNoIDPartNumber;
    
    // handling the part, each one incrementing the 'position'
    // of the part groups, used for handling nesting/overlapping
    int                       fCurrentPartsPosition;
    vector<S_msrPart>         fPartsVector;
    
    void                      showPartsVector (
                                int inputLineNumber);

    S_msrPart                 fCurrentPart; // used throughout


    // staff handling
    // ------------------------------------------------------
    
    int                       fCurrentStaffMusicXMLNumber; // used throughout
    
    S_msrStaff                createStaffInCurrentPartIfNotYetDone (
                                int            inputLineNumber,
                                int            staffNumber);


    // voice handling
    // ------------------------------------------------------
    
    int                       fCurrentVoiceMusicXMLNumber; // used throughout

    S_msrVoice                createRegularVoiceInStaffIfNotYetDone (
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


    // lyrics handling
    // ------------------------------------------------------
        
    string                    fCurrentStanzaNumber;
    string                    fCurrentStanzaName;
    

    // harmonies handling
    // ------------------------------------------------------

    bool                      fThereAreHarmoniesToBeAttachedToCurrentNote;
    int                       fHarmonyVoicesCounter;

    S_msrVoice                createHarmonyVoiceForVoiceIfNotYetDone (
                                int        inputLineNumber,
                                S_msrVoice voice);


    // figured bass handling
    // ------------------------------------------------------
    
};


}

#endif
