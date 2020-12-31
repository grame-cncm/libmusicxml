#ifndef ___bsrPageElements___
#define ___bsrPageElements___

#include "bsrElements.h"


namespace MusicXML2
{

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

/* JMI
    void                  setBsrPageUpLink (
                            S_bsrPage bsrPageUpLink);

    S_bsrPage             getBsrPageUpLink () const;
*/

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

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    string                asShortString () const override;

    void                  print (ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

/*
    S_bsrPage             fBsrPageUpLink;
*/

    int                   fSpacesBefore;
 // JMI   int                   fSpacesAfter;
};
typedef SMARTP<bsrPageElement> S_bsrPageElement;
EXP ostream& operator<< (ostream& os, const S_bsrPageElement& elt);


}


#endif
