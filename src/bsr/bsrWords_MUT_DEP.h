/*
  This file is to be included only by bsrMutualDependencies.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrWords_MUT_DEP___
#define ___bsrWords_MUT_DEP___

//______________________________________________________________________________
// brailling characters and words
S_bsrCellsList brailleCharacter (
  int  inputLineNumber,
  char ch);

S_bsrCellsList brailleWord (
  int    inputLineNumber,
  string str);

//______________________________________________________________________________
class bsrWords : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrWords> create (
      int    inputLineNumber,
      string wordContents);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrWords (
      int    inputLineNumber,
      string wordContents);

    virtual ~bsrWords ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getWordContents () const
                              { return fWordContents; }

    // services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const override
                              { return fWordCellsList; }

    int                   fetchCellsNumber () const override;

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

    // fields
    // ------------------------------------------------------

    string                fWordContents;

    S_bsrCellsList        fWordCellsList;
};
typedef SMARTP<bsrWords> S_bsrWords;
EXP ostream& operator<< (ostream& os, const S_bsrWords& elt);


#endif