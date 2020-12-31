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

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

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
