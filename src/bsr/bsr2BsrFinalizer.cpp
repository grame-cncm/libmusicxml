/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...
#include <cmath>
#include <string>

#include "bsr2BsrFinalizer.h"

#include "mxmlTreeOah.h"
#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
bsr2BsrFinalizer::bsr2BsrFinalizer (
  S_bsrOah&  bsrOpts,
  ostream&   logOstream,
  S_bsrScore bScore)
#ifdef TRACE_OAH
    : fLogOutputStream (logOstream)
#endif
{
  fBsrOah = bsrOpts;

  // the MSR score we're relying on
  fBaseMsrScore = bScore->getMsrScore ();

  // the BSR score we're visiting
  fVisitedBsrScore = bScore;
};

bsr2BsrFinalizer::~bsr2BsrFinalizer ()
{}

//________________________________________________________________________
void bsr2BsrFinalizer::generateFinalizedBsrScoreFromBsrScore ()
{
  if (fVisitedBsrScore) {
    // browse a bsrScore browser
    bsrBrowser<bsrScore> browser (this);
    browser.browse (*fVisitedBsrScore);
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrScore& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // create the BSR score
  fBsrScore =
    bsrScore::create (
      K_NO_INPUT_LINE_NUMBER,
      fBaseMsrScore);
}

void bsr2BsrFinalizer::visitEnd (S_bsrScore& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrSpaces& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
/* JMI
  fCurrentLine->
    appendSpacesToLine (elt);
    */
}

void bsr2BsrFinalizer::visitEnd (S_bsrSpaces& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrBarline& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentMeasure->
    appendBarlineToMeasure (elt);
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrTranscriptionNotes& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2BsrFinalizer::visitEnd (S_bsrTranscriptionNotes& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrTranscriptionNotesElement& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // JMI create the BSR score
  fBsrScore->
    appendTranscriptionNotesElementToScore (
      elt);
}

void bsr2BsrFinalizer::visitEnd (S_bsrTranscriptionNotesElement& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrPage& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentPage = elt->createPageNewbornClone ();

  fBsrScore->
    appendPageToScore (fCurrentPage);
}

void bsr2BsrFinalizer::visitEnd (S_bsrPage& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrPageHeading& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

 // JMI fCurrentPageHeading = elt->createPageHeadingNewbornClone ();

  fCurrentPageHeading = elt;

  fCurrentPage->
    appendPageHeadingToPage (fCurrentPageHeading);
}

void bsr2BsrFinalizer::visitEnd (S_bsrPageHeading& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentPageHeading = nullptr;
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrMusicHeading& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

 // JMI fCurrentPageHeading = elt->createPageHeadingNewbornClone ();

  fCurrentMusicHeading = elt;

  fCurrentPage->
    appendMusicHeadingToPage (fCurrentMusicHeading);
}

void bsr2BsrFinalizer::visitEnd (S_bsrMusicHeading& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentMusicHeading = nullptr;
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrFootNotes& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

 // JMI fCurrentPageHeading = elt->createPageHeadingNewbornClone ();

  fCurrentFootNotes = elt;

  fCurrentPage->
    appendFootNotesToPage (fCurrentFootNotes);
}

void bsr2BsrFinalizer::visitEnd (S_bsrFootNotes& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentFootNotes = nullptr;
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrLine& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentLine = elt->createLineNewbornClone ();

  fCurrentPage->
    appendLineToPage (fCurrentLine);
}

void bsr2BsrFinalizer::visitEnd (S_bsrLine& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrLineContents& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/* JMI ???
  fCurrentLine = elt->createLineNewbornClone ();

  fCurrentPage->
    appendLineToPage (fCurrentLine);
    */
}

void bsr2BsrFinalizer::visitEnd (S_bsrLineContents& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrMeasure& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentMeasure =
    elt->createMeasureNewbornClone ();

  fCurrentLine->
    appendMeasureToLine (fCurrentMeasure);
}

void bsr2BsrFinalizer::visitEnd (S_bsrMeasure& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrNumber& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentMeasure->
    appendNumberToMeasure (elt);
}

void bsr2BsrFinalizer::visitEnd (S_bsrNumber& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrWords& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrWords '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentMeasure->
    appendWordsToMeasure (elt);
}

void bsr2BsrFinalizer::visitEnd (S_bsrWords& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrWords '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrClef& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentMeasure->
    appendClefToMeasure (elt);
}

void bsr2BsrFinalizer::visitEnd (S_bsrClef& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrKey& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fCurrentMusicHeading) {
    fCurrentMusicHeading->
      setMusicHeadingKey (elt);
  }
  else {
    fCurrentLine->
      appendKeyToLine (elt);
  }
}

void bsr2BsrFinalizer::visitEnd (S_bsrKey& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrKey '" <<
      elt->asString () <<
      "'"  <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrTime& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fCurrentMusicHeading) {
    fCurrentMusicHeading->
      setMusicHeadingTime (elt);
  }
  else {
    fCurrentLine->
      appendTimeToLine (elt);
  }
}

void bsr2BsrFinalizer::visitEnd (S_bsrTime& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrTempo& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/* JMI ???
  if (fCurrentMusicHeading) {
    fCurrentMusicHeading->
      setMusicHeadingTempo (elt);
  }
  else {
  */
    fCurrentLine->
      appendTempoToLine (elt);
 // }
}

void bsr2BsrFinalizer::visitEnd (S_bsrTempo& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrNote& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentMeasure->
    appendNoteToMeasure (elt);
}

void bsr2BsrFinalizer::visitEnd (S_bsrNote& elt)
{
#ifdef TRACE_OAH
  if (globalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}


} // namespace
