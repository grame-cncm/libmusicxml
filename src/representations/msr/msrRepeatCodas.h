#ifndef ___msrRepeatCodas___
#define ___msrRepeatCodas___

#include "msrElements.h"
#include "msrSegments.h"
#include "msrRepeats.h"

namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrRepeatCoda : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCoda> create (
      int          inputLineNumber,
      S_msrSegment segment,
      S_msrRepeat  repeatUpLink);

    SMARTP<msrRepeatCoda> createRepeatCodaNewbornClone (
      S_msrRepeat containingRepeat);

    SMARTP<msrRepeatCoda> createRepeatCodaDeepCopy (
      S_msrRepeat containingRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatCoda (
      int          inputLineNumber,
      S_msrSegment segment,
      S_msrRepeat  repeatUpLink);

    virtual               ~msrRepeatCoda ();

  public:

    // set and get
    // ------------------------------------------------------

    // segment
    S_msrSegment          getRepeatCodaSegment () const
                              { return fRepeatCodaSegment; }

    // upLinks
    S_msrRepeat           getRepeatCodaRepeatUpLink () const
                              { return fRepeatCodaRepeatUpLink; }

  public:

    // public services
    // ------------------------------------------------------

 //  JMI void                  appendElementToRepeatCoda (S_msrElement elem);

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

    // segment
    S_msrSegment          fRepeatCodaSegment;

    // upLinks
    S_msrRepeat           fRepeatCodaRepeatUpLink;
};
typedef SMARTP<msrRepeatCoda> S_msrRepeatCoda;
EXP ostream& operator<< (ostream& os, const S_msrRepeatCoda& elt);


}


#endif
