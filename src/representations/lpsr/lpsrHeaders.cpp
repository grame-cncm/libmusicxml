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

#include "utilities.h"

#include "lpsrHeaders.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "lpsrOah.h"
#include "lpsr2lilypondOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lpsrHeader lpsrHeader::create (
  int inputLineNumber)
{
  lpsrHeader* o =
    new lpsrHeader (
      inputLineNumber);
  assert (o!=0);
  return o;
}

lpsrHeader::lpsrHeader (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{}

lpsrHeader::~lpsrHeader ()
{}

unsigned int lpsrHeader::maxLilypondVariablesNamesLength ()
{
  unsigned int result = 0;

  // LilyPond informations

  if (fLilypondDedication.size () > result) {
      result = fLilypondDedication.size ();
  }

  if (fLilypondPiece.size () > result) {
      result = fLilypondPiece.size ();
  }

  if (fLilypondOpus.size () > result) {
      result = fLilypondOpus.size ();
  }

  if (fLilypondTitle.size () > result) {
      result = fLilypondTitle.size ();
  }

  if (fLilypondSubTitle.size () > result) {
      result = fLilypondSubTitle.size ();
  }

  if (fLilypondSubSubTitle.size () > result) {
      result = fLilypondSubSubTitle.size ();
  }

  if (fLilypondInstrument.size () > result) {
      result = fLilypondInstrument.size ();
  }

  if (fLilypondMeter.size () > result) {
      result = fLilypondMeter.size ();
  }

  if (fLilypondCopyright.size () > result) {
      result = fLilypondCopyright.size ();
  }

  if (fLilypondTagline.size () > result) {
      result = fLilypondTagline.size ();
  }

  return result;
}

void lpsrHeader::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrHeader::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrHeader::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrHeader::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrHeader::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrHeader::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrHeader::browseData (basevisitor* v)
{}

string lpsrHeader::asString () const
{
  stringstream s;

  s <<
    "[LpsrHeader" <<
    ", lilypondTitle: \" " <<  fLilypondTitle << "\"" <<
//    ", workNumber: \" " <<  fWorkNumber << "\"" <<
//    ", workTitle: \" " <<  fWorkTitle << "\"" <<
    ", lilypondOpus: \" " <<  fLilypondOpus << "\"" <<
//    ", movementNumber: \" " <<  fMovementNumber << "\"" <<
//    ", movementTitle: \" " <<  fMovementTitle << "\"" <<
    ", ..." <<
    "]";

  return s.str ();
}

void lpsrHeader::print (ostream& os) const
{
  os <<
    "Header" <<
    endl;

  bool emptyHeader = true;

  gIndenter++;

  const int fieldWidth = 16;

  // identification

  os <<
    "HeaderIdentification:";

  if (fHeaderIdentification) {
    os << endl;
    gIndenter++;
    os <<
      fHeaderIdentification <<
      endl;
    gIndenter--;
  }
  else {
    os << " none" << endl;
  }

  // LilyPond informations

  if (fLilypondDedication.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondDedication" << " : " <<
      fLilypondDedication <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondPiece.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondPiece" << " : " <<
      fLilypondPiece <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondOpus.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondOpus" << " : " <<
      fLilypondOpus <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondTitle" << " : " <<
      fLilypondTitle <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondSubTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondSubTitle" << " : " <<
      fLilypondSubTitle <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondSubSubTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondSubSubTitle" << " : " <<
      fLilypondSubTitle <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondInstrument.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondInstrument" << " : " <<
      fLilypondInstrument <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondMeter.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondMeter" << " : " <<
      fLilypondMeter <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondCopyright.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondCopyright" << " : " <<
      fLilypondCopyright <<
      endl;

    emptyHeader = false;
  }

  if (fLilypondTagline.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lilypondTagline" << " : " <<
      fLilypondTagline <<
      endl;

    emptyHeader = false;
  }

  if (emptyHeader) {
    os <<
      " " << "nothing specified" <<
      endl << endl; // JMI
  }

  gIndenter--;
}

void lpsrHeader::printShort (ostream& os) const
{
  print (os); // JMI
}

ostream& operator<< (ostream& os, const S_lpsrHeader& elt)
{
  elt->print (os);
  return os;
}


}

/*
void lpsrHeader::appendRight (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending right \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fRightsList.push_back (value);
}

void lpsrHeader::appendComposer (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending composer \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fComposersList.push_back (value);
}

void lpsrHeader::appendArranger (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending arranger \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fArrangersList.push_back (value);
}

void lpsrHeader::appendLyricist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending lyricist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fLyricistsList.push_back (value);
}

void lpsrHeader::appendPoet (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending poet \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fPoetsList.push_back (value);
}

void lpsrHeader::removeAllPoets (
  int    inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Removing all poets from lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fPoetsList.clear ();
}

void lpsrHeader::appendTranslator (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending translator \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fTranslatorsList.push_back (value);
}

void lpsrHeader::appendArtist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending artist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fArtistsList.push_back (value);
}

void lpsrHeader::appendSoftware (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending software \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fSoftwaresList.push_back (value);
}

*/
