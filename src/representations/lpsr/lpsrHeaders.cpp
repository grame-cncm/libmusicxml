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
{
  // grab values from the LilyPond options if any
  if (gGlobalLpsr2lilypondOahGroup->getDedication ().size ()) {
    setLilypondDedication (
      gGlobalLpsr2lilypondOahGroup->getDedication ());
  }

  if (gGlobalLpsr2lilypondOahGroup->getPiece ().size ()) {
    setLilypondPiece (
      gGlobalLpsr2lilypondOahGroup->getPiece ());
  }

  if (gGlobalLpsr2lilypondOahGroup->getOpus ().size ()) {
    setLilypondOpus (
      gGlobalLpsr2lilypondOahGroup->getOpus ());
  }

  if (gGlobalLpsr2lilypondOahGroup->getTitle ().size ()) {
    setLilypondTitle (
      gGlobalLpsr2lilypondOahGroup->getTitle ());
  }
  if (gGlobalLpsr2lilypondOahGroup->getSubTitle ().size ()) {
    setLilypondSubTitle (
      gGlobalLpsr2lilypondOahGroup->getSubTitle ());
  }
  if (gGlobalLpsr2lilypondOahGroup->getSubSubTitle ().size ()) {
    setLilypondSubSubTitle (
      gGlobalLpsr2lilypondOahGroup->getSubSubTitle ());
  }

  if (gGlobalLpsr2lilypondOahGroup->getInstrument ().size ()) {
    setLilypondInstrument (
      gGlobalLpsr2lilypondOahGroup->getInstrument ());
  }
  if (gGlobalLpsr2lilypondOahGroup->getMeter ().size ()) {
    setLilypondMeter (
      gGlobalLpsr2lilypondOahGroup->getMeter ());
  }

  if (gGlobalLpsr2lilypondOahGroup->getCopyright ().size ()) {
    setLilypondCopyright (
      gGlobalLpsr2lilypondOahGroup->getCopyright ());
  }
  if (gGlobalLpsr2lilypondOahGroup->getTagline ().size ()) {
    setLilypondTagline (
      gGlobalLpsr2lilypondOahGroup->getTagline ());
  }
}

lpsrHeader::~lpsrHeader ()
{}

void lpsrHeader::appendRights (
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

int lpsrHeader::maxLilypondVariablesNamesLength ()
{
  int result = 0;

  // MusicXML informations

  if (fWorkNumber.size () > result) {
      result = fWorkNumber.size ();
  }

  if (fWorkTitle.size () > result) {
      result = fWorkTitle.size ();
  }

  if (fOpus.size () > result) {
      result = fOpus.size ();
  }

  if (fMovementNumber.size () > result) {
      result = fMovementNumber.size ();
  }

  if (fMovementTitle.size () > result) {
      result = fMovementTitle.size ();
  }

  if (fRightsList.size ()) {
    list<string>::const_iterator i;

    for (i=fRightsList.begin (); i!=fRightsList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fComposersList.size ()) {
    list<string>::const_iterator i;

    for (i=fComposersList.begin (); i!=fComposersList.end (); i++) {
      int length = (*i).size ();
      if (length > result){
        result = length;
      }
    } // for
  }

  if (fArrangersList.size ()) {
    list<string>::const_iterator i;

    for (i=fArrangersList.begin (); i!=fArrangersList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fLyricistsList.size ()) {
    list<string>::const_iterator i;

    for (i=fLyricistsList.begin (); i!=fLyricistsList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fPoetsList.size ()) {
    list<string>::const_iterator i;

    for (i=fPoetsList.begin (); i!=fPoetsList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fTranslatorsList.size ()) {
    list<string>::const_iterator i;

    for (i=fTranslatorsList.begin (); i!=fTranslatorsList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fSoftwaresList.size ()) {
    list<string>::const_iterator i;

    for (i=fSoftwaresList.begin (); i!=fSoftwaresList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fScoreInstrument.size () > result) {
      result = fScoreInstrument.size ();
  }

  if (fEncodingDate.size () > result) {
      result = fEncodingDate.size ();
  }

  if (fMiscellaneousField.size () > result) {
      result = fMiscellaneousField.size ();
  }

  // Lilypond informations

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
    ", workNumber: \" " <<  fWorkNumber << "\"" <<
    ", workTitle: \" " <<  fWorkTitle << "\"" <<
    ", opus: \" " <<  fOpus << "\"" <<
    ", movementNumber: \" " <<  fMovementNumber << "\"" <<
    ", movementTitle: \" " <<  fMovementTitle << "\"" <<
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

  // LilyPond informations

  // MusicXML informations

  if (fLilypondTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "title" << " : " <<
      fLilypondTitle <<
      endl;

    emptyHeader = false;
  }

  if (fWorkNumber.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "workNumber" << " : " <<
      fWorkNumber <<
      endl;

    emptyHeader = false;
  }

  if (fWorkTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "workTitle" << " : " <<
      fWorkTitle <<
      endl;

    emptyHeader = false;
  }

  if (fOpus.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "opus" << " : " <<
      fOpus <<
      endl;

    emptyHeader = false;
  }

  if (fMovementNumber.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "movementNumber" << " : " <<
      fMovementNumber <<
      endl;

    emptyHeader = false;
  }

  if (fMovementTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "movementTitle" << " : " <<
      fMovementTitle <<
      endl;

    emptyHeader = false;
  }

  if (fRightsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "rightsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fRightsList.begin ();
      i != fRightsList.end ();
      i++
    ) {
      gIndenter++;
      os << (*i) << endl;
      gIndenter--;
    } // for

    emptyHeader = false;
  }

  if (fComposersList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "composersList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fComposersList.begin ();
      i != fComposersList.end ();
      i++
    ) {
      gIndenter++;
      os << (*i) << endl;
      gIndenter--;
    } // for

    emptyHeader = false;
  }

  if (fArrangersList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "arrangers" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fArrangersList.begin ();
      i != fArrangersList.end ();
      i++
    ) {
      gIndenter++;
      os << (*i) << endl;
      gIndenter--;
    } // for

    emptyHeader = false;
  }

  if (fLyricistsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lyricists" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fLyricistsList.begin ();
      i != fLyricistsList.end ();
      i++
    ) {
      gIndenter++;
      os << (*i) << endl;
      gIndenter--;
    } // for

    emptyHeader = false;
  }

  if (fPoetsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "poetsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fPoetsList.begin ();
      i != fPoetsList.end ();
      i++
    ) {
      gIndenter++;
      os << (*i) << endl;
      gIndenter--;
    } // for

    emptyHeader = false;
  }

  if (fTranslatorsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "translatorsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fTranslatorsList.begin ();
      i != fTranslatorsList.end ();
      i++
    ) {
      gIndenter++;
      os << (*i) << endl;
      gIndenter--;
    } // for

    emptyHeader = false;
  }

  if (fSoftwaresList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "softwaresList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fSoftwaresList.begin ();
      i != fSoftwaresList.end ();
      i++
    ) {
      gIndenter++;
      os << (*i) << endl;
      gIndenter--;
    } // for

    emptyHeader = false;
  }

  if (fScoreInstrument.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "scoreInstrument" << " : " <<
      fScoreInstrument <<
      endl;

    emptyHeader = false;
  }

  if (fEncodingDate.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "encodingDate" << " : " <<
      fEncodingDate <<
      endl;

    emptyHeader = false;
  }

  if (fMiscellaneousField.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "miscellaneousField" << " : " <<
      fMiscellaneousField <<
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
