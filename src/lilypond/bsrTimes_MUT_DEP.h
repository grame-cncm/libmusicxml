/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

//______________________________________________________________________________
class bsrTimeItem;
typedef SMARTP<bsrTimeItem> S_bsrTimeItem;

class bsrTime;
typedef SMARTP<bsrTime> S_bsrTime;

//______________________________________________________________________________
class bsrTimeItem : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTimeItem> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTimeItem (
      int inputLineNumber);

    virtual ~bsrTimeItem ();

  public:

    // set and get
    // ------------------------------------------------------

    const vector<int>&    getTimeBeatsNumbersVector ()
                              { return fTimeBeatsNumbersVector; }

    void                  setTimeBeatValue (int timeBeatValue);

    int                   getTimeBeatValue () const
                              { return fTimeBeatValue; }

    // services
    // ------------------------------------------------------

    bool                  isEqualTo (S_bsrTimeItem otherTimeItem) const;

    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeBeatsNumber () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    vector<int>           fTimeBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeBeatValue;
};
typedef SMARTP<bsrTimeItem> S_bsrTimeItem;
EXP ostream& operator<< (ostream& os, const S_bsrTimeItem& elt);

//______________________________________________________________________________
class bsrTime : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrTimeKind {
        kTimeNone,
        kTimeCommon,
        kTimeCut,
        kTimeNumerical, // JMI
        kTimeNote,
        kTimeDottedNote,
        kTimeSingleNumber,
        kTimeSenzaMisura };

    static string timeKindAsString (
      bsrTimeKind timeKind);

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTime> create (
      int         inputLineNumber,
      bsrTimeKind timeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTime (
      int         inputLineNumber,
      bsrTimeKind timeKind);

    virtual ~bsrTime ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrTimeKind           getTimeKind () const
                              { return fTimeKind; }

    const vector<S_bsrTimeItem>&
                          getTimeItemsVector ()
                              { return fTimeItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendTimeItem (S_bsrTimeItem timeItem);

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

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

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    bsrTimeKind           fTimeKind;

    vector<S_bsrTimeItem> fTimeItemsVector;
};
typedef SMARTP<bsrTime> S_bsrTime;
EXP ostream& operator<< (ostream& os, const S_bsrTime& elt);

