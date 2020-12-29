/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrFiguredBasses_MUT_DEP.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrFigure msrFigure::create (
  int                 inputLineNumber,
  S_msrPart           figurePartUpLink,
  msrFigurePrefixKind figurePrefixKind,
  int                 figureNumber,
  msrFigureSuffixKind figureSuffixKind)
{
  msrFigure* o =
    new msrFigure (
      inputLineNumber,
      figurePartUpLink,
      figurePrefixKind,
      figureNumber,
      figureSuffixKind);
  assert (o!=0);

  return o;
}

msrFigure::msrFigure (
  int                 inputLineNumber,
  S_msrPart           figurePartUpLink,
  msrFigurePrefixKind figurePrefixKind,
  int                 figureNumber,
  msrFigureSuffixKind figureSuffixKind)
    : msrElement (inputLineNumber)
{
  // sanity check
  msgAssert(
    figurePartUpLink != nullptr,
    "figurePartUpLink is null");

  // set figured's part upLink
  fFigurePartUpLink =
    figurePartUpLink;

  fFigurePrefixKind = figurePrefixKind;
  fFigureNumber     = figureNumber;
  fFigureSuffixKind = figureSuffixKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrFigure::~msrFigure ()
{}

S_msrFigure msrFigure::createFigureNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating a newborn clone of figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert(
    containingPart != nullptr,
    "containingPart is null");

  S_msrFigure
    newbornClone =
      msrFigure::create (
        fInputLineNumber,
        containingPart,
        fFigurePrefixKind,
        fFigureNumber,
        fFigureSuffixKind);

  return newbornClone;
}

S_msrFigure msrFigure::createFigureDeepCopy (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating a deep copy of figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert(
    containingPart != nullptr,
    "containingPart is null");

  S_msrFigure
    figureDeepCopy =
      msrFigure::create (
        fInputLineNumber,
        containingPart,
        fFigurePrefixKind,
        fFigureNumber,
        fFigureSuffixKind);

  return figureDeepCopy;
}

string msrFigure::figurePrefixKindAsString (
  msrFigurePrefixKind figurePrefixKind)
{
  string result;

  switch (figurePrefixKind) {
    case msrFigure::k_NoFigurePrefix:
      result = "none";
      break;
    case msrFigure::kDoubleFlatPrefix:
      result = "double flat";
      break;
    case msrFigure::kFlatPrefix:
      result = "flat";
      break;
    case msrFigure::kFlatFlatPrefix:
      result = "flat flat";
      break;
    case msrFigure::kNaturalPrefix:
      result = "natural";
      break;
    case msrFigure::kSharpSharpPrefix:
      result = "sharp sharp";
      break;
    case msrFigure::kSharpPrefix:
      result = "sharp";
      break;
    case msrFigure::kDoubleSharpPrefix:
      result = "souble sharp";
      break;
  } // switch

  return result;
}

string msrFigure::figureSuffixKindAsString (
  msrFigureSuffixKind figureSuffixKind)
{
  string result;

  switch (figureSuffixKind) {
    case msrFigure::k_NoFigureSuffix:
      result = "none";
      break;
    case msrFigure::kDoubleFlatSuffix:
      result = "double flat";
      break;
    case msrFigure::kFlatSuffix:
      result = "flat";
      break;
    case msrFigure::kFlatFlatSuffix:
      result = "flat flat";
      break;
    case msrFigure::kNaturalSuffix:
      result = "natural";
      break;
    case msrFigure::kSharpSharpSuffix:
      result = "sharp sharp";
      break;
    case msrFigure::kSharpSuffix:
      result = "sharp";
      break;
    case msrFigure::kDoubleSharpSuffix:
      result = "souble sharp";
      break;
    case msrFigure::kSlashSuffix:
      result = "slash";
      break;
  } // switch

  return result;
}

string msrFigure::asString () const
{
  stringstream s;

  s <<
    "Figure" <<
      "'" << fFigureNumber <<
    "', prefix: " <<
    figurePrefixKindAsString (
      fFigurePrefixKind) <<
    ", suffix: " <<
    figureSuffixKindAsString (
      fFigureSuffixKind);

/* JMI
  if (fFigurePartUpLink) { // JMI ???
    s <<
      ":" <<
      wholeNotesAsMsrString (
        fInputLineNumber,
        fFigureSoundingWholeNotes);
  }
*/

  return s.str ();
}

void msrFigure::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFigure::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFigure::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFigure::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFigure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFigure::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFigure::browseData (basevisitor* v)
{}

