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
#include <stdlib.h>     /* abort, NULL */
#include <cassert>
#include <vector>

#include "rational.h"

#include <msrUtilities.h>

using namespace std;

namespace MusicXML2 {


//______________________________________________________________________________
indenter::indenter (string spacer)
{
  fIndent = 0;
  fSpacer = spacer;
}

indenter::~indenter () {}

indenter& indenter::operator++ (const int value)
{
  fIndent++;
  return *this;
}

indenter& indenter::operator-- (const int value)
{
  fIndent--;
  
  if (fIndent < 0) {
    cerr <<
      endl <<
      "% ### Indentation has become negative..." <<
      endl << endl;
  //  assert(false);
  }
    
  return *this;
}

ostream& operator<< (ostream& os, const indenter& idtr) {
  idtr.print(os);
  return os;
}

void indenter::print (ostream& os) const
{ 
  int i = fIndent;
  while (i-- > 0) os << fSpacer;
}

indenter indenter::gIndenter;

//______________________________________________________________________________
outputLineElementsCounter::outputLineElementsCounter (int maxElementsPerLine)
{
  fElementsCounter    = 0;
  fMaxElementsPerLine = maxElementsPerLine;
}

outputLineElementsCounter::~outputLineElementsCounter () {}

void outputLineElementsCounter::setMaxElementsPerLine (
  int maxElementsPerLine)
{
  fMaxElementsPerLine = maxElementsPerLine;
}

void outputLineElementsCounter::reset (int value)
{
  fElementsCounter = value;
}

outputLineElementsCounter& outputLineElementsCounter::operator++ (int value)
{
  fElementsCounter++;
  
  if (fElementsCounter >= fMaxElementsPerLine) {
    cerr <<
      "%{!%}" <<
      endl <<
      indenter::gIndenter;

    fElementsCounter = 0;
  }
    
  return *this;
}

bool outputLineElementsCounter::operator< (int value)
{
  return fElementsCounter < value;
}

bool outputLineElementsCounter::operator<= (int value)
{
  return fElementsCounter <= value;
}

bool outputLineElementsCounter::operator== (int value)
{
  return fElementsCounter == value;
}

bool outputLineElementsCounter::operator!= (int value)
{
  return fElementsCounter != value;
}

bool outputLineElementsCounter::operator>= (int value)
{
  return fElementsCounter >= value;
}

bool outputLineElementsCounter::operator> (int value)
{
  return fElementsCounter > value;
}

outputLineElementsCounter
outputLineElementsCounter::gOutputLineElementsCounter;

//______________________________________________________________________________
string int2EnglishWord (int n)
{
  stringstream s;

  if (n < 0) {
    s << "Minus_";
    n = -n;

    assert(false); // JMI
  }

  if     (n >= 1000) {
    int nDiv1000    = n / 1000;
    int nModulo1000 = n % 1000;

    return
      int2EnglishWord (nDiv1000) +
      "Thousand" +
      int2EnglishWord (nModulo1000);    
  }

  else if (n >= 100) {
  //return "LargerThanNinetyNine";
    int nDiv100    = n / 100;
    int nModulo100 = n % 100;

    return
      int2EnglishWord (nDiv100) +
      "HundredAnd" +
      int2EnglishWord (nModulo100);    
  }

  else {
    switch (n) {
      case 0:
        s << "Zero";
        break;
      case 1:
        s << "One";
        break;
      case 2:
        s << "Two";
        break;
      case 3:
        s << "Three";
        break;
      case 4:
        s << "Four";
        break;
      case 5:
        s << "Five";
        break;
      case 6:
        s << "Six";
        break;
      case 7:
        s << "Seven";
        break;
      case 8:
        s << "Eight";
        break;
      case 9:
        s << "Nine";
        break;
     case 10:
        s << "Ten";
        break;
      case 11:
        s << "Eleven";
        break;
      case 12:
        s << "Twelve";
        break;
      case 13:
        s << "Thirteen";
        break;
      case 14:
        s << "Fourteen";
        break;
      case 15:
        s << "Fifteen";
        break;
      case 16:
        s << "Sixteen";
        break;
      case 17:
        s << "Seventeen";
        break;
      case 18:
        s << "Eighteen";
        break;
      case 19:
        s << "Nineteen";
        break;
        
      default: {
        // n >= 20
        int nDiv10    = n / 10;
        int nModulo10 = n % 10;
        
        switch (nDiv10) {
          case 2:
            s << "Twenty";
            break;
          case 3:
            s << "Thirty";
            break;
          case 4:
            s << "Fourty";
            break;
          case 5:
            s << "Fifty";
            break;
          case 6:
            s << "Sixty";
            break;
          case 7:
            s << "Seventy";
            break;
          case 8:
            s << "Eighty";
            break;
          case 9:
            s << "Ninety";
            break;
        } // switch
        s << int2EnglishWord (nModulo10);
      } // default
    } // switch
  }
  
  return s.str();
}

//______________________________________________________________________________
string stringNumbersToEnglishWords (string str)
{
  if (! str.size ())
    return "NONE";
  
  enum workState {
    kInitialState, kWorkingOnDigits, kWorkingOnNonDigits };
      
  vector<string> chunks;
  vector<int>    states;
    
  workState state = kInitialState;
  
  string::const_iterator
    iBegin = str.begin(),
    iEnd   = str.end(),
    i      = iBegin;
  for ( ; ; ) {
    char ch = *i;
    if( isdigit(ch)) {
      // digit
      if (state != kWorkingOnDigits) {
        // create a new chunck for digits
        chunks.push_back ("");
        states.push_back (kWorkingOnDigits);
        state = kWorkingOnDigits;
      }
      chunks.back().push_back(ch);
    }
    else {
      // non digit
      if (state != kWorkingOnNonDigits) {
        // create a new chunck for non digits
        chunks.push_back ("");
        states.push_back (kWorkingOnNonDigits);
        state = kWorkingOnNonDigits;
      }
      chunks.back().push_back(ch);
    }
    if (++i == iEnd) break;
  } // for
   
  string result = "";

  for (unsigned int i = 0; i < chunks.size(); i++) {
    if (states[i] == kWorkingOnDigits)
      result += int2EnglishWord(atoi(chunks[i].c_str()));
    else
      result += chunks[i];
  } // for
    
  return result;
};

//______________________________________________________________________________
int consumeDecimalNumber (
  std::string::const_iterator  theStringIterator,
  std::string::const_iterator& remainingStringIterator,
  bool   debugMode)
{
  std::string::const_iterator cursor = theStringIterator;
  int    number = 0;

  if (! isdigit (*cursor))
    cerr <<
      "consumeDecimalNumber (" << *cursor <<
      "), " << *cursor << " is no decimal digit!" <<
      endl;

  while (isdigit (* cursor)) {
    if (debugMode)
      cerr <<
        "--> consumeDecimalNumber: cursor = |" <<
        *cursor <<
        "|" <<
        endl;

    number = number*10 + (*cursor-'0');
    
    cursor++;
  } // while

  remainingStringIterator = cursor;

  if (debugMode)
    cerr <<
      "--> consumeDecimalNumber: number = " << number <<
      ", *remainingStringIterator = |" << *remainingStringIterator <<
      "|" <<
      endl;

  return number;
}

//______________________________________________________________________________
set<int> decipherNumbersSetSpecification (
  std::string theString,
  bool        debugMode)
{
//  A numbersSetSpecification sample is: "7,15-19,^16-17"

  std::set<int> selectedNumbers;
  
  if (debugMode)
    cerr <<
      "--> decipherNumbersSetSpecification, theString = |" << theString <<
      "|" <<
      endl;

  std::string::const_iterator
    cursor = theString.begin();

  while (1) {
    if (debugMode)
      cerr <<
        "--> decipherNumbersSpecification: cursor = |" <<
        *cursor << "|" <<
        endl;

    int negated = 0;

    if (*cursor == '^') {
      cursor++;
      negated = 1;
    }

    int
      intervalStartNumber =
        consumeDecimalNumber (cursor, cursor, debugMode),
      intervalEndNumber;

    if (*cursor == '-') {
      cursor++;

      if (debugMode)
        cerr <<
          "--> decipherNumbersSpecification after '-' : cursor = |" <<
          *cursor <<
          "|" <<
          endl << endl;
  
      intervalEndNumber =
        consumeDecimalNumber (cursor, cursor, debugMode);
    }

    else {
      intervalEndNumber = intervalStartNumber;
    }

    if (debugMode)
      cerr <<
        "--> decipherNumbersSpecification, intervalStartNumber = " << intervalStartNumber <<
        ", intervalEndNumber = " << intervalEndNumber <<
        ": *cursor = |" << *cursor << "|" <<
        endl;

    for (int i = intervalStartNumber; i <= intervalEndNumber; i ++) {
      if (negated)
        selectedNumbers.erase (i);
      else
        selectedNumbers.insert (i);
    } // for

    if (*cursor != ',') {
      if (debugMode)
        cerr <<
          "--> decipherNumbersSpecification, after non ',' : cursor = |" <<
          *cursor <<
          "|" <<
          endl << endl;
      break; 
    }

    cursor++;

    if (debugMode)
      cerr <<
        "--> decipherNumbersSpecification after ',' : cursor = |" <<
        *cursor <<
        "|"
        << endl << endl;
  } // while 

  if (* cursor != '\0') {
    cerr <<
      "--> Extraneous characters |" << *cursor <<
      "| in numbers spec" <<
      endl << endl;
  }

  return selectedNumbers;
}

//______________________________________________________________________________
std::list<int> extractNumbersFromString (
  std::string theString, // can contain "1, 2, 17"
  bool        debugMode)
{
  std::list<int> foundNumbers;

  if (debugMode)
    cerr <<
      "--> extractNumbersFromString, theString = |" << theString <<
      "|" <<
      endl;

  std::string::const_iterator
    cursor = theString.begin();

  while (1) {
    if (cursor == theString.end())
      break;
      
    if (debugMode)
      cerr <<
        "--> extractNumbersFromString: cursor = |" <<
        *cursor << "|" <<
        endl;

    if (isdigit (*cursor)) {
      // consume a decimal number
      int n = 0;
      while (isdigit (*cursor)) {
        n = n * 10 + (*cursor - '0');
        cursor++;
      } // while

      // append the number to the list
      foundNumbers.push_back (n);
    }
    else {
      cursor++;
    }
  } // while

  return foundNumbers;
}


//______________________________________________________________________________
std::pair<std::string, std::string> extractNamesPairFromString (
  std::string theString, // can contain "P1 = Bassoon"
  char        separator,
  bool        debugMode)
{
  std::string name1;
  std::string name2;

  if (debugMode)
    cerr <<
      "--> extractNamesPairFromString, theString = |" << theString <<
      "|" <<
      endl;

  std::string::const_iterator
    cursor = theString.begin();

  // fetch name1
  while (1) {
    if (cursor == theString.end())
      break;
      
    if (debugMode)
      cerr <<
        "--> extractNamesPairFromString: cursor = |" <<
        *cursor << "|" <<
        endl;

    if ((*cursor) == separator) {
      // found the separator
      break;
    }
    
    // append the character to name1
    name1 += *cursor;
    cursor++;
  } // while

  name1 = trim (name1);
  if (! name1.size ()) {
    // found an empty name1
    cerr <<
      "### ERROR: the first name before the " << separator <<
      " separator is empty in '" << theString << "'" <<
      endl;
  }

  if (cursor == theString.end())
    cerr <<
      "### ERROR: the " << separator <<
      " separator is missing in string '" <<
      theString << "'" <<
      endl;
  else
    // overtake the separator
    cursor++;

  // fetch name2
  while (1) {
    if (cursor == theString.end())
      break;
      
    if (debugMode)
      cerr <<
        "--> extractNamesPairFromString: cursor = |" <<
        *cursor << "|" <<
        endl;

    if ((*cursor) == '=') {
      // found the separator
      cerr <<
        "### ERROR: the " << separator <<
        " separator occurs more than once in string '" <<
        theString << "'" <<
        endl;
      break;
    }
    
    // append the character to name2
    name2 += *cursor;
    cursor++;
  } // while

  name2 = trim (name2);
  if (! name2.size ()) {
    // found an empty name2
    cerr <<
      "### ERROR: the second name after the " << separator <<
      " separator is empty in '" << theString << "'" <<
      endl;
  }

  return make_pair (name1, name2);
}

//______________________________________________________________________________
string divisionsAsMSRDuration (
  int     divisions,
  int     divisionsPerWholeNote,
  string& errorMessage,
  bool    debugMode)
{
  // MusicXML divisions are per quater note,
  // MSR and LilyPond durations are in whole notes

  //debugMode = true; // TEMP, for tests
  
  if (divisions <= 0) {
    stringstream s;
    
    s << 
      "### divisionsAsMSRDuration ():" <<
      endl <<
      "divisions = " << divisions <<
      ": should be positive !" <<
      endl;
      
    errorMessage = s.str();
    
    return "?";
  }

  if (divisionsPerWholeNote <= 0) {
    stringstream s;
    
    s << 
      "### divisionsAsMSRDuration ():" <<
      endl <<
      "divisions = " << divisions <<
      endl <<
      "divisionsPerWholeNote = " << divisionsPerWholeNote <<
      ": should be positive !" <<
      endl;
      
    errorMessage = s.str();

    return "?";
  }

  stringstream s;

  div_t
    divresult =
      div (divisions, divisionsPerWholeNote);
       
  int div = divresult.quot;
  int mod = divresult.rem;

  if (debugMode)
    cerr << endl <<
      "% --> divisions = " << divisions <<
      ", divisionsPerWholeNote = " << divisionsPerWholeNote << endl <<
      "% --> div = " << div << ", mod = " << mod << endl;

  int divisionsAccountedFor = divisions - mod;
  int remainingDivisions    = mod;

  int limit;
  
  switch (div) {
    case 15:
    case 14:
    case 13:
    case 12:
    case 11:
    case 10:
    case 9:
    case 8:
      s << "\\maxima";
      break;
      
    case 7:
    case 6:
    case 5:
    case 4:
      s << "\\longa";
      break;
      
    case 3:
    case 2:
      s << "\\breve";
      break;
      
    case 1:
      s << "1";
      break;
      
    case 0:
      {
      // this duration is shorter than a whole note,
      // display it as a fration of such

      // compute the reciprocal, i.e. the fraction's denominator,
      // trying 1/2, 1/4, 1/8... in order
      int
        reciprocal = 2, // half note
        n          = divisions * 2;

      if (debugMode)
        cerr <<
          "% --> reciprocal = " << reciprocal << ", n = " << n <<
          endl;
      
      while (n < divisionsPerWholeNote) {
        if (debugMode)
          cerr <<
            "% --> n = " << n << ", reciprocal = " << reciprocal <<
            endl;
            
        reciprocal *= 2;
        n *= 2;
      } // while

      // generate the reciprocal
      s << reciprocal;

      divisionsAccountedFor = divisionsPerWholeNote / reciprocal;
      remainingDivisions    = mod - divisionsAccountedFor;

      limit = divisionsAccountedFor * 2;
        // an infinite sequence of dots tends to this

      }
      break;
      
    default:
      {
      stringstream s;

      rational r (divisions, divisionsPerWholeNote);
      r.rationalise ();
      
      s << 
        "% note divisions " <<
          divisions <<
        "/" <<
        divisionsPerWholeNote <<
        " (" << r.getNumerator () << "/" <<
        r.getDenominator () << ")" <<
        " exceeds a maxima" << endl;
        
      errorMessage = s.str();

      return "?";
      }
  } // switch

  if (debugMode)
    cerr <<
      endl << endl <<
      "% --> we've got '" << s.str() << "'" << endl << 
      "% --> divisionsAccountedFor = " << divisionsAccountedFor << endl <<
      "% --> remainingDivisions    = " << remainingDivisions << endl <<
      endl;

  // compute the number of dots if any
  if (remainingDivisions > 0) {
    int
      m               = remainingDivisions,
      currentWeight   = divisionsAccountedFor / 2,
      cumulatedWeight = 0,
      numberOfDots    = 0;

    while (cumulatedWeight < remainingDivisions) {
      numberOfDots++;
      
      cumulatedWeight += currentWeight;
      currentWeight /= 2;
      
      if (debugMode)
        cerr <<
          "% cumulatedWeight = " << cumulatedWeight <<
          ", currentWeight = " << currentWeight <<
          ", " << numberOfDots <<
          endl;      
    } // while
    
    for (int i = 0; i < numberOfDots; i++)
      s << ".";
  }

  return s.str();
}

//______________________________________________________________________________
string divisionsAsMSRDuration (
  int     divisions,
  int     divisionsPerWholeNote,
  int     inputSourceSuppliedNumberOfDots,
  int&    computedNumberOfDots,
  string& errorMessage,
  bool    debugMode)
{
  // MusicXML divisions are per quater note,
  // MSR and LilyPond durations are in whole notes

  computedNumberOfDots = 0;

  //debugMode = true; // TEMP, for tests
  
  if (divisions <= 0) {
    stringstream s;
    
    s << 
      "### divisionsAsMSRDuration ():" <<
      endl <<
      "divisions = " << divisions <<
      ": should be positive !" <<
      endl;
      
    errorMessage = s.str();
    
    return "?";
  }

  if (divisionsPerWholeNote <= 0) {
    stringstream s;
    
    s << 
      "### divisionsAsMSRDuration ():" <<
      endl <<
      "divisions = " << divisions <<
      endl <<
      "divisionsPerWholeNote = " << divisionsPerWholeNote <<
      ": should be positive !" <<
      endl;
      
    errorMessage = s.str();
    
    return "?";
  }

  stringstream s;

  div_t
    divresult =
      div (divisions, divisionsPerWholeNote);
       
  int div = divresult.quot;
  int mod = divresult.rem;

  if (debugMode)
    cerr << endl <<
      "% --> divisions = " << divisions <<
      ", divisionsPerWholeNote = " << divisionsPerWholeNote << endl <<
      "% --> div = " << div << ", mod = " << mod << endl;

  int divisionsAccountedFor = divisions - mod;
  int remainingDivisions    = mod;

  int limit;
  
  switch (div) {
    case 15:
    case 14:
    case 13:
    case 12:
    case 11:
    case 10:
    case 9:
    case 8:
      s << "\\maxima";
      break;
      
    case 7:
    case 6:
    case 5:
    case 4:
      s << "\\longa";
      break;
      
    case 3:
    case 2:
      s << "\\breve";
      break;
      
    case 1:
      s << "1";
      break;
      
    case 0:
      {
      // this note is shorter than a whole note,
      // display it as a fration of such

      // compute the reciprocal, i.e. the fraction's denominator,
      // trying 1/2, 1/4, 1/8... in order
      int
        reciprocal = 2, // half note
        n          = divisions * 2;

      if (debugMode)
        cerr <<
          "% --> reciprocal = " << reciprocal << ", n = " << n <<
          endl;
      
      while (n < divisionsPerWholeNote) {
        if (debugMode)
          cerr <<
            "% --> n = " << n << ", reciprocal = " << reciprocal <<
            endl;
            
        reciprocal *= 2;
        n *= 2;
      } // while

      // generate the reciprocal
      s << reciprocal;

      divisionsAccountedFor = divisionsPerWholeNote / reciprocal;
      remainingDivisions    = mod - divisionsAccountedFor;

      limit = divisionsAccountedFor * 2;
        // an infinite sequence of dots tends to this

      }
      break;
      
    default:
      {
      stringstream s;
       // JMI
      s << 
        "% note divisions " <<
          divisions <<
        "/" <<
        divisionsPerWholeNote <<
        " exceeds a maxima" << endl;
        
      errorMessage = s.str();

      return "?";
      }
  } // switch

// TEST  debugMode = true;

  if (debugMode)
    cerr <<
      endl << endl <<
      "% --> we've got '" << s.str() << "'" << endl << 
      "% --> divisionsAccountedFor = " << divisionsAccountedFor << endl <<
      "% --> remainingDivisions    = " << remainingDivisions << endl <<
      endl;

  // compute the number of dots if any
  if (remainingDivisions > 0) {
    int
      m               = remainingDivisions,
      currentWeight   = divisionsAccountedFor / 2,
      cumulatedWeight = 0,
      numberOfDots    = 0;

    while (cumulatedWeight < remainingDivisions) {
      numberOfDots++;
      
      cumulatedWeight += currentWeight;
      currentWeight /= 2;
      
      if (debugMode)
        cerr <<
          "% cumulatedWeight = " << cumulatedWeight <<
          ", currentWeight = " << currentWeight <<
          ", " << numberOfDots <<
          endl;

      if (numberOfDots > 1) // JMI, safety
        break;
    } // while
/*
    if (debugMode)
      cerr << endl << "% limit = " << limit << endl;
    
    while (m < limit) {
 // TEMP     s << ".";
      numberOfDots++;
      m *= 2;
      if (debugMode)
        cerr << "% m = " << m << ", numberOfDots = " << numberOfDots << endl;
    } // while
 */

    
//    for (int i = 0; i < numberOfDots; i++)
    for (int i = 0; i < inputSourceSuppliedNumberOfDots; i++) // TEMP JMI
      s << ".";

    if (numberOfDots != inputSourceSuppliedNumberOfDots) {
      s <<
        endl <<
        indenter::gIndenter <<
        " %{ ### " <<
        "divs. " << divisions <<
        "/" <<
        "dpwn: " << divisionsPerWholeNote <<
        ", rem. " << remainingDivisions <<
        ", lim. " << limit <<
        ", " << numberOfDots <<
        " != " << inputSourceSuppliedNumberOfDots <<
        " %} " <<
        endl;
        
      /*
      if (debugMode) {
        s << endl;
        
        if (divisions == 1)
          s << "1 division needs ";
        else
          s << divisions << " divisions need ";
          
        if (inputSourceSuppliedNumberOfDots == 1)
          s << "1 dot ";
        else
          s << inputSourceSuppliedNumberOfDots << " dots ";
    
        s <<
          "with " << divisionsPerWholeNote << " per whole note" <<
          ", not " << computedNumberOfDots <<
          endl;
          */
    }

    computedNumberOfDots = numberOfDots;
  }

  return s.str();
}

//______________________________________________________________________________
int noteTypeAsDivisions (
  string  noteType,
  int     divisionsPerWholeNote,
  string& errorMessage,
  bool    debugMode)
{
  int result;

  //debugMode = true; // for tests
  
  if      (noteType == "256th")   { result = divisionsPerWholeNote / 256; }
  else if (noteType == "128th")   { result = divisionsPerWholeNote / 128; } 
  else if (noteType == "64th")    { result = divisionsPerWholeNote / 64; } 
  else if (noteType == "32nd")    { result = divisionsPerWholeNote / 32; } 
  else if (noteType == "16th")    { result = divisionsPerWholeNote / 16; } 
  else if (noteType == "eighth")  { result = divisionsPerWholeNote / 8; } 
  else if (noteType == "quarter") { result = divisionsPerWholeNote / 4; } 
  else if (noteType == "half")    { result = divisionsPerWholeNote / 2; } 
  else if (noteType == "whole")   { result = divisionsPerWholeNote / 1; } 
  else if (noteType == "breve")   { result = divisionsPerWholeNote * 2; } 
  else if (noteType == "long")    { result = divisionsPerWholeNote * 4; }
  else {
    stringstream s;
    
    s <<
      endl << 
      "--> unknown tuplet member type " << noteType <<
      endl;
      
   errorMessage = s.str();

   return 0;
  }

  if (debugMode)
    cerr << endl <<
      "% --> noteTypeAsDivisions ()" << endl <<
      "% noteType = " << noteType << endl <<
      "divisionsPerWholeNote = " << divisionsPerWholeNote << endl <<
      "result = " << result << endl <<
      endl;

  return result;
}

//______________________________________________________________________________
string noteTypeAsMSRDuration (
  string  noteType,
  string& errorMessage)
{
  string result;
  
  if      (noteType == "256th")   { result =   "256"; }
  else if (noteType == "128th")   { result =   "128"; } 
  else if (noteType == "64th")    { result =    "64"; } 
  else if (noteType == "32nd")    { result =    "32"; } 
  else if (noteType == "16th")    { result =    "16"; } 
  else if (noteType == "eighth")  { result =     "8"; } 
  else if (noteType == "quarter") { result =     "4"; } 
  else if (noteType == "half")    { result =     "2"; } 
  else if (noteType == "whole")   { result =     "1"; } 
  else if (noteType == "breve")   { result = "breve"; } 
  else if (noteType == "long")    { result =  "long"; }
  else {
    stringstream s;
    
    s << 
      endl << 
      "--> unknown tuplet member type " << noteType <<
      endl;
      
   errorMessage = s.str();

   return "?";
  }

  return result;
}

//______________________________________________________________________________
string quoteStringIfNonAlpha (
  string     theString)
{
  string result;
  bool   stringShouldBeQuoted = false;
  
  for (
    string::const_iterator i = theString.begin ();
    i != theString.end ();
    i++) {
    result += (*i);

    if (! isalpha ((*i)))
      stringShouldBeQuoted = true;
      
    if ((*i) == '"') {
      result += '"';
      stringShouldBeQuoted = true;
    }
  } // for

  if (stringShouldBeQuoted)
    return "\"" + result + "\"";
  else
    return result;
}

//______________________________________________________________________________
string booleanAsString (bool value)
{
  return
    string (
      value
        ? "true"
        : "false");
}

//______________________________________________________________________________
string singularOrPlural (
  int number, string singularName, string pluralName)
{
  stringstream s;

  s <<
    number << " ";
  
  if (number <= 1)
    s <<
      singularName;
  else
    s <<
      pluralName;
      
  return s.str ();
}


}
