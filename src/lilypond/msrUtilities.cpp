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

ostream& operator<< (ostream& os, const indenter& idtr) {
  idtr.print(os);
  return os;
}

void indenter::print(ostream& os) const
{ 
  int i = fIndent;
  while (i-- > 0) os << fSpacer;
}

// JMI indenter indenter::gIndenter;

//______________________________________________________________________________
string int2EnglishWord (int n)
{
  stringstream s;

  if (n < 0) {
    s << "Minus_";
    n = -n;
  }

  if (n > 99) return "LargerThanNinetyNine";
  

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
      int digit1 = n / 10;
      int digit2 = n % 10;
      
      switch (digit1) {
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
      s << int2EnglishWord(digit2);
    } // default
  } // switch
  
  return s.str();
}

//______________________________________________________________________________
string stringNumbersToEnglishWords (string str)
{
  if (! str.size ())
    return "NONE";
  
  typedef enum { kInitialState, kWorkingOnDigits, kWorkingOnNonDigits } workState;
      
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
        chunks.push_back("");
        states.push_back(kWorkingOnDigits);
        state = kWorkingOnDigits;
      }
      chunks.back().push_back(ch);
    }
    else {
      // non digit
      if (state != kWorkingOnNonDigits) {
        // create a new chunck for non digits
        chunks.push_back("");
        states.push_back(kWorkingOnNonDigits);
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
    cout <<
      "consumeDecimalNumber (" << *cursor <<
      "), " << *cursor << " is no decimal digit!" <<
      endl;

  while (isdigit (* cursor)) {
    if (debugMode)
      cout <<
        "--> consumeDecimalNumber: cursor = |" <<
        *cursor <<
        "|" <<
        endl;

    number = number*10 + (*cursor-'0');
    
    cursor++;
  } // while

  remainingStringIterator = cursor;

  if (debugMode)
    cout <<
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
    cout <<
      "--> decipherNumbersSetSpecification, theString = |" << theString <<
      "|" <<
      endl;

  std::string::const_iterator
    cursor = theString.begin();

  while (1) {
    if (debugMode)
      cout <<
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
        cout <<
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
      cout <<
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
      cout <<
        "--> decipherNumbersSpecification, after non ',' : cursor = |" <<
        *cursor <<
        "|" <<
        endl << endl;
      break; 
    }

    cursor++;

    cout <<
      "--> decipherNumbersSpecification after ',' : cursor = |" <<
      *cursor <<
      "|"
      << endl << endl;
  } // while 

  if (* cursor != '\0') {
    cout <<
      "--> Extraneous characters |" << *cursor <<
      "| in numbers spec" <<
      endl << endl;
  }

  return selectedNumbers;
}


}