void msrFigure::print (ostream& os) const
{
  os <<
    "Figure" <<
    " '" << fFigureNumber <<
    "', prefix: " <<
    figurePrefixKindAsString (
      fFigurePrefixKind) <<
    ", suffix: " <<
    figureSuffixKindAsString (
      fFigureSuffixKind) <<
     ", line " << fInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrFigure& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrFiguredBass msrFiguredBass::create (
  // no figuredBassVoiceUpLink yet
  int       inputLineNumber) // ,
// JMI  S_msrPart figuredBassPartUpLink)
{
  msrFiguredBass* o =
    new msrFiguredBass (
      inputLineNumber,
//    figuredBassPartUpLink,
      rational (0, 1),           // figuredBassSoundingWholeNotes
      rational (0, 1),           // figuredBassDisplayWholeNotes
      kFiguredBassParenthesesNo,
      msrTupletFactor (1, 1));
  assert (o!=0);

  return o;
}

S_msrFiguredBass msrFiguredBass::create (
  int                           inputLineNumber,
//   S_msrPart                     figuredBassPartUpLink,
  rational                      figuredBassSoundingWholeNotes,
  rational                      figuredBassDisplayWholeNotes,
  msrFiguredBassParenthesesKind figuredBassParenthesesKind,
  msrTupletFactor               figuredBassTupletFactor)
{
  msrFiguredBass* o =
    new msrFiguredBass (
      inputLineNumber,
 //     figuredBassPartUpLink,
      figuredBassSoundingWholeNotes,
      figuredBassDisplayWholeNotes,
      figuredBassParenthesesKind,
      figuredBassTupletFactor);
  assert (o!=0);

  return o;
}

msrFiguredBass::msrFiguredBass (
  int                           inputLineNumber,
//   S_msrPart                     figuredBassPartUpLink,
  rational                      figuredBassSoundingWholeNotes,
  rational                      figuredBassDisplayWholeNotes,
  msrFiguredBassParenthesesKind figuredBassParenthesesKind,
  msrTupletFactor               figuredBassTupletFactor)
    : msrMeasureElement (inputLineNumber),
      fFiguredBassTupletFactor (figuredBassTupletFactor)
{
  /* JMI
  // sanity check
  msgAssert(
    figuredBassPartUpLink != nullptr,
    "figuredBassPartUpLink is null");

  // set figuredBass's part upLink
  fFiguredBassPartUpLink =
    figuredBassPartUpLink;
    */

  fMeasureElementSoundingWholeNotes =
    figuredBassSoundingWholeNotes;
  fFiguredBassDisplayWholeNotes =
    figuredBassDisplayWholeNotes;

  fFiguredBassParenthesesKind =
    figuredBassParenthesesKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating figuredBass " <<
      asString () <<
      endl;
  }
#endif
}

msrFiguredBass::~msrFiguredBass ()
{}

S_msrFiguredBass msrFiguredBass::createFiguredBassNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating a newborn clone of figured bass '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrFiguredBass
    newbornClone =
      msrFiguredBass::create (
        fInputLineNumber,
 //       containingPart,
// JMI        fFiguredBassSoundingWholeNotes,
        fMeasureElementSoundingWholeNotes,
        fFiguredBassDisplayWholeNotes,
        fFiguredBassParenthesesKind,
        fFiguredBassTupletFactor);

  return newbornClone;
}

S_msrFiguredBass msrFiguredBass::createFiguredBassDeepCopy ()
 // S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Creating a deep copy of figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

/* JMi
  // sanity check
  msgAssert(
    containingPart != nullptr,
    "containingPart is null");
    */

  S_msrFiguredBass
    figuredBassDeepCopy =
      msrFiguredBass::create (
        fInputLineNumber,
   //     containingPart,
// JMI        fFiguredBassSoundingWholeNotes,
        fMeasureElementSoundingWholeNotes,
        fFiguredBassDisplayWholeNotes,
        fFiguredBassParenthesesKind,
        fFiguredBassTupletFactor);

  return figuredBassDeepCopy;
}

void msrFiguredBass::setFiguredBassPositionInMeasure (
  rational positionInMeasure)
{
  // set the figured bass position in measure

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting figured bass position in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      endl;
  }
