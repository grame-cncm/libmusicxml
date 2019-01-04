/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "msrDivisions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"
#include "xml2lyOptionsHandling.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrDivisions msrDivisions::create (
  int inputLineNumber,
  int divisionsPerQuarterNote)
{
  msrDivisions* o =
    new msrDivisions (
      inputLineNumber, divisionsPerQuarterNote);
  assert(o!=0);
  return o;
}

S_msrDivisions msrDivisions::createDivisionsNewbornClone ()
{
#ifdef TRACE_OPTIONS
 if (gTraceOptions->fTraceDivisions) {
    gLogIOstream <<
      "Creating a newborn clone of divisions '" <<
      divisionsAsString () <<
      "'" <<
      endl;
  }
#endif

  S_msrDivisions
    newbornClone =
      msrDivisions::create (
        fInputLineNumber,
        fDivisionsPerQuarterNote);

  return newbornClone;
}

msrDivisions::msrDivisions (
  int inputLineNumber,
  int divisionsPerQuarterNote)
    : msrElement (inputLineNumber)
{
  // set divisions per quarter note
  fDivisionsPerQuarterNote = divisionsPerQuarterNote;

  // initialize the divisions
  initializeDivisions ();
}

void msrDivisions::initializeDivisions ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDivisions) {
  gLogIOstream <<
    "Initializing divisions" <<
    ", divisionsPerQuarterNote = " << fDivisionsPerQuarterNote <<
    ", line " << fInputLineNumber <<
    endl;
  }
#endif

  gIndenter++;
  
  // forget fDurationsToDivisions's contents
  fDurationKindsToDivisions.clear ();
  
  // positive powers of 2 of a quarter note
  int bigDivisions = fDivisionsPerQuarterNote;
  
  for (int i = kQuarter; i >= kMaxima; i--) {
    /*
    gLogIOstream <<
      msrDurationAsString (msrDuration (i)) <<
      " -> " <<
      bigDivisions <<
      endl;
    */
    
    fDurationKindsToDivisions.push_front (
      make_pair (
        msrDurationKind (i), bigDivisions));
      
    bigDivisions *= 2;
  } // for

  if (fDivisionsPerQuarterNote > 1) {
    // negative powers of 2 of a quarter note
    int
      smallDivisions =
        fDivisionsPerQuarterNote / 2;
    msrDurationKind
      currentDurationKind =
        kEighth;
    
    while (smallDivisions >= 1) {
      /*
      gLogIOstream <<
        msrDurationAsString (currentDuration) <<
        " % --> " <<
        smallDivisions <<
        endl;
      */

      fDurationKindsToDivisions.push_back (
        make_pair (currentDurationKind, smallDivisions));
        
      currentDurationKind =
        msrDurationKind (
          currentDurationKind + 1);
      smallDivisions /= 2;
    } // while
  }

  // print the durations divisions if needed
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDivisions) {
    printDurationKindsDivisions (gLogIOstream);
  }
#endif

  gIndenter--;
}

int msrDivisions::durationKindAsDivisions (
  int             inputLineNumber,
  msrDurationKind durationKind)
{
  for (
    list<pair<msrDurationKind, int> >::const_iterator i =
      fDurationKindsToDivisions.begin ();
    i != fDurationKindsToDivisions.end ();
    i++) {
    if ((*i).first == durationKind)
      return
        (*i).second;
  } // for

  stringstream s;

  s <<
    "duration " << durationKind <<
    " cannot be converted to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    endl;

  printDurationKindsDivisions (s);
  
  msrInternalError (
    gXml2lyOptions->fInputSourceName,
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());

  return -1; // never reached
}

