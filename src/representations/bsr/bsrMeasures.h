/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrMeasures___
#define ___bsrMeasures___

#include "bsrBarlines.h"
#include "bsrClefs.h"
#include "bsrCellsLists.h"
#include "bsrDynamics.h"
#include "bsrLineContentsElements.h"
#include "bsrNotes.h"
#include "bsrNumbers.h"
#include "bsrWords.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrMeasure : public bsrLineContentsElement
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

  public:

    // public services
    // ------------------------------------------------------

    void                  appendClefToMeasure (S_bsrClef clef);

    void                  appendBarlineToMeasure (S_bsrBarline barline);

    void                  appendNumberToMeasure (S_bsrNumber number);

    void                  appendWordsToMeasure (S_bsrWords str);

    void                  appendNoteToMeasure (S_bsrNote note);

    void                  appendPaddingSkipNoteToMeasure (S_bsrNote note);

    void                  appendDynamicsToMeasure (S_bsrDynamics dynamics);

    S_bsrCellsList        fetchCellsList () const override
                              { return buildCellsList (); }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

    void                  appendLineElementToMeasure (
                            S_bsrLineContentsElement lineElement);

    S_bsrCellsList        buildCellsList () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const override;

    virtual string        asDebugString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    string                fPrintMeasureNumber;
    string                fBrailleMeasureNumber;

    list<S_bsrLineContentsElement>
                          fMeasureLineElementsList;
};
typedef SMARTP<bsrMeasure> S_bsrMeasure;
EXP ostream& operator<< (ostream& os, const S_bsrMeasure& elt);


}


#endif
