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
#include <map>

#include <iomanip>      // setw, ...

#include "rational.h"

#include "utilities.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_timingItem createTimingItem (
  string                     activity,
  timingItem::timingItemKind kind,
  clock_t                    startClock,
  clock_t                    endClock)
{
  timingItem* o = new timingItem (
    activity,
    kind,
    startClock,
    endClock);
  assert(o!=0);
  return o;
}

timingItem::timingItem (
  string         activity,
  timingItemKind kind,
  clock_t        startClock,
  clock_t        endClock)
{
  fActivity   = activity;
  fKind       = kind;
  fStartClock = startClock;
  fEndClock   = endClock;
}

timing::timing ()
{}

timing::~timing ()
{}

void timing::appendTimingItem (
  string                     activity,
  timingItem::timingItemKind kind,
  clock_t                    startClock,
  clock_t                    endClock)
{
  S_timingItem
    timingItem =
      createTimingItem (
        activity,
        kind,
        startClock,
        endClock);
    
  fTimingItemsList.push_back (timingItem);
}

ostream& operator<< (ostream& os, const timing& tim) {
  tim.print(os);
  return os;
}

void timing::print (ostream& os) const
{
  const int
    activityWidth     = 42,
    kindWidth         = 12,
    secondsWidth      =  8,
    secondsPrecision  =  6;

  clock_t
    totalClock          = 0.0,
    totalMandatoryClock = 0.0,
    totalOptionalClock  = 0.0;
    
  os <<
    endl <<
    "Timing information:" <<
// JMI    __FILE__ << ":" << __LINE__ <<
    endl << endl <<
    setw (activityWidth) << "Activity" <<
    setw (kindWidth)     << "Kind" <<
    setw (secondsWidth)  << "CPU (s)" <<
    endl <<
    setw (activityWidth) << "--------" <<
    setw (kindWidth)     << "----" <<
    setw (secondsWidth)  << "-------" <<
    endl << endl;

  for (
    list<S_timingItem>::const_iterator i=fTimingItemsList.begin();
    i!=fTimingItemsList.end();
    i++) {
    clock_t
      timingItemClock =
        (*i)->fEndClock - (*i)->fStartClock;

    totalClock += timingItemClock;
    
    os << left <<
      setw (activityWidth) << (*i)->fActivity;

    switch ((*i)->fKind) {
      case timingItem::kMandatory:
        totalMandatoryClock += timingItemClock;
        os <<
          setw (kindWidth) << "mandatory";
        break;
      case timingItem::kOptional:
        totalOptionalClock += timingItemClock;
        os <<
          setw (kindWidth) << "optional";
        break;
    } // switch

    os <<
      setw (secondsWidth) <<
      setprecision(secondsPrecision) <<
      left <<
      float(timingItemClock) / CLOCKS_PER_SEC <<
      endl;
  } // for

  const int
    totalClockWidth          = 12,
    totalMandatoryClockWidth = 12,
    totalOptionalClockWidth  = 12,
    totalsPrecision          =  6;

  os <<
    endl <<
    left <<
    setw (totalClockWidth)            << "Total" <<
    setw (totalMandatoryClockWidth)   << "Mandatory" <<
    setw (totalOptionalClockWidth)    << "Optional" <<
    endl <<
    setw (totalClockWidth) << "-----" <<
    setw (totalMandatoryClockWidth)   << "---------" <<
    setw (totalOptionalClockWidth)    << "--------" <<
    endl <<
    setprecision(totalsPrecision) <<
    left <<
    setw (totalClockWidth) <<
    float(totalClock) / CLOCKS_PER_SEC <<
    setw (totalMandatoryClockWidth) <<
    float(totalMandatoryClock) / CLOCKS_PER_SEC <<
    setw (totalOptionalClockWidth) <<
    float(totalOptionalClock) / CLOCKS_PER_SEC <<
    endl << endl;
}

timing timing::gTiming;

//______________________________________________________________________________
//#define DEBUG_INDENTER

indenter::indenter (string spacer)
{
  fIndent = 0;
  fSpacer = spacer;
}

indenter::~indenter ()
{}

indenter& indenter::operator++ (const int value)
{
  fIndent++;
  
#ifdef DEBUG_INDENTER
  cerr <<
    "% INDENTER: " << fIndent <<
    endl;
#endif

  return *this;
}

indenter& indenter::operator-- (const int value)
{
  fIndent--;

  if (fIndent < 0) {
    cerr <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndent <<
      endl << endl;

#ifdef DEBUG_INDENTER
    assert(false);
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    cerr <<
      "% INDENTER: " << fIndent <<
      endl;
  }
#endif

  return *this;
}

