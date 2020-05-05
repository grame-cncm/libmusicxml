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

#include "bsrMutualDependencies.h"

#include "bsrOah.h"
#include "bsr2BrailleOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrMusicHeading bsrMusicHeading::create (
  int inputLineNumber)
{
  bsrMusicHeading* o =
    new bsrMusicHeading (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrMusicHeading::bsrMusicHeading (
  int inputLineNumber)
    : bsrLine (
        inputLineNumber,
        0, // JMI ???
        gBsr2BrailleOah->fCellsPerLine)
{}

bsrMusicHeading::~bsrMusicHeading ()
{}

S_bsrCellsList bsrMusicHeading::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append the tempo to result
  if (fMusicHeadingTempo) {
    result->appendCellsListToCellsList (
      fMusicHeadingTempo->fetchCellsList ());
  }

  // append 1 space to result if needed
  if (fMusicHeadingTempo) {
    result->appendCellsListToCellsList (
      bsrSpaces::create (
        fInputLineNumber, 1)->
          fetchCellsList ());
  }

  // append the key to result if any
  if (fMusicHeadingKey) {
    result->appendCellsListToCellsList (
      fMusicHeadingKey->fetchCellsList ());
  }

  // append the time to result if any
  if (fMusicHeadingTime) {
    result->appendCellsListToCellsList (
      fMusicHeadingTime->fetchCellsList ());
  }

  return result;
}

void bsrMusicHeading::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrMusicHeading::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrMusicHeading>*
    p =
      dynamic_cast<visitor<S_bsrMusicHeading>*> (v)) {
        S_bsrMusicHeading elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrMusicHeading::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrMusicHeading::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrMusicHeading::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrMusicHeading>*
    p =
      dynamic_cast<visitor<S_bsrMusicHeading>*> (v)) {
        S_bsrMusicHeading elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrMusicHeading::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrMusicHeading::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrScore::browseData ()" <<
      endl;
  }
#endif

  if (fMusicHeadingTempo) {
    // browse the tempo
    msrBrowser<bsrTempo> browser (v);
    browser.browse (*fMusicHeadingTempo);
  }

  if (fMusicHeadingKey) {
    // browse the key
    msrBrowser<bsrKey> browser (v);
    browser.browse (*fMusicHeadingKey);
  }

  if (fMusicHeadingTime) {
    // browse the time
    msrBrowser<bsrTime> browser (v);
    browser.browse (*fMusicHeadingTime);
  }

#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% <== bsrScore::browseData ()" <<
      endl;
  }
#endif
}

string bsrMusicHeading::asString () const
{
  stringstream s;

  s <<
    "MusicHeading" <<
    ", musicHeadingTempo: ";

  if (fMusicHeadingTempo) {
    s <<
      fMusicHeadingTempo->asShortString ();
  }
  else {
    s <<
      "none";
  }

  s <<
      ", musicHeadingTempo: ";

  if (fMusicHeadingTempo) {
    s <<
      fMusicHeadingTempo->asShortString ();
  }
  else {
    s <<
      "none";
  }

  s <<
      ", musicHeadingTime: ";

  if (fMusicHeadingTime) {
    s <<
      fMusicHeadingTime->asShortString ();
  }
  else {
    s <<
      "none";
  }

  s <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrMusicHeading::asDebugString () const
{
  stringstream s;

  if (fMusicHeadingTempo) {
    s <<
      fMusicHeadingTempo->asDebugString ();
  }

  if (fMusicHeadingTempo) {
    s <<
      fMusicHeadingTempo->asDebugString ();
  }

  if (fMusicHeadingTime) {
    s <<
      fMusicHeadingTime->asDebugString ();
  }

  return s.str ();
}

void bsrMusicHeading::print (ostream& os) const
{
  os <<
    "MusicHeading" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

// JMI  const int fieldWidth = 22;

  os <<
    "musicHeadingTempo: ";
  if (fMusicHeadingTempo) {
    os << endl;

    gIndenter++;

    os <<
      fMusicHeadingTempo;

    gIndenter--;
  }
  else {
    os <<
      "none" <<
      endl;
  }

  os <<
    "musicHeadingKey: ";
  if (fMusicHeadingKey) {
    os << endl;

    gIndenter++;

    os <<
      fMusicHeadingKey;

    gIndenter--;
  }
  else {
    os <<
      "none" <<
      endl;
  }

  os <<
    "musicHeadingTime: ";
  if (fMusicHeadingTime) {
    os << endl;

    gIndenter++;

    os <<
      fMusicHeadingTime;

    gIndenter--;
  }
  else {
    os <<
      "none" <<
      endl;
  }

  os <<
    "musicHeadingCellsList: " <<
    endl;
  gIndenter++;
  os <<
    buildCellsList () <<
    endl;
  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrMusicHeading& elt)
{
  elt->print (os);
  return os;
}


}