void msrDivisions::printDurationKindsDivisions (ostream& os)
{
  os <<
    "The mapping of durations to divisions with " <<
    fDivisionsPerQuarterNote << " divisions per quarter note" <<
    " is:" <<
    endl;

  gIndenter++;
  
  if (fDurationKindsToDivisions.size ()) {
    list<pair<msrDurationKind, int> >::const_iterator
      iBegin = fDurationKindsToDivisions.begin (),
      iEnd   = fDurationKindsToDivisions.end (),
      i      = iBegin;
          
    for ( ; ; ) {
      os <<
        setw (6) << left <<
        msrDurationKindAsString (
          msrDurationKind ((*i).first)) <<
        ": " <<
        setw (4) << right <<
        (*i).second;

      if (++i == iEnd) break;
      
      os << endl;
    } // for

/* JMI

    for (
      list<pair<msrDuration, int> >::const_iterator i =
        fDurationsToDivisions.begin ();
      i != fDurationsToDivisions.end ();
      i++) {
      os <<
        setw (6) << left <<
        msrDurationAsString (msrDuration((*i).first)) <<
        ": " <<
        setw (4) << right <<
        (*i).second <<
        endl;
    } // for
*/
  }
  
  else
    os <<
      "an empty list";

  os << endl;

  gIndenter--;
}

string msrDivisions::divisionsAsMsrString (
  int  inputLineNumber,
  int  divisions,
  int& numberOfDotsNeeded)
{  
  string result;

  // the result is a base duration, followed by a suffix made of
  // either a sequence of dots or a multiplication factor

  const int fieldWidth = 22;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream <<
     "--> divisionsAsMsrString ():" <<
      endl <<
      gTab << setw (fieldWidth) <<
      "inputLineNumber" << " = " << inputLineNumber <<
      endl <<
      gTab << setw (fieldWidth) <<
      "divisions" << " = " << divisions <<
      endl;
  }
#endif
    
  msrDurationKind baseDurationKind          = k1024th;
  int             baseDurationDivisions = -1;
  
  // search fDurationsToDivisions in longer to shortest order
  list<pair<msrDurationKind, int> >::const_iterator
    iBegin = fDurationKindsToDivisions.begin (),
    iEnd   = fDurationKindsToDivisions.end (),
    i      = iBegin;
    
  for ( ; ; ) {
    if (i == iEnd) {
      stringstream s;

      s <<
        "divisions " << divisions <<
        " could not be handled by divisionsAsMsrString () with:" <<
        endl;

      printDurationKindsDivisions (gLogIOstream);

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      break;
    }

    if ((*i).second <= divisions) {
      // found base duration in list
      baseDurationKind      = (*i).first;
      baseDurationDivisions = (*i).second;

      result =
        msrDurationKindAsString (baseDurationKind);
      
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream <<
            gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          endl <<
            gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
            gTab << setw (fieldWidth) <<
          "result" << " = " << result <<
          endl <<
          endl;
      }
#endif

      break;
    }
        
    // next please!
    i++;
  } // for

  int dotsNumber = 0;

  if (divisions > baseDurationDivisions) {
    // divisions is not a power of 2 of a quarter note
    
    // the next element in the list is half as long as (*i)
    int remainingDivisions =
      divisions - baseDurationDivisions;
    int nextDivisionsInList =
      baseDurationDivisions / 2;

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceDivisions) {
      gLogIOstream <<
        gTab << setw (fieldWidth) <<
        "divisions" << " = " << divisions <<
        endl <<
        gTab << setw (fieldWidth) <<
      "baseDurationDivisions" << " = " << baseDurationDivisions <<
        endl <<
        gTab << setw (fieldWidth) <<
      "nextDivisionsInList" << " = " << nextDivisionsInList <<
        endl <<
        gTab << setw (fieldWidth) <<
      "remainingDivisions" << " = " << remainingDivisions <<
        endl <<
        endl;
    }
#endif

    if (remainingDivisions < nextDivisionsInList) {
      // the suffix is a multiplication factor
      rational r (
        divisions,
        baseDurationDivisions);
      r.rationalise ();

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceDivisions) { // JMI
        gLogIOstream <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "r" << " = " << r <<
          endl <<
          endl;
      }
