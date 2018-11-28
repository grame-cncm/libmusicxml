/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrMutuallyDependent___
#define ___bsrMutuallyDependent___

#include "bsrSpaces.h"

#include "bsrPageHeadings.h"

#include "bsrNumbers.h"

#include "bsrClefs.h"
#include "bsrKeys.h"
#include "bsrTimes.h"

#include "bsrNotes.h"

#include "bsrOptions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrLineElement;
typedef SMARTP<bsrLineElement> S_bsrLineElement;

class bsrLine;
typedef SMARTP<bsrLine> S_bsrLine;

class bsrPageElement;
typedef SMARTP<bsrPageElement> S_bsrPageElement;

class bsrPage;
typedef SMARTP<bsrPage> S_bsrPage;

//______________________________________________________________________________
class bsrMeasure : public bsrElement
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

    S_bsrCellsList      getTimeCellsList () const
                              { return fMeasureCellsList; }
                              
  public:

    // public services
    // ------------------------------------------------------

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

    S_bsrCellsList      asCellsList ();
    
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

    S_bsrCellsList      fMeasureCellsList;
};
typedef SMARTP<bsrMeasure> S_bsrMeasure;
EXP ostream& operator<< (ostream& os, const S_bsrMeasure& elt);

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
/*
    void                  setBarLineUplink (
                            S_bsrLine barlineUplink);
                              
    S_bsrLine             getBarLineUplink () const;
*/
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

    S_bsrLine             fBarLineUplink;

    bool                  fASpaceIsNeededBefore;
    bool                  fASpaceIsNeededAfter;
};
typedef SMARTP<bsrLineElement> S_bsrLineElement;
EXP std::ostream& operator<< (std::ostream& os, const S_bsrLineElement& elt);

//______________________________________________________________________________
class bsrParallel : public bsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrParallelLayoutKind {
        kParallelLayoutBarOverBarKind,
        kParallelLayoutLineOverLineKind };

    static string parallelLayoutKindAsString (
      bsrParallelLayoutKind parallelLayoutKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrParallel> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrParallel (
      int inputLineNumber);
      
    virtual ~bsrParallel ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintParallelNumber () const
                              { return fPrintParallelNumber; }

    int                   getBrailleParallelNumber () const
                              { return fBrailleParallelNumber; }
                              
  public:

    // public services
    // ------------------------------------------------------

    void                  appendLineToParallel (S_bsrLine line)
                              { fParallelElementsList.push_back (line); }
                              
  public:


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

    int                   fPrintParallelNumber;    
    int                   fBrailleParallelNumber;

    list<S_bsrElement>
                          fParallelElementsList;
};
typedef SMARTP<bsrParallel> S_bsrParallel;
EXP ostream& operator<< (ostream& os, const S_bsrParallel& elt);

//______________________________________________________________________________
class bsrPage : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPage> create (
      int inputLineNumber,
      int printPageNumber,
      int linesPerPage);
    
    SMARTP<bsrPage> createPageNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPage (
      int inputLineNumber,
      int printPageNumber,
      int linesPerPage);
      
    virtual ~bsrPage ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintPageNumber () const
                              { return fPrintPageNumber; }

    int                   getBraillePageNumber () const
                              { return fBraillePageNumber; }
                              
    int                   getLinesPerPage () const
                              { return fLinesPerPage; }
                              
  public:

    // public services
    // ------------------------------------------------------

    void                  appendPageHeadingToPage (S_bsrPageHeading pageHeading)
                              { fPageElementsList.push_back (pageHeading); }
                              
    void                  appendLineToPage (S_bsrLine line)
                              { fPageElementsList.push_back (line); }
                              
  public:


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

    int                   fPrintPageNumber;    
    int                   fBraillePageNumber;

    int                   fLinesPerPage;    

    list<S_bsrElement>
                          fPageElementsList;
};
typedef SMARTP<bsrPage> S_bsrPage;
EXP ostream& operator<< (ostream& os, const S_bsrPage& elt);

//______________________________________________________________________________
class bsrPageElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPageElement> create (
      int inputLineNumber);

  protected:
         
    // constructors/destructor
    // ------------------------------------------------------

    bsrPageElement (
      int inputLineNumber);

    virtual ~bsrPageElement ();

  public:

    // set and get
    // ------------------------------------------------------

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

};
typedef SMARTP<bsrPageElement> S_bsrPageElement;
EXP std::ostream& operator<< (std::ostream& os, const S_bsrPageElement& elt);


}


#endif
