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

#include <lpsrUtilities.h>

using namespace std;

namespace MusicXML2 {


//______________________________________________________________________________
void lpsrMusicXMLWarning (string message)
{
  cerr <<
    "--> MusicXML WARNING : " << message << " ???" << endl;
}

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

//______________________________________________________________________________
/*
ostream& operator<< (ostream& os, const haendel& endl)
{
  endl.print(os);
  return os;
}

void haendel::print(ostream& os) const { 
  int i = fIndent;
  os << endl;
  while (i-- > 0)  os << fSpacer;
}
*/

//______________________________________________________________________________
string int2EnglishWord (int n)
{
  if (n < 0)  return "None";
  if (n > 99) return "LargerThanNinetyNine";
  
  stringstream s;

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

  string result;
  
  s >> result;
  return result;
}

//______________________________________________________________________________
string stringNumbersToEnglishWords (string str)
{
  if (! str.size ())
    return "None";
  
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
/* JMI
 *   pair <int,int> foo;
  pair <int,int> bar;

  foo = make_pair (10,20);
  bar = make_pair (10.5,'A'); // ok: implicit conversion from pair<double,char>

  cout << "foo: " << foo.first << ", " << foo.second << '\n';
  cout << "bar: " << bar.first << ", " << bar.second << '\n';


  list<S_lpsrdynamics>::const_iterator i1;
  for (i1=fChordDynamics.begin(); i1!=fChordDynamics.end(); i1++) {
    os << " " << (*i1);
  } // for
*/
}
