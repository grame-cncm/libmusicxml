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
#include <algorithm>    // for_each

#include "utilities.h"

#include "lpsrVarValAssocs.h"

#include "oahOah.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string const lpsrVarValAssoc::g_LilyPondVarValAssocNoUnit    = "";
string const lpsrVarValAssoc::g_LilyPondVarValAssocNoComment = "";

S_lpsrVarValAssoc lpsrVarValAssoc::create (
  int                     inputLineNumber,
  lpsrCommentedKind       commentedKind,
  lpsrBackSlashKind       backSlashKind,
  lpsrVarValAssocKind
                          lilyPondVarValAssocKind,
  lpsrVarValSeparatorKind varValSeparatorKind,
  lpsrQuotesKind          quotesKind,
  string                  value,
  string                  unit,
  msrFontStyleKind        varValFontStyleKind,
  msrFontWeightKind       varValFontWeightKind,
  string                  comment,
  lpsrEndlKind            endlKind)
{
  lpsrVarValAssoc* o =
    new lpsrVarValAssoc(
      inputLineNumber,
      commentedKind,
      backSlashKind,
      lilyPondVarValAssocKind,
      varValSeparatorKind,
      quotesKind,
      value,
      unit,
      varValFontStyleKind,
      varValFontWeightKind,
      comment,
      endlKind);
  assert (o!=0);
  return o;
}

lpsrVarValAssoc::lpsrVarValAssoc (
  int                     inputLineNumber,
  lpsrCommentedKind       commentedKind,
  lpsrBackSlashKind       backSlashKind,
  lpsrVarValAssocKind
                          lilyPondVarValAssocKind,
  lpsrVarValSeparatorKind varValSeparatorKind,
  lpsrQuotesKind          quotesKind,
  string                  value,
  string                  unit,
  msrFontStyleKind        varValFontStyleKind,
  msrFontWeightKind       varValFontWeightKind,
  string                  comment,
  lpsrEndlKind            endlKind)
    : lpsrElement (inputLineNumber)
{
  fBackSlashKind           = backSlashKind;
  fLilyPondVarValAssocKind = lilyPondVarValAssocKind;
  fVarValSeparatorKind     = varValSeparatorKind;
  fQuotesKind              = quotesKind;

  fVariableValue           = value;
  fCommentedKind           = commentedKind;
  fUnit                    = unit;

  fVarValFontStyleKind      = varValFontStyleKind;
  fVarValFontWeightKind     = varValFontWeightKind;

  fComment                  = comment;

  fEndlKind                 = endlKind;
}

lpsrVarValAssoc::~lpsrVarValAssoc ()
{}

