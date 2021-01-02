/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "utilities.h"

#include "lpsrComments.h"

#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lpsrComment lpsrComment::create (
  int                inputLineNumber,
  string             contents,
  lpsrCommentGapKind commentGapKind)
{
  lpsrComment* o = new
    lpsrComment (
      inputLineNumber, contents, commentGapKind);
  assert (o!=0);
  return o;
}

lpsrComment::lpsrComment (
  int                inputLineNumber,
  string             contents,
  lpsrCommentGapKind commentGapKind)
    : lpsrElement (inputLineNumber)
{
  fContents       = contents;
  fCommentGapKind = commentGapKind;
}

lpsrComment::~lpsrComment ()
{}

void lpsrComment::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrComment::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrComment::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrComment::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrComment::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrComment>*
    p =
      dynamic_cast<visitor<S_lpsrComment>*> (v)) {
        S_lpsrComment elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrComment::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrComment::browseData (basevisitor* v)
{}

void lpsrComment::print (ostream& os) const
{
  os <<
    "Comment" <<
    endl;

  ++gIndenter;

  os <<
    "% " << fContents <<
    endl;

  if (fCommentGapKind == kGapAfterwards)
    os << endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_lpsrComment& elt)
{
  elt->print (os);
  return os;
}


}
