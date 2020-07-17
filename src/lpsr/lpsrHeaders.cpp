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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "lpsrOah.h"
#include "lpsr2LilypondOah.h"


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
  assert(o!=0);
  return o;
}

lpsrHeader::lpsrHeader (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  // grab values from the LilyPond options if any
  if (gLpsr2LilypondOah->fDedication.size ()) {
    setLilypondDedication (
      inputLineNumber,
      gLpsr2LilypondOah->fDedication,
      kFontStyleNone,
      kFontWeightNone);
  }

  if (gLpsr2LilypondOah->fPiece.size ()) {
    setLilypondPiece (
      inputLineNumber,
      gLpsr2LilypondOah->fPiece,
      kFontStyleNone,
      kFontWeightNone);
  }

  if (gLpsr2LilypondOah->fOpus.size ()) {
    setLilypondOpus (
      inputLineNumber,
      gLpsr2LilypondOah->fOpus,
      kFontStyleNone,
      kFontWeightNone);
  }

  if (gLpsr2LilypondOah->fTitle.size ()) {
    setLilypondTitle (
      inputLineNumber,
      gLpsr2LilypondOah->fTitle,
      kFontStyleNone,
      kFontWeightNone);
  }
  if (gLpsr2LilypondOah->fSubTitle.size ()) {
    setLilypondSubTitle (
      inputLineNumber,
      gLpsr2LilypondOah->fSubTitle,
      kFontStyleNone,
      kFontWeightNone);
  }
  if (gLpsr2LilypondOah->fSubSubTitle.size ()) {
    setLilypondSubSubTitle (
      inputLineNumber,
      gLpsr2LilypondOah->fSubSubTitle,
      kFontStyleNone,
      kFontWeightNone);
  }

  if (gLpsr2LilypondOah->fInstrument.size ()) {
    setLilypondInstrument (
      inputLineNumber,
      gLpsr2LilypondOah->fInstrument,
      kFontStyleNone,
      kFontWeightNone);
  }
  if (gLpsr2LilypondOah->fMeter.size ()) {
    setLilypondMeter (
      inputLineNumber,
      gLpsr2LilypondOah->fMeter,
      kFontStyleNone,
      kFontWeightNone);
  }

  if (gLpsr2LilypondOah->fCopyright.size ()) {
    setLilypondCopyright (
      inputLineNumber,
      gLpsr2LilypondOah->fCopyright,
      kFontStyleNone,
      kFontWeightNone);
  }
  if (gLpsr2LilypondOah->fTagline.size ()) {
    setLilypondTagline (
      inputLineNumber,
      gLpsr2LilypondOah->fTagline,
      kFontStyleNone,
      kFontWeightNone);
  }
}

lpsrHeader::~lpsrHeader ()
{}