void lpsrVarValAssoc::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrVarValAssoc::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValAssoc>*> (v)) {
        S_lpsrVarValAssoc elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrVarValAssoc::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrVarValAssoc::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrVarValAssoc::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrVarValAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValAssoc>*> (v)) {
        S_lpsrVarValAssoc elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrVarValAssoc::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrVarValAssoc::browseData (basevisitor* v)
{}

string lpsrVarValAssoc::lilyPondVarValAssocKindAsString (
  lpsrVarValAssocKind lilyPondVarValAssocKind)
{
  string result;

  switch (lilyPondVarValAssocKind) {
    // library

    case lpsrVarValAssoc::kLibraryVersion:
      result = "version";
      break;

    // MusicXML informations

    case lpsrVarValAssoc::kMusicXMLWorkNumber:
      result = "workNumber";
      break;
    case lpsrVarValAssoc::kMusicXMLWorkTitle:
      result = "workTitle";
      break;
    case lpsrVarValAssoc::kMusicXMLOpus:
      result = "opus";
      break;
    case lpsrVarValAssoc::kMusicXMLMovementNumber:
      result = "movementNumber";
      break;
    case lpsrVarValAssoc::kMusicXMLMovementTitle:
      result = "movementTitle";
      break;

    case lpsrVarValAssoc::kMusicXMLEncodingDate:
      result = "encodingDate";
      break;

    case lpsrVarValAssoc::kMusicXMLScoreInstrument:
      result = "scoreInstrument";
      break;

    case lpsrVarValAssoc::kMusicXMLMiscellaneousField:
      result = "miscellaneousField";
      break;

    // LilyPond informations

    case lpsrVarValAssoc::kLilypondDedication:
      result = "dedication";
      break;

    case lpsrVarValAssoc::kLilypondPiece:
      result = "piece";
      break;
    case lpsrVarValAssoc::kLilypondOpus:
      result = "opus";
      break;

    case lpsrVarValAssoc::kLilypondTitle:
      result = "title";
      break;
    case lpsrVarValAssoc::kLilypondSubTitle:
      result = "subTitle";
      break;
    case lpsrVarValAssoc::kLilypondSubSubTitle:
      result = "subSubTitle";
      break;

    case lpsrVarValAssoc::kLilypondInstrument:
      result = "instrument";
      break;
    case lpsrVarValAssoc::kLilypondMeter:
      result = "meter";
      break;

    case lpsrVarValAssoc::kLilypondTagline:
      result = "tagline";
      break;
    case lpsrVarValAssoc::kLilypondCopyright:
      result = "copyright";
      break;

    case lpsrVarValAssoc::kLilypondMyBreak:
      result = "myBreak";
      break;
    case lpsrVarValAssoc::kLilypondMyPageBreak:
      result = "myPageBreak";
      break;

    case lpsrVarValAssoc::kLilypondGlobal:
      result = "global";
      break;
  } // switch

  return result;
}

string lpsrVarValAssoc::commentedKindAsString (
  lpsrCommentedKind commentedKind)
{
  string result;

  switch (commentedKind) {
    case lpsrVarValAssoc::kCommentedYes:
      result = "commentedYes";
      break;
    case lpsrVarValAssoc::kCommentedNo:
      result = "commentedNo";
      break;
  } // switch

  return result;
}

string lpsrVarValAssoc::backSlashKindAsString (
  lpsrBackSlashKind backSlashKind)
{
  string result;

  switch (backSlashKind) {
    case lpsrVarValAssoc::kWithBackSlashYes:
      result = "withBackSlashYes";
      break;
    case lpsrVarValAssoc::kWithBackSlashNo:
      result = "withBackSlashNo";
      break;
  } // switch

  return result;
}

string lpsrVarValAssoc::varValSeparatorKindAsString (
  lpsrVarValSeparatorKind varValSeparatorKind)
{
  string result;

  switch (varValSeparatorKind) {
    case lpsrVarValAssoc::kVarValSeparatorSpace:
      result = "varValSeparatorSpace";
      break;
    case lpsrVarValAssoc::kVarValSeparatorEqualSign:
      result = "varValSeparatorEqualSign";
      break;
  } // switch

  return result;
}

string lpsrVarValAssoc::quotesKindAsString (
  lpsrQuotesKind quotesKind)
{
  string result;

  switch (quotesKind) {
    case lpsrVarValAssoc::kQuotesAroundValueYes:
      result = "quotesAroundValueYes";
      break;
    case lpsrVarValAssoc::kQuotesAroundValueNo:
      result = "quotesAroundValueNo";
      break;
  } // switch

  return result;
}

string lpsrVarValAssoc::endlKindAsString (
  lpsrEndlKind endlKind)
{
  string result;

  switch (endlKind) {
    case lpsrVarValAssoc::kEndlNone:
      result = "endlNone";
      break;
    case lpsrVarValAssoc::kEndlOnce:
      result = "endlOnce";
      break;
    case lpsrVarValAssoc::kEndlTwice:
      result = "endlTwice";
      break;
  } // switch

  return result;
}

void lpsrVarValAssoc::print (ostream& os) const
{
  os <<
    "LPSR VarValAssoc" <<
    endl;

  gIndenter++;

  // escape quotes if any
  string variableValue;

  for_each (
    fVariableValue.begin (),
    fVariableValue.end (),
    stringQuoteEscaper (variableValue));

  const int fieldWidth = 24;

  os << left <<
    setw (fieldWidth) <<
    "lilyPondVarValAssocKind" << " : " <<
    lilyPondVarValAssocKindAsString () <<
    endl <<

    setw (fieldWidth) <<
    "variableValue" << " : \"" <<
    variableValue <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
   "varValFontStyleKind" << " : " <<
    msrFontStyleKindAsString (
      fVarValFontStyleKind) <<
    endl <<

    setw (fieldWidth) <<
   "varValFontWeightKind" << " : " <<
    msrFontWeightKindAsString (
      fVarValFontWeightKind) <<
    endl <<

    setw (fieldWidth) <<
   "commentedKind" << " : " <<
    commentedKindAsString (
      fCommentedKind) <<
    endl <<

    setw (fieldWidth) <<
   "backSlashKind" << " : " <<
    backSlashKindAsString (
      fBackSlashKind) <<
    endl <<

    setw (fieldWidth) <<
   "varValSeparatorKind" << " : " <<
    varValSeparatorKindAsString (
      fVarValSeparatorKind) <<
    endl <<

    setw (fieldWidth) <<
   "quotesKind" << " : " <<
    quotesKindAsString (
      fQuotesKind) <<
    endl <<

    setw (fieldWidth) <<
   "unit" << " : ";

  if (fUnit.size ()) {
    os << "\"" << fUnit << "\"";
  }
  else {
    os << "none";
  }
  os << endl;

  os <<
    setw (fieldWidth) <<
   "comment" << " : ";

  if (fComment.size ()) {
    os << "\"" << fComment << "\"";
  }
  else {
    os << "none";
  }
  os << endl;

  os <<
    setw (fieldWidth) <<
   "endlKind" << " : " <<
    endlKindAsString (
      fEndlKind) <<
    endl;

  gIndenter--;
}

void lpsrVarValAssoc::printShort (ostream& os) const
{
  os <<
    "LPSR VarValAssoc" <<
    endl;

  gIndenter++;

  // escape quotes if any
  string variableValue;

  for_each (
    fVariableValue.begin (),
    fVariableValue.end (),
    stringQuoteEscaper (variableValue));

  const int fieldWidth = 24;

  os << left <<
    setw (fieldWidth) <<
    "lilyPondVarValAssocKind" << " : " <<
    lilyPondVarValAssocKindAsString () <<
    endl <<

    setw (fieldWidth) <<
    "variableValue" << " : \"" <<
    variableValue <<
    "\"" <<
    endl;

/*
  os << left <<
    setw (fieldWidth) <<
   "varValFontStyleKind" << " : " <<
    msrFontStyleKindAsString (
      fVarValFontStyleKind) <<
    endl <<

    setw (fieldWidth) <<
   "varValFontWeightKind" << " : " <<
    msrFontWeightKindAsString (
      fVarValFontWeightKind) <<
    endl;
*/

  os << left <<
    setw (fieldWidth) <<
   "commentedKind" << " : " <<
    commentedKindAsString (
      fCommentedKind) <<
    endl;

/*
  os << left <<
    setw (fieldWidth) <<
   "backSlashKind" << " : " <<
    backSlashKindAsString (
      fBackSlashKind) <<
    endl <<

    setw (fieldWidth) <<
   "varValSeparatorKind" << " : " <<
    varValSeparatorKindAsString (
      fVarValSeparatorKind) <<
    endl <<

    setw (fieldWidth) <<
   "quotesKind" << " : " <<
    quotesKindAsString (
      fQuotesKind) <<
    endl;
*/

  os << left <<
    setw (fieldWidth) <<
   "unit" << " : ";

  if (fUnit.size ()) {
    os << "\"" << fUnit << "\"";
  }
  else {
    os << "none";
  }
  os << endl;

/*
  os <<
    setw (fieldWidth) <<
   "comment" << " : ";

  if (fComment.size ()) {
    os << "\"" << fComment << "\"";
  }
  else {
    os << "none";
  }
  os << endl;

  os <<
    setw (fieldWidth) <<
   "endlKind" << " : " <<
    endlKindAsString (
      fEndlKind) <<
    endl;
*/

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrVarValAssoc& assoc) {
  assoc->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrVarValsListAssoc lpsrVarValsListAssoc::create (
  int                      inputLineNumber,
  lpsrVarValsListAssocKind varValsListAssocKind)
{
  lpsrVarValsListAssoc* o =
    new lpsrVarValsListAssoc (
      inputLineNumber,
      varValsListAssocKind);
  assert (o!=0);
  return o;
}

lpsrVarValsListAssoc::lpsrVarValsListAssoc (
  int                      inputLineNumber,
  lpsrVarValsListAssocKind varValsListAssocKind)
    : lpsrElement (inputLineNumber)
{
  fVarValsListAssocKind = varValsListAssocKind;
}

lpsrVarValsListAssoc::~lpsrVarValsListAssoc ()
{}

void lpsrVarValsListAssoc::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrVarValsListAssoc::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrVarValsListAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValsListAssoc>*> (v)) {
        S_lpsrVarValsListAssoc elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrVarValsListAssoc::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrVarValsListAssoc::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrVarValsListAssoc::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrVarValsListAssoc>*
    p =
      dynamic_cast<visitor<S_lpsrVarValsListAssoc>*> (v)) {
        S_lpsrVarValsListAssoc elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrVarValsListAssoc::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrVarValsListAssoc::browseData (basevisitor* v)
{}

string lpsrVarValsListAssoc::lilyPondVarValsListAssocValuesAsString (
  lpsrVarValsListAssocKind varValsListAssocKind)
{
  string result;

  switch (varValsListAssocKind) {
    // MusicXML informations

    case lpsrVarValsListAssoc::kMusicXMLRights:
      result = "rights";
      break;

    case lpsrVarValsListAssoc::kMusicXMLComposer:
      result = "composer";
      break;
    case lpsrVarValsListAssoc::kMusicXMLArranger:
      result = "arranger";
      break;
    case lpsrVarValsListAssoc::kMusicXMLPoet:
      result = "poet";
      break;
    case lpsrVarValsListAssoc::kMusicXMLLyricist:
      result = "lyricist";
      break;
    case lpsrVarValsListAssoc::kMusicXMLTranslator:
      result = "translator";
      break;
    case lpsrVarValsListAssoc::kMusicXMLArtist:
      result = "artist";
      break;

    case lpsrVarValsListAssoc::kMusicXMLSoftware:
      result = "software";
      break;
  } // switch

  return result;
}

string lpsrVarValsListAssoc::lilyPondVarValsListAssocValuesAsString () const
{
  stringstream s;

  list<string>::const_iterator
    iBegin = fVariableValuesList.begin (),
    iEnd   = fVariableValuesList.end (),
    i      = iBegin;

  s << "[";
  for ( ; ; ) {
    s << "\"" << (*i) << "\"";
    if (++i == iEnd) break;
    s << ", ";
  } // for
  s << "]";

  return s.str ();
}

void lpsrVarValsListAssoc::print (ostream& os) const
{
  os <<
    "LPSR VarValsListAssoc" <<
    endl;

  gIndenter++;

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "assoc kind" << " : " <<
    lilyPondVarValsListAssocKindAsString () <<
    endl <<

    setw (fieldWidth) <<
    "variable values" << " : " <<
    endl;

  gIndenter++;

  list<string>::const_iterator
    iBegin = fVariableValuesList.begin (),
    iEnd   = fVariableValuesList.end (),
    i      = iBegin;

  for ( ; ; ) {
    os << "\"" << (*i) << "\"";
    if (++i == iEnd) break;
    os << endl;
  } // for

  os << endl;

  gIndenter--;

  gIndenter--;
}

void lpsrVarValsListAssoc::printShort (ostream& os) const
{
  os <<
    "LPSR VarValsListAssoc" <<
    endl;

  gIndenter++;

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "assoc kind" << " : " <<
    lilyPondVarValsListAssocKindAsString () <<
    endl <<

    setw (fieldWidth) <<
    "variable values" << " : " <<
    endl;

  gIndenter++;

  list<string>::const_iterator
    iBegin = fVariableValuesList.begin (),
    iEnd   = fVariableValuesList.end (),
    i      = iBegin;

  for ( ; ; ) {
    os << "\"" << (*i) << "\"";
    if (++i == iEnd) break;
    os << endl;
  } // for

  os << endl;

  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrVarValsListAssoc& assoc) {
  assoc->print (os);
  return os;
}


}
