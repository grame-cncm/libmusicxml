/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrFootNotes_MUT_DEP___
#define ___bsrFootNotes_MUT_DEP___

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


#endif
