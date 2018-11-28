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
        logIOstream)
{
  fBsrOptions = bsrOpts;
  
  // the BSR score we're visiting
  fVisitedBsrScore = bsrScore;

  switch (gBrailleOptions->fUTFKind) {
    case kUTF8:
      fBrailleGenerator =
        bsrUTF8BrailleGenerator::create (
          gBrailleOptions->fByteOrderingKind,
          brailleOutputStream);
      break;
      
    case kUTF16:
      switch (gBrailleOptions->fByteOrderingKind) {
        case kByteOrderingNone:
          break;
          
        case kByteOrderingBigEndian:
          fBrailleGenerator =
            bsrUTF16BigEndianBrailleGenerator::create (
              gBrailleOptions->fByteOrderingKind,
              brailleOutputStream);
          break;
          
        case kByteOrderingSmallEndian:
          break;
          fBrailleGenerator =
            bsrUTF16SmallEndianBrailleGenerator::create (
              gBrailleOptions->fByteOrderingKind,
              brailleOutputStream);
      } // switch
      break;
  } // switch
};
  
bsr2BrailleTranslator::~bsr2BrailleTranslator ()
{}

//________________________________________________________________________
void bsr2BrailleTranslator::generateBrailleCodeFromBsrScore ()
{
  if (fVisitedBsrScore) {
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
void bsr2BrailleTranslator::visitStart (S_bsrSpaces& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleGenerator->generateCodeForCellsList (
    elt->getSpacesCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrSpaces& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrSpaces" <<
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

  fBrailleGenerator->generateCodeForCellsList (
    elt->getTranscriptionNotesElementCellsList ());
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

  fBrailleGenerator->generateCodeForBrailleCell (
    kCellEOP);
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

  fBrailleGenerator->generateCodeForCellsList (
    elt->getPageHeadingCellsList ());
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

  fBrailleGenerator->generateCodeForBrailleCell (
    kCellEOL);
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrMeasure& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrMeasure& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrMeasure '" <<
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
    fLogOutputStream <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleGenerator->generateCodeForCellsList (
    elt->getNumberCellsList ());
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

  fBrailleGenerator->generateCodeForCellsList (
    elt->getClefCellsList ());
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

  fBrailleGenerator->generateCodeForCellsList (
    elt->getKeyCellsList ());
    /*
  bsrKey::bsrKeyKind
    keyKind = elt->getKeyKind ();
    
  int
    numberOfAlterations =
      elt->getNumberOfAlterations ();

  bsrCellKind alteration = kCellNatural;
   
  switch (keyKind) {
    case bsrKey::kKeyKindFlats:
      alteration = kCellFlat;
      break;
    case bsrKey::kKeyKindNaturals:
      alteration = kCellNatural;
      break;
    case bsrKey::kKeyKindSharps:
      alteration = kCellSharp;
      break;
  } // switch

  switch (numberOfAlterations) {
    case 0:
      break;
    case 1:
      fBrailleGenerator->generateCodeForBrailleCell (
        alteration);
      break;
    case 2:
      fBrailleGenerator->generateCodeForBrailleCell (
        alteration);
      fBrailleGenerator->generateCodeForBrailleCell (
        alteration);
      break;
    case 3:
      fBrailleGenerator->generateCodeForBrailleCell (
        alteration);
      fBrailleGenerator->generateCodeForBrailleCell (
        alteration);
      fBrailleGenerator->generateCodeForBrailleCell (
        alteration);
      break;
    default:
      fBrailleGenerator->generateCodeForBrailleCell (
        alteration);
      fBrailleCodeIOstream <<
        kCellNumberSign <<
        braille (numberOfAlterations);
  } // switch
  */
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

  fBrailleGenerator->generateCodeForCellsList (
    elt->getTimeCellsList ());
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

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrNote& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleGenerator->generateCodeForCellsList (
    elt->getNoteCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrNote& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}


} // namespace
