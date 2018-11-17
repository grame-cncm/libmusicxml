/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iomanip>      // setw, setprecision, ...
#include <cmath>
#include <string>

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "musicXMLOptions.h"
#include "bsrOptions.h"

#include "xml2brlOptionsHandling.h"

#include "bsr2BsrFinalizer.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
bsr2BsrFinalizer::bsr2BsrFinalizer (
  S_bsrOptions&    bsrOpts,
  indentedOstream& logIOstream,
  S_bsrScore       bScore)
    : fLogOutputStream (
        logIOstream)
{
  fBsrOptions = bsrOpts;

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
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // create the BSR score
  fBsrScore =
    bsrScore::create (
      NO_INPUT_LINE_NUMBER,
      fBaseMsrScore);
}

void bsr2BsrFinalizer::visitEnd (S_bsrScore& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrTranscriptionNotes& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BsrFinalizer::visitEnd (S_bsrTranscriptionNotes& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrTranscriptionNotesElement& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // JMI create the BSR score
  fBsrScore->
    appendTranscriptionNotesElementToScore (
      elt);
}

void bsr2BsrFinalizer::visitEnd (S_bsrTranscriptionNotesElement& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrPage& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentPage = elt;
  
  fBsrScore->
    appendPageToScore (fCurrentPage);
}

void bsr2BsrFinalizer::visitEnd (S_bsrPage& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrPageHeading& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BsrFinalizer::visitEnd (S_bsrPageHeading& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrLine& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BsrFinalizer::visitEnd (S_bsrLine& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrNumber& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BsrFinalizer::visitEnd (S_bsrNumber& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrClef& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BsrFinalizer::visitEnd (S_bsrClef& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrKey& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BsrFinalizer::visitEnd (S_bsrKey& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrKey '" <<
      elt->asString () <<
      "'"  <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BsrFinalizer::visitStart (S_bsrTime& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BsrFinalizer::visitEnd (S_bsrTime& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}


} // namespace
