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

#include "bsr2brailleTranslator.h"

#include "oahOah.h"

#include "mxmlTreeOah.h"
#include "bsrOah.h"
#include "bsr2brailleOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
bsr2brailleTranslator::bsr2brailleTranslator (
  S_bsrScore     bsrScore,
  S_bsrOahGroup& bsrOpts,
  ostream&       brailleOutputStream)
  : fBrailleOutputStream (
      brailleOutputStream)
{
  fBsrOahGroup = bsrOpts;

  // the BSR score we're visiting
  fVisitedBsrScore = bsrScore;

#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsr ()) {
    gLogStream <<
      "gGlobalBsr2brailleOahGroup->getBrailleOutputKind (): " <<
      bsrBrailleOutputKindAsString (
        gGlobalBsr2brailleOahGroup->getBrailleOutputKind ()) <<
      ", gGlobalBsr2brailleOahGroup->getByteOrderingKind (): " <<
      bsrByteOrderingKindAsString (
        gGlobalBsr2brailleOahGroup->getByteOrderingKind ()) <<
      endl;
  }
#endif

  switch (gGlobalBsr2brailleOahGroup->getBrailleOutputKind ()) {
    case kBrailleOutputAscii:
      fBrailleGenerator =
        bsrAsciiBrailleGenerator::create (
          fBrailleOutputStream);
      break;

    case kBrailleOutputUTF8:
      fBrailleGenerator =
        bsrUTF8BrailleGenerator::create (
          gGlobalBsr2brailleOahGroup->getByteOrderingKind (),
          fBrailleOutputStream);
      break;

    case kBrailleOutputUTF8Debug:
      fBrailleGenerator =
        bsrUTF8DebugBrailleGenerator::create (
          gGlobalBsr2brailleOahGroup->getByteOrderingKind (),
          fBrailleOutputStream);
      break;

    case kBrailleOutputUTF16:
      switch (gGlobalBsr2brailleOahGroup->getByteOrderingKind ()) {
        case kByteOrderingNone:
          {
            stringstream s;

            s << "bsr2brailleTranslator: " ;

            string message = s.str ();

            bsrInternalError (
              gGlobalOahOahGroup->getInputSourceName (),
              1, // JMI ???
              __FILE__, __LINE__,
              message);

            throw msrScoreToBsrScoreException (message);
          }
          break;

        case kByteOrderingBigEndian:
          fBrailleGenerator =
            bsrUTF16BigEndianBrailleGenerator::create (
              gGlobalBsr2brailleOahGroup->getByteOrderingKind (),
              fBrailleOutputStream);
          break;

        case kByteOrderingSmallEndian:
          fBrailleGenerator =
            bsrUTF16SmallEndianBrailleGenerator::create (
              gGlobalBsr2brailleOahGroup->getByteOrderingKind (),
              fBrailleOutputStream);
          break;
      } // switch
      break;
  } // switch

  msgAssert (
    fBrailleGenerator != nullptr,
    "fBrailleGenerator is null");
}

bsr2brailleTranslator::~bsr2brailleTranslator ()
{}

//________________________________________________________________________
void bsr2brailleTranslator::generateBrailleCodeFromBsrScore ()
{
  if (fVisitedBsrScore) {
    // browse the visited bsrScore
    bsrBrowser<bsrScore> browser (this);
    browser.browse (*fVisitedBsrScore);
  }
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrScore& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrSpaces& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/* JMI
  gLogStream <<
    "% --> bsrSpaces" <<
    ", numberOfSpaces: " << elt->getNumberOfSpaces () <<
    ", spacesCellsList: " << elt->fetchCellsList () <<
    ", line " << elt->getInputLineNumber () <<
    endl;
*/

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrSpaces& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrSpaces" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrBarline& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTranscriptionNotes& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrTranscriptionNotes& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting S_bsrTranscriptionNotes" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTranscriptionNotesElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrTranscriptionNotesElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrPage& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrPage '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrPage& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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

void bsr2brailleTranslator::visitStart (S_bsrPageElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrPageElement '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrPageElement& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrPageElement '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrPagination& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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

void bsr2brailleTranslator::visitEnd (S_bsrPagination& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrPagination '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrPageHeading& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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

void bsr2brailleTranslator::visitEnd (S_bsrPageHeading& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrPageHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrMusicHeading& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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
  for (int i = 0; i < 6; ++i) {   // JMI ???
    if (true) {
      gLogStream <<
        "Generating a space before S_bsrMusicHeading" <<
        endl;
    }

    fBrailleGenerator->
      generateCodeForBrailleCell (
        kCellSpace);
  //      kCellA);
  } // for
  */
}

void bsr2brailleTranslator::visitEnd (S_bsrMusicHeading& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrMusicHeading '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "Generating an EOL after S_bsrMusicHeading" <<
      endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForBrailleCell (
      kCellEOL);
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrFootNotes& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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

void bsr2brailleTranslator::visitEnd (S_bsrFootNotes& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrFootNotes '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrLine& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrLine& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrLine '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrLineContents& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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

void bsr2brailleTranslator::visitEnd (S_bsrLineContents& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrLineContents '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (false) { // JMI
    gLogStream <<
      "Generating an EOL after S_bsrLineContents" <<
      endl;
  }
#endif

  fBrailleGenerator->
    generateCodeForBrailleCell (
      kCellEOL);
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrMeasure& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void bsr2brailleTranslator::visitEnd (S_bsrMeasure& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrMeasure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrNumber& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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

void bsr2brailleTranslator::visitEnd (S_bsrNumber& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrNumber '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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

void bsr2brailleTranslator::visitEnd (S_bsrWords& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrWords '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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

void bsr2brailleTranslator::visitEnd (S_bsrClef& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrKey& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
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

void bsr2brailleTranslator::visitEnd (S_bsrKey& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrKey '" <<
      elt->asString () <<
      "'"  <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTime& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

// JMI  fBrailleGenerator->generateCodeForCellsList (
 //   elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrTime& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fBrailleGenerator->generateCodeForCellsList (
    elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrTempo& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrTempo " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void bsr2brailleTranslator::visitStart (S_bsrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> Start visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

// JMI  fBrailleGenerator->generateCodeForCellsList (
 //   elt->fetchCellsList ());
}

void bsr2brailleTranslator::visitEnd (S_bsrNote& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% --> End visiting bsrNote " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}


} // namespace

/* JMI
  switch (gGlobalBsr2brailleOahGroup->getUTFKind ()) {
    case kUTF8:
      fBrailleGenerator =
        bsrUTF8BrailleGenerator::create (
          gGlobalBsr2brailleOahGroup->getByteOrderingKind (),
          fBrailleOutputStream);
      break;

    case kUTF16:
      switch (gGlobalBsr2brailleOahGroup->getByteOrderingKind ()) {
        case kByteOrderingNone:
          break;

        case kByteOrderingBigEndian:
          fBrailleGenerator =
            bsrUTF16BigEndianBrailleGenerator::create (
              gGlobalBsr2brailleOahGroup->getByteOrderingKind (),
              fBrailleOutputStream);
          break;

        case kByteOrderingSmallEndian:
          break;
          fBrailleGenerator =
            bsrUTF16SmallEndianBrailleGenerator::create (
              gGlobalBsr2brailleOahGroup->getByteOrderingKind (),
              fBrailleOutputStream);
      } // switch
      break;
  } // switch
  */
