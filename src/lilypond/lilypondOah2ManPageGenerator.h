/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lilypondOah2ManPageGenerator___
#define ___lilypondOah2ManPageGenerator___

#include "lilypondOah.h"

#include "oah2ManPageGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class lilypondOah2ManPageGenerator : virtual public oah2ManPageGenerator,

  public visitor<S_lilypondScoreOutputKindAtom>,

  public visitor<S_lilypondAbsoluteOctaveEntryAtom>,

  public visitor<S_lilypondRelativeOctaveEntryAtom>,

  public visitor<S_lilypondFixedOctaveEntryAtom>,

  public visitor<S_lilypondResetMeasureNumberAtom>,

  public visitor<S_lilypondAccidentalStyleKindAtom>,

  public visitor<S_lilypondChordsDisplayAtom>,

  public visitor<S_lilypondLyricsAlignmentKindAtom>,

  public visitor<S_lilypondMidiTempoAtom>

{
  public:

    lilypondOah2ManPageGenerator (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

    virtual ~lilypondOah2ManPageGenerator ();

  protected:

    virtual void visitStart (S_lilypondScoreOutputKindAtom& elt);
    virtual void visitEnd   (S_lilypondScoreOutputKindAtom& elt);

    virtual void visitStart (S_lilypondAbsoluteOctaveEntryAtom& elt);
    virtual void visitEnd   (S_lilypondAbsoluteOctaveEntryAtom& elt);

    virtual void visitStart (S_lilypondRelativeOctaveEntryAtom& elt);
    virtual void visitEnd   (S_lilypondRelativeOctaveEntryAtom& elt);

    virtual void visitStart (S_lilypondFixedOctaveEntryAtom& elt);
    virtual void visitEnd   (S_lilypondFixedOctaveEntryAtom& elt);

    virtual void visitStart (S_lilypondResetMeasureNumberAtom& elt);
    virtual void visitEnd   (S_lilypondResetMeasureNumberAtom& elt);

    virtual void visitStart (S_lilypondAccidentalStyleKindAtom& elt);
    virtual void visitEnd   (S_lilypondAccidentalStyleKindAtom& elt);

    virtual void visitStart (S_lilypondChordsDisplayAtom& elt);
    virtual void visitEnd   (S_lilypondChordsDisplayAtom& elt);

    virtual void visitStart (S_lilypondLyricsAlignmentKindAtom& elt);
    virtual void visitEnd   (S_lilypondLyricsAlignmentKindAtom& elt);

    virtual void visitStart (S_lilypondMidiTempoAtom& elt);
    virtual void visitEnd   (S_lilypondMidiTempoAtom& elt);
};
typedef SMARTP<lilypondOah2ManPageGenerator> S_lilypondOah2ManPageGenerator;


}


#endif
