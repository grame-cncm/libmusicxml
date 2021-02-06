#ifndef ___bsrLines___
#define ___bsrLines___

#include "bsrPageElements.h"
#include "bsrCellsLists.h"
#include "bsrLineContents.h"

#include "bsrKeys.h"
#include "bsrTimes.h"
#include "bsrTempos.h"
#include "bsrMeasures.h"
#include "bsrNotes.h"
#include "bsrSpaces.h"

#include "bsrLineContentsElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrLineContents;
typedef SMARTP<bsrLineContents> S_bsrLineContents;

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

    virtual               ~bsrLine ();

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

    void                  appendKeyToLine (S_bsrKey key);

    void                  appendMeasureToLine (S_bsrMeasure measure);

    void                  appendNoteToLine (S_bsrNote note);

    void                  appendSpacesToLine (S_bsrSpaces spaces);

    void                  appendTimeToLine (S_bsrTime time);

    void                  insertTimeBeforeLastElementOfLine (S_bsrTime time);

    void                  appendTempoToLine (S_bsrTempo tempo);

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
                            S_bsrLineContentsElement lineElement);

    void                  insertElementBeforeLastElementOfLine (
                            S_bsrLineContentsElement lineElement);

    void                  appendLineElementToLastMeasureOfLine (
                            S_bsrLineContentsElement lineElement);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    virtual string        asDebugString () const;

    void                  print (ostream& os) const override;

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
