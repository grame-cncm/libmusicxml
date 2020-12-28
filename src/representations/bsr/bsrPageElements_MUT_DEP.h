/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrPageElements_MUT_DEP___
#define ___bsrPageElements_MUT_DEP___


namespace MusicXML2
{

//______________________________________________________________________________
class bsrPage;
typedef SMARTP<bsrPage> S_bsrPage;

//______________________________________________________________________________
class EXP bsrPageElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

/*
    static SMARTP<bsrPageElement> create (
      int inputLineNumber);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPageElement (
      int inputLineNumber);

    virtual ~bsrPageElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBsrPageUpLink (
                            S_bsrPage bsrPageUpLink);

    S_bsrPage             getBsrPageUpLink () const;

    void                  setSpacesBefore (int value)
                              { fSpacesBefore = value; }

    int                   getSpacesBefore () const
                              { return fSpacesBefore; }

/*
    int                   getSpacesAfter () const
                              { return fSpacesAfter; }
*/

  public:

    // public services
    // ------------------------------------------------------

    virtual int           fetchLineContentsNumber () const  // JMI ???
                              { return 0; }

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

    virtual void          print (ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    S_bsrPage             fBsrPageUpLink;

    int                   fSpacesBefore;
 // JMI   int                   fSpacesAfter;
};
typedef SMARTP<bsrPageElement> S_bsrPageElement;
EXP ostream& operator<< (ostream& os, const S_bsrPageElement& elt);


}


#endif
