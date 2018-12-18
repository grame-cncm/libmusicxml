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
#include "bsrPageElements.h"

#include "bsrSpaces.h"

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
class bsrLineContents : public bsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrLineContentsKind {
        kLineContentsRegular,
        kLineContentsContinuation };
        
    static string lineContentsKindAsString (
      bsrLineContentsKind lineContentsKind);
      
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrLineContents> create (
      int                 inputLineNumber,
      bsrLineContentsKind lineContentsKind);

    SMARTP<bsrLineContents> createLineNewbornClone ();

  protected:
         
    // constructors/destructor
    // ------------------------------------------------------

    bsrLineContents (
      int                 inputLineNumber,
      bsrLineContentsKind lineContentsKind);

    virtual ~bsrLineContents ();

  public:

    // set and get
    // ------------------------------------------------------
/*
    void                  setBsrLineUplink (
                            S_bsrLine bsrLineUplink);
                              
    S_bsrLine             getBsrLineUplink () const;
*/

    bsrLineContentsKind   getLineContentsKind () const
                              { return fLineContentsKind; }

    const list<S_bsrLineElement>&
                          getLineElementsList () const
                              { return fLineElementsList; }

  public:

    // services
    // ------------------------------------------------------

    void                  appendLineElementToLineContents (
                            S_bsrLineElement lineElement);
  
    int                   fetchCellsNumber () const;

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

/*
    S_bsrLine             fBsrLineUplink;
*/

    bsrLineContentsKind   fLineContentsKind;

    list<S_bsrLineElement>
                          fLineElementsList;
};
typedef SMARTP<bsrLineContents> S_bsrLineContents;
EXP std::ostream& operator<< (std::ostream& os, const S_bsrLineContents& elt);

//______________________________________________________________________________
class bsrLine : public bsrPageElement
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

    void                  appendSpacesToLine (S_bsrSpaces spaces);
                              
    void                  appendMeasureToLine (S_bsrMeasure measure);

    void                  appendKeyToLine (S_bsrKey key);
                              
    void                  appendTimeToLine (S_bsrTime time);
                                  
    void                  appendTempoToLine (S_bsrTempo tempo);
                                                            
    S_bsrCellsList        lineNumbersAsCellsList () const;

    int                   fetchLineContentsNumber () const
                              { return fLineContentsList.size (); }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    void                  appendLineElementToLine (
                            S_bsrLineElement lineElement);

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

  protected:

    // fields
    // ------------------------------------------------------

    int                   fPrintLineNumber;    
    int                   fBrailleLineNumber;

    int                   fCellsPerLine;
    
    list<S_bsrLineContents>
                          fLineContentsList;
    bool                  fASpaceIsNeeded;
};
typedef SMARTP<bsrLine> S_bsrLine;
EXP ostream& operator<< (ostream& os, const S_bsrLine& elt);


}


#endif
