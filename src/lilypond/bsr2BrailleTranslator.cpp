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
#include "brailleOptions.h"

#include "xml2brlOptionsHandling.h"

#include "bsr2BrailleTranslator.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
bsr2BrailleTranslator::bsr2BrailleTranslator (
  S_bsrOptions&    bsrOpts,
  indentedOstream& logIOstream,
  ostream&         brailleOutputStream,
  S_bsrScore       bsrScore)
    : fLogOutputStream (
        logIOstream),
      fBrailleCodeIOstream (
        brailleOutputStream)
{
  fBsrOptions = bsrOpts;
  
  // the BSR score we're visiting
  fVisitedBsrScore = bsrScore;
};
  
bsr2BrailleTranslator::~bsr2BrailleTranslator ()
{}

//________________________________________________________________________
void bsr2BrailleTranslator::generateBrailleCodeFromBsrScore ()
{
  if (fVisitedBsrScore) {    
    // generate a BOM if requested
    switch (gBrailleOptions->fBsrByteOrderingKind) {
      case brailleOptions::kByteOrderingBigEndian:
        fBrailleCodeIOstream <<
          kBOMBigEndian;
        break;
      case brailleOptions::kByteOrderingSmallEndian:
        fBrailleCodeIOstream <<
          kBOMSmallEndian;
        break;
    } // switch

    if (true) { // JMI TEMP
      // generate the table of all Dots 6 cells
      for (wchar_t wch = L'\u2800'; wch <= L'\u280f'; wch++) {
        fBrailleCodeIOstream << wch;
      } // for
      fBrailleCodeIOstream << kBrailleEOL;
      for (wchar_t wch = L'\u2810'; wch <= L'\u281f'; wch++) {
        fBrailleCodeIOstream << wch;
      } // for
      fBrailleCodeIOstream << kBrailleEOL;
      fBrailleCodeIOstream << kBrailleEOP;
      
      for (wchar_t wch = L'\u2820'; wch <= L'\u282f'; wch++) {
        fBrailleCodeIOstream << wch;
      } // for
      fBrailleCodeIOstream << kBrailleEOL;
      for (wchar_t wch = L'\u2830'; wch <= L'\u283f'; wch++) {
        fBrailleCodeIOstream << wch;
      } // for
      fBrailleCodeIOstream << kBrailleEOL;
      fBrailleCodeIOstream << kBrailleEOP;
    }

    // browse a bsrScore browser
    bsrBrowser<bsrScore> browser (this);
    browser.browse (*fVisitedBsrScore);
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrScore& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrScore& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrTranscriptionNotes& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrTranscriptionNotes& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrTranscriptionNotesElement& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrTranscriptionNotesElement& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPage& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrPage& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    kBrailleEOP;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPageHeading& elt)
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

void bsr2BrailleTranslator::visitEnd (S_bsrPageHeading& elt)
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
void bsr2BrailleTranslator::visitStart (S_bsrLine& elt)
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

void bsr2BrailleTranslator::visitEnd (S_bsrLine& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    kBrailleEOL;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrNumber& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  switch (elt->getNumberSignIsNeededKind ()) {
    case bsrNumber::kNumberSignIsNeededYes:
      fBrailleCodeIOstream <<
        kBrlNumberSign;
      break;
    case bsrNumber::kNumberSignIsNeededNo:
      break;
  } // switch

  fBrailleCodeIOstream <<
    braille (elt->getNumberValue ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrNumber& elt)
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
void bsr2BrailleTranslator::visitStart (S_bsrClef& elt)
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

void bsr2BrailleTranslator::visitEnd (S_bsrClef& elt)
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
void bsr2BrailleTranslator::visitStart (S_bsrKey& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  bsrKey::bsrKeyKind
    keyKind = elt->getKeyKind ();
    
  int
    numberOfAlterations =
      elt->getNumberOfAlterations ();

  wchar_t alteration = kBrlNatural;
   
  switch (keyKind) {
    case bsrKey::kKeyKindFlats:
      alteration = kBrlFlat;
      break;
    case bsrKey::kKeyKindSharps:
      alteration = kBrlSharp;
      break;
  } // switch

  switch (numberOfAlterations) {
    case 0:
      break;
    case 1:
      fBrailleCodeIOstream <<
        alteration;
      break;
    case 2:
      fBrailleCodeIOstream <<
        alteration <<
        alteration;
      break;
    case 3:
      fBrailleCodeIOstream <<
        alteration <<
        alteration <<
        alteration;
      break;
    default:
      fBrailleCodeIOstream <<
        kBrlNumberSign <<
        braille (numberOfAlterations);    
  } // switch
}

void bsr2BrailleTranslator::visitEnd (S_bsrKey& elt)
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
void bsr2BrailleTranslator::visitStart (S_bsrTime& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrTime& elt)
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
