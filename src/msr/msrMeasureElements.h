/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrMeasureElements___
#define ___msrMeasureElements___

#include "msrElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
/*
  Various elements can found in measures,
  hence class msrMeasureElement
*/

class msrMeasureElement : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_MEASURE_NUMBER "K_NO_MEASURE_NUMBER"

    #define K_NO_POSITION       rational (-222222, 1)

    #define K_NO_WHOLE_NOTES    rational (-444444, 1)

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasureElement (
      int inputLineNumber);

    virtual ~msrMeasureElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureElementSoundingWholeNotes (
                            rational wholeNotes,
                            string   context);

    rational              getMeasureElementSoundingWholeNotes () const
                              { return fMeasureElementSoundingWholeNotes; }

    void                  setMeasureElementMeasureNumber (
                            string positionInMeasure)
                              {
                                fMeasureElementMeasureNumber = positionInMeasure;
                              }

    string                getMeasureElementMeasureNumber ()
                              { return fMeasureElementMeasureNumber; }

    void                  setMeasureElementPositionInMeasure (
                            rational positionInMeasure,
                            string   context);

    rational              getMeasureElementPositionInMeasure ()
                              { return fMeasureElementPositionInMeasure; }

    void                  setMeasureElementPositionInVoice (
                            rational positionInVoice,
                            string   context);

    rational              getMeasureElementPositionInVoice ()
                              { return fMeasureElementPositionInVoice; }

  public:

    // public services
    // ------------------------------------------------------

    static bool           compareMeasureElementsByIncreasingPositionInMeasure (
                            const SMARTP<msrMeasureElement>& first,
                            const SMARTP<msrMeasureElement>& second);

    virtual void          assignMeasureElementPositionInVoice (
                            rational& positionInVoice,
                            string    context);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override = 0; // JMI ???

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asShortString () const override;
    virtual std::string   asString () const override;

    virtual void          print (ostream& os) const override;

    virtual void          printSummary (ostream& os) const override {}

  protected:

    // protected fields
    // ------------------------------------------------------

    /*
      The measure uplink is declared in the sub-classes,
      to allow for separate *.h files, C++ constraint
    */

    rational              fMeasureElementSoundingWholeNotes;

    string                fMeasureElementMeasureNumber;

    rational              fMeasureElementPositionInMeasure;
    rational              fMeasureElementPositionInVoice;
};
typedef SMARTP<msrMeasureElement> S_msrMeasureElement;
EXP ostream& operator<< (ostream& os, const S_msrMeasureElement& elt);


} // namespace MusicXML2


#endif
