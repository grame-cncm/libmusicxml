/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrCredits___
#define ___msrCredits___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrCreditWords : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCreditWords> create (
      int         inputLineNumber,
      string      creditWordsContents,
      string      creditWordsFontFamily,
      float       creditWordsFontSize,
      string      creditWordsFontWeight,
      string      creditWordsFontJustify,
      string      creditWordsFontHAlign,
      string      creditWordsFontVAlign,
      string      creditWordsFontXMLLanguage);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrCreditWords (
      int         inputLineNumber,
      string      creditWordsContents,
      string      creditWordsFontFamily,
      float       creditWordsFontSize,
      string      creditWordsFontWeight,
      string      creditWordsFontJustify,
      string      creditWordsFontHAlign,
      string      creditWordsFontVAlign,
      string      creditWordsFontXMLLanguage);
      
    virtual ~msrCreditWords ();
  
  public:

    // set and get
    // ------------------------------------------------------
    string                getCreditWordsContents () const
                              { return fCreditWordsContents; }

    float                 getCreditWordsFontSize () const
                              { return fCreditWordsFontSize; }

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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

/*
    <credit-words default-x="607" default-y="1443" font-family="ＭＳ ゴシック" font-size="24" font-weight="bold" justify="center" valign="top" xml:lang="ja">越後獅子</credit-words>
*/

    string                  fCreditWordsContents;

    string                  fCreditWordsFontFamily;
    float                   fCreditWordsFontSize;
    string                  fCreditWordsFontWeight;
    string                  fCreditWordsFontJustify;
    string                  fCreditWordsFontHAlign;
    string                  fCreditWordsFontVAlign;
    string                  fCreditWordsFontXMLLanguage;
};
typedef SMARTP<msrCreditWords> S_msrCreditWords;
EXP ostream& operator<< (ostream& os, const S_msrCreditWords& elt);

//______________________________________________________________________________
class msrCredit : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCredit> create (
      int inputLineNumber,
      int creditPageNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrCredit (
      int inputLineNumber,
      int creditPageNumber);
      
    virtual ~msrCredit ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getCreditPageNumber () const
                              { return fCreditPageNumber; }

    const vector<S_msrCreditWords>&
                          getCreditWordsList () const
                              { return fCreditWordsList; }

    // services
    // ------------------------------------------------------

    void                  appendCreditWordsToCredit (
                            S_msrCreditWords creditWords);

    string                asString () const;

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

    int                       fCreditPageNumber;
    
    vector<S_msrCreditWords>  fCreditWordsList;
};
typedef SMARTP<msrCredit> S_msrCredit;
EXP ostream& operator<< (ostream& os, const S_msrCredit& elt);


} // namespace MusicXML2


#endif
