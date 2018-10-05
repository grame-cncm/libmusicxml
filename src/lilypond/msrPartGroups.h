/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrPartGroups___
#define ___msrPartGroups___

#include "msr.h" // TEMP
#include "msrScores.h" // TEMP


namespace MusicXML2 
{
 
//______________________________________________________________________________
class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class msrScore;
typedef SMARTP<msrScore> S_msrScore;

//______________________________________________________________________________
class msrPartGroup : public msrElement
{
  public:

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

    // data types
    // ------------------------------------------------------

    enum msrPartGroupImplicitKind {
        kPartGroupImplicitYes, kPartGroupImplicitNo};
          
    static string partGroupImplicitKindAsString (
      msrPartGroupImplicitKind partGroupImplicitKind);
      
    enum msrPartGroupTypeKind {
        kPartGroupTypeNone,
        kPartGroupTypeStart, kPartGroupTypeStop };
          
    static string partGroupTypeKindAsString (
      msrPartGroupTypeKind partGroupTypeKind);
      
    enum msrPartGroupSymbolKind {
        kPartGroupSymbolNone,
        kPartGroupSymbolBrace, kPartGroupSymbolBracket,
        kPartGroupSymbolLine, kPartGroupSymbolSquare};
          
    static string partGroupSymbolKindAsString (
      msrPartGroupSymbolKind partGroupSymbolKind);
      
    enum msrPartGroupBarlineKind {
        kPartGroupBarlineYes, kPartGroupBarlineNo};
          