string indenter::indentMultiLineString (string value)
{
  stringstream  s;
  
  // add indentation before all end of lines inside 'value'
  istringstream ss (value);
  string        line;
  
  while (getline (ss, line)) {
    s <<  line;

    if (ss.eof()) break;

    s << endl;
    this->print (s);
  } // while

  return s.str ();
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
//#define DEBUG_OLEC

outputLineElementsCounter::outputLineElementsCounter (
  ostream& ostream,
  int      maxElementsPerLine)
    : fOstream (ostream)
{  
  fElementsCounter    = 0;
  fMaxElementsPerLine = maxElementsPerLine;
}

outputLineElementsCounter::~outputLineElementsCounter ()
{}

outputLineElementsCounter& outputLineElementsCounter::operator++ (int value)
{
  fElementsCounter++;
  
  if (fElementsCounter >= fMaxElementsPerLine) {
#ifdef DEBUG_OLEC
    fOstream <<
      "%{ ~" << fMaxElementsPerLine << " elems %}";
#endif

    fOstream <<
      endl <<
      indenter::gIndenter;

    fElementsCounter = 0;
  }
    
  return *this;
}

outputLineElementsCounter& outputLineElementsCounter::increment (int value)
{
  fElementsCounter += value;
  
  if (fElementsCounter >= fMaxElementsPerLine) {
#ifdef DEBUG_OLEC
    fOstream <<
      "%{ ~" << fMaxElementsPerLine << " elems %}";
#endif

    fOstream <<
      endl <<
      indenter::gIndenter;

    fElementsCounter = 0;
  }
    
  return *this;
}

outputLineElementsCounter
outputLineElementsCounter::gOutputLineElementsCounter (cout);

//______________________________________________________________________________
string int2EnglishWord (int n)
{
  stringstream s;

  if (n < 0) {
    s << "Minus_";
    n = -n;

//    assert(false); // JMI
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
  
  return s.str ();
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
    char ch = (*i);
    
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
  string::const_iterator  theStringIterator,
  string::const_iterator& remainingStringIterator,
  bool   debugMode)
{
  string::const_iterator cursor = theStringIterator;
  int    number = 0;

  if (! isdigit (*cursor))
    cerr <<
      "consumeDecimalNumber (" << *cursor <<
      "), " << *cursor << " is no decimal digit!" <<
      endl;

  while (isdigit (*cursor)) {
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
  string theString,
  bool        debugMode)
{
//  A numbersSetSpecification sample is: "7,15-19,^16-17"

  set<int> selectedNumbers;
  
  if (debugMode)
    cerr <<
      "--> decipherNumbersSetSpecification, theString = |" << theString <<
      "|" <<
      endl;

  string::const_iterator
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
list<int> extractNumbersFromString (
  string theString, // can contain "1, 2, 17"
  bool        debugMode)
{
  list<int> foundNumbers;

  if (debugMode)
    cerr <<
      "--> extractNumbersFromString, theString = |" << theString <<
      "|" <<
      endl;

  string::const_iterator
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
pair<string, string> extractNamesPairFromString (
  string theString, // can contain "P1 = Bassoon"
  char        separator,
  bool        debugMode)
{
  string name1;
  string name2;

  if (debugMode)
    cerr <<
      "--> extractNamesPairFromString, theString = |" << theString <<
      "|" <<
      endl;

  string::const_iterator
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
string quoteStringIfNonAlpha (
  string theString)
{
  string result;
  
  bool   stringShouldBeQuoted = false;
  
  for (
    string::const_iterator i = theString.begin ();
    i != theString.end ();
    i++) {

    if (
      ((*i) >= 'a' && (*i) <= 'z')
        ||
      ((*i) >= 'A' && (*i) <= 'Z')) {
      // (*i) is a letter
      result += (*i);
    }

    else {
      // (*i) is not a letter
      if ((*i) == ' ')
        result += " "; // TEMP JMI
      else
        result += (*i);

      stringShouldBeQuoted = true;
    }
  } // for

  if (stringShouldBeQuoted)
    return "\"" + result + "\"";
  else
    return result;
}

//______________________________________________________________________________
string quoteString (
  string theString)
{
  string result;
    
  for (
    string::const_iterator i = theString.begin ();
    i != theString.end ();
    i++) {

    if (
      ((*i) >= 'a' && (*i) <= 'z')
        ||
      ((*i) >= 'A' && (*i) <= 'Z')) {
      // (*i) is a letter
      result += (*i);
    }

    else {
      // (*i) is not a letter
      if ((*i) == ' ')
        result += " "; // TEMP JMI
      else
        result += (*i);
    }
  } // for

  return "\"" + result + "\"";
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

string singularOrPluralWithoutNumber (
  int number, string singularName, string pluralName)
{
  stringstream s;

  if (number <= 1)
    s <<
      singularName;
  else
    s <<
      pluralName;
      
  return s.str ();
}

//______________________________________________________________________________
void optionError (string errorMessage)
{
  cerr <<
    endl <<
    endl <<
    "### ERROR in the options:" <<
    endl <<
    errorMessage <<
    endl <<
    endl;
}

//______________________________________________________________________________

  /* JMI

*/

void convertHTMLEntitiesToPlainCharacters (string& s)
{
  map<string, string> conversionMap;
  
  conversionMap ["&"] = "&amp;";
  conversionMap ["\""] = "&quot;";
  conversionMap ["'"] = "&apos;";
  conversionMap ["<"] = "&lt;";
  conversionMap [">"] = "&gt;";

  map<string, string>::const_iterator i = conversionMap.begin();

  for (i; i != conversionMap.end(); ++i) {
    string
      lookedFor = i->second,
      ersatz    = i->first;
    
    // replace all occurrences of lookedFor by ersatz
    for ( ; ; ) {
      size_t found = s.find (lookedFor);

      if (found == string::npos)
        break;

      s.replace (found, lookedFor.size (), ersatz);
    } // for
    
  } // for
}
/*
int main(int argc, char* argv[])
{
  string s = "gsdf&quot;gsdfg&amp;&amp;fgg";
  htmlEntitiesDecode(s);
  
  return 0;
}

    */

//______________________________________________________________________________
string baseName (const string &filename)
{
  if (filename.empty ()) {
      return {};
  }

  auto len   = filename.length ();
  auto index = filename.find_last_of ("/\\");

  if (index == string::npos) {
      return filename;
  }

  if (index + 1 >= len) {
    len--;
    index = filename.substr (0, len).find_last_of ("/\\");

    if (len == 0) {
      return filename;
    }

    if (index == 0) {
      return filename.substr (1, len - 1);
    }

    if (index == string::npos) {
      return filename.substr (0, len);
    }

    return filename.substr (index + 1, len - index - 1);
  }

  return filename.substr (index + 1, len - index);
}


}