#endif
      
      result +=
        "*" + r.toString ();
    }

    else {
      dotsNumber = 1; // account for next element in the list
      
      while (remainingDivisions > nextDivisionsInList) {
        dotsNumber++;
        remainingDivisions -= nextDivisionsInList;
        nextDivisionsInList /= 2;
  
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceDivisions) {
          gLogIOstream <<
            gTab << setw (fieldWidth) <<
            "divisions" << " = " << divisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "baseDurationDivisions" << " = " << baseDurationDivisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "nextDivisionsInList" << " = " << nextDivisionsInList <<
            endl <<
            gTab << setw (fieldWidth) <<
            "remainingDivisions" << " = " << remainingDivisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "dotsNumber" << " = " << dotsNumber <<
            endl <<
            endl;
        }
#endif
          
        if (dotsNumber > 5 ) {
          break; // JMI
        }
      } // while
  
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "nextDivisionsInList" << " = " << nextDivisionsInList <<
          endl <<
          gTab << setw (fieldWidth) <<
          "remainingDivisions" << " = " << remainingDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "dotsNumber" << " = " << dotsNumber <<
          endl <<
          endl;
      }
#endif
          
      if (remainingDivisions - nextDivisionsInList == 0) {
        // the suffix is composed of dots
      for (int k = 0; k < dotsNumber; k++)
        result += ".";
      }
    }
  }

  numberOfDotsNeeded = dotsNumber;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream <<
      "<-- divisionsAsMsrString (): returns " << result <<
      endl <<
      endl;
  }
#endif
  
  return result;
}

/* JMI
string msrDivisions::wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     numberOfDotsNeeded)
{  
  string result;

  // the result is a base duration, followed by a suffix made of
  // either a sequence of dots or a multiplication factor

  const int fieldWidth = 22;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream << left <<
      endl <<
      "--> wholeNotesAsMsrString ():" <<
      endl <<
      gTab << setw (fieldWidth) <<
      "inputLineNumber" << " = " << inputLineNumber <<
      endl <<
      gTab << setw (fieldWidth) <<
      "wholeNotes" << " = " << wholeNotes <<
      endl <<
      endl;
  }
#endif

  // are there 0 whole notes?
  if (wholeNotes.getNumerator () == 0)
    return "ZERO wholeNotes";
    
  // compute note's number of divisions
  int divisions =
    wholeNotes.getNumerator ()
      *
    fDivisionsPerQuarterNote * 4 // hence a whole note
      /
    wholeNotes.getDenominator ();      
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream << left <<
      gTab << "divisions" << " = " << divisions <<
      endl;
  }
#endif
    
  msrDurationKind baseDurationKind          = k1024th;
  int             baseDurationDivisions = -1;
  
  // search fDurationsToDivisions in longer to shortest order
  list<pair<msrDurationKind, int> >::const_iterator
    iBegin = fDurationKindsToDivisions.begin (),
    iEnd   = fDurationKindsToDivisions.end (),
    i      = iBegin;
    
  for ( ; ; ) {
    if (i == iEnd) {
      stringstream s;

      s <<
        "divisions " << divisions <<
        " could not be handled by wholeNotesAsMsrString () with:" <<
        endl;

      printDurationKindsDivisions (gLogIOstream);

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      break;
    }

    if ((*i).second <= divisions) {
      // found base duration in list
      baseDurationKind      = (*i).first;
      baseDurationDivisions = (*i).second;

      result =
        msrDurationKindAsString (baseDurationKind);
      
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream << left <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDuration" << " = " <<
          msrDurationKindAsString (baseDurationKind) <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " <<
          baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "result" << " = " << result <<
          endl <<
          endl;
      }
#endif
      break;
    }
        
    // next please!
    i++;
  } // for

  int dotsNumber = 0;

  if (divisions > baseDurationDivisions) {
    // divisions is not a power of 2 of a quarter note
    
    // the next element in the list is half as long as (*i)
    int remainingDivisions =
      divisions - baseDurationDivisions;
    int nextDivisionsInList =
      baseDurationDivisions / 2;

#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceDivisions) {
      gLogIOstream << left <<
        gTab << setw (fieldWidth) <<
        "divisions" << " = " << divisions <<
        endl <<
        gTab << setw (fieldWidth) <<
        "baseDurationDivisions" << " = " << baseDurationDivisions <<
        endl <<
        gTab << setw (fieldWidth) <<
        "nextDivisionsInList" << " = " << nextDivisionsInList <<
        endl <<
        gTab << setw (fieldWidth) <<
        "remainingDivisions" << " = " << remainingDivisions <<
        endl <<
        endl;
    }
#endif

    if (remainingDivisions < nextDivisionsInList) {
      // the suffix is a multiplication factor
      rational r (
        divisions,
        baseDurationDivisions);
      r.rationalise ();

#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream << left <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "r" << " = " << r <<
          endl <<
        endl;
      }
#endif
      
      result +=
        "*" + r.toString ();
    }

    else {
      dotsNumber = 1; // account for next element in the list
      
      while (remainingDivisions > nextDivisionsInList) {
        dotsNumber++;
        remainingDivisions -= nextDivisionsInList;
        nextDivisionsInList /= 2;
  
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceDivisions) {
          gLogIOstream << left <<
            gTab << setw (fieldWidth) <<
            "divisions" << " = " << divisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "baseDurationDivisions" << " = " << baseDurationDivisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "nextDivisionsInList" << " = " << nextDivisionsInList <<
            endl <<
            gTab << setw (fieldWidth) <<
            "remainingDivisions" << " = " << remainingDivisions <<
            endl <<
            gTab << setw (fieldWidth) <<
            "dotsNumber" << " = " << dotsNumber <<
            endl <<
            endl;
        }
#endif
          
        if (dotsNumber > 5 ) {
          break; // JMI
        }
      } // while
  
#ifdef TRACE_OPTIONS
      if (gTraceOptions->fTraceDivisions) {
        gLogIOstream << left <<
          gTab << setw (fieldWidth) <<
          "divisions" << " = " << divisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "baseDurationDivisions" << " = " << baseDurationDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "nextDivisionsInList" << " = " << nextDivisionsInList <<
          endl <<
          gTab << setw (fieldWidth) <<
          "remainingDivisions" << " = " << remainingDivisions <<
          endl <<
          gTab << setw (fieldWidth) <<
          "dotsNumber" << " = " << dotsNumber <<
          endl <<
          endl;
      }
#endif
          
      if (remainingDivisions - nextDivisionsInList == 0) {
        // the suffix is composed of dots
        for (int k = 0; k < dotsNumber; k++)
          result += ".";
      }
    }
  }

  numberOfDotsNeeded = dotsNumber;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceDivisions) {
    gLogIOstream <<
      "<-- wholeNotesAsMsrString (): returns " << result <<
      endl <<
      endl;
  }
#endif
  
  return result;
}

string msrDivisions::wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes)
{  
  int numberOfDots; // to be ignored

  return
    wholeNotesAsMsrString (
      inputLineNumber,
      wholeNotes,
      numberOfDots);
}
*/

