/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrScores___
#define ___msrScores___

#include <iostream>

#include <list>
#include <set>

#include "msrIdentification.h"
#include "msrPageGeometry.h"

#include "msr.h" // TEMP


namespace MusicXML2 
{

//______________________________________________________________________________
class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

//______________________________________________________________________________
class msrScore : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrScore> create (
      int inputLineNumber);

    SMARTP<msrScore> createScoreNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrScore (
      int inputLineNumber);
      
    virtual ~msrScore ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrIdentification   getIdentification () const
                              { return fIdentification; }

    void                  setPageGeometry (
                            S_msrPageGeometry pageGeometry)
                              { fPageGeometry = pageGeometry; }

    S_msrPageGeometry     getPageGeometry () const
                              { return fPageGeometry; }

    const list<S_msrPartGroup>&
                          getPartGroupsList () const
                              { return fPartGroupsList; }

    const list<S_msrCredit>&
                          getCreditsList () const
                              { return fCreditsList; }

    void                  setScoreNumberOfMeasures (
                            int scoreNumberOfMeasures)
                              {
                                fScoreNumberOfMeasures =
                                  scoreNumberOfMeasures;
                              }
                                  
    int                   getScoreNumberOfMeasures () const
                              { return fScoreNumberOfMeasures; }
        
    // part group names max length

    void                  setScorePartGroupNamesMaxLength (int value)
                              { fScorePartGroupNamesMaxLength = value; }

    int                   getScorePartGroupNamesMaxLength () const
                              { return fScorePartGroupNamesMaxLength; }

    // part names max length
    
    void                  setScorePartNamesMaxLength (int value)
                              { fScorePartNamesMaxLength = value; }

    int                   getScorePartNamesMaxLength () const
                              { return fScorePartNamesMaxLength; }

    // instrument names max lengthes

    void                  setScoreInstrumentNamesMaxLength (int value)
                              { fScoreInstrumentNamesMaxLength = value; }

    int                   getScoreInstrumentNamesMaxLength () const
                              { return fScoreInstrumentNamesMaxLength; }

    void                  setScoreInstrumentAbbreviationsMaxLength (int value)
                              { fScoreInstrumentAbbreviationsMaxLength = value; }

    int                   getScoreInstrumentAbbreviationsMaxLength () const
                              { return fScoreInstrumentAbbreviationsMaxLength; }

    // inhibiting browsing

    void                  setInhibitMeasuresRepeatReplicasBrowsing ()
                              {
                                fInhibitMeasuresRepeatReplicasBrowsing =
                                  true;
                              }
                            
    bool                  getInhibitMeasuresRepeatReplicasBrowsing ()
                            const
                              {
                                return
                                  fInhibitMeasuresRepeatReplicasBrowsing;
                              };

    void                  setInhibitMultipleRestMeasuresBrowsing ()
                              {
                                fInhibitMultipleRestMeasuresBrowsing =
                                  true;
                              }
                            
    bool                  getInhibitMultipleRestMeasuresBrowsing ()
                            const
                              {
                                return
                                  fInhibitMultipleRestMeasuresBrowsing;
                              };

    // services
    // ------------------------------------------------------

    void                  addPartGroupToScore (
                            S_msrPartGroup partGroup);

    void                  appendCreditToScore (
                            S_msrCredit credit);
                              
    S_msrPart             fetchPartFromScoreByItsPartID (
                            int    inputLineNumber,
                            string partID);

    void                  collectScorePartsList (
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

    virtual void          print (ostream& os);
    
    virtual void          printSummary (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrIdentification   fIdentification;

    S_msrPageGeometry     fPageGeometry;
    
    list<S_msrCredit>     fCreditsList;

    set<S_msrPartGroup>   fScorePartGroupsSet;
    
    list<S_msrPartGroup>  fPartGroupsList;

    // number of measures
    
    int                   fScoreNumberOfMeasures;

    // part group names max length

    int                   fScorePartGroupNamesMaxLength;

    // part names max length
    
    int                   fScorePartNamesMaxLength;

    // instrument names max lengthes
    
    int                   fScoreInstrumentNamesMaxLength;
    int                   fScoreInstrumentAbbreviationsMaxLength;

    // inhibiting browsing
    
    // in <measure-repeat/>, the measure replicas are explicit,
    // whereas LilyPond only needs the repeated measure
    bool                  fInhibitMeasuresRepeatReplicasBrowsing;

    // in <multiple-rest/>, the rest measures are explicit,
    // whereas LilyPond only needs the number of rest measures
    bool                  fInhibitMultipleRestMeasuresBrowsing;
};
typedef SMARTP<msrScore> S_msrScore;
EXP ostream& operator<< (ostream& os, const S_msrScore& elt);


}

#endif
