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
#include "lilypondOptions.h"

#include "xml2brlOptionsHandling.h"

#include "bsr2BrailleTranslator.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
bsr2BrailleTranslator::bsr2BrailleTranslator (
  S_bsrOptions&    bsrOpts,
  indentedOstream& logIOstream,
  indentedOstream& lilypondOutputStream,
  S_bsrScore       bsrScore)
    : fLogOutputStream (
        logIOstream),
      fBrailleCodeIOstream (
        lilypondOutputStream)
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
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
  
  // initial empty line in LilyPond code
  // to help copy/paste it
// JMI  fBrailleCodeIOstream << endl;
}

void bsr2BrailleTranslator::visitEnd (S_bsrScore& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // final empty line in LilyPond code
  // to help copy/paste it
  fBrailleCodeIOstream <<
    endl;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPage& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
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
    fBrailleCodeIOstream <<
      "% --> End visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPageHeading& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
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
    fBrailleCodeIOstream <<
      "% --> End visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrNumber& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrNumber& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
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
    fBrailleCodeIOstream <<
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
    fBrailleCodeIOstream <<
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
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrKey& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
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
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrTime& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}


} // namespace