#endif

  string context =
    "setFiguredBassPositionInMeasure()";

  setMeasureElementPositionInMeasure (
    positionInMeasure,
    context);

  // compute figured bass's position in voice
  S_msrMeasure
    measure =
      fFiguredBassNoteUpLink->
        getNoteMeasureUpLink ();

  if (false) { // JMI
  rational
    positionInVoice =
      measure->
        getMeasurePositionInVoice ()
        +
      positionInMeasure;
  positionInVoice.rationalise ();

  // set figured bass's position in voice
  setMeasureElementPositionInVoice (
    positionInVoice,
    context);
}

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureVoiceUpLink ();

  voice->
    incrementCurrentPositionInVoice (
      fFiguredBassNoteUpLink->getNoteSoundingWholeNotes ());
}

void msrFiguredBass::appendFigureToFiguredBass (
  S_msrFigure figure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Appending figure '" << figure->asString () <<
      "' to figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fFiguredBassFiguresList.push_back (figure);
}

void msrFiguredBass::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFiguredBass::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFiguredBass::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFiguredBass::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrFiguredBass::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrFiguredBass::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFiguredBass::browseData (basevisitor* v)
{
  for (
    list<S_msrFigure>::const_iterator i = fFiguredBassFiguresList.begin ();
    i != fFiguredBassFiguresList.end ();
    i++
  ) {
    // browse the figure
    msrBrowser<msrFigure> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrFiguredBass::figuredBassParenthesesKindAsString (
  msrFiguredBassParenthesesKind figuredBassParenthesesKind)
{
  string result;

  switch (figuredBassParenthesesKind) {
    case msrFiguredBass::kFiguredBassParenthesesYes:
      result = "figuredBassParenthesesYes";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      result = "figuredBassParenthesesNo";
      break;
  } // switch

  return result;
}

string msrFiguredBass::asString () const
{
  stringstream s;

  s <<
    "[Figured bass" <<
    ", measureElementSoundingWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureElementSoundingWholeNotes) <<
    ", figuredBassDisplayWholeNotes: " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFiguredBassDisplayWholeNotes) <<

    ", figuredBassParenthesesKind: " <<
    figuredBassParenthesesKindAsString (
      fFiguredBassParenthesesKind) <<

    ", figuredBassTupletFactor: " <<
    fFiguredBassTupletFactor.asString ();

  if (fFiguredBassFiguresList.size ()) {
    s << ", figuredBassFiguresList: [";

    list<S_msrFigure>::const_iterator
      iBegin = fFiguredBassFiguresList.begin (),
      iEnd   = fFiguredBassFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << " ";
    } // for
  s << "]";
  }

  // print the figured bass position in measure
  s <<
    ", positionInMeasure: " << fMeasureElementPositionInMeasure;

/* JMI
  if (fFiguredBassPartUpLink) { // JMI ???
    s <<
      ":" <<
      wholeNotesAsMsrString (
        fInputLineNumber,
        fFiguredBassSoundingWholeNotes);
  }
*/

  // print the figured bass position in voice
  s <<
    ", positionInVoice: " << fMeasureElementPositionInVoice;

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrFiguredBass::print (ostream& os) const
{
  os <<
    "FiguredBass" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 32;

  os <<
    setw (fieldWidth) <<
    "figuredBassNoteUpLink" << " : ";
  if (fFiguredBassNoteUpLink) {
    os << fFiguredBassNoteUpLink->asString ();
  }
  else {
    os << "none";
  }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "figuredBassVoiceUpLink" << " : ";
  if (fFiguredBassVoiceUpLink) {
    os << fFiguredBassVoiceUpLink->asString ();
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "measureElementSoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "figuredBassDisplayWholeNotes" << " : " <<
    fFiguredBassDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "figuredBassParenthesesKind" << " : " <<
    figuredBassParenthesesKindAsString (
      fFiguredBassParenthesesKind) <<
    endl <<

    setw (fieldWidth) <<
    "figuredBassTupletFactor" << " : " <<
    fFiguredBassTupletFactor.asString () <<
    endl;

  if (fFiguredBassFiguresList.size ()) {
    gIndenter++;

    list<S_msrFigure>::const_iterator
      iBegin = fFiguredBassFiguresList.begin (),
      iEnd   = fFiguredBassFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    gIndenter--;
  }

  // print the figured bass position in measure
  os <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " << fMeasureElementPositionInMeasure <<
    endl;

  // print the figured bass position in voice
  os <<
    setw (fieldWidth) <<
    "positionInVoice" << " : " << fMeasureElementPositionInVoice <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrFiguredBass& elt)
{
  elt->print (os);
  return os;
}


}
