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

#include "messagesHandling.h"

#include "bsr2BrailleTranslator.h"

#include "oahOah.h"

#include "mxmlTreeOah.h"
#include "bsrOah.h"
#include "bsr2BrailleOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
bsr2BrailleTranslator::bsr2BrailleTranslator (
  S_bsrScore bsrScore,
  S_bsrOah&  bsrOpts,
  ostream&   logOstream,
  ostream&   brailleCodeOutputStream)
#ifdef TRACE_OAH
    : fLogOutputStream (logOstream)
#endif
{
  fBsrOah = bsrOpts;

  // the BSR score we're visiting
  fVisitedBsrScore = bsrScore;

  // set generation to ASCII braille by default
  // should not be necessary JMI ???
  fBrailleGenerator =
    bsrAsciiBrailleGenerator::create (
      brailleCodeOutputStream);

  switch (gGlobalBsr2BrailleOah->fBrailleOutputKind) {
    case kBrailleOutputAscii:
      fBrailleGenerator =
        bsrAsciiBrailleGenerator::create (
          brailleCodeOutputStream);
      break;

    case kBrailleOutputUTF8:
      fBrailleGenerator =
        bsrUTF8BrailleGenerator::create (
          gGlobalBsr2BrailleOah->fByteOrderingKind,
          brailleCodeOutputStream);
      break;

    case kBrailleOutputUTF8Debug:
      fBrailleGenerator =
        bsrUTF8DebugBrailleGenerator::create (
          gGlobalBsr2BrailleOah->fByteOrderingKind,
          brailleCodeOutputStream);
      break;

    case kBrailleOutputUTF16:
      switch (gGlobalBsr2BrailleOah->fByteOrderingKind) {
        case kByteOrderingNone:
          {
            stringstream s;

            s << "bsr2BrailleTranslator: " ;

            string message = s.str ();

            bsrInternalError (
              gGlobalOahOah->fInputSourceName,
              1, // JMI ???
              __FILE__, __LINE__,
              message);

            throw msrScoreToBsrScoreException (message);
          }
          break;

        case kByteOrderingBigEndian:
          fBrailleGenerator =
            bsrUTF16BigEndianBrailleGenerator::create (
              gGlobalBsr2BrailleOah->fByteOrderingKind,
              brailleCodeOutputStream);
          break;

        case kByteOrderingSmallEndian:
          fBrailleGenerator =
            bsrUTF16SmallEndianBrailleGenerator::create (
              gGlobalBsr2BrailleOah->fByteOrderingKind,
              brailleCodeOutputStream);
          break;
      } // switch
      break;
  } // switch

/* JMI
  switch (gGlobalBsr2BrailleOah->fUTFKind) {
    case kUTF8:
      fBrailleGenerator =
        bsrUTF8BrailleGenerator::create (
          gGlobalBsr2BrailleOah->fByteOrderingKind,
          brailleCodeOutputStream);
      break;

    case kUTF16:
      switch (gGlobalBsr2BrailleOah->fByteOrderingKind) {
        case kByteOrderingNone:
          break;

        case kByteOrderingBigEndian:
          fBrailleGenerator =
            bsrUTF16BigEndianBrailleGenerator::create (
              gGlobalBsr2BrailleOah->fByteOrderingKind,
              brailleCodeOutputStream);
          break;

        case kByteOrderingSmallEndian:
          break;
          fBrailleGenerator =
            bsrUTF16SmallEndianBrailleGenerator::create (
              gGlobalBsr2BrailleOah->fByteOrderingKind,
              brailleCodeOutputStream);
      } // switch
      break;
  } // switch
  */
}

bsr2BrailleTranslator::~bsr2BrailleTranslator ()
{}

//________________________________________________________________________
void bsr2BrailleTranslator::generateBrailleCodeFromBsrScore ()
{
  if (fVisitedBsrScore) {
    // browse the visited bsrScore
    bsrBrowser<bsrScore> browser (this);
    browser.browse (*fVisitedBsrScore);
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrScore& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2BrailleTranslator::visitEnd (S_bsrScore& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrSpaces& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/* JMI
  fLogOutputStream <<
    "% --> bsrSpaces" <<
    ", numberOfSpaces: " << elt->getNumberOfSpaces () <<
    ", spacesCellsList: " << elt->fetchCellsList () <<
    ", line " << elt->getInputLineNumber () <<
    endl;
*/

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrSpaces& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrBarline& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrTranscriptionNotes& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2BrailleTranslator::visitEnd (S_bsrTranscriptionNotes& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrTranscriptionNotesElement& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrTranscriptionNotesElement& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPage& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2BrailleTranslator::visitEnd (S_bsrPage& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForBrailleCell (
      kCellEOP);
}

void bsr2BrailleTranslator::visitStart (S_bsrPageElement& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrPageElement '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2BrailleTranslator::visitEnd (S_bsrPageElement& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrPageElement '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPagination& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrPagination '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrPagination& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrPagination '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPageHeading& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrPageHeading& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrMusicHeading& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForMusicHeading (elt);

/* JMI
  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
    */

/*
  for (int i = 0; i < 6; i++) {   // JMI ???
    if (true) {
      fLogOutputStream <<
        "Generating a space before S_bsrMusicHeading" <<
        endl;
    }

    fBrailleGenerator->generateCodeForBrailleCell (
      kCellSpace);
//      kCellA);
  } // for
  */
}

void bsr2BrailleTranslator::visitEnd (S_bsrMusicHeading& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (false) { // JMI
    fLogOutputStream <<
      "Generating an EOL after S_bsrMusicHeading" <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForBrailleCell (
    kCellEOL);
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrFootNotes& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/* JMI
  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
    */
}

void bsr2BrailleTranslator::visitEnd (S_bsrFootNotes& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrLine& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2BrailleTranslator::visitEnd (S_bsrLine& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
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
void bsr2BrailleTranslator::visitStart (S_bsrLineContents& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForLineContents (elt);
}

void bsr2BrailleTranslator::visitEnd (S_bsrLineContents& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (false) { // JMI
    fLogOutputStream <<
      "Generating an EOL after S_bsrLineContents" <<
      endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForBrailleCell (
      kCellEOL);
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrMeasure& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2BrailleTranslator::visitEnd (S_bsrMeasure& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
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
void bsr2BrailleTranslator::visitStart (S_bsrNumber& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrNumber& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
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
void bsr2BrailleTranslator::visitStart (S_bsrWords& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrWords '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrWords& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
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
void bsr2BrailleTranslator::visitStart (S_bsrClef& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrClef& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
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
void bsr2BrailleTranslator::visitStart (S_bsrKey& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrKey& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
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
void bsr2BrailleTranslator::visitStart (S_bsrTime& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

// JMI  fBrailleGenerator->generateCodeForCellsList (
 //   elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrTime& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrTempo& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrTempo& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrNote& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> Start visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

// JMI  fBrailleGenerator->generateCodeForCellsList (
 //   elt->fetchCellsList ());
}

void bsr2BrailleTranslator::visitEnd (S_bsrNote& elt)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    fLogOutputStream <<
      "% --> End visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}


} // namespace
