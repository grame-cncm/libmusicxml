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

#include "msrPartGroupElements.h"

#include "msr_MUTUAL_DEPENDENCIES.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class msrScore;
typedef SMARTP<msrScore> S_msrScore;

//______________________________________________________________________________
class msrPartGroup : public msrPartGroupElement
{
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

  public:

    // data types
    // ------------------------------------------------------

    enum msrPartGroupImplicitKind { // an MSR concept, not present in MusicXML
        kPartGroupImplicitYes, kPartGroupImplicitNo
      };

    static string partGroupImplicitKindAsString (
      msrPartGroupImplicitKind partGroupImplicitKind);

    enum msrPartGroupTypeKind {
      kPartGroupTypeNone,
      kPartGroupTypeStart, kPartGroupTypeStop
    };

    static string partGroupTypeKindAsString (
      msrPartGroupTypeKind partGroupTypeKind);

    enum msrPartGroupSymbolKind {
      kPartGroupSymbolNone,
      kPartGroupSymbolBrace, kPartGroupSymbolBracket,
      kPartGroupSymbolLine, kPartGroupSymbolSquare
    };

    static string partGroupSymbolKindAsString (
      msrPartGroupSymbolKind partGroupSymbolKind);

    enum msrPartGroupBarlineKind {
      kPartGroupBarlineYes, kPartGroupBarlineNo
    };

    static string partGroupBarlineKindAsString (
      msrPartGroupBarlineKind partGroupBarlineKind);

  public:

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
      msrPartGroupImplicitKind partGroupImplicitKind,
      msrPartGroupBarlineKind  partGroupBarlineKind,
      S_msrPartGroup           partGroupPartGroupUpLink,
      S_msrScore               partGroupScoreUpLink);

    static SMARTP<msrPartGroup> createImplicitPartGroup (
      int                      partGroupNumber,
      int                      partGroupAbsoluteNumber,
      string                   partGroupName,
      string                   partGroupNameDisplayText,
      string                   partGroupAccidentalText,
      string                   partGroupAbbreviation,
      msrPartGroupBarlineKind  partGroupBarlineKind,
      S_msrScore               partGroupScoreUpLink);

    SMARTP<msrPartGroup> createPartGroupNewbornClone (
      S_msrPartGroup partGroupClone, // the upLink for nested part groups
      S_msrScore     scoreClone);

    // creation from the API
    // ------------------------------------------------------

    static SMARTP<msrPartGroup> create (
      int                      inputLineNumber,
      int                      partGroupNumber,
      int                      partGroupAbsoluteNumber,
      string                   partGroupName,
      S_msrPartGroup           partGroupPartGroupUpLink,
      S_msrScore               partGroupScoreUpLink);

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
      S_msrPartGroup           partGroupPartGroupUpLink,
      S_msrScore               partGroupScoreUpLink);

    msrPartGroup (
      int                      inputLineNumber,
      int                      partGroupNumber,
      int                      partGroupAbsoluteNumber,
      string                   partGroupName,
      S_msrPartGroup           partGroupPartGroupUpLink,
      S_msrScore               partGroupScoreUpLink);

    virtual ~msrPartGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // upLinks
    void                  setPartGroupPartGroupUpLink (
                            S_msrPartGroup partGroup)
                              {
                                fPartGroupPartGroupUpLink =
                                  partGroup;
                              }

    S_msrPartGroup        getPartGroupPartGroupUpLink () const
                              { return fPartGroupPartGroupUpLink; }

    S_msrScore            getPartGroupScoreUpLink () const
                              { return fPartGroupScoreUpLink; }

    // numbers
    int                   getPartGroupAbsoluteNumber () const
                              { return fPartGroupAbsoluteNumber; }

    int                   getPartGroupNumber () const
                              { return fPartGroupNumber; }

    // names

    string                getPartGroupCombinedName () const;

    string                getPartGroupCombinedNameWithoutEndOfLines () const;
                            // for comments in LilyPond code

    // miscellaneous

    string                getPartGroupNameDisplayText () const
                              { return fPartGroupNameDisplayText; }

    string                getPartGroupAccidentalText () const
                              { return fPartGroupAccidentalText; }

    string                getPartGroupName () const
                              { return fPartGroupName; }

    string                getPartGroupAbbreviation () const
                              { return fPartGroupAbbreviation; }

    // symbol

    msrPartGroupSymbolKind
                          getPartGroupSymbolKind () const
                              { return fPartGroupSymbolKind; }

    int                   getPartGroupSymbolDefaultX () const
                              { return fPartGroupSymbolDefaultX; }

    // implicit

    msrPartGroupImplicitKind
                          getPartGroupImplicitKind () const
                              { return fPartGroupImplicitKind; }

    // type


    // barline

    msrPartGroupBarlineKind
                          getPartGroupBarlineKind () const
                              { return fPartGroupBarlineKind; }

    // instrument name

    void                  setPartGroupInstrumentName (string name);

    string                getPartGroupInstrumentName () const
                              { return fPartGroupInstrumentName; }

    // elements

    const list<S_msrPartGroupElement>&
                          getPartGroupElements () const
                              { return fPartGroupElements; }

  public:

    // public services
    // ------------------------------------------------------

    S_msrPart             appendPartToPartGroupByItsPartID ( // JMI superflous
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

    // voices

    void                  registerVoiceInPartGroupAllVoicesList (
                            S_msrVoice voice);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

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

    virtual string        asString () const override;

    virtual void          print (ostream& os) const override;

    virtual void          printShort (ostream& os) const override;

    virtual void          printSummary (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks

    S_msrPartGroup        fPartGroupPartGroupUpLink;
                            // part groups can be nested

    S_msrScore            fPartGroupScoreUpLink;

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

    // type


    // bar line

    msrPartGroupBarlineKind
                          fPartGroupBarlineKind;

    // instrument name

    string                fPartGroupInstrumentName; // JMI

    // accessing parts by name
    map<string, S_msrPart>
                          fPartGroupPartsMap;

    // allowing for both parts and (sub-)part groups as elements
    list<S_msrPartGroupElement>
                          fPartGroupElements;

    // voices

    list<S_msrVoice>      fPartGroupAllVoicesList;
};
typedef SMARTP<msrPartGroup> S_msrPartGroup;
EXP ostream& operator<< (ostream& os, const S_msrPartGroup& elt);


}

#endif
