/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrVarValAssocs___
#define ___lpsrVarValAssocs___

#include <string>
#include <list>

#include "lpsrElements.h"

#include "msrBasicTypes.h"


using namespace std;

namespace MusicXML2

{

//______________________________________________________________________________
class lpsrVarValAssoc : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    // data types
    // ------------------------------------------------------

    enum lpsrVarValAssocKind {
      // library

      kLibraryVersion,

      // MusicXML informations

      kMusicXMLWorkNumber, kMusicXMLWorkTitle, kMusicXMLOpus,
      kMusicXMLMovementNumber, kMusicXMLMovementTitle,
      kMusicXMLEncodingDate,
      kMusicXMLScoreInstrument,
      kMusicXMLMiscellaneousField,

      // LilyPond informations

      kLilypondDedication,
      kLilypondPiece, kLilypondOpus,
      kLilypondTitle, kLilypondSubTitle, kLilypondSubSubTitle,
      kLilypondInstrument, kLilypondMeter,
      kLilypondTagline, kLilypondCopyright,

      kLilypondMyBreak, kLilypondMyPageBreak,
      kLilypondGlobal };

    static string lilyPondVarValAssocKindAsString (
      lpsrVarValAssocKind lilyPondVarValAssocKind);

    enum lpsrCommentedKind {
      kCommentedYes, kCommentedNo };

    static string commentedKindAsString (
      lpsrCommentedKind commentedKind);

    enum lpsrBackSlashKind {
      kWithBackSlashYes, kWithBackSlashNo };

    static string backSlashKindAsString (
      lpsrBackSlashKind backSlashKind);

    enum lpsrVarValSeparatorKind {
      kVarValSeparatorSpace, kVarValSeparatorEqualSign };

    static string varValSeparatorKindAsString (
      lpsrVarValSeparatorKind varValSeparatorKind);

    enum lpsrQuotesKind {
      kQuotesAroundValueYes, kQuotesAroundValueNo };

    static string quotesKindAsString (
      lpsrQuotesKind quotesKind);

    enum lpsrEndlKind {
      kEndlNone, kEndlOnce, kEndlTwice };

    static string endlKindAsString (
      lpsrEndlKind endlKind);

