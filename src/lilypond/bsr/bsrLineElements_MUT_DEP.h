/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrLineElements_MUT_DEP___
#define ___bsrLineElements_MUT_DEP___

//______________________________________________________________________________
class bsrLineElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

/*
    static SMARTP<bsrLineElement> create (
      int inputLineNumber);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrLineElement (
      int inputLineNumber);

    virtual ~bsrLineElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBsrLineUpLink (
                            S_bsrLine bsrLineUpLink);

    S_bsrLine             getBsrLineUpLink () const;


    void                  setSpacesBefore (int value)
                              { fSpacesBefore = value; }

    int                   getSpacesBefore () const
                              { return fSpacesBefore; }

  public:

    // services
    // ------------------------------------------------------


    virtual S_bsrCellsList
                          fetchCellsList () const = 0;

    virtual int           fetchCellsNumber () const = 0;

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

    virtual std::string   asDebugString () const = 0;

    virtual void          print (ostream& os) const;

  protected:

    // fields
    // ------------------------------------------------------

    S_bsrLine             fBsrLineUpLink;

    int                   fSpacesBefore;
};
typedef SMARTP<bsrLineElement> S_bsrLineElement;
EXP ostream& operator<< (ostream& os, const S_bsrLineElement& elt);


#endif

