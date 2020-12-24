/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsr2brailleTranslator___
#define ___bsr2brailleTranslator___

#include "bsr.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "bsrOah.h"

#include "bsr2brailleCodeGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class EXP bsr2brailleTranslator :

  public visitor<S_bsrScore>,

  public visitor<S_bsrSpaces>,

  public visitor<S_bsrBarline>,

  public visitor<S_bsrTranscriptionNotes>,
  public visitor<S_bsrTranscriptionNotesElement>,

  public visitor<S_bsrPage>,

  public visitor<S_bsrPagination>,

  public visitor<S_bsrPageHeading>,
  public visitor<S_bsrMusicHeading>,

  public visitor<S_bsrFootNotes>,

  public visitor<S_bsrLine>,
  public visitor<S_bsrLineContents>,

  public visitor<S_bsrMeasure>,

  public visitor<S_bsrNumber>,
  public visitor<S_bsrWords>,

  public visitor<S_bsrClef>,
  public visitor<S_bsrKey>,
  public visitor<S_bsrTime>,

  public visitor<S_bsrTempo>,

  public visitor<S_bsrNote>

{
  public:

    bsr2brailleTranslator (
      S_bsrScore     bsrScore,
      S_bsrOahGroup& bsrOpts,
      ostream&       brailleOutputStream);

    virtual ~bsr2brailleTranslator ();

    void generateBrailleCodeFromBsrScore ();

  protected:

    virtual void visitStart (S_bsrScore& elt);
    virtual void visitEnd   (S_bsrScore& elt);

    virtual void visitStart (S_bsrSpaces& elt);
    virtual void visitEnd   (S_bsrSpaces& elt);

    virtual void visitStart (S_bsrBarline& elt);

    virtual void visitStart (S_bsrTranscriptionNotes& elt);
    virtual void visitEnd   (S_bsrTranscriptionNotes& elt);

    virtual void visitStart (S_bsrTranscriptionNotesElement& elt);
    virtual void visitEnd   (S_bsrTranscriptionNotesElement& elt);

    virtual void visitStart (S_bsrPage& elt);
    virtual void visitEnd   (S_bsrPage& elt);
    virtual void visitStart (S_bsrPageElement& elt);
    virtual void visitEnd   (S_bsrPageElement& elt);

    virtual void visitStart (S_bsrPagination& elt);
    virtual void visitEnd   (S_bsrPagination& elt);

    virtual void visitStart (S_bsrPageHeading& elt);
    virtual void visitEnd   (S_bsrPageHeading& elt);
    virtual void visitStart (S_bsrMusicHeading& elt);
    virtual void visitEnd   (S_bsrMusicHeading& elt);

    virtual void visitStart (S_bsrFootNotes& elt);
    virtual void visitEnd   (S_bsrFootNotes& elt);

    virtual void visitStart (S_bsrLine& elt);
    virtual void visitEnd   (S_bsrLine& elt);
    virtual void visitStart (S_bsrLineContents& elt);
    virtual void visitEnd   (S_bsrLineContents& elt);

    virtual void visitStart (S_bsrMeasure& elt);
    virtual void visitEnd   (S_bsrMeasure& elt);

    virtual void visitStart (S_bsrNumber& elt);
    virtual void visitEnd   (S_bsrNumber& elt);

    virtual void visitStart (S_bsrWords& elt);
    virtual void visitEnd   (S_bsrWords& elt);

    virtual void visitStart (S_bsrClef& elt);
    virtual void visitEnd   (S_bsrClef& elt);

    virtual void visitStart (S_bsrKey& elt);
    virtual void visitEnd   (S_bsrKey& elt);

    virtual void visitStart (S_bsrTime& elt);
    virtual void visitEnd   (S_bsrTime& elt);

    virtual void visitStart (S_bsrTempo& elt);
    virtual void visitEnd   (S_bsrTempo& elt);

    virtual void visitStart (S_bsrNote& elt);
    virtual void visitEnd   (S_bsrNote& elt);

  private:

    S_bsrScore            fVisitedBsrScore;

    S_bsrOahGroup         fBsrOahGroup;

    S_bsrBrailleGenerator fBrailleGenerator;

    ostream&              fBrailleOutputStream;
};


}


#endif
