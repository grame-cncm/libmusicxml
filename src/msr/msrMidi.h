/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrMidi___
#define ___msrMidi___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrMidi : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMidi> create (
      int    inputLineNumber,
      string midiTempoDuration,
      int    midiTempoPerSecond);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMidi (
      int    inputLineNumber,
      string midiTempoDuration,
      int    midiTempoPerSecond);
      
    virtual ~msrMidi ();
  
  public:

    // set and get
    // ------------------------------------------------------
    string                getMidiTempoDuration () const
                              { return fMidiTempoDuration; }

    int                   getMidiTempoPerSecond () const
                              { return fMidiTempoPerSecond; }

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

    string                fMidiTempoDuration;
    int                   fMidiTempoPerSecond;
};
typedef SMARTP<msrMidi> S_msrMidi;
EXP ostream& operator<< (ostream& os, const S_msrMidi& elt);


} // namespace MusicXML2


#endif