void lpsrHeader::setLilypondDedication (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondDedication =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondDedication,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setLilypondPiece (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondPiece =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondPiece,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setLilypondOpus (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondOpus =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondOpus,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setLilypondTitle (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondTitle,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setLilypondSubTitle (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondSubTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondSubTitle,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setLilypondSubSubTitle (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondSubSubTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondSubSubTitle,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setLilypondInstrument (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondInstrument =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondInstrument,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setLilypondMeter (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondMeter =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondMeter,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setLilypondCopyright (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondCopyright =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondCopyright,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setLilypondTagline (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fLilypondTagline =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kLilypondTagline,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setWorkNumber (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fWorkNumber =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kMusicXMLWorkNumber,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setWorkTitle (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
  {
  fWorkTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kMusicXMLWorkTitle,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
  }

void lpsrHeader::setOpus (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
  {
  fOpus =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kMusicXMLOpus,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
  }

void lpsrHeader::setMovementNumber (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
  {
  fMovementNumber =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kMusicXMLMovementNumber,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
  }

void lpsrHeader::setMovementTitle (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fMovementTitle =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kMusicXMLMovementTitle,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setEncodingDate (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fEncodingDate =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kMusicXMLEncodingDate,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setScoreInstrument (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fScoreInstrument =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kMusicXMLScoreInstrument,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::setMiscellaneousField (
  int               inputLineNumber,
  string            val,
  msrFontStyleKind  fontStyleKind,
  msrFontWeightKind fontWeightKind)
{
  fMiscellaneousField =
    lpsrVarValAssoc::create (
      inputLineNumber,
      lpsrVarValAssoc::kCommentedNo,
      lpsrVarValAssoc::kWithBackSlashNo,
      lpsrVarValAssoc::kMusicXMLMiscellaneousField,
      lpsrVarValAssoc::kVarValSeparatorEqualSign,
      lpsrVarValAssoc::kQuotesAroundValueYes,
      val,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit,
      fontStyleKind,
      fontWeightKind,
      lpsrVarValAssoc::g_LilyPondVarValAssocNoComment,
      lpsrVarValAssoc::kEndlNone);
}

void lpsrHeader::addRights (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVarValAssocs) {
    gLogOstream <<
      "Adding right \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fRights) {
    fRights =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLRights);
  }

  fRights->
    addAssocVariableValue (value);
}

void lpsrHeader::addComposer (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVarValAssocs) {
    gLogOstream <<
      "Adding composer \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fComposers) {
    fComposers =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLComposer);
  }

  fComposers->
    addAssocVariableValue (value);
}

void lpsrHeader::addArranger (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVarValAssocs) {
    gLogOstream <<
      "Adding arranger \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fArrangers) {
    fArrangers =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLArranger);
  }

  fArrangers->
    addAssocVariableValue (value);
}

void lpsrHeader::addLyricist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVarValAssocs) {
    gLogOstream <<
      "Adding lyricist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fLyricists) {
    fLyricists =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLLyricist);
  }

  fLyricists->
    addAssocVariableValue (value);
}

void lpsrHeader::addPoet (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVarValAssocs) {
    gLogOstream <<
      "Adding poet \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fPoets) {
    fPoets =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLPoet);
  }

  fPoets->
    addAssocVariableValue (value);
}

void lpsrHeader::addTranslator (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVarValAssocs) {
    gLogOstream <<
      "Adding translator \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fTranslators) {
    fTranslators =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLTranslator);
  }

  fTranslators->
    addAssocVariableValue (value);
}

void lpsrHeader::addArtist (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVarValAssocs) {
    gLogOstream <<
      "Adding artist \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fArtists) {
    fArtists =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLArtist);
  }

  fArtists->
    addAssocVariableValue (value);
}

void lpsrHeader::addSoftware (
  int    inputLineNumber,
  string value)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVarValAssocs) {
    gLogOstream <<
      "Adding software \"" <<
      value <<
      "\" to lpsrHeader" <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  if (! fSoftwares) {
    fSoftwares =
      lpsrVarValsListAssoc::create (
        inputLineNumber,
        lpsrVarValsListAssoc::kMusicXMLSoftware);
  }

  fSoftwares->
    addAssocVariableValue (value);
}

int lpsrHeader::maxLilypondVariablesNamesLength ()
{
  int result = 0;

  // MusicXML informations

  if (fWorkNumber) {
    int length =
      fWorkNumber->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fWorkTitle) {
    int length =
      fWorkTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fOpus) {
    int length =
      fOpus->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fMovementNumber) {
    int length =
      fMovementNumber->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fMovementTitle) {
    int length =
      fMovementTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fRights) {
    const list<string>&
      variableValuesList =
        fRights->getVariableValuesList ();

    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fComposers) {
    const list<string>&
      variableValuesList =
        fComposers->getVariableValuesList ();

    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result){
        result = length;
      }
    } // for
  }

  if (fArrangers) {
    const list<string>&
      variableValuesList =
        fArrangers->getVariableValuesList ();

    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fLyricists) {
    const list<string>&
      variableValuesList =
        fLyricists->getVariableValuesList ();

    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fPoets) {
    const list<string>&
      variableValuesList =
        fPoets->getVariableValuesList ();

    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fTranslators) {
    const list<string>&
      variableValuesList =
        fTranslators->getVariableValuesList ();

    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fSoftwares) {
    const list<string>&
      variableValuesList =
        fSoftwares->getVariableValuesList ();

    list<string>::const_iterator i;
    for (i=variableValuesList.begin (); i!=variableValuesList.end (); i++) {
      int length = (*i).size ();
      if (length > result) {
        result = length;
      }
    } // for
  }

  if (fScoreInstrument) {
    int length = fScoreInstrument->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fEncodingDate) {
    int length = fEncodingDate->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fMiscellaneousField) {
    int length = fMiscellaneousField->lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  // Lilypond informations

  if (fLilypondDedication) {
    int length =
      fLilypondDedication->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondPiece) {
    int length =
      fLilypondPiece->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondOpus) {
    int length =
      fLilypondOpus->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondTitle) {
    int length =
      fLilypondTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondSubTitle) {
    int length =
      fLilypondSubTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondSubSubTitle) {
    int length =
      fLilypondSubSubTitle->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondInstrument) {
    int length =
      fLilypondInstrument->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondMeter) {
    int length =
      fLilypondMeter->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondCopyright) {
    int length =
      fLilypondCopyright->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  if (fLilypondTagline) {
    int length =
      fLilypondTagline->
        lilyPondVarValAssocKindAsString ().size ();
    if (length > result) {
      result = length;
    }
  }

  return result;
}

void lpsrHeader::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> lpsrHeader::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef TRACE_OAH
        if (gLpsrOah->fTraceLpsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrHeader::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrHeader::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> lpsrHeader::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrHeader>*
    p =
      dynamic_cast<visitor<S_lpsrHeader>*> (v)) {
        S_lpsrHeader elem = this;

#ifdef TRACE_OAH
        if (gLpsrOah->fTraceLpsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrHeader::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrHeader::browseData (basevisitor* v)
{
  if (fWorkNumber) {
    // browse fWorkNumber
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fWorkNumber);
  }

  if (fWorkTitle) {
    // browse fWorkTitle
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fWorkTitle);
  }

  if (fOpus) {
    // browse fOpus
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fOpus);
  }

  if (fMovementNumber) {
    // browse fMovementNumber
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMovementNumber);
  }

  if (fMovementTitle) {
    // browse fMovementTitle
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMovementTitle);
  }

  if (fRights) {
    // browse fRights
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fRights);
  }

  if (fComposers) {
    // browse fComposers
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fComposers);
  }

  if (fArrangers) {
    // browse fArrangers
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fArrangers);
  }

  if (fLyricists) {
    // browse fLyricists
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fLyricists);
  }

  if (fPoets) {
    // browse fPoets
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fPoets);
  }

  if (fTranslators) {
    // browse fTranslators
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fTranslators);
  }

  if (fSoftwares) {
    // browse fSoftwares
    msrBrowser<lpsrVarValsListAssoc> browser (v);
    browser.browse (*fSoftwares);
  }

  if (fScoreInstrument) {
    // browse fScoreInstrument
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fScoreInstrument);
  }

  if (fEncodingDate) {
    // browse fEncodingDate
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fEncodingDate);
  }

  if (fMiscellaneousField) {
    // browse fMiscellaneousField
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*fMiscellaneousField);
  }
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

  if (fLilypondTitle) {
    os << left <<
      setw (fieldWidth) <<
      fLilypondTitle->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fLilypondTitle <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fWorkNumber) {
    os << left <<
      setw (fieldWidth) <<
      fWorkNumber->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fWorkNumber <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fWorkTitle) {
    os << left <<
      setw (fieldWidth) <<
      fWorkTitle->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fWorkTitle <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fOpus) {
    os << left <<
      setw (fieldWidth) <<
      fOpus->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fOpus <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fMovementNumber) {
    os << left <<
      setw (fieldWidth) <<
      fMovementNumber->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fMovementNumber <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fMovementTitle) {
    os << left <<
      setw (fieldWidth) <<
      fMovementTitle->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fMovementTitle <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fRights) {
    os << left <<
      setw (fieldWidth) <<
      fRights->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fRights <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fComposers) {
    os << left <<
      setw (fieldWidth) <<
      fComposers->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fComposers <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fArrangers) {
    os << left <<
      setw (fieldWidth) <<
      fArrangers->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fArrangers <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fLyricists) {
    os << left <<
      setw (fieldWidth) <<
      fLyricists->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fLyricists <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fPoets) {
    os << left <<
      setw (fieldWidth) <<
      fPoets->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fPoets <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fTranslators) {
    os << left <<
      setw (fieldWidth) <<
      fTranslators->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fTranslators <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fSoftwares) {
    os << left <<
      setw (fieldWidth) <<
      fSoftwares->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fSoftwares <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fScoreInstrument) {
    os << left <<
      setw (fieldWidth) <<
      fScoreInstrument->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fScoreInstrument <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fEncodingDate) {
    os << left <<
      setw (fieldWidth) <<
      fEncodingDate->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fEncodingDate <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fMiscellaneousField) {
    os << left <<
      setw (fieldWidth) <<
      fMiscellaneousField->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;

    os <<
      fMiscellaneousField <<
      endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (emptyHeader) {
    os <<
      " " << "nothing specified" <<
      endl <<
      endl; // JMI
  }

  gIndenter--;
}