    static string const g_LilyPondVarValAssocNoUnit;
    static string const g_LilyPondVarValAssocNoComment;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrVarValAssoc> create (
      int                     inputLineNumber,
      lpsrCommentedKind       commentedKind,
      lpsrBackSlashKind       backSlashKind,
      lpsrVarValAssocKind
                              lilyPondVarValAssocKind,
      lpsrVarValSeparatorKind varValSeparatorKind,
      lpsrQuotesKind          quotesKind,
      string                  value,
      string                  unit,
      msrFontStyleKind        varValFontStyleKind,
      msrFontWeightKind       varValFontWeightKind,
      string                  comment,
      lpsrEndlKind            endlKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrVarValAssoc (
      int                     inputLineNumber,
      lpsrCommentedKind       commentedKind,
      lpsrBackSlashKind       backSlashKind,
      lpsrVarValAssocKind
                              lilyPondVarValAssocKind,
      lpsrVarValSeparatorKind varValSeparatorKind,
      lpsrQuotesKind          quotesKind,
      string                  value,
      string                  unit,
      msrFontStyleKind        varValFontStyleKind,
      msrFontWeightKind       varValFontWeightKind,
      string                  comment,
      lpsrEndlKind            endlKind);

    virtual ~lpsrVarValAssoc ();

  public:

    // set and get
    // ------------------------------------------------------

    lpsrCommentedKind     getCommentedKind () const
                              { return fCommentedKind; }

    lpsrBackSlashKind     getBackSlashKind () const
                              { return fBackSlashKind; }

    lpsrVarValAssocKind
                          getLilyPondVarValAssocKind () const
                              { return fLilyPondVarValAssocKind; }

    lpsrVarValSeparatorKind
                          getVarValSeparatorKind () const
                              { return fVarValSeparatorKind; }

    lpsrQuotesKind        getQuotesKind () const
                              { return fQuotesKind; }

    void                  setVariableValue (string value)
                              { fVariableValue = value; }

    string                getVariableValue () const
                              { return fVariableValue; }

    string                getUnit () const
                              { return fUnit; }

    msrFontStyleKind      getVarValFontStyleKind () const
                              { return fVarValFontStyleKind; }

    msrFontWeightKind      getVarValFontWeightKind () const
                              { return fVarValFontWeightKind; }

    string                getComment  () const
                              { return fComment; }

    lpsrEndlKind          getEndlKind () const
                              { return fEndlKind; }

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

    string                lilyPondVarValAssocKindAsString () const
                              {
                                return
                                  lilyPondVarValAssocKindAsString (
                                    fLilyPondVarValAssocKind);
                              }

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    lpsrCommentedKind     fCommentedKind;
    lpsrBackSlashKind     fBackSlashKind;
    lpsrVarValAssocKind
                          fLilyPondVarValAssocKind;
    lpsrVarValSeparatorKind
                          fVarValSeparatorKind;
    lpsrQuotesKind        fQuotesKind;

    string                fVariableValue;
    string                fUnit;

    msrFontStyleKind      fVarValFontStyleKind;
    msrFontWeightKind     fVarValFontWeightKind;

    string                fComment;

    lpsrEndlKind          fEndlKind;
};
typedef SMARTP<lpsrVarValAssoc> S_lpsrVarValAssoc;
EXP ostream& operator<< (ostream& os, const S_lpsrVarValAssoc& elt);

//______________________________________________________________________________
class lpsrVarValsListAssoc : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum lpsrVarValsListAssocKind {
      // MusicXML informations
      kMusicXMLRights,
      kMusicXMLComposer, kMusicXMLArranger,
      kMusicXMLPoet, kMusicXMLLyricist, kMusicXMLTranslator,
      kMusicXMLArtist,
      kMusicXMLSoftware };

/*
      % The following fields are centered
    dedication = "Dedication"
    title = "Title"
    subtitle = "Subtitle"
    subsubtitle = "Subsubtitle"
      % The following fields are evenly spread on one line
      % the field "instrument" also appears on following pages
    instrument = \markup \with-color #green "Instrument"
    poet = "Poet"
    composer = "Composer"
      % The following fields are placed at opposite ends of the same line
    meter = "Meter"
    arranger = "Arranger"
      % The following fields are centered at the bottom
    tagline = "tagline goes at the bottom of the last page"
    copyright = "copyright goes at the bottom of the first page"
*/

    static string lilyPondVarValsListAssocValuesAsString (
      lpsrVarValsListAssocKind varValsListAssocKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrVarValsListAssoc> create (
      int                      inputLineNumber,
      lpsrVarValsListAssocKind varValsListAssocKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrVarValsListAssoc (
      int                      inputLineNumber,
      lpsrVarValsListAssocKind varValsListAssocKind);

    virtual ~lpsrVarValsListAssoc ();

  public:

    // set and get
    // ------------------------------------------------------

    lpsrVarValsListAssocKind
                          getVarValsListAssocKind () const
                              { return fVarValsListAssocKind; }

    const list<string>&   getVariableValuesList ()
                              { return fVariableValuesList; }

    // services
    // ------------------------------------------------------

    void                  addAssocVariableValue (string value)
                              {
                                fVariableValuesList.push_back (value);
                              }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                lilyPondVarValsListAssocKindAsString () const
                              {
                                return
                                  lilyPondVarValsListAssocValuesAsString (
                                    fVarValsListAssocKind);
                              }

    string                lilyPondVarValsListAssocValuesAsString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    lpsrVarValsListAssocKind
                          fVarValsListAssocKind;

    list<string>          fVariableValuesList;
};
typedef SMARTP<lpsrVarValsListAssoc> S_lpsrVarValsListAssoc;
EXP ostream& operator<< (ostream& os, const S_lpsrVarValsListAssoc& elt);


} // namespace MusicXML2


#endif
