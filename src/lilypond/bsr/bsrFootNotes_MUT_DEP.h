/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrFootNotes_MUT_DEP___
#define ___bsrFootNotes_MUT_DEP___

//______________________________________________________________________________
class bsrFootNotesElement : public bsrElement
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

    // services
    // ------------------------------------------------------

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

    string                fFootNoteText;

    S_bsrCellsList        fFootNotesElementCellsList;
};
typedef SMARTP<bsrFootNotesElement> S_bsrFootNotesElement;
EXP ostream& operator<< (ostream& os, const S_bsrFootNotesElement& elt);

//______________________________________________________________________________
class bsrFootNotes : public bsrLine
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

    // services
    // ------------------------------------------------------

    void                  appendElementToFootNotes (
                            S_bsrFootNotesElement footNotesElement)
                              {
                                fFootNotesElementsList.push_back (
                                  footNotesElement);
                              }

    int                   fetchLineContentsNumber () const
                              { return fLineContentsList.size (); }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    list<S_bsrFootNotesElement>
                          fFootNotesElementsList;
};
typedef SMARTP<bsrFootNotes> S_bsrFootNotes;
EXP ostream& operator<< (ostream& os, const S_bsrFootNotes& elt);


#endif