void lpsrHeader::printShort (ostream& os) const
{
  os <<
    "Header" <<
    endl;

  bool emptyHeader = true;

  gIndenter++;

  const int fieldWidth = 16;

  // LilyPond informations

  // MusicXML informations

  if (fLilypondTitle) {
    os << left <<
      setw (fieldWidth) <<
      fLilypondTitle->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fLilypondTitle->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fWorkNumber) {
    os << left <<
      setw (fieldWidth) <<
      fWorkNumber->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fWorkNumber->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fWorkTitle) {
    os << left <<
      setw (fieldWidth) <<
      fWorkTitle->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fWorkTitle->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fOpus) {
    os << left <<
      setw (fieldWidth) <<
      fOpus->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fOpus->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fMovementNumber) {
    os << left <<
      setw (fieldWidth) <<
      fMovementNumber->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fMovementNumber->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fMovementTitle) {
    os << left <<
      setw (fieldWidth) <<
      fMovementTitle->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fMovementTitle->printShort (os);
    os << endl;

    gIndenter--;

    emptyHeader = false;
  }

  if (fRights) {
    os << left <<
      setw (fieldWidth) <<
      fRights->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fRights->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fComposers) {
    os << left <<
      setw (fieldWidth) <<
      fComposers->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fComposers->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fArrangers) {
    os << left <<
      setw (fieldWidth) <<
      fArrangers->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fArrangers->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fLyricists) {
    os << left <<
      setw (fieldWidth) <<
      fLyricists->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fLyricists->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fPoets) {
    os << left <<
      setw (fieldWidth) <<
      fPoets->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fPoets->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fTranslators) {
    os << left <<
      setw (fieldWidth) <<
      fTranslators->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fTranslators->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fSoftwares) {
    os << left <<
      setw (fieldWidth) <<
      fSoftwares->lilyPondVarValsListAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fSoftwares->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fScoreInstrument) {
    os << left <<
      setw (fieldWidth) <<
      fScoreInstrument->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fScoreInstrument->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fEncodingDate) {
    os << left <<
      setw (fieldWidth) <<
      fEncodingDate->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fEncodingDate->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (fMiscellaneousField) {
    os << left <<
      setw (fieldWidth) <<
      fMiscellaneousField->lilyPondVarValAssocKindAsString () << " : " <<
      endl;

    gIndenter++;
    fMiscellaneousField->printShort (os);
    os << endl;
    gIndenter--;

    emptyHeader = false;
  }

  if (emptyHeader) {
    os <<
      " " << "nothing specified" <<
      endl <<
      endl; // JMI
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrHeader& elt)
{
  elt->print (os);
  return os;
}


}
