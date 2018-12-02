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
      int    inputLineNumber,
      string printMeasureNumber);
    
    SMARTP<bsrMeasure> createMeasureNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrMeasure (
      int    inputLineNumber,
      string printMeasureNumber);
      
    virtual ~bsrMeasure ();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getPrintMeasureNumber () const
                              { return fPrintMeasureNumber; }

    string                getBrailleMeasureNumber () const
                              { return fBrailleMeasureNumber; }

    S_bsrCellsList        getTimeCellsList () const
                              { return fMeasureCellsList; }
                              
  public:

    // public services
    // ------------------------------------------------------

    void                  appendBarlineToMeasure (S_bsrBarline barline);
                              
    void                  appendNumberToMeasure (S_bsrNumber number);
                              
    void                  appendClefToMeasure (S_bsrClef clef);
                              
    void                  appendKeyToMeasure (S_bsrKey key);
                              
    void                  appendTimeToMeasure (S_bsrTime time);
                                                            
    void                  appendNoteToMeasure (S_bsrNote note);

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

    virtual string        asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    string                fPrintMeasureNumber;    
    string                fBrailleMeasureNumber;

    list<S_bsrElement>
                          fMeasureElementsList;

    S_bsrCellsList        fMeasureCellsList;
};
typedef SMARTP<bsrMeasure> S_bsrMeasure;
EXP ostream& operator<< (ostream& os, const S_bsrMeasure& elt);


}


#endif
