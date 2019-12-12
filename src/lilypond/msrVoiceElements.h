/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrVoiceElements___
#define ___msrVoiceElements___

#include "msrElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrVoiceElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    // cloning
    // ------------------------------------------------------

  protected:

    msrVoiceElement (
      int inputLineNumber);

    virtual ~msrVoiceElement ();

  public:

/* JMI
    // set and get
    // ------------------------------------------------------

    void                  setVoiceNumber (
                            string positionInVoice)
                              {
                                fVoiceNumber =
                                  positionInVoice;
                              }

    string                getVoiceNumber ()
                              { return fVoiceNumber; }

    void                  setPositionInVoice (
                            rational positionInVoice)
                              {
                                fPositionInVoice =
                                  positionInVoice;
                              }

    rational              getPositionInVoice ()
                              { return fPositionInVoice; }

    // services
    // ------------------------------------------------------

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

  protected:

    // fields
    // ------------------------------------------------------

    string                fVoiceNumber;
    rational              fPositionInVoice;
    */
};
typedef SMARTP<msrVoiceElement> S_msrVoiceElement;
EXP ostream& operator<< (ostream& os, const S_msrVoiceElement& elt);


} // namespace MusicXML2


#endif
