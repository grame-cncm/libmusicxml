/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrLinesContents_MUT_DEP___
#define ___bsrLinesContents_MUT_DEP___

#include "bsrCellsLists.h"
#include "bsrElements.h"
#include "bsrLineElements_MUT_DEP.h"

namespace MusicXML2
{

//______________________________________________________________________________
class bsrLine;
typedef SMARTP<bsrLine> S_bsrLine;

//______________________________________________________________________________
class EXP bsrLineContents : public bsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrLineContentsKind {
      kLineContentsRegular,
      kLineContentsContinuation
    };

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

    void                  setBsrLineUpLink (
                            S_bsrLine bsrLineUpLink);

    S_bsrLine             getBsrLineUpLink () const;

    bsrLineContentsKind   getLineContentsKind () const
                              { return fLineContentsKind; }

    const list<S_bsrLineElement>&
                          getLineContentsElementsList () const
                              { return fLineContentsElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendLineElementToLineContents (
                            S_bsrLineElement lineElement);

    void                  insertLineElementBeforeLastElementOfLineContents (
                            S_bsrLineElement lineElement);

    S_bsrCellsList        fetchCellsList () const
                              { return buildLineContentsElementsList (); } // JMI ???

    int                   fetchCellsNumber () const;

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

    virtual std::string   asShortString () const override;

    virtual std::string   asDebugString () const;

    virtual void          print (ostream& os) const override;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildLineContentsElementsList () const;

  protected:

    // protected fields
    // ------------------------------------------------------

    S_bsrLine             fBsrLineUpLink;

    bsrLineContentsKind   fLineContentsKind;

    list<S_bsrLineElement>
                          fLineContentsElementsList;
};
typedef SMARTP<bsrLineContents> S_bsrLineContents;
EXP ostream& operator<< (ostream& os, const S_bsrLineContents& elt);


}


#endif
