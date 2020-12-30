/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrFootNotesElements___
#define ___bsrFootNotesElements___

#include "bsrElements.h"
#include "bsrCellsLists.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrFootNotesElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrFootNotesElement> create (
      int    inputLineNumber,
      string footNoteText);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrFootNotesElement (
      int    inputLineNumber,
      string footNoteText);

    virtual ~bsrFootNotesElement ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getFootNoteText () const
                              { return fFootNoteText; }

    S_bsrCellsList        getFootNotesElementCellsList () const
                              { return fFootNotesElementCellsList ;}

  public:

    // public services
    // ------------------------------------------------------

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

    string                fFootNoteText;

    S_bsrCellsList        fFootNotesElementCellsList;
};
typedef SMARTP<bsrFootNotesElement> S_bsrFootNotesElement;
EXP ostream& operator<< (ostream& os, const S_bsrFootNotesElement& elt);


}


#endif
