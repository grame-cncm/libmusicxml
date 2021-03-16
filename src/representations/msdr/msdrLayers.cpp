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

#include <vector>

#include "waeMessagesHandling.h"

#include "msdrLayers.h"

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
S_msdrLayer msdrLayer::create (
  int    inputLineNumber,
  string layerNumber)
{
  msdrLayer* o =
    new msdrLayer (
      inputLineNumber,
      layerNumber);
  assert (o != nullptr);

  return o;
}

msdrLayer::msdrLayer (
  int    inputLineNumber,
  string layerNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Creating MSDR music \"" << asString () << "\"" <<
      endl;
  }
#endif

  fLayerNumber = layerNumber;

  fInputLineNumber = inputLineNumber;
}

void msdrLayer::initializeLayer ()
{
  // JMI
}

msdrLayer::~msdrLayer ()
{}

void msdrLayer::addClefToLayer (
  int       inputLineNumber,
  int       measureNumber,
  S_msrClef clef)
{
}

void msdrLayer::addKeyToLayer (
  int      inputLineNumber,
  int      measureNumber,
  S_msrKey key)
{
}

void msdrLayer::addTimeToLayer (
  int       inputLineNumber,
  int       measureNumber,
  S_msrTime time)
{
}

void msdrLayer::addNoteToLayer (
  int       inputLineNumber,
  int       measureNumber,
  S_msrNote note)
{
}

void msdrLayer::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrLayer::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msdrLayer>*
    p =
      dynamic_cast<visitor<S_msdrLayer>*> (v)) {
        S_msdrLayer elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrLayer::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msdrLayer::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrLayer::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msdrLayer>*
    p =
      dynamic_cast<visitor<S_msdrLayer>*> (v)) {
        S_msdrLayer elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msdrLayer::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msdrLayer::browseData (basevisitor* v)
{
/* JMI
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msdrLayer::browseData ()" <<
      endl;
  }

  // browse the voice initial elements
  if (fInitialMusicElementsList.size ()) {
    for (
      list<S_msdrLayerElement>::const_iterator i = fInitialMusicElementsList.begin ();
      i != fInitialMusicElementsList.end ();
      ++i
    ) {
      // browse the element
      msrBrowser<msdrLayerElement> browser (v);
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
      "% <== msdrLayer::browseData ()" <<
      endl;
  }
  */
}

string msdrLayer::asShortString () const
{
  stringstream s;

  s <<
    "MSDR layer '" << fLayerNumber << "', " <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msdrLayer::asString () const
{
  stringstream s;

  s <<
    "MSDR layer '" << fLayerNumber << "', " <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msdrLayer::displayLayer (
  int    inputLineNumber,
  string context) const
{
  gLogStream <<
    endl <<
    "*********>> Displaying MSDR layer '" <<
    fLayerNumber <<
    "' (" << context << ")" <<
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

void msdrLayer::print (ostream& os) const
{
  os <<
    "MSDR measure '" << fLayerNumber <<
    "', line " << fInputLineNumber <<
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

void msdrLayer::printShort (ostream& os) const
{
  os <<
    "MSDR layer '" <<
    fLayerNumber <<
    "', line " << fInputLineNumber <<
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

ostream& operator<< (ostream& os, const S_msdrLayer& elt)
{
  elt->print (os);
  return os;
}


}
