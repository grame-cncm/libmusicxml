/*
  This file is to be included only by bsr_MUTUAL_DEPENDENCIES.h,
  to satisfy declarations mutual dependencies.
*/

#ifndef ___bsrPages_MUT_DEP___
#define ___bsrPages_MUT_DEP___

namespace MusicXML2
{

//______________________________________________________________________________
class EXP bsrPage : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPage> create (
      int inputLineNumber,
      int printPageNumber,
      int linesPerPage);

    SMARTP<bsrPage> createPageNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPage (
      int inputLineNumber,
      int printPageNumber,
      int linesPerPage);

    virtual ~bsrPage ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintPageNumber () const
                              { return fPrintPageNumber; }

    int                   getBraillePageNumber () const
                              { return fBraillePageNumber; }

    int                   getLinesPerPage () const
                              { return fLinesPerPage; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPageHeadingToPage (S_bsrPageHeading pageHeading)
                              { fPageElementsList.push_back (pageHeading); }

    void                  appendMusicHeadingToPage (S_bsrMusicHeading musicHeading)
                              { fPageElementsList.push_back (musicHeading); }

    void                  appendFootNotesToPage (S_bsrFootNotes footNotes)
                              { fPageElementsList.push_back (footNotes); }

    void                  appendLineToPage (S_bsrLine line)
                              { fPageElementsList.push_back (line); }

    int                   fetchLineContentsNumber () const;

  public:


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

    virtual void          print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    int                   fPrintPageNumber;
    int                   fBraillePageNumber;

    int                   fLinesPerPage;

    list<S_bsrPageElement>
                          fPageElementsList;
};
typedef SMARTP<bsrPage> S_bsrPage;
EXP ostream& operator<< (ostream& os, const S_bsrPage& elt);


}


#endif
