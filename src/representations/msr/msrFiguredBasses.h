#ifndef ___msrFiguredBasses___
#define ___msrFiguredBasses___

#include "msrElements.h"
#include "msrMeasureElements.h"
#include "msrNotes.h"
#include "msrParts.h"


namespace MusicXML2
{

//______________________________________________________________________________
/*
class msrPart;
typedef SMARTP<msrPart> S_msrPart; // superflous ??? JMI

class msrNote;
typedef SMARTP<msrNote> S_msrNote; // superflous ??? JMI
*/
//______________________________________________________________________________
class EXP msrFigure : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFigurePrefixKind {
      k_NoFigurePrefix,
      kDoubleFlatPrefix, kFlatPrefix, kFlatFlatPrefix,
      kNaturalPrefix,
      kSharpSharpPrefix, kSharpPrefix, kDoubleSharpPrefix
    };

    static string figurePrefixKindAsString (
      msrFigurePrefixKind figurePrefixKind);

    enum msrFigureSuffixKind {
      k_NoFigureSuffix,
      kDoubleFlatSuffix, kFlatSuffix, kFlatFlatSuffix,
      kNaturalSuffix,
      kSharpSharpSuffix, kSharpSuffix, kDoubleSharpSuffix,
      kSlashSuffix
    };

    static string figureSuffixKindAsString (
      msrFigureSuffixKind figureSuffixKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFigure> create (
      int                 inputLineNumber,
      S_msrPart           figurePartUpLink,
      msrFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrFigureSuffixKind figureSuffixKind);

    SMARTP<msrFigure> createFigureNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrFigure> createFigureDeepCopy ( // JMI ???
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFigure (
      int                 inputLineNumber,
      S_msrPart           figurePartUpLink,
      msrFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrFigureSuffixKind figureSuffixKind);

    virtual               ~msrFigure ();

  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getFigurePartUpLink () const
                              { return fFigurePartUpLink; }

    msrFigurePrefixKind
                          getFigurePrefixKind () const
                              { return fFigurePrefixKind; }

    int                   getFigureNumber () const
                              { return fFigureNumber; }

    msrFigureSuffixKind
                          getFigureSuffixKind () const
                              { return fFigureSuffixKind; }

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

    string                figurePrefixKindAsString () const;
    string                figureSuffixKindAsShortString () const;

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrPart             fFigurePartUpLink;

    msrFigurePrefixKind
                          fFigurePrefixKind;
    int                   fFigureNumber;
    msrFigureSuffixKind
                          fFigureSuffixKind;
};
typedef SMARTP<msrFigure> S_msrFigure;
EXP ostream& operator<< (ostream& os, const S_msrFigure& elt);

//______________________________________________________________________________
class EXP msrFiguredBass : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFiguredBassParenthesesKind {
      kFiguredBassParenthesesYes, kFiguredBassParenthesesNo
    };

    static string figuredBassParenthesesKindAsString (
      msrFiguredBassParenthesesKind figuredBassParenthesesKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFiguredBass> create (
      int       inputLineNumber); // JMI ???

    static SMARTP<msrFiguredBass> create (
                            int                           inputLineNumber,
                            rational                      figuredBassSoundingWholeNotes,
                            rational                      figuredBassDisplayWholeNotes,
                            msrFiguredBassParenthesesKind figuredBassParenthesesKind,
                            msrTupletFactor               figuredBassTupletFactor);

    SMARTP<msrFiguredBass> createFiguredBassNewbornClone (
                            S_msrVoice containingVoice);

    SMARTP<msrFiguredBass> createFiguredBassDeepCopy (); // JMI ???

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrFiguredBass (
                            int                           inputLineNumber,
                            rational                      figuredBassSoundingWholeNotes,
                            rational                      figuredBassDisplayWholeNotes,
                            msrFiguredBassParenthesesKind figuredBassParenthesesKind,
                            msrTupletFactor               figuredBassTupletFactor);

    virtual               ~msrFiguredBass ();

  public:

    // set and get
    // ------------------------------------------------------
    void                  setFiguredBassNoteUpLink (
                            S_msrNote note)
                              { fFiguredBassNoteUpLink = note; }

    S_msrNote             getFiguredBassNoteUpLink () const
                             { return fFiguredBassNoteUpLink; }

    void                  setFiguredBassVoiceUpLink (
                            S_msrVoice voice)
                              { fFiguredBassVoiceUpLink = voice; }

    S_msrVoice            getFiguredBassVoiceUpLink () const
                             { return fFiguredBassVoiceUpLink; }

/* JMI
    S_msrPart             getFiguredBassPartUpLink () const
                              { return fFiguredBassPartUpLink; }
*/

    void                  setFiguredBassDisplayWholeNotes (
                            rational wholeNotes)
                              { fFiguredBassDisplayWholeNotes = wholeNotes; }

    rational              getFiguredBassDisplayWholeNotes () const
                              { return fFiguredBassDisplayWholeNotes; }

    msrFiguredBassParenthesesKind
                          getFiguredBassParenthesesKind () const
                              { return fFiguredBassParenthesesKind; }

    const list<S_msrFigure>&
                          getFiguredBassFiguresList () const
                              { return fFiguredBassFiguresList;  }

    void                  setFiguredBassTupletFactor (
                            msrTupletFactor tupletFactor)
                              { fFiguredBassTupletFactor = tupletFactor; }

    msrTupletFactor       getFiguredBassTupletFactor () const
                              { return fFiguredBassTupletFactor; }

    void                  setFiguredBassPositionInMeasure (
                            rational positionInMeasure);

  public:

    // public services
    // ------------------------------------------------------

    void                  appendFigureToFiguredBass (
                            S_msrFigure figure);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // upLinks
    S_msrNote             fFiguredBassNoteUpLink;
    S_msrVoice            fFiguredBassVoiceUpLink; // for use in figured bass voices JMI

    rational              fFiguredBassDisplayWholeNotes;

    msrFiguredBassParenthesesKind
                          fFiguredBassParenthesesKind;

    list<S_msrFigure>     fFiguredBassFiguresList;

    msrTupletFactor       fFiguredBassTupletFactor;
};
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;
EXP ostream& operator<< (ostream& os, const S_msrFiguredBass& elt);


}


#endif
