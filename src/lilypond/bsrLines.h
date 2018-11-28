/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrLines___
#define ___bsrLines___

#include "bsrLineElements.h"

#include "bsrSpaces.h"

#include "bsrPageHeadings.h"

#include "bsrMeasures.h"

/* JMI
#include "bsrNumbers.h"

#include "bsrClefs.h"
#include "bsrKeys.h"
#include "bsrTimes.h"

#include "bsrNotes.h"
*/

#include "bsrOptions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrLine : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrLine> create (
      int inputLineNumber,
      int printLineNumber,
      int cellsPerLine);
    
    SMARTP<bsrLine> createLineNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrLine (
      int inputLineNumber,
      int printLineNumber,
      int cellsPerLine);
      
    virtual ~bsrLine ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintLineNumber () const
                              { return fPrintLineNumber; }

    int                   getBrailleLineNumber () const
                              { return fBrailleLineNumber; }
                              
    int                   getCellsPerLine () const
                              { return fCellsPerLine; }
                              
  public:

    // public services
    // ------------------------------------------------------

    void                  appendSpacesToLine (S_bsrSpaces spaces)
                              { fLineElementsList.push_back (spaces); }
                              
    void                  appendMeasureToLine (S_bsrMeasure measure)
                              { fLineElementsList.push_back (measure); }

/*                              
    void                  appendNumberToLine (S_bsrNumber number)
                              { fLineElementsList.push_back (number); }
                              
    void                  appendClefToLine (S_bsrClef clef)
                              { fLineElementsList.push_back (clef); }
                              
    void                  appendKeyToLine (S_bsrKey key)
                              { fLineElementsList.push_back (key); }
                              
    void                  appendTimeToLine (S_bsrTime time)
                              { fLineElementsList.push_back (time); }
                              
    void                  appendNoteToLine (S_bsrNote note)
                              { fLineElementsList.push_back (note); }
*/

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual std::string   asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fPrintLineNumber;    
    int                   fBrailleLineNumber;

    int                   fCellsPerLine;
    
    list<S_bsrElement>
                          fLineElementsList;
};
typedef SMARTP<bsrLine> S_bsrLine;
EXP ostream& operator<< (ostream& os, const S_bsrLine& elt);

/*
//______________________________________________________________________________
class bsrLineElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrLineElement> create (
      int inputLineNumber);

  protected:
         
    // constructors/destructor
    // ------------------------------------------------------

    bsrLineElement (
      int inputLineNumber);

    virtual ~bsrLineElement ();

  public:

    // set and get
    // ------------------------------------------------------
/ *
    void                  setBsrLineUplink (
                            S_bsrLine bsrLineUplink);
                              
    S_bsrLine             getBsrLineUplink () const;
* /
    bool                  getASpaceIsNeededBefore () const
                              { return fASpaceIsNeededBefore; }

    bool                  getASpaceIsNeededAfter () const
                              { return fASpaceIsNeededAfter; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const;
    
    virtual std::string   asShortString () const;

    virtual void          print (std::ostream& os);

  protected:
  
    // fields
    // ------------------------------------------------------

/ *
    S_bsrLine             fBsrLineUplink;
* /

    bool                  fASpaceIsNeededBefore;
    bool                  fASpaceIsNeededAfter;
};
typedef SMARTP<bsrLineElement> S_bsrLineElement;
EXP std::ostream& operator<< (std::ostream& os, const S_bsrLineElement& elt);
*/


}


#endif
