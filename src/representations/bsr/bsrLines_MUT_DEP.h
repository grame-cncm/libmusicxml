/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

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
