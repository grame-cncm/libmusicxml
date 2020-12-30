/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrParts.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lpsrPartBlock lpsrPartBlock::create (
  S_msrPart part)
{
  lpsrPartBlock* o = new lpsrPartBlock (
    part);
  assert (o!=0);
  return o;
}

lpsrPartBlock::lpsrPartBlock (
  S_msrPart part)
    : lpsrElement (0) // JMI
{
  // sanity check
  msgAssert (
    part != nullptr,
    "part is null");

  fPart = part;

  // set part block instrument names default values // JMI
  fPartBlockInstrumentName =
    fPart->
      getPartName ();
  fPartBlockShortInstrumentName =
    fPart->
      getPartAbbreviation ();
}

lpsrPartBlock::~lpsrPartBlock ()
{}

bool lpsrPartBlock::compareStaffBlockWithOtherElement (
  const S_lpsrStaffBlock& staffBlock,
  const S_msrElement&     otherElement)
{
  bool result = true;

  if (
    S_lpsrStaffBlock
      secondStaffBlock =
        dynamic_cast<lpsrStaffBlock*>(&(*otherElement))
    ) {
    // otherElement is a staff block
    result =
      staffBlock->getStaff ()->getStaffNumber ()
        <
      secondStaffBlock->getStaff ()->getStaffNumber ();
  }

  else if (
    S_lpsrChordNamesContext
      secondChordNamesContext =
        dynamic_cast<lpsrChordNamesContext*>(&(*otherElement))
    ) {
    // otherElement is a chord names context
    result =
      staffBlock->getStaff ()->getStaffNumber ()
        <
      secondChordNamesContext->
        getContextVoice ()->
          getVoiceStaffUpLink ()->
            getStaffNumber ();
  }

  else if (
    S_lpsrFiguredBassContext
      secondFiguredBassContext =
        dynamic_cast<lpsrFiguredBassContext*>(&(*otherElement))
    ) {
    // otherElement is a figured bass context
  }

  else {
    stringstream s;

    s <<
      "part block element '" <<
      otherElement->asShortString () <<
      " is not a staff nor a chord names or figured bass context";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      otherElement->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

/* JMI
  gLogStream <<
    endl <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    endl <<
    "compareStaffBlockWithOtherElement:" <<
    endl <<
    staffBlock <<
    endl <<
    otherElement <<
    endl <<
    "===> " << booleanAsString (result) <<
    endl << endl;
    */

  return result;
}

bool lpsrPartBlock::compareChordNamesContextWithOtherElement (
  const S_lpsrChordNamesContext& chordNamesContext,
  const S_msrElement&            otherElement)
{
  bool result = true;

  if (
    S_lpsrStaffBlock
      secondStaffBlock =
        dynamic_cast<lpsrStaffBlock*>(&(*otherElement))
    ) {
    // otherElement is a staff block
    S_msrVoice
      chordNamesVoice =
        chordNamesContext->
          getContextVoice ();

    int
      chordNamesContextStaffNumber =
        chordNamesVoice->
          getVoiceStaffUpLink ()->
            getStaffNumber (),
      secondStaffBlockStaffNumber =
        secondStaffBlock->
          getStaff ()->getStaffNumber ();

    if (chordNamesContextStaffNumber == secondStaffBlockStaffNumber) {
      // chord names should precede the staff
      result = true;
    }
    else {
      result =
        chordNamesContextStaffNumber
          <
        secondStaffBlockStaffNumber;
    }
  }

  else if (
    S_lpsrChordNamesContext
      secondChordNamesContext =
        dynamic_cast<lpsrChordNamesContext*>(&(*otherElement))
    ) {
    // otherElement is a chord names context
    S_msrVoice
      chordNamesVoice =
        chordNamesContext->
          getContextVoice (),
      secondChordNamesVoice =
        secondChordNamesContext->
          getContextVoice ();

    int
      chordNamesContextStaffNumber =
        chordNamesVoice->
          getVoiceStaffUpLink ()->
            getStaffNumber (),
      secondChordNamesContextStaffNumber =
        secondChordNamesVoice->
          getVoiceStaffUpLink ()->
            getStaffNumber ();

    if (chordNamesContextStaffNumber == secondChordNamesContextStaffNumber) {
      result =
        chordNamesVoice->getVoiceNumber ()
          <
        secondChordNamesVoice->getVoiceNumber ();
    }
    else {
      result =
        chordNamesContextStaffNumber
          <
        secondChordNamesContextStaffNumber;
    }
  }

  else if (
    S_lpsrFiguredBassContext
      secondFiguredBassContext =
        dynamic_cast<lpsrFiguredBassContext*>(&(*otherElement))
    ) {
    // otherElement is a figured bass context
  }

  else {
    stringstream s;

    s <<
      "part block element '" <<
      otherElement->asShortString () <<
      " is not a staff nor a chord names or figured bass context";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      otherElement->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

/* JMI
  gLogStream <<
    endl <<
    "!!!!!!!!!!!!!!!!!!!!!!!!!" <<
    endl <<
    "compareChordNamesContextWithOtherElement:" <<
    endl <<
    chordNamesContext <<
    endl <<
    otherElement <<
    endl <<
    "===> " << booleanAsString (result) <<
    endl << endl;
*/

  return result;
}

bool lpsrPartBlock::compareElementsToHaveHarmoniesAboveCorrespondingStaff (
  const S_msrElement& first,
  const S_msrElement& second)
{
  bool result = true;

  if (
    S_lpsrStaffBlock
      firstStaffBlock =
        dynamic_cast<lpsrStaffBlock*>(&(*first))
    ) {
    // first is a staff block
    result =
      compareStaffBlockWithOtherElement (
        firstStaffBlock,
        second);
  }

  else if (
    S_lpsrChordNamesContext
      firstChordNamesContext =
        dynamic_cast<lpsrChordNamesContext*>(&(*first))
    ) {
    // first is a chord names context
    result =
      compareChordNamesContextWithOtherElement (
        firstChordNamesContext,
        second);
  }

  else if (
    S_lpsrFiguredBassContext
      firstFiguredBassContext =
        dynamic_cast<lpsrFiguredBassContext*>(&(*first))
    ) {
    // first is a figured bass context
    result = true;
  }

  else {
    stringstream s;

    s <<
      "part block element '" <<
      first->asShortString () <<
      " is not a staff nor a chord names or figured bass context";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      first->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

void lpsrPartBlock::appendStaffBlockToPartBlock (
  S_lpsrStaffBlock staffBlock)
{
  fPartBlockElementsList.push_back (staffBlock);
}

void lpsrPartBlock::appendChordNamesContextToPartBlock (
  int                     inputLineNumber,
  S_lpsrChordNamesContext chordNamesContext)
{
  // appent chordNamesContext to the part block elements list
  fPartBlockElementsList.push_back (chordNamesContext);

  // sort the list if necessary
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Sorting the voices in part block for part \"" <<
      fPart->getPartCombinedName () << "\"" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  gLogStream <<
    endl << endl <<
    "@@@@@@@@@@@@@@@@ fPartBlockElementsList contains initially:" <<
    endl << endl;

  for (
    list<S_msrElement>::const_iterator i = fPartBlockElementsList.begin ();
    i != fPartBlockElementsList.end ();
    i++
  ) {
    S_msrElement
      element = (*i);

    gLogStream <<
      element->asShortString () <<
      endl;
  } // for
  gLogStream <<
    endl << endl;
*/

  // sort fPartBlockElementsList, to have harmonies just before
  // the corresponding voice
  if (fPartBlockElementsList.size ()) {
    fPartBlockElementsList.sort (
      compareElementsToHaveHarmoniesAboveCorrespondingStaff);
  }

/* JMI
  gLogStream <<
    endl << endl <<
    "@@@@@@@@@@@@@@@@ fPartBlockElementsList contains after sort:" <<
    endl << endl;

  for (
    list<S_msrElement>::const_iterator i = fPartBlockElementsList.begin ();
    i != fPartBlockElementsList.end ();
    i++
  ) {
    S_msrElement
      element = (*i);

    gLogStream <<
      element->asShortString () <<
      endl;
  } // for
  gLogStream <<
    endl << endl;
*/
}

void lpsrPartBlock::appendFiguredBassContextToPartBlock (
  S_lpsrFiguredBassContext context)
{
  fPartBlockElementsList.push_back (context);
}

void lpsrPartBlock::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrPartBlock::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartBlock>*> (v)) {
        S_lpsrPartBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrPartBlock::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrPartBlock::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrPartBlock::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartBlock>*> (v)) {
        S_lpsrPartBlock elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrPartBlock::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrPartBlock::browseData (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrPartBlock::browseData ()" <<
      endl;
  }
#endif

  for (
    list<S_msrElement>::const_iterator i = fPartBlockElementsList.begin ();
    i != fPartBlockElementsList.end ();
    i++
  ) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% <== lpsrPartBlock::browseData ()" <<
      endl;
  }
#endif
}

void lpsrPartBlock::print (ostream& os) const
{
  os <<
    "PartBlock" << " " <<
    "for part " << fPart->getPartCombinedName () <<
    ", " <<
    singularOrPlural (
      fPartBlockElementsList.size (), "element", "elements") <<
    endl;

  gIndenter++;

  const unsigned int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) << string ("") + "partName" << " = \"" <<
    fPart->getPartName () << "\"" <<
    endl <<
    setw (fieldWidth) << string ("") + "partAbbreviation" << " = \"" <<
    fPart->getPartAbbreviation () << "\"" <<
    endl <<

    setw (fieldWidth) << "partBlockInstrumentName" << " = \"" <<
    fPartBlockInstrumentName <<
    "\"" <<
    endl <<
    setw (fieldWidth) << "partBlockShortInstrumentName" << " = \"" <<
    fPartBlockShortInstrumentName <<
    "\"" <<
    endl;

  os << endl;

  if (fPartBlockElementsList.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fPartBlockElementsList.begin (),
      iEnd   = fPartBlockElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrPartBlock& scr)
{
  scr->print (os);
  return os;
}


}
