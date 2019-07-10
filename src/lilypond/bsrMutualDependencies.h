/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrMutualDependencies___
#define ___bsrMutualDependencies___

#include "bsrElements.h"

#include "bsrCellsLists.h"

/* JMI
#include "bsrSpaces.h"

#include "bsrPages.h"

#include "bsrClefs.h"
#include "bsrKeys.h"
#include "bsrTimes.h"
*/

#include "msrTempos.h"
#include "msrDynamics.h"


namespace MusicXML2
{

/*
  This file declares classes that cannot be placed in 'individual' headers
  due to mutual dependancies
*/

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class bsrLine;
typedef SMARTP<bsrLine> S_bsrLine;

class bsrSpaces;
typedef SMARTP<bsrSpaces> S_bsrSpaces;

class bsrPage;
typedef SMARTP<bsrPage> S_bsrPage;

class bsrMeasure;
typedef SMARTP<bsrMeasure> S_bsrMeasure;

class bsrKey;
typedef SMARTP<bsrKey> S_bsrKey;

class bsrTime;
typedef SMARTP<bsrTime> S_bsrTime;

class bsrTempo;
typedef SMARTP<bsrTempo> S_bsrTempo;

class bsrNote;
typedef SMARTP<bsrNote> S_bsrNote;

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

    virtual void          print (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    S_bsrLine             fBsrLineUpLink;

    int                   fSpacesBefore;
};
typedef SMARTP<bsrLineElement> S_bsrLineElement;
EXP ostream& operator<< (ostream& os, const S_bsrLineElement& elt);

//______________________________________________________________________________
class bsrPageElement : public bsrElement
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

    // services
    // ------------------------------------------------------

    virtual int           fetchLineContentsNumber () const  // JMI ???
                              { return 0; }

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

    virtual void          print (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    S_bsrPage             fBsrPageUpLink;

    int                   fSpacesBefore;
 // JMI   int                   fSpacesAfter;
};
typedef SMARTP<bsrPageElement> S_bsrPageElement;
EXP ostream& operator<< (ostream& os, const S_bsrPageElement& elt);

//______________________________________________________________________________
class bsrLineContents : public bsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrLineContentsKind {
        kLineContentsRegular,
        kLineContentsContinuation };

    static string lineContentsKindAsString (
      bsrLineContentsKind lineContentsKind);

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrLineContents> create (
      int                 inputLineNumber,
      bsrLineContentsKind lineContentsKind);

    SMARTP<bsrLineContents> createLineNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrLineContents (
      int                 inputLineNumber,
      bsrLineContentsKind lineContentsKind);

    virtual ~bsrLineContents ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBsrLineUpLink (
                            S_bsrLine bsrLineUpLink);

    S_bsrLine             getBsrLineUpLink () const;

    bsrLineContentsKind   getLineContentsKind () const
                              { return fLineContentsKind; }

    const list<S_bsrLineElement>&
                          getLineContentsElementsList () const
                              { return fLineContentsElementsList; }

  public:

    // services
    // ------------------------------------------------------

    void                  appendLineElementToLineContents (
                            S_bsrLineElement lineElement);

    void                  insertLineElementBeforeLastElementOfLineContents (
                            S_bsrLineElement lineElement);

    int                   fetchCellsNumber () const;

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

    virtual void          print (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    S_bsrLine             fBsrLineUpLink;

    bsrLineContentsKind   fLineContentsKind;

    list<S_bsrLineElement>
                          fLineContentsElementsList;
};
typedef SMARTP<bsrLineContents> S_bsrLineContents;
EXP ostream& operator<< (ostream& os, const S_bsrLineContents& elt);

//______________________________________________________________________________
const bsrCellKind
  kCellSpace = kDotsNone;

//______________________________________________________________________________
class bsrSpaces : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrSpaces> create (
      int inputLineNumber,
      int numberOfSpaces);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrSpaces (
      int inputLineNumber,
      int numberOfSpaces);