string msrDivisions::divisionsAsMsrString (
  int  inputLineNumber,
  int  divisions)
{
  int numberOfDots; // to be ignored

  return
    divisionsAsMsrString (
      inputLineNumber,
      divisions,
      numberOfDots);
}

string msrDivisions::tupletDivisionsAsMsrString (
  int inputLineNumber,
  int divisions,
  int actualNotes,
  int normalNotes)
{
  return
    divisionsAsMsrString (
      inputLineNumber,
      divisions * actualNotes / normalNotes);
}

string msrDivisions::tupletWholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int      actualNotes,
  int      normalNotes)
{
  return
    wholeNotesAsMsrString (
      inputLineNumber,
      wholeNotes
        *
      rational (actualNotes, normalNotes));
}

msrDivisions::~msrDivisions ()
{}

void msrDivisions::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDivisions::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDivisions::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrDivisions::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrDivisions::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrDivisions>*
    p =
      dynamic_cast<visitor<S_msrDivisions>*> (v)) {
        S_msrDivisions elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrDivisions::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrDivisions::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrDivisions& elt)
{
  elt->print (os);
  return os;
}

string msrDivisions::divisionsAsString () const
{
  stringstream s;

  s <<
    "Divisions" <<
    ", " << fDivisionsPerQuarterNote <<
    " per quarter note";

  return s.str ();
}

void msrDivisions::print (ostream& os)
{
  os <<
    divisionsAsString () <<
    endl;
}


}
