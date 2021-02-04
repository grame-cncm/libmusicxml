/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrWords___
#define ___msrWords___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrWords : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrWordsFontStyleKind {
      kNormalStyle, KItalicStyle
    };

    static string wordsFontStyleKindAsString (
      msrWordsFontStyleKind wordsFontStyleKind);

    enum msrWordsFontWeightKind {
      kNormalWeight, kBoldWeight
    };

    static string msrWordsFontWeightKindAsString (
      msrWordsFontWeightKind wordsFontWeightKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrWords> create (
                            int                        inputLineNumber,
                            msrPlacementKind           wordsPlacementKind,
                            string                     wordsContents,
                            msrJustifyKind             wordsJustifyKind,
                            msrHorizontalAlignmentKind wordsHorizontalAlignmentKind,
                            msrVerticalAlignmentKind   wordsVerticalAlignmentKind,
                            msrFontStyleKind           wordsFontStyleKind,
                            S_msrFontSize              wordsFontSize,
                            msrFontWeightKind          wordsFontWeightKind,
                            msrXMLLangKind             wordsXMLLangKind,
                            int                        wordsStaffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrWords (
                            int                        inputLineNumber,
                            msrPlacementKind           wordsPlacementKind,
                            string                     wordsContents,
                            msrJustifyKind             wordsJustifyKind,
                            msrHorizontalAlignmentKind wordsHorizontalAlignmentKind,
                            msrVerticalAlignmentKind   wordsVerticalAlignmentKind,
                            msrFontStyleKind           wordsFontStyleKind,
                            S_msrFontSize              wordsFontSize,
                            msrFontWeightKind          wordsFontWeightKind,
                            msrXMLLangKind             wordsXMLLangKind,
                            int                        wordsStaffNumber);

    virtual               ~msrWords ();

  public:

    // set and get
    // ------------------------------------------------------

    msrPlacementKind      getWordsPlacementKind () const
                              { return fWordsPlacementKind; }

    string                getWordsContents () const
                              { return fWordsContents; }

    msrJustifyKind        getWordsJustifyKind () const
                              { return fWordsJustifyKind; }

    msrHorizontalAlignmentKind
                          getWordsHorizontalAlignmentKind () const
                              { return fWordsHorizontalAlignmentKind; }

    msrVerticalAlignmentKind
                          getWordsVerticalAlignmentKind () const
                              { return fWordsVerticalAlignmentKind; }

    msrFontStyleKind      getWordsFontStyleKind () const
                              { return fWordsFontStyleKind; }

    S_msrFontSize         getWordsFontSize () const
                              { return fWordsFontSize; }

    msrFontWeightKind     getWordsFontWeightKind () const
                              { return fWordsFontWeightKind; }

    msrXMLLangKind        getWordsXMLLangKind () const
                              { return fWordsXMLLangKind; }

    int                   getWordsStaffNumber () const
                              { return fWordsStaffNumber; }

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                wordsPlacementKindAsString () const;

    string                wordsJustifyKindAsString () const;

    string                wordsHorizontalAlignmentKindAsString () const;
    string                wordsVerticalAlignmentKindAsString () const;

    string                wordsFontStyleKindAsString () const;

    string                wordsFontSizeAsString () const;

    string                wordsFontWeightKindAsString () const;

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrPlacementKind      fWordsPlacementKind;

    string                fWordsContents;

    msrJustifyKind        fWordsJustifyKind;

    msrHorizontalAlignmentKind
                          fWordsHorizontalAlignmentKind;
    msrVerticalAlignmentKind
                          fWordsVerticalAlignmentKind;

    msrFontStyleKind      fWordsFontStyleKind;
    S_msrFontSize         fWordsFontSize;
    msrFontWeightKind     fWordsFontWeightKind;

    msrXMLLangKind        fWordsXMLLangKind;

    int                   fWordsStaffNumber;
};
typedef SMARTP<msrWords> S_msrWords;
EXP ostream& operator<< (ostream& os, const S_msrWords& elt);


} // namespace MusicXML2


#endif
