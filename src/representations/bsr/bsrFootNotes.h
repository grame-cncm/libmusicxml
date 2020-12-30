/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrFootNotes___
#define ___bsrFootNotes___

#include "bsrLines.h"
#include "bsrFootNotesElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrFootNotes : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrFootNotes> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrFootNotes (
      int inputLineNumber);

    virtual ~bsrFootNotes ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  appendElementToFootNotes (
                            S_bsrFootNotesElement footNotesElement)
                              {
                                fFootNotesElementsList.push_back (
                                  footNotesElement);
                              }

    int                   fetchLineContentsNumber () const override
                              { return fLineContentsList.size (); }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    list<S_bsrFootNotesElement>
                          fFootNotesElementsList;
};
typedef SMARTP<bsrFootNotes> S_bsrFootNotes;
EXP ostream& operator<< (ostream& os, const S_bsrFootNotes& elt);


}


#endif
