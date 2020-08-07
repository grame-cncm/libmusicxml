/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsr2lilypondOah2ManPageGenerator___
#define ___lpsr2lilypondOah2ManPageGenerator___

#include "lpsr2LilypondOah.h"

#include "oah2ManPageGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class lpsr2lilypondOah2ManPageGenerator : virtual public oah2ManPageGenerator,

  public visitor<S_lilypondScoreOutputKindAtom>,

  public visitor<S_lilypondTransposePartNameAtom>,

  public visitor<S_lilypondAbsoluteOctaveEntryAtom>,

  public visitor<S_lilypondRelativeOctaveEntryAtom>,

  public visitor<S_lilypondFixedOctaveEntryAtom>,

  public visitor<S_lilypondAccidentalStyleKindAtom>,

  public visitor<S_lilypondChordsDisplayAtom>,

  public visitor<S_lilypondLyricsDurationsKindAtom>,

  public visitor<S_oahMidiTempoAtom>

{
  public:

    lpsr2lilypondOah2ManPageGenerator (
      const S_oahHandler handler,
      ostream&           logOstream,
      ostream&           manPageOutputStream);

    virtual ~lpsr2lilypondOah2ManPageGenerator ();

  protected:

    virtual void visitStart (S_lilypondScoreOutputKindAtom& elt);
    virtual void visitEnd   (S_lilypondScoreOutputKindAtom& elt);

    virtual void visitStart (S_lilypondTransposePartNameAtom& elt);
    virtual void visitEnd   (S_lilypondTransposePartNameAtom& elt);

    virtual void visitStart (S_lilypondAbsoluteOctaveEntryAtom& elt);
    virtual void visitEnd   (S_lilypondAbsoluteOctaveEntryAtom& elt);

    virtual void visitStart (S_lilypondRelativeOctaveEntryAtom& elt);
    virtual void visitEnd   (S_lilypondRelativeOctaveEntryAtom& elt);

    virtual void visitStart (S_lilypondFixedOctaveEntryAtom& elt);
    virtual void visitEnd   (S_lilypondFixedOctaveEntryAtom& elt);

    virtual void visitStart (S_lilypondAccidentalStyleKindAtom& elt);
    virtual void visitEnd   (S_lilypondAccidentalStyleKindAtom& elt);

    virtual void visitStart (S_lilypondChordsDisplayAtom& elt);
    virtual void visitEnd   (S_lilypondChordsDisplayAtom& elt);

    virtual void visitStart (S_lilypondLyricsDurationsKindAtom& elt);
    virtual void visitEnd   (S_lilypondLyricsDurationsKindAtom& elt);

    virtual void visitStart (S_oahMidiTempoAtom& elt);
    virtual void visitEnd   (S_oahMidiTempoAtom& elt);
};
typedef SMARTP<lpsr2lilypondOah2ManPageGenerator> S_lpsr2lilypondOah2ManPageGenerator;


}


#endif