    virtual ~bsrSpaces ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNumberOfSpaces () const
                              { return fNumberOfSpaces; }

    // services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fSpacesCellsList; }

    int                   fetchCellsNumber () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fNumberOfSpaces;

    S_bsrCellsList        fSpacesCellsList;
};
typedef SMARTP<bsrSpaces> S_bsrSpaces;
EXP ostream& operator<< (ostream& os, const S_bsrSpaces& elt);

//______________________________________________________________________________
class bsrPagination : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPagination> create (
      int inputLineNumber,
      int printPageNumber,
      int braillePageNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPagination (
      int inputLineNumber,
      int printPageNumber,
      int braillePageNumber);

    virtual ~bsrPagination ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintPageNumber () const
                              { return fPrintPageNumber; }

    int                   getBraillePageNumber () const
                              { return fBraillePageNumber; }

    S_bsrCellsList        fetchCellsList () const
                              { return fPaginationCellsList; }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fPrintPageNumber;
    int                   fBraillePageNumber;

    S_bsrCellsList        fPaginationCellsList;
};
typedef SMARTP<bsrPagination> S_bsrPagination;
EXP ostream& operator<< (ostream& os, const S_bsrPagination& elt);

//______________________________________________________________________________
class bsrClef : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrClefKind {
        kClefKindNone,

        kClefKindGTreble,
        kClefKindFBass,
        kClefKindCAlto,
        kClefKindGSoprano, // first line, french violin
        kClefKindFBaritone,
        kClefKindCTenor,
        kClefKindGOttavaAlta,
        kClefKindGOttavaBassa,
        kClefKindModifiedBassForRightHandPart,
        kClefKindModifiedTrebleForLeftHandPart };

    static string clefKindAsString (
      bsrClefKind clefKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrClef> create (
      int         inputLineNumber,
      bsrClefKind clefKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrClef (
      int         inputLineNumber,
      bsrClefKind clefKind);

    virtual ~bsrClef ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrClefKind           getClefKind () const
                              { return fClefKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fClefCellsList; }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    bsrClefKind           fClefKind;

    S_bsrCellsList        fClefCellsList;
};
typedef SMARTP<bsrClef> S_bsrClef;
EXP ostream& operator<< (ostream& os, const S_bsrClef& elt);

//______________________________________________________________________________
class bsrTimeItem;
typedef SMARTP<bsrTimeItem> S_bsrTimeItem;

class bsrTime;
typedef SMARTP<bsrTime> S_bsrTime;

//______________________________________________________________________________
class bsrTimeItem : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTimeItem> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTimeItem (
      int inputLineNumber);

    virtual ~bsrTimeItem ();

  public:

    // set and get
    // ------------------------------------------------------

    const vector<int>&    getTimeBeatsNumbersVector ()
                              { return fTimeBeatsNumbersVector; }

    void                  setTimeBeatValue (int timeBeatValue);

    int                   getTimeBeatValue () const
                              { return fTimeBeatValue; }

    // services
    // ------------------------------------------------------

    bool                  isEqualTo (S_bsrTimeItem otherTimeItem) const;

    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeBeatsNumber () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    vector<int>           fTimeBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeBeatValue;
};
typedef SMARTP<bsrTimeItem> S_bsrTimeItem;
EXP ostream& operator<< (ostream& os, const S_bsrTimeItem& elt);

//______________________________________________________________________________
class bsrKey;
typedef SMARTP<bsrKey> S_bsrKey;

class bsrKey : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrKeyKind {
        kKeyKindNone,
        kKeyKindFlats, kKeyKindNaturals, kKeyKindSharps };

    static string keyKindAsString (
      bsrKeyKind keyKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrKey> create (
      int        inputLineNumber,
      bsrKeyKind keyKind,
      int        numberOfAlterations);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrKey (
      int        inputLineNumber,
      bsrKeyKind keyKind,
      int        numberOfAlterations);

    virtual ~bsrKey ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrKeyKind            getKeyKind () const
                              { return fKeyKind; }

    int                   getNumberOfAlterations () const
                              { return fNumberOfAlterations; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fKeyCellsList; }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        keyKindAsCellsList () const;

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    bsrKeyKind            fKeyKind;
    int                   fNumberOfAlterations;

    S_bsrCellsList        fKeyCellsList;
};
typedef SMARTP<bsrKey> S_bsrKey;
EXP ostream& operator<< (ostream& os, const S_bsrKey& elt);

