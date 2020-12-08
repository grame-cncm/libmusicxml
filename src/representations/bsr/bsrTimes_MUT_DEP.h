/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrTimes_MUT_DEP___
#define ___bsrTimes_MUT_DEP___

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

  public:

    // public services
    // ------------------------------------------------------

    bool                  isEqualTo (S_bsrTimeItem otherTimeItem) const;

    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeBeatsNumber () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
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

    static string timeKindAsDebugString (
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
                          getTimeItemsVector () const
                              { return fTimeItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendTimeItem (S_bsrTimeItem timeItem);

    S_bsrCellsList        fetchCellsList () const override
                              { return buildCellsList (); }

    int                   fetchCellsNumber () const override;

  private:

    // private services
    // ------------------------------------------------------

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

    string                asString () const override;

    virtual string        asDebugString () const override;

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    bsrTimeKind           fTimeKind;

    vector<S_bsrTimeItem> fTimeItemsVector;
};
typedef SMARTP<bsrTime> S_bsrTime;
EXP ostream& operator<< (ostream& os, const S_bsrTime& elt);


#endif
