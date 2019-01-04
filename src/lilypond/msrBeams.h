/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrBeams___
#define ___msrBeams___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrBeam : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrBeamKind {
      k_NoBeam,
      kBeginBeam, kContinueBeam, kEndBeam,
      kForwardHookBeam, kBackwardHookBeam};
    
    static string beamKindAsString (
      msrBeamKind beamKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeam> create (
      int        inputLineNumber,
      int         number,
      msrBeamKind beamKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBeam (
      int         inputLineNumber,
      int         number,
      msrBeamKind beamKind);
      
    virtual ~msrBeam ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrBeamKind           getBeamKind   () const
                              { return fBeamKind; }
                              
    int                   getBeamNumber () const
                              { return fBeamNumber; }

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

    int                   fBeamNumber;
    msrBeamKind           fBeamKind;
};
typedef SMARTP<msrBeam> S_msrBeam;
EXP ostream& operator<< (ostream& os, const S_msrBeam& elt);


} // namespace MusicXML2


#endif
