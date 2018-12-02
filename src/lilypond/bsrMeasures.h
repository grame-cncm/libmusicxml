/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrMeasures___
#define ___bsrMeasures___

#include "bsrLineElements.h"

#include "bsrSpaces.h"

#include "bsrBarlines.h"

#include "bsrNumbers.h"

#include "bsrClefs.h"
#include "bsrKeys.h"
#include "bsrTimes.h"

#include "bsrNotes.h"

//#include "bsrOptions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrMeasure : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrMeasure> create (
      int inputLineNumber);
    
    SMARTP<bsrMeasure> createMeasureNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrMeasure (
      int inputLineNumber);
      
    virtual ~bsrMeasure ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintMeasureNumber () const
                              { return fPrintMeasureNumber; }

    int                   getBrailleMeasureNumber () const
                              { return fBrailleMeasureNumber; }

    S_bsrCellsList        getTimeCellsList () const
                              { return fMeasureCellsList; }
                              
  public:

    // public services
    // ------------------------------------------------------

    void                  appendBarlineToMeasure (S_bsrBarline barline)
                              { fMeasureElementsList.push_back (barline); }
                              
    void                  appendNumberToMeasure (S_bsrNumber number)
                              { fMeasureElementsList.push_back (number); }
                              
    void                  appendClefToMeasure (S_bsrClef clef)
                              { fMeasureElementsList.push_back (clef); }
                              
    void                  appendKeyToMeasure (S_bsrKey key)
                              { fMeasureElementsList.push_back (key); }
                              
    void                  appendTimeToMeasure (S_bsrTime time)
                              { fMeasureElementsList.push_back (time); }
                                                            
    void                  appendNoteToMeasure (S_bsrNote note)
                              { fMeasureElementsList.push_back (note); }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        asCellsList () const;
    
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

  private:

    // fields
    // ------------------------------------------------------

    int                   fPrintMeasureNumber;    
    int                   fBrailleMeasureNumber;

    list<S_bsrElement>
                          fMeasureElementsList;

    S_bsrCellsList        fMeasureCellsList;
};
typedef SMARTP<bsrMeasure> S_bsrMeasure;
EXP ostream& operator<< (ostream& os, const S_bsrMeasure& elt);


}


#endif