    static string partGroupBarlineKindAsString (
      msrPartGroupBarlineKind partGroupBarlineKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPartGroup> create (
      int                      inputLineNumber,
      int                      partGroupNumber,
      int                      partGroupAbsoluteNumber,
      string                   partGroupName,
      string                   partGroupNameDisplayText,
      string                   partGroupAccidentalText,
      string                   partGroupAbbreviation,
      msrPartGroupSymbolKind   partGroupSymbolKind,
      int                      partGroupSymbolDefaultX,
      msrPartGroupBarlineKind  partGroupBarlineKind,
      S_msrPartGroup           partGroupPartGroupUplink,
      S_msrScore               partGroupScoreUplink);

    static SMARTP<msrPartGroup> createImplicitPartGroup (
      int                      partGroupNumber,
      int                      partGroupAbsoluteNumber,
      string                   partGroupName,
      string                   partGroupNameDisplayText,
      string                   partGroupAccidentalText,
      string                   partGroupAbbreviation,
      msrPartGroupBarlineKind  partGroupBarlineKind,
      S_msrScore               partGroupScoreUplink);

    SMARTP<msrPartGroup> createPartGroupNewbornClone (
      S_msrPartGroup partGroupClone, // the uplink for embeddeed part groups
      S_msrScore     scoreClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPartGroup (
      int                      inputLineNumber,
      int                      partGroupNumber,
      int                      partGroupAbsoluteNumber,
      string                   partGroupName,
      string                   partGroupNameDisplayText,
      string                   partGroupAccidentalText,
      string                   partGroupAbbreviation,
      msrPartGroupSymbolKind   partGroupSymbolKind,
      int                      partGroupSymbolDefaultX,
      msrPartGroupImplicitKind partGroupImplicitKind,
      msrPartGroupBarlineKind  partGroupBarlineKind,
      S_msrPartGroup           partGroupPartGroupUplink,
      S_msrScore               partGroupScoreUplink);
            
    virtual ~msrPartGroup ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setPartGroupPartGroupUplink (
                            S_msrPartGroup partGroup)
                              {
                                fPartGroupPartGroupUplink =
                                  partGroup;
                              }

    S_msrPartGroup        getPartGroupPartGroupUplink () const
                              { return fPartGroupPartGroupUplink; }

    S_msrScore            getPartGroupScoreUplink () const
                              { return fPartGroupScoreUplink; }

    // numbers
    int                   getPartGroupAbsoluteNumber () const
                              { return fPartGroupAbsoluteNumber; }
    
    int                   getPartGroupNumber () const
                              { return fPartGroupNumber; }

    // names
    
    string                getPartGroupCombinedName () const;

    // miscellaneous

    string                getPartGroupNameDisplayText () const
                              { return fPartGroupNameDisplayText; }

    string                getPartGroupAccidentalText () const
                              { return fPartGroupAccidentalText; }

    string                getPartGroupName () const
                              { return fPartGroupName; }

    string                getPartGroupAbbreviation () const
                              { return fPartGroupAbbreviation; }

    msrPartGroupSymbolKind
                          getPartGroupSymbolKind () const
                              { return fPartGroupSymbolKind; }

    int                   getPartGroupSymbolDefaultX () const
                              { return fPartGroupSymbolDefaultX; }

    msrPartGroupImplicitKind
                          getPartGroupImplicitKind () const
                              { return fPartGroupImplicitKind; }
    
    msrPartGroupBarlineKind
                          getPartGroupBarlineKind () const
                              { return fPartGroupBarlineKind; }
    
    void                  setPartGroupInstrumentName (string name);
                
    string                getPartGroupInstrumentName () const
                              { return fPartGroupInstrumentName; }
                
    const list<S_msrElement>&
                          getPartGroupElements () const
                              { return fPartGroupElements; }

    // services
    // ------------------------------------------------------

    S_msrPart             appendPartToPartGroupByItsPartID (
                            int    inputLineNumber,
                            string partID);
    
    void                  appendPartToPartGroup (S_msrPart part);
    
    void                  removePartFromPartGroup (
                            int       inputLineNumber,
                            S_msrPart partToBeRemoved);
                
    void                  prependSubPartGroupToPartGroup (
                            S_msrPartGroup partGroup);

    void                  appendSubPartGroupToPartGroup (
                            S_msrPartGroup partGroup);

    S_msrPart             fetchPartFromPartGroupByItsPartID (
                            int    inputLineNumber,
                            string partID);

    void                  collectPartGroupPartsList (
                            int    inputLineNumber,
                            list<S_msrPart>& partsList);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                partGroupSymbolKindAsString () const
                              {
                                return
                                  partGroupSymbolKindAsString (
                                    fPartGroupSymbolKind);
                              }

    void                  printPartGroupParts (
                            int      inputLineNumber,
                            ostream& os);
  
    string                partGroupImplicitKindAsString () const;
    
    string                partGroupBarlineKindAsString () const;

    virtual string        asString () const;
    
    virtual void          print (ostream& os);

    virtual void          printSummary (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    
    S_msrPartGroup        fPartGroupPartGroupUplink;
                            // part groups can be nested

    S_msrScore            fPartGroupScoreUplink;

    // numbers
    
    int                   fPartGroupNumber;
    int                   fPartGroupAbsoluteNumber;    

    // name
    
    string                fPartGroupName;
    string                fPartGroupNameDisplayText;
    
    string                fPartGroupAccidentalText;
    
    string                fPartGroupAbbreviation;

    // symbol kind
    
    msrPartGroupSymbolKind
                          fPartGroupSymbolKind;

    // default X
    
    int                   fPartGroupSymbolDefaultX;

    // implicit
    
    msrPartGroupImplicitKind
                          fPartGroupImplicitKind;

    // bar line
    
    msrPartGroupBarlineKind
                          fPartGroupBarlineKind;

    // instrument name

    string                fPartGroupInstrumentName; // JMI

    // accessing parts by name
    map<string, S_msrPart>
                          fPartGroupPartsMap;

    // allowing for both parts and (sub-)part groups as elements
    list<S_msrElement>    fPartGroupElements;
};
typedef SMARTP<msrPartGroup> S_msrPartGroup;
EXP ostream& operator<< (ostream& os, const S_msrPartGroup& elt);


}

#endif
