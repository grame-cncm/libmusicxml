/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

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

  public:

    // public services
    // ------------------------------------------------------

    void                  appendClefToMeasure (S_bsrClef clef);

    void                  appendBarlineToMeasure (S_bsrBarline barline);

    void                  appendNumberToMeasure (S_bsrNumber number);

    void                  appendWordsToMeasure (S_bsrWords str);

    void                  appendNoteToMeasure (S_bsrNote note);

    void                  appendPaddingNoteToMeasure (S_bsrNote note);

    void                  appendDynamicsToMeasure (S_bsrDynamics dynamics);

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    void                  appendLineElementToMeasure (
                            S_bsrLineElement lineElement);

    S_bsrCellsList        buildCellsList () const;

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

    virtual string        asDebugString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    string                fPrintMeasureNumber;
    string                fBrailleMeasureNumber;

    list<S_bsrLineElement>
                          fMeasureLineElementsList;
};
typedef SMARTP<bsrMeasure> S_bsrMeasure;
EXP ostream& operator<< (ostream& os, const S_bsrMeasure& elt);