//______________________________________________________________________________
class bsrTime : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrTimeKind {
        kTimeNone,
        kTimeCommon,
        kTimeCut,
        kTimeNumerical, // JMI
        kTimeNote,
        kTimeDottedNote,
        kTimeSingleNumber,
        kTimeSenzaMisura };

    static string timeKindAsString (
      bsrTimeKind timeKind);

  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTime> create (
      int         inputLineNumber,
      bsrTimeKind timeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTime (
      int         inputLineNumber,
      bsrTimeKind timeKind);

    virtual ~bsrTime ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrTimeKind           getTimeKind () const
                              { return fTimeKind; }

    const vector<S_bsrTimeItem>&
                          getTimeItemsVector ()
                              { return fTimeItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendTimeItem (S_bsrTimeItem timeItem);

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    bsrTimeKind           fTimeKind;

    vector<S_bsrTimeItem> fTimeItemsVector;
};
typedef SMARTP<bsrTime> S_bsrTime;
EXP ostream& operator<< (ostream& os, const S_bsrTime& elt);

//______________________________________________________________________________
class bsrBarline;
typedef SMARTP<bsrBarline> S_bsrBarline;

class bsrBarline : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrBarlineKind {
        kBarlineKindNone,
        kBarlineKindSpecial, // for dotted or dashed barlines
        kBarlineKindUnusual, // irregular spacing, ...
        kBarlineKindFinalDouble, kBarlineKindSectionalDouble };

    static string barlineKindAsString (
      bsrBarlineKind barlineKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrBarline> create (
      int            inputLineNumber,
      bsrBarlineKind barlineKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrBarline (
      int            inputLineNumber,
      bsrBarlineKind barlineKin);

    virtual ~bsrBarline ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrBarlineKind        getBarlineKind () const
                              { return fBarlineKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fBarlineCellsList; }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        barlineKindAsCellsList ();

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    bsrBarlineKind        fBarlineKind;

    S_bsrCellsList        fBarlineCellsList;
};
typedef SMARTP<bsrBarline> S_bsrBarline;
EXP ostream& operator<< (ostream& os, const S_bsrBarline& elt);

//______________________________________________________________________________
class bsrNumber : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrNumberSignIsNeededKind {
        kNumberSignIsNeededYes, kNumberSignIsNeededNo};

    static string numberSignIsNeededKindAsString (
      bsrNumberSignIsNeededKind numberSignIsNeededKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrNumber> create (
      int       inputLineNumber,
      int       numberValue,
      bsrNumberSignIsNeededKind
                numberSignIsNeededKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrNumber (
      int       inputLineNumber,
      int       numberValue,
      bsrNumberSignIsNeededKind
                numberSignIsNeededKind);

    virtual ~bsrNumber ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getNumberValue () const
                              { return fNumberValue; }

    void                  setNumberSignIsNeededKind (
                            bsrNumberSignIsNeededKind
                              numberSignIsNeededKindvalue)
                              {
                                fNumberSignIsNeededKind =
                                  numberSignIsNeededKindvalue;
                              }

    bsrNumberSignIsNeededKind
                          getNumberSignIsNeededKind () const
                              { return fNumberSignIsNeededKind; }

    S_bsrCellsList        getNumberCellsList () const
                              { return fNumberCellsList; }

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fNumberCellsList; }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        numberValueAsCellsList () const;

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fNumberValue;

    bsrNumberSignIsNeededKind
                          fNumberSignIsNeededKind;

    S_bsrCellsList        fNumberCellsList;
};
typedef SMARTP<bsrNumber> S_bsrNumber;
EXP ostream& operator<< (ostream& os, const S_bsrNumber& elt);

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

    S_bsrCellsList        fetchCellsList () const
                              { return fWordCellsList; }

    int                   fetchCellsNumber () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asWord () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    string                fWordContents;

    S_bsrCellsList        fWordCellsList;
};
typedef SMARTP<bsrWords> S_bsrWords;
EXP ostream& operator<< (ostream& os, const S_bsrWords& elt);

//______________________________________________________________________________
class bsrNote : public bsrLineElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum bsrNoteValueKind {
        kNoteValueNone,

        kNoteValueRestBreve,
        kNoteValueRestWhole,
        kNoteValueRestHalf,
        kNoteValueRestQuarter,
        kNoteValueRest8th,
        kNoteValueRest16th,
        kNoteValueRest32nd,
        kNoteValueRest64th,
        kNoteValueRest128th,
        kNoteValueRest256th,

        kNoteValueCBreve,
        kNoteValueCWhole,
        kNoteValueCHalf,
        kNoteValueCQuarter,
        kNoteValueC8th,
        kNoteValueC16th,
        kNoteValueC32nd,
        kNoteValueC64th,
        kNoteValueC128th,
        kNoteValueC256th,

        kNoteValueDBreve,
        kNoteValueDWhole,
        kNoteValueDHalf,
        kNoteValueDQuarter,
        kNoteValueD8th,
        kNoteValueD16th,
        kNoteValueD32nd,
        kNoteValueD64th,
        kNoteValueD128th,
        kNoteValueD256th,

        kNoteValueEBreve,
        kNoteValueEWhole,
        kNoteValueEHalf,
        kNoteValueEQuarter,
        kNoteValueE8th,
        kNoteValueE16th,
        kNoteValueE32nd,
        kNoteValueE64th,
        kNoteValueE128th,
        kNoteValueE256th,

        kNoteValueFBreve,
        kNoteValueFWhole,
        kNoteValueFHalf,
        kNoteValueFQuarter,
        kNoteValueF8th,
        kNoteValueF16th,
        kNoteValueF32nd,
        kNoteValueF64th,
        kNoteValueF128th,
        kNoteValueF256th,

        kNoteValueGBreve,
        kNoteValueGWhole,
        kNoteValueGHalf,
        kNoteValueGQuarter,
        kNoteValueG8th,
        kNoteValueG16th,
        kNoteValueG32nd,
        kNoteValueG64th,
        kNoteValueG128th,
        kNoteValueG256th,

        kNoteValueABreve,
        kNoteValueAWhole,
        kNoteValueAHalf,
        kNoteValueAQuarter,
        kNoteValueA8th,
        kNoteValueA16th,
        kNoteValueA32nd,
        kNoteValueA64th,
        kNoteValueA128th,
        kNoteValueA256th,

        kNoteValueBBreve,
        kNoteValueBWhole,
        kNoteValueBHalf,
        kNoteValueBQuarter,
        kNoteValueB8th,
        kNoteValueB16th,
        kNoteValueB32nd,
        kNoteValueB64th,
        kNoteValueB128th,
        kNoteValueB256th };

    static string noteValueKindAsString (
      bsrNoteValueKind noteValueKind);

    static S_bsrCellsList noteValueKindAsCellsList (
      int              inputLineNumber,
      bsrNoteValueKind noteValueKind);

    enum bsrNoteValueSizeKind {
        kNoteValueSizeNone,
        kNoteValueSizeLarger,
        kNoteValueSizeSmaller };

    static string noteValueSizeKindAsString (
      bsrNoteValueSizeKind noteValueSizeKind);

    static bsrNoteValueSizeKind noteValueSizeKindFromNoteValueKind (
      bsrNoteValueKind noteValueKind);

    static S_bsrCellsList noteValueSizeKindAsCellsList (
      int                  inputLineNumber,
      bsrNoteValueSizeKind noteValueSizeKind);

    enum bsrNoteOctaveKind {
        kNoteOctaveNone,
        kNoteOctaveBelow1,
        kNoteOctave1,
        kNoteOctave2,
        kNoteOctave3,
        kNoteOctave4,
        kNoteOctave5,
        kNoteOctave6,
        kNoteOctave7,
        kNoteOctaveAbove7 };

    static string noteOctaveKindAsString (
      bsrNoteOctaveKind noteOctaveKind);

    static S_bsrCellsList noteOctaveKindAsCellsList (
      int               inputLineNumber,
      bsrNoteOctaveKind noteOctaveKind);

    enum bsrNoteOctaveIsNeeded {
        kNoteOctaveIsNeededYes, kNoteOctaveIsNeededNo};

    static string noteOctaveIsNeededAsString (
      bsrNoteOctaveIsNeeded noteOctaveIsNeeded);

    enum bsrNoteAccidentalKind {
      kNoteAccidentalNone,
      kNoteAccidentalSharp, kNoteAccidentalNatural, kNoteAccidentalFlat,
      kNoteAccidentalDoubleFlat, kNoteAccidentalDoubleSharp,
      kNoteAccidentalQuarterSharp, kNoteAccidentalQuarterFlat,
      kNoteAccidentalThreeQuarterSharp, kNoteAccidentalThreeQuarterFlat };

    static string noteAccidentalKindAsString (
      bsrNoteAccidentalKind noteAccidentalKind);

    static S_bsrCellsList noteAccidentalKindAsCellsList (
      int                   inputLineNumber,
      bsrNoteAccidentalKind noteAccidentalKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrNote> create (
      int                   inputLineNumber,
      bsrNoteValueKind      noteValueKind,
      int                   noteDotsNumber,
      bsrNoteOctaveKind     noteOctaveKind,
      bsrNoteOctaveIsNeeded noteOctaveIsNeeded,
      bsrNoteAccidentalKind noteAccidentalKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrNote (
      int                   inputLineNumber,
      bsrNoteValueKind      noteValueKind,
      int                   noteDotsNumber,
      bsrNoteOctaveKind     noteOctaveKind,
      bsrNoteOctaveIsNeeded noteOctaveIsNeeded,
      bsrNoteAccidentalKind noteAccidentalKind);

    virtual ~bsrNote ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrNoteValueKind      getNoteValueKind () const
                              { return fNoteValueKind; }

    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }

    void                  setNoteValueSizeIsNeeded ()
                              { fNoteValueSizeIsNeeded = true; }

    bool                  getNoteValueSizeIsNeeded () const
                              { return fNoteValueSizeIsNeeded; }

    void                  setNoteOctaveIsNeeded (
                            bsrNoteOctaveIsNeeded value)
                              { fNoteOctaveIsNeeded = value; }

    bsrNoteOctaveIsNeeded getNoteOctaveIsNeeded () const
                              { return fNoteOctaveIsNeeded; }

    bsrNoteAccidentalKind getNoteAccidentalKind () const
                              { return fNoteAccidentalKind; }

   public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        noteValueKindAsCellsList () const;

    S_bsrCellsList        noteValueSizeKindAsCellsList () const;

    S_bsrCellsList        noteOctaveKindAsCellsList () const;

    S_bsrCellsList        noteAccidentalKindAsCellsList () const;

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------


    bsrNoteValueKind      fNoteValueKind;
    int                   fNoteDotsNumber;

    bool                  fNoteValueSizeIsNeeded;

    bsrNoteOctaveKind     fNoteOctaveKind;
    bsrNoteOctaveIsNeeded fNoteOctaveIsNeeded;

    bsrNoteAccidentalKind fNoteAccidentalKind;
};
typedef SMARTP<bsrNote> S_bsrNote;
EXP ostream& operator<< (ostream& os, const S_bsrNote& elt);

//______________________________________________________________________________
class bsrDynamics : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrDynamics> create (
      int                          inputLineNumber,
      msrDynamics::msrDynamicsKind dynamicsKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrDynamics (
      int                          inputLineNumber,
      msrDynamics::msrDynamicsKind dynamicsKind);

    virtual ~bsrDynamics ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDynamics::msrDynamicsKind
                          getDynamicsKind () const
                              { return fDynamicsKind; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fDynamicsCellsList; }


    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        noteValueKindAsCellsList () const;

    S_bsrCellsList        noteOctaveKindAsCellsList () const;

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    msrDynamics::msrDynamicsKind
                          fDynamicsKind;

    S_bsrCellsList        fDynamicsCellsList;
};
typedef SMARTP<bsrDynamics> S_bsrDynamics;
EXP ostream& operator<< (ostream& os, const S_bsrDynamics& elt);

//______________________________________________________________________________
class bsrMeasure : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrMeasure> create (
      int    inputLineNumber,
      string printMeasureNumber);

    SMARTP<bsrMeasure> createMeasureNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrMeasure (
      int    inputLineNumber,
      string printMeasureNumber);

    virtual ~bsrMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getPrintMeasureNumber () const
                              { return fPrintMeasureNumber; }

    string                getBrailleMeasureNumber () const
                              { return fBrailleMeasureNumber; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendClefToMeasure (S_bsrClef clef);

    void                  appendBarlineToMeasure (S_bsrBarline barline);

    void                  appendNumberToMeasure (S_bsrNumber number);

    void                  appendWordsToMeasure (S_bsrWords str);

    void                  appendNoteToMeasure (S_bsrNote note);

    void                  appendPaddingNoteToMeasure (S_bsrNote note);

    void                  appendDynamicsToMeasure (S_bsrDynamics dynamics);

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    void                  appendLineElementToMeasure (
                            S_bsrLineElement lineElement);

    S_bsrCellsList        buildCellsList () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    string                fPrintMeasureNumber;
    string                fBrailleMeasureNumber;

    list<S_bsrLineElement>
                          fMeasureLineElementsList;
};
typedef SMARTP<bsrMeasure> S_bsrMeasure;
EXP ostream& operator<< (ostream& os, const S_bsrMeasure& elt);

//______________________________________________________________________________
const bsrCellKind
  kCellTempoEquals = kDots2356,
  kCellTempoHyphen = kDots36;

class bsrTempo : public bsrLineElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTempo> create (
      int        inputLineNumber,
      S_msrTempo mTempo);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTempo (
      int        inputLineNumber,
      S_msrTempo mTempo);

    virtual ~bsrTempo ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrTempo            getMsrTempo () const
                              { return fMsrTempo; }

    S_bsrCellsList        getTempoCellsList () const
                              { return fTempoCellsList; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return fTempoCellsList; }

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        noteValueKindAsCellsList () const;

    S_bsrCellsList        noteOctaveKindAsCellsList () const;

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrTempo            fMsrTempo;

    S_bsrCellsList        fTempoCellsList;
};
typedef SMARTP<bsrTempo> S_bsrTempo;
EXP ostream& operator<< (ostream& os, const S_bsrTempo& elt);

