/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrLines_MUT_DEP___
#define ___bsrLines_MUT_DEP___

#include "bsrCellsLists.h"
#include "bsrKeys_MUT_DEP.h"
#include "bsrTimes_MUT_DEP.h"
#include "bsrTempos_MUT_DEP.h"
#include "bsrLineContents.h"
#include "bsrLineElements_MUT_DEP.h"
#include "bsrMeasures_MUT_DEP.h"
#include "bsrNotes_MUT_DEP.h"
#include "bsrPageElements_MUT_DEP.h"
#include "bsrSpaces_MUT_DEP.h"

namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrLine : public bsrPageElement
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

    void                  insertTimeBeforeLastElementOfLine (S_bsrTime time);

    void                  appendTempoToLine (S_bsrTempo tempo);

    void                  appendNoteToLine (S_bsrNote note);

    int                   fetchLineContentsNumber () const override
                              { return fLineContentsList.size (); }

    S_bsrCellsList        fetchCellsList () const
                              { return buildLineNumberCellsList (); } // JMI ???

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildLineNumberCellsList () const;

    void                  appendLineElementToLine (
                            S_bsrLineElement lineElement);

    void                  insertElementBeforeLastElementOfLine (
                            S_bsrLineElement lineElement);

    void                  appendLineElementToLastMeasureOfLine (
                            S_bsrLineElement lineElement);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const override;

    virtual std::string   asDebugString () const;

    virtual void          print (ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    int                   fPrintLineNumber;
    int                   fBrailleLineNumber;

    int                   fCellsPerLine;

    S_bsrCellsList        fLineNumberCellsList;

    list<S_bsrLineContents>
                          fLineContentsList;

    bool                  fASpaceIsNeededInLine;
};
typedef SMARTP<bsrLine> S_bsrLine;
EXP ostream& operator<< (ostream& os, const S_bsrLine& elt);


}


#endif
