/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "msrIdentification.h"

#include "utilities.h"

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
S_msrIdentification msrIdentification::create (
  int inputLineNumber)
{
  msrIdentification* o =
    new msrIdentification (
      inputLineNumber);
  assert (o!=0);
  return o;
}

msrIdentification::msrIdentification (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrIdentification::~msrIdentification ()
{}

void msrIdentification::setWorkNumber (
  int    inputLineNumber,
  string val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting workNumber to \"" << val << "\"" <<
      endl;
  }
#endif

  fWorkNumber = val;
}

void msrIdentification::setWorkTitle (
  int    inputLineNumber,
  string val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting workTitle to \"" << val << "\"" <<
      endl;
  }
#endif

  fWorkTitle = val;
}

void msrIdentification::setOpus (
  int    inputLineNumber,
  string val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting opus to \"" << val << "\"" <<
      endl;
  }
#endif

  fOpus = val;
}

void msrIdentification::setMovementNumber (
  int    inputLineNumber,
  string val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting movementNumber to \"" << val << "\"" <<
      endl;
  }
#endif

  fMovementNumber = val;
}

void msrIdentification::setMovementTitle (
  int    inputLineNumber,
  string val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting movementTitle to \"" << val << "\"" <<
      endl;
  }
#endif

  fMovementTitle = val;
}

void msrIdentification::setEncodingDate (
  int    inputLineNumber,
  string val)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting encodingDate to \"" << val << "\"" <<
      endl;
  }
#endif

  fEncodingDate = val;
}

void msrIdentification::setMiscellaneousField (
  int    inputLineNumber,
  string val)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting miscellaneousField to \"" << val << "\"" <<
      endl;
  }
#endif

  fMiscellaneousField = val;
}

void msrIdentification::setScoreInstrument (
  int    inputLineNumber,
  string val)
{
 #ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Setting scoreInstrument to \"" << val << "\"" <<
      endl;
  }
#endif

  fScoreInstrument = val;
}

void msrIdentification::appendComposer (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending composer \"" << value << "\"" <<
      endl;
  }
#endif

  fComposersList.push_back (value);
}

void msrIdentification::appendLyricist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending lyricist \"" << value << "\"" <<
      endl;
  }
#endif

  fLyricistsList.push_back (value);
}

void msrIdentification::appendArranger (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending arranger \"" << value << "\"" <<
      endl;
  }
#endif

  fArrangersList.push_back (value);
}

void msrIdentification::appendPoet (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending poet \"" << value << "\"" <<
      endl;
  }
#endif

  fPoetsList.push_back (value);
}

void msrIdentification::appendTranslator (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending translator \"" << value << "\"" <<
      endl;
  }
#endif

  fTranslatorsList.push_back (value);
}

void msrIdentification::appendArtist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending artist \"" << value << "\"" <<
      endl;
  }
#endif

  fArtistsList.push_back (value);
}

void msrIdentification::appendSoftware (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending software \"" << value << "\"" <<
      endl;
  }
#endif

  fSoftwaresList.push_back (value);
}

void msrIdentification::appendRight (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending right \"" << value << "\"" <<
      endl;
  }
#endif

  fRightsList.push_back (value);
}

void msrIdentification::appendSource (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appendings source \"" << value << "\"" <<
      endl;
  }
#endif

  fSourcesList.push_back (value);
}

void msrIdentification::appendRelation (
  int    inputLineNumber,
  string value)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceIdentification ()) {
    gLogStream <<
      "Appending relation \"" << value << "\"" <<
      endl;
  }
#endif

  fRelationsList.push_back (value);
}

int msrIdentification::maxIdentificationNamesLength ()
{
  int result = 0;

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

  if (fEncodingDate.size () > result) {
      result = fEncodingDate.size ();
  }

  if (fMiscellaneousField.size () > result) {
      result = fMiscellaneousField.size ();
  }

  if (fScoreInstrument.size () > result) {
      result = fScoreInstrument.size ();
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

  if (fLyricistsList.size ()) {
    list<string>::const_iterator i;

    for (i=fLyricistsList.begin (); i!=fLyricistsList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
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

  if (fArtistsList.size ()) {
    list<string>::const_iterator i;

    for (i=fArtistsList.begin (); i!=fArtistsList.end (); i++) {
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

  if (fRightsList.size ()) {
    list<string>::const_iterator i;

    for (i=fRightsList.begin (); i!=fRightsList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fSourcesList.size ()) {
    list<string>::const_iterator i;

    for (i=fSourcesList.begin (); i!=fSourcesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fRelationsList.size ()) {
    list<string>::const_iterator i;

    for (i=fRelationsList.begin (); i!=fRelationsList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  return result;
}

void msrIdentification::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrIdentification::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrIdentification::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrIdentification::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrIdentification::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrIdentification>*
    p =
      dynamic_cast<visitor<S_msrIdentification>*> (v)) {
        S_msrIdentification elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrIdentification::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrIdentification::browseData (basevisitor* v)
{}

string msrIdentification::asString () const
{
  stringstream s;

  s <<
    "[ Identification" <<
    ", workNumber: \" " <<  fWorkNumber << "\"" <<
    ", workTitle: \" " <<  fWorkTitle << "\"" <<
    ", opus: \" " <<  fOpus << "\"" <<
    ", movementNumber: \" " <<  fMovementNumber << "\"" <<
    ", movementTitle: \" " <<  fMovementTitle << "\"" <<
    ", ..." <<
    "]";

  return s.str ();
}

void msrIdentification::print (ostream& os) const
{
  os <<
    "Identification" <<
    endl;

  const int fieldWidth = 20;

  bool emptyIdentification = true;

  gIndenter++;

  if (fWorkNumber.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "workNumber" << " : \"" <<
      fWorkNumber <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fWorkTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "workTitle" << " : \"" <<
      fWorkTitle <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fOpus.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "opus" << " : \"" <<
      fOpus <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fMovementNumber.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "movementNumber" << " : \"" <<
      fMovementNumber <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fMovementTitle.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "movementTitle" << " : \"" <<
      fMovementTitle <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fEncodingDate.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "encodingDate" << " : \"" <<
      fEncodingDate <<
      "\"" <<
      endl;

    emptyIdentification = false;
  }

  if (fMiscellaneousField.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "miscellaneousField" << " : \"" <<
      fMiscellaneousField <<
      "\"" <<
      endl;

    emptyIdentification = false;
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
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fLyricistsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "lyricistsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fLyricistsList.begin ();
      i != fLyricistsList.end ();
      i++
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fArrangersList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "arrangersList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fArrangersList.begin ();
      i != fArrangersList.end ();
      i++
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
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
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
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
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
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
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
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
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fSourcesList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "sourcesList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fSourcesList.begin ();
      i != fSourcesList.end ();
      i++
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (fRelationsList.size ()) {
    os << left <<
      setw (fieldWidth) <<
      "relationsList" << " : " <<
      endl;

    for (
      list<string>::const_iterator i = fRelationsList.begin ();
      i != fRelationsList.end ();
      i++
    ) {
      os <<
        gTab << "\"" << (*i) << "\"" <<
        endl;
    } // for

    emptyIdentification = false;
  }

  if (emptyIdentification) {
    os <<
      " " << "nothing specified" <<
      endl;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrIdentification& elt)
{
  elt->print (os);
  return os;
}


}
