/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrTupletElements___
#define ___msrTupletElements___

#include "msrSegnosAndCodas.h"


namespace MusicXML2
{

//______________________________________________________________________________
/*
  Tuplet elements, i.e. notes, chords and tuplets, can also be found in measures, hence:
*/
class msrTupletElement : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

    msrTupletElement (
      int inputLineNumber);

    virtual ~msrTupletElement ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setPositionInTuplet (
                            int positionInTuplet)
                              {
                                fPositionInTuplet =
                                  positionInTuplet;
                              }

    int                   getPositionInTuplet ()
                              { return fPositionInTuplet; }

  public:

    // services
    // ------------------------------------------------------

/* JMI
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v) = 0;

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asShortString () const;
    virtual std::string   asString () const;

    virtual void          print (ostream& os) const;

    virtual void          printSummary (ostream& os) {}
    */

  protected:

    // fields
    // ------------------------------------------------------

    int                   fPositionInTuplet;
};
typedef SMARTP<msrTupletElement> S_msrTupletElement;
EXP ostream& operator<< (ostream& os, const S_msrTupletElement& elt);


} // namespace MusicXML2


#endif