//______________________________________________________________________________
class bsrLine : public bsrPageElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrLine> create (
      int inputLineNumber,
      int printLineNumber,
      int cellsPerLine);

    SMARTP<bsrLine> createLineNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrLine (
      int inputLineNumber,
      int printLineNumber,
      int cellsPerLine);

    virtual ~bsrLine ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintLineNumber () const
                              { return fPrintLineNumber; }

    int                   getBrailleLineNumber () const
                              { return fBrailleLineNumber; }

    int                   getCellsPerLine () const
                              { return fCellsPerLine; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendSpacesToLine (S_bsrSpaces spaces);

    void                  appendMeasureToLine (S_bsrMeasure measure);

    void                  appendKeyToLine (S_bsrKey key);

    void                  appendTimeToLine (S_bsrTime time);

    void                  insertTimeBeforeLastElementOfLine (S_bsrTime time);

    void                  appendTempoToLine (S_bsrTempo tempo);

    void                  appendNoteToLine (S_bsrNote note);

    int                   fetchLineContentsNumber () const
                              { return fLineContentsList.size (); }

    S_bsrCellsList        fetchCellsList () const
                              { return buildLineNumberCellsList (); } // JMI ???

    int                   fetchCellsNumber () const;

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildLineNumberCellsList () const;

    void                  appendLineElementToLine (
                            S_bsrLineElement lineElement);

    void                  insertElementBeforeLastElementOfLine (
                            S_bsrLineElement lineElement);

    void                  appendLineElementToLastMeasureOfLine (
                            S_bsrLineElement lineElement);

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

    virtual void          print (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    int                   fPrintLineNumber;
    int                   fBrailleLineNumber;

    int                   fCellsPerLine;

    S_bsrCellsList        fLineNumberCellsList;
    list<S_bsrLineContents>
                          fLineContentsList;
    bool                  fASpaceIsNeededInLine;
};
typedef SMARTP<bsrLine> S_bsrLine;
EXP ostream& operator<< (ostream& os, const S_bsrLine& elt);

//______________________________________________________________________________
class bsrPageHeading : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPageHeading> create (
      int             inputLineNumber,
      string          pageHeadingTitle,
      S_bsrPagination pageHeadingPagination,
      int             pageHeadingNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPageHeading (
      int             inputLineNumber,
      string          pageHeadingTitle,
      S_bsrPagination pageHeadingPagination,
      int             pageHeadingNumber);

    virtual ~bsrPageHeading ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getPageHeadingTitle () const
                              { return fPageHeadingTitle; }

    S_bsrPagination       getPageHeadingPagination () const
                              { return fPageHeadingPagination; }

    int                   getPageHeadingNumber () const
                              { return fPageHeadingNumber; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    string                fPageHeadingTitle;

    S_bsrPagination       fPageHeadingPagination; // before title
    int                   fPageHeadingNumber;     // after title ??? JMI
};
typedef SMARTP<bsrPageHeading> S_bsrPageHeading;
EXP ostream& operator<< (ostream& os, const S_bsrPageHeading& elt);

//______________________________________________________________________________
class bsrMusicHeading : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrMusicHeading> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrMusicHeading (
      int inputLineNumber);

    virtual ~bsrMusicHeading ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMusicHeadingTempo (S_bsrTempo tempo)
                              { fMusicHeadingTempo = tempo; }

    S_bsrTempo            getMusicHeadingTempo () const
                              { return fMusicHeadingTempo; }

    S_bsrKey              getMusicHeadingKey () const
                              { return fMusicHeadingKey; }

    void                  setMusicHeadingKey (S_bsrKey key)
                              { fMusicHeadingKey = key; }

    S_bsrTime             getMusicHeadingTime () const
                              { return fMusicHeadingTime; }

    void                  setMusicHeadingTime (S_bsrTime time)
                              { fMusicHeadingTime = time; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }

  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_bsrTempo            fMusicHeadingTempo;
    S_bsrKey              fMusicHeadingKey;
    S_bsrTime             fMusicHeadingTime;
};
typedef SMARTP<bsrMusicHeading> S_bsrMusicHeading;
EXP ostream& operator<< (ostream& os, const S_bsrMusicHeading& elt);

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

    virtual void          print (ostream& os);

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    list<S_bsrFootNotesElement>
                          fFootNotesElementsList;
};
typedef SMARTP<bsrFootNotes> S_bsrFootNotes;
EXP ostream& operator<< (ostream& os, const S_bsrFootNotes& elt);

//______________________________________________________________________________
class bsrPage : public bsrElement
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

    int                   fetchLineContentsNumber ();

  public:


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

    virtual void          print (ostream& os);

  private:

    // fields
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
