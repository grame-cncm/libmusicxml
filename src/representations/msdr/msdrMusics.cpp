/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX

#include "waeMessagesHandling.h"

#include "msdrMusics.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

//#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msdrMusic msdrMusic::create (
  int    inputLineNumber,
  string musicName)
{
  msdrMusic* o =
    new msdrMusic (
      inputLineNumber,
      musicName);
  assert (o != nullptr);

  return o;
}

msdrMusic::msdrMusic (
  int    inputLineNumber,
  string musicName)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating MSDR music \"" << asString () << "\"" <<
      endl;
  }
#endif

  fMusicName = musicName;

  fInputLineNumber = inputLineNumber;
}

void msdrMusic::initializeMusic ()
{
  // JMI
}

msdrMusic::~msdrMusic ()
{}

void msdrMusic::addMeasureToMusic (
  int           inputLineNumber,
  int           measureNumber,
  S_msdrMeasure measure)
{
}

void msdrMusic::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrMusic::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msdrMusic>*
    p =
      dynamic_cast<visitor<S_msdrMusic>*> (v)) {
        S_msdrMusic elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrMusic::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msdrMusic::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrMusic::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msdrMusic>*
    p =
      dynamic_cast<visitor<S_msdrMusic>*> (v)) {
        S_msdrMusic elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrMusic::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msdrMusic::browseData (basevisitor* v)
{
/* JMI
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrMusic::browseData ()" <<
      endl;
  }

  // browse the voice initial elements
  if (fInitialMusicElementsList.size ()) {
    for (
      list<S_msdrMusicElement>::const_iterator i = fInitialMusicElementsList.begin ();
      i != fInitialMusicElementsList.end ();
      ++i
    ) {
      // browse the element
      msrBrowser<msdrMusicElement> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fMusicLastSegment) {
    // browse the voice last segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMusicLastSegment);
  }

  // browse the voice stanzas
  if (fMusicStanzasMap.size ()) {
    for (
      map<string, S_msrStanza>::const_iterator i = fMusicStanzasMap.begin ();
      i != fMusicStanzasMap.end ();
      ++i
    ) {
      S_msrStanza stanza = (*i).second;

      if (stanza->getStanzaTextPresent ()) {
        // browse the stanza
        msrBrowser<msrStanza> browser (v);
        browser.browse (*(stanza));
      }
    } // for
  }

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msdrMusic::browseData ()" <<
      endl;
  }
  */
}

string msdrMusic::asShortString () const
{
  stringstream s;

  s <<
    "MSDR voice \"" << fMusicName << "\", " <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msdrMusic::asString () const
{
  stringstream s;

  s <<
    "MSDR music \"" << fMusicName << "\", " <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msdrMusic::displayMusic (
  int    inputLineNumber,
  string context) const
{
  gLogStream <<
    endl <<
    "*********>> Displaying MSDR music \"" <<
    fMusicName <<
    "\" (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    endl << endl;
}

void msdrMusic::print (ostream& os) const
{
  os <<
    "MSDR music \"" << fMusicName <<
    "\", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/* JMI
  os << left <<
    "(" <<
    singularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    singularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured basses") <<
     ", " <<
    singularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    singularOrPlural (
      fVoiceRestsCounter, "rest", "rests") <<
     ", " <<
    singularOrPlural (
      fVoiceSkipsCounter, "skip", "skips") <<
     ", " <<
    singularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ")" <<
    endl;

  const unsigned int fieldWidth = 41;

  os << left <<
    setw (fieldWidth) <<
    "staffUpLink" << " : " <<
    fVoiceStaffUpLink->getStaffName () <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureNumber" << " : \"" <<
    fVoiceCurrentMeasureNumber <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasureOrdinalNumber" << " : " <<
    fVoiceCurrentMeasureOrdinalNumber <<
    endl <<

    setw (fieldWidth) <<
    "voiceCurrentMeasurePuristNumber" << " : " <<
    fVoiceCurrentMeasurePuristNumber <<
    endl <<

    setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << " : " <<
    regularVoiceStaffSequentialNumberAsString () <<
    endl;

#ifdef TRACING_IS_ENABLED
  // regular measure ends detection
  os << left <<
    setw (fieldWidth) <<
    "wholeNotesSinceLastRegularMeasureEnd" << " : " <<
    fWholeNotesSinceLastRegularMeasureEnd <<
    endl;

  // incomplete measures after repeats detection
  os << left <<
    setw (fieldWidth) <<
    "currentVoiceRepeatPhaseKind" << " : " <<
    voiceRepeatPhaseKindAsString (
      fCurrentVoiceRepeatPhaseKind) <<
    endl;

  }
*/

  --gIndenter;
}

void msdrMusic::printShort (ostream& os) const
{
  os <<
    "MSDR music \"" << fMusicName <<
    "\", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/* JMI
  os << left <<
    "(" <<
    singularOrPlural (
      fVoiceActualHarmoniesCounter, "harmony", "harmonies") <<
     ", " <<
    singularOrPlural (
      fVoiceActualFiguredBassesCounter, "figured bass", "figured basses") <<
     ", " <<
    singularOrPlural (
      fVoiceActualNotesCounter, "actual note", "actual notes") <<
     ", " <<
    singularOrPlural (
      fVoiceRestsCounter, "rest", "rests") <<
     ", " <<
    singularOrPlural (
      fVoiceSkipsCounter, "skip", "skips") <<
     ", " <<
    singularOrPlural (
      fVoiceStanzasMap.size (), "stanza", "stanzas") <<
    ")" <<
    endl;

  const unsigned int fieldWidth = 41;

#ifdef TRACING_IS_ENABLED
  // print the voice measures flat list
  unsigned int voiceMeasuresFlatListSize =
    fVoiceMeasuresFlatList.size ();

  os <<
    setw (fieldWidth) <<
    "voiceMeasuresFlatList";
  if (voiceMeasuresFlatListSize) {
    os <<
      " : " <<  voiceMeasuresFlatListSize << " elements";
  }
  else {
    os <<
      " : " << "empty";
  }
  os << endl;

  if (voiceMeasuresFlatListSize) {
    ++gIndenter;

    list<S_msrMeasure>::const_iterator
      iBegin = fVoiceMeasuresFlatList.begin (),
      iEnd   = fVoiceMeasuresFlatList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the measure
      if (gGlobalTraceOahGroup->getTraceMeasures ()) {
        os << (*i)->asShortString ();
      }
      else {
        os << (*i)->getMeasureElementMeasureNumber ();
      }
      if (++i == iEnd) break;
      os << ' ';
    } // for
    os << endl;

    --gIndenter;
  }
#endif
*/

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdrMusic& elt)
{
  elt->print (os);
  return os;
}


}
