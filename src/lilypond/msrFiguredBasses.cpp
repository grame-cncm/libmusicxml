/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrMutualDependencies.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrFigure msrFigure::create (
  int                 inputLineNumber,
  S_msrPart           figurePartUplink,
  msrFigurePrefixKind figurePrefixKind,
  int                 figureNumber,
  msrFigureSuffixKind figureSuffixKind)
{
  msrFigure* o =
    new msrFigure (
      inputLineNumber,
      figurePartUplink,
      figurePrefixKind,
      figureNumber,
      figureSuffixKind);
  assert(o!=0);

  return o;
}

msrFigure::msrFigure (
  int                 inputLineNumber,
  S_msrPart           figurePartUplink,
  msrFigurePrefixKind figurePrefixKind,
  int                 figureNumber,
  msrFigureSuffixKind figureSuffixKind)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    figurePartUplink != nullptr,
    "figurePartUplink is null");

  // set figured's part uplink
  fFigurePartUplink =
    figurePartUplink;

  fFigurePrefixKind = figurePrefixKind;
  fFigureNumber     = figureNumber;
  fFigureSuffixKind = figureSuffixKind;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBasses) {
    gLogIOstream <<
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBasses) {
    gLogIOstream <<
      "Creating a newborn clone of figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
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
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBasses) {
    gLogIOstream <<
      "Creating a deep copy of figure '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
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
  if (fFigurePartUplink) { // JMI ???
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
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFigure::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFigure::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFigure::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFigure::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFigure>*
    p =
      dynamic_cast<visitor<S_msrFigure>*> (v)) {
        S_msrFigure elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFigure::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrFigure::browseData (basevisitor* v)
{}

void msrFigure::print (ostream& os)
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
  // no harmonyVoiceUplink yet
  int       inputLineNumber) // ,
// JMI  S_msrPart figuredBassPartUplink)
{
  msrFiguredBass* o =
    new msrFiguredBass (
      inputLineNumber,
  //    figuredBassPartUplink,
      rational (0, 1),    // figuredBassSoundingWholeNotes
      kFiguredBassParenthesesNo);
  assert(o!=0);

  return o;
}

S_msrFiguredBass msrFiguredBass::create (
  int       inputLineNumber,
 // S_msrPart figuredBassPartUplink,
  rational  figuredBassSoundingWholeNotes,
    msrFiguredBassParenthesesKind
              figuredBassParenthesesKind)
{
  msrFiguredBass* o =
    new msrFiguredBass (
      inputLineNumber,
 //     figuredBassPartUplink,
      figuredBassSoundingWholeNotes,
      figuredBassParenthesesKind);
  assert(o!=0);

  return o;
}

msrFiguredBass::msrFiguredBass (
  int       inputLineNumber,
 // S_msrPart figuredBassPartUplink,
  rational  figuredBassSoundingWholeNotes,
  msrFiguredBassParenthesesKind
            figuredBassParenthesesKind)
    : msrMeasureElement (inputLineNumber)
{
  /* JMI
  // sanity check
  msrAssert(
    figuredBassPartUplink != nullptr,
    "figuredBassPartUplink is null");
*/
     /* JMI
  // set figuredBass's part uplink
  fFiguredBassPartUplink =
    figuredBassPartUplink;
    */

  fFiguredBassSoundingWholeNotes =
    figuredBassSoundingWholeNotes;

  fFiguredBassParenthesesKind =
    figuredBassParenthesesKind;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBasses) {
    gLogIOstream <<
      "Creating figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrFiguredBass::~msrFiguredBass ()
{}

S_msrFiguredBass msrFiguredBass::createFiguredBassNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBasses) {
    gLogIOstream <<
      "Creating a newborn clone of figured bass '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrFiguredBass
    newbornClone =
      msrFiguredBass::create (
        fInputLineNumber,
 //       containingPart,
        fFiguredBassSoundingWholeNotes,
        fFiguredBassParenthesesKind);

  return newbornClone;
}

S_msrFiguredBass msrFiguredBass::createFiguredBassDeepCopy ()
 // S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBasses) {
    gLogIOstream <<
      "Creating a deep copy of figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

/* JMi
  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");
    */

  S_msrFiguredBass
    figuredBassDeepCopy =
      msrFiguredBass::create (
        fInputLineNumber,
   //     containingPart,
        fFiguredBassSoundingWholeNotes,
        fFiguredBassParenthesesKind);

  return figuredBassDeepCopy;
}

void msrFiguredBass::appendFiguredFigureToFiguredBass (
  S_msrFigure figure)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceFiguredBasses) {
    gLogIOstream <<
      "Appending figure'" << figure->asString () <<
      "' to figuredBass '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fFiguredBassFiguresList.push_back (figure);
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
    "Figured bass" <<
    ": " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFiguredBassSoundingWholeNotes) <<
    " sounding whole notes" <<
    ", " <<
    figuredBassParenthesesKindAsString (
      fFiguredBassParenthesesKind) <<
    ", line " << fInputLineNumber;

  if (fFiguredBassFiguresList.size ()) {
    s << ", ";

    list<S_msrFigure>::const_iterator
      iBegin = fFiguredBassFiguresList.begin (),
      iEnd   = fFiguredBassFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

/* JMI
  if (fFiguredBassPartUplink) { // JMI ???
    s <<
      ":" <<
      wholeNotesAsMsrString (
        fInputLineNumber,
        fFiguredBassSoundingWholeNotes);
  }
*/

  return s.str ();
}

void msrFiguredBass::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFiguredBass::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrFiguredBass::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrFiguredBass::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrFiguredBass::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrFiguredBass>*
    p =
      dynamic_cast<visitor<S_msrFiguredBass>*> (v)) {
        S_msrFiguredBass elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
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

void msrFiguredBass::print (ostream& os)
{
  os <<
    "FiguredBass" <<
    ": " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fFiguredBassSoundingWholeNotes) <<
    " sounding whole notes" <<
    ", " <<
    figuredBassParenthesesKindAsString (
      fFiguredBassParenthesesKind) <<
      ", line " << fInputLineNumber <<
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
}

ostream& operator<< (ostream& os, const S_msrFiguredBass& elt)
{
  elt->print (os);
  return os;
}


}
