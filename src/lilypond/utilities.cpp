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

#ifdef WIN32
  // JMI
#else
  #include "unistd.h"
#endif

#include <iostream>
#include <sstream>
#include <cassert>

#include <vector>
#include <map>

#include <iomanip>      // setw, ...

#include "rational.h"
#include "utilities.h"

using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_timingItem timingItem::createTimingItem (
  string         activity,
  string         description,
  timingItemKind kind,
  clock_t        startClock,
  clock_t        endClock)
{
  timingItem* o = new timingItem (
    activity,
    description,
    kind,
    startClock,
    endClock);
  assert(o!=0);
  return o;
}

timingItem::timingItem (
  string         activity,
  string         description,
  timingItemKind kind,
  clock_t        startClock,
  clock_t        endClock)
{
  fActivity    = activity;
  fDescription = description;
  fKind        = kind;
  fStartClock  = startClock;
  fEndClock    = endClock;
}

timing::timing ()
{}

timing::~timing ()
{}


void timing::appendTimingItem (
  string         activity,
  string         description,
  timingItem::timingItemKind
                 kind,
  clock_t        startClock,
  clock_t        endClock)
{
  S_timingItem
    timingItem =
      timingItem::createTimingItem (
        activity,
        description,
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
    activityWidth     =  8,
    descriptionWidth  = 31,
    kindWidth         =  9,
    secondsWidth      =  9,
    secondsPrecision  =  6;

  clock_t
    totalClock          = 0.0,
    totalMandatoryClock = 0.0,
    totalOptionalClock  = 0.0;
    
  os << endl << "Timing information:" << endl << endl <<
    setw (activityWidth) << "Activity" << "  " <<
    setw (descriptionWidth) << "Description" << "  " <<
    setw (kindWidth)     << "Kind" << "  " <<
    setw (secondsWidth)  << "CPU (sec)" << endl <<
    setw (activityWidth) << replicateString ("-", activityWidth) << "  " <<
    setw (descriptionWidth) << replicateString ("-", descriptionWidth) << "  " <<
    setw (kindWidth) << replicateString ("-", kindWidth) << "  " <<
    setw (secondsWidth) << replicateString ("-", secondsWidth) << endl << endl;

  for ( list<S_timingItem>::const_iterator i=fTimingItemsList.begin (); i!=fTimingItemsList.end (); i++) {
    clock_t timingItemClock = (*i)->fEndClock - (*i)->fStartClock;
    totalClock += timingItemClock;
    
    os << left <<
      setw (activityWidth) << (*i)->fActivity << "  " <<
      setw (descriptionWidth) << (*i)->fDescription << "  ";

    switch ((*i)->fKind) {
      case timingItem::kMandatory:
        totalMandatoryClock += timingItemClock;
        os << setw (kindWidth) << "mandatory";
        break;
      case timingItem::kOptional:
        totalOptionalClock += timingItemClock;
        os << setw (kindWidth) << "optional";
        break;
    } // switch

    os << "  " <<
      setw (secondsWidth) << setprecision(secondsPrecision) <<
    left << float(timingItemClock) / CLOCKS_PER_SEC << endl;
  } // for

  const int
    totalClockWidth          =  7,
    totalMandatoryClockWidth =  9,
    totalOptionalClockWidth  = 10,
    totalsPrecision          =  6;

  os << left <<
    endl <<
    setw (totalClockWidth)            << "Total" <<
    "    " <<
    setw (totalMandatoryClockWidth)   << "Mandatory" <<
    "  " <<
    setw (totalOptionalClockWidth)    << "Optional" <<
    endl <<
    
    setw (totalClockWidth) <<
    replicateString ("-", totalClockWidth) <<
    "    " <<
    setw (totalMandatoryClockWidth) <<
    replicateString ("-", totalMandatoryClockWidth) <<
    "  " <<
    setw (secondsWidth) <<
    replicateString ("-", secondsWidth) <<
    setprecision(totalsPrecision) <<
    endl <<
    
    setw (totalClockWidth) <<
    float(totalClock) / CLOCKS_PER_SEC <<
    "    " <<
    setw (totalMandatoryClockWidth) <<
    float(totalMandatoryClock) / CLOCKS_PER_SEC <<
    "  " <<
    setw (totalOptionalClockWidth) <<
    float(totalOptionalClock) / CLOCKS_PER_SEC <<
    endl <<
    endl;
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
  gLogIOstream <<
    "% INDENTER: " << fIndent <<
    endl;
#endif

  return *this;
}

indenter& indenter::operator-- (const int value)
{
  fIndent--;

  if (fIndent < 0) {
    gLogIOstream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndent <<
      endl << endl;

#ifdef DEBUG_INDENTER
    assert(false);
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLogIOstream <<
      "% INDENTER: " << fIndent <<
      endl;
  }
#endif

  return *this;
}

indenter& indenter::increment (int value)
{
  fIndent += value;

  if (fIndent < 0) {
    gLogIOstream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndent <<
      endl << endl;

#ifdef DEBUG_INDENTER
    assert(false);
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLogIOstream <<
      "% INDENTER: " << fIndent <<
      endl;
  }
#endif

  return *this;
}

indenter& indenter::decrement (int value)
{
  fIndent -= value;

  if (fIndent < 0) {
    gLogIOstream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndent <<
      endl << endl;

#ifdef DEBUG_INDENTER
    assert(false);
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLogIOstream <<
      "% INDENTER: " << fIndent <<
      endl;
  }
#endif

  return *this;
}

string indenter::indentMultiLineString (string value)
{
  stringstream  s;
  
  // add indentation ahead of all lines inside 'value'
  istringstream inputStream (value);
  string        line;
  
  while (getline (inputStream, line)) {
    s << line;

    if (inputStream.eof()) break;

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
indentedOstream indentedOstream::gOutputIndentedOstream (
  cout, indenter::gIndenter);

indentedOstream indentedOstream::gLogIndentedOstream (
  cerr, indenter::gIndenter);

// code taken from:
// http://comp.lang.cpp.moderated.narkive.com/fylLGJgp/redirect-output-to-dev-null
template<typename Ch, typename Traits = std::char_traits<Ch> >
struct basic_nullbuf : std::basic_streambuf<Ch, Traits>
{
  typedef std::basic_streambuf<Ch, Traits> base_type;
  typedef typename base_type::int_type int_type;
  typedef typename base_type::traits_type traits_type;
  
  virtual int_type overflow (int_type c) {
    return traits_type::not_eof (c);
  }
};

// convenient typedefs
typedef basic_nullbuf <char>    nullbuf;
//typedef basic_nullbuf <wchar_t> wnullbuf;

// buffers and streams
nullbuf cnull_obj;
//wnullbuf wcnull_obj;

std::ostream cnull  (& cnull_obj);
//std::wostream wcnull (& wcnull_obj);

indentedOstream indentedOstream::gNullIndentedOstream (
  cnull, indenter::gIndenter);

//______________________________________________________________________________
string replicateString (
  string str,
  int    times)
{
  string result;
  
  for (int i = 0; i < times; i++)
    result += str;

  return result;
}

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
  if (! str.size ()) {
    return "NONE";
  }
  
  enum workState {
    kInitialState, kWorkingOnDigits, kWorkingOnNonDigits };
      
  vector<string> chunks;
  vector<int>    states;
    
  workState state = kInitialState;
  
  string::const_iterator
    iBegin = str.begin (),
    iEnd   = str.end (),
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

  for (unsigned int i = 0; i < chunks.size (); i++) {
    if (states[i] == kWorkingOnDigits) {
      int integerValue;
    
      istringstream inputStream (chunks[i]);
    
      inputStream >> integerValue;
      
      result += int2EnglishWord (integerValue);
    }
    else {
      result += chunks[i];
    }
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

  if (! isdigit (*cursor)) {
    gLogIOstream <<
      "consumeDecimalNumber (" << *cursor <<
      "), " << *cursor << " is no decimal digit!" <<
      endl;
    }

  while (isdigit (*cursor)) {
    if (debugMode) {
      gLogIOstream <<
        "--> consumeDecimalNumber: cursor = |" <<
        *cursor <<
        "|" <<
        endl;
    }

    number = number*10 + (*cursor-'0');
    
    cursor++;
  } // while

  remainingStringIterator = cursor;

  if (debugMode) {
    gLogIOstream <<
      "--> consumeDecimalNumber: number = " << number <<
      ", *remainingStringIterator = |" << *remainingStringIterator <<
      "|" <<
      endl;
  }

  return number;
}

//______________________________________________________________________________
set<int> decipherNumbersSetSpecification (
  string theString,
  bool        debugMode)
{
//  A numbersSetSpecification sample is: "7,15-19,^16-17"

  set<int> selectedNumbers;
  
  if (debugMode) {
    gLogIOstream <<
      "--> decipherNumbersSetSpecification, theString = |" << theString <<
      "|" <<
      endl;
  }

  string::const_iterator
    cursor = theString.begin ();

  while (1) {
    if (debugMode) {
      gLogIOstream <<
        "--> decipherNumbersSpecification: cursor = |" <<
        *cursor << "|" <<
        endl;
    }

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

      if (debugMode) {
        gLogIOstream <<
          "--> decipherNumbersSpecification after '-' : cursor = |" <<
          *cursor <<
          "|" <<
          endl <<
          endl;
      }
  
      intervalEndNumber =
        consumeDecimalNumber (cursor, cursor, debugMode);
    }

    else {
      intervalEndNumber = intervalStartNumber;
    }

    if (debugMode) {
      gLogIOstream <<
        "--> decipherNumbersSpecification, intervalStartNumber = " << intervalStartNumber <<
        ", intervalEndNumber = " << intervalEndNumber <<
        ": *cursor = |" << *cursor << "|" <<
        endl;
    }

    for (int i = intervalStartNumber; i <= intervalEndNumber; i ++) {
      if (negated) {
        selectedNumbers.erase (i);
      }
      else {
        selectedNumbers.insert (i);
      }
    } // for

    if (*cursor != ',') {
      if (debugMode) {
        gLogIOstream <<
          "--> decipherNumbersSpecification, after non ',' : cursor = |" <<
          *cursor <<
          "|" <<
          endl <<
          endl;
      }
      break; 
    }

    cursor++;

    if (debugMode) {
      gLogIOstream <<
        "--> decipherNumbersSpecification after ',' : cursor = |" <<
        *cursor <<
        "|"
        << endl << 
        endl;
    }
  } // while 

  if (* cursor != '\0') {
    gLogIOstream <<
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

  if (debugMode) {
    gLogIOstream <<
      "--> extractNumbersFromString, theString = |" << theString <<
      "|" <<
      endl;
  }

  string::const_iterator
    cursor = theString.begin ();

  while (1) {
    if (cursor == theString.end ())
      break;
      
    if (debugMode) {
      gLogIOstream <<
        "--> extractNumbersFromString: cursor = |" <<
        *cursor << "|" <<
        endl;
    }

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

  if (debugMode) {
    gLogIOstream <<
      "--> extractNamesPairFromString, theString = |" << theString <<
      "|" <<
      endl;
  }

  string::const_iterator
    cursor = theString.begin ();

  // fetch name1
  while (1) {
    if (cursor == theString.end ())
      break;
      
    if (debugMode) {
      gLogIOstream <<
        "--> extractNamesPairFromString: cursor = |" <<
        *cursor << "|" <<
        endl;
    }

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
    gLogIOstream <<
      "### ERROR: the first name before the " << separator <<
      " separator is empty in '" << theString << "'" <<
      endl;
  }

  if (cursor == theString.end ())
    gLogIOstream <<
      "### ERROR: the " << separator <<
      " separator is missing in string '" <<
      theString << "'" <<
      endl;
  else
    // overtake the separator
    cursor++;

  // fetch name2
  while (1) {
    if (cursor == theString.end ())
      break;
      
    if (debugMode) {
      gLogIOstream <<
        "--> extractNamesPairFromString: cursor = |" <<
        *cursor << "|" <<
        endl;
    }

    if ((*cursor) == '=') {
      // found the separator
      gLogIOstream <<
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
    gLogIOstream <<
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

  if (stringShouldBeQuoted) {
    return "\"" + result + "\"";
  }
  else {
    return result;
  }
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
  
  if (number <= 1) {
    s <<
      singularName;
  }
  else {
    s <<
      pluralName;
  }
      
  return s.str ();
}

string singularOrPluralWithoutNumber (
  int number, string singularName, string pluralName)
{
  stringstream s;

  if (number <= 1) {
    s <<
      singularName;
  }
  else {
    s <<
      pluralName;
  }
      
  return s.str ();
}

//______________________________________________________________________________
void optionError (string errorMessage)
{
  gLogIOstream <<
    endl <<
    "### ERROR in the options:" <<
    endl <<
    errorMessage <<
    endl <<
    endl;
}

//______________________________________________________________________________
string escapeQuotes (string s)
{
  string result;

  for_each (
    s.begin (),
    s.end (),
    stringQuoteEscaper (result));

  return result;
}

//______________________________________________________________________________
void convertHTMLEntitiesToPlainCharacters (string& s)
{
  map<string, string> conversionMap;
  
  conversionMap ["&"] = "&amp;";
  conversionMap ["\""] = "&quot;";
  conversionMap ["'"] = "&apos;";
  conversionMap ["<"] = "&lt;";
  conversionMap [">"] = "&gt;";

  map<string, string>::const_iterator i;

  for (i = conversionMap.begin (); i != conversionMap.end (); ++i) {
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

//______________________________________________________________________________
void splitRegularStringContainingEndOfLines (
  string        theString,
  list<string>& chunksList)
{
//#define DEBUG_SPLITTING

#ifdef DEBUG_SPLITTING
  gLogIOstream <<
    "---> splitting |" << theString << "|" <<
    endl <<
    endl;
#endif

  int theStringSize = theString.size ();
  
  size_t currentPosition = 0;

#ifdef DEBUG_SPLITTING
  string remainder = theString;
#endif

  string lookedFor     = "\n";
  int    lookedForSize = lookedFor.size ();

  map<string, string>::const_iterator i;

  while (1) {
    size_t found =
      theString.find (lookedFor, currentPosition);

    if (found == string::npos) {
      // fetch the last chunk
      // we have a last chunk
      // from currentPosition to theStringSize
      int chunkLength = theStringSize - currentPosition;
      
      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);
      
      chunksList.push_back (
        chunk);

#ifdef DEBUG_SPLITTING
      gLogIOstream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl <<
        endl;
#endif

      break;
    }
    
    else {
      // we have a chunk from currentPosition to found
      int chunkLength = found - currentPosition;
      
      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      // append it to the chunks list
      chunksList.push_back (
        chunk);

      // advance the cursor
      currentPosition +=
        chunkLength + lookedForSize;

      // there can be an end of line JMI
      if (theString [currentPosition] == '\n')
        currentPosition++;

#ifdef DEBUG_SPLITTING
      // set remainder
      remainder =
        theString.substr (
          currentPosition);

      gLogIOstream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "found = " << found <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl <<
        endl;      
#endif
    }
  } // while
}

//______________________________________________________________________________
void splitHTMLStringContainingEndOfLines (
  string        theString,
  list<string>& chunksList)
{
//#define DEBUG_SPLITTING

#ifdef DEBUG_SPLITTING
  gLogIOstream <<
    "---> splitting |" << theString << "|" <<
    endl <<
    endl;
#endif

  int theStringSize = theString.size ();
  
  map<string, string> conversionMap; // JMI
  
  conversionMap ["&"] = "&amp;";
  conversionMap ["\""] = "&quot;";
  conversionMap ["'"] = "&apos;";
  conversionMap ["<"] = "&lt;";
  conversionMap [">"] = "&gt;";
  
  size_t currentPosition = 0;

#ifdef DEBUG_SPLITTING
  string remainder = theString;
#endif

// JMI  string lookedFor     = "&#xd;";
  string lookedFor     = "\n";
  int    lookedForSize = lookedFor.size ();

  map<string, string>::const_iterator i;

/*
  for (i = conversionMap.begin (); i != conversionMap.end (); ++i) {
    string
      lookedFor = i->second,
      ersatz    = i->first;
    // replace all occurrences of lookedFor by ersatz
  } // for
*/

  while (1) {
    size_t found =
      theString.find (lookedFor, currentPosition);

    if (found == string::npos) {
      // fetch the last chunk
      // we have a last chunk
      // from currentPosition to theStringSize
      int chunkLength = theStringSize - currentPosition;
      
      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);
      
      chunksList.push_back (
        chunk);

#ifdef DEBUG_SPLITTING
      gLogIOstream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl <<
        endl;
#endif

      break;
    }
    
    else {
      // we have a chunk from currentPosition to found
      int chunkLength = found - currentPosition;
      
      string
        chunk =
          theString.substr (
            currentPosition,
            chunkLength);

      // append it to the chunks list
      chunksList.push_back (
        chunk);

      // advance the cursor
      currentPosition +=
        chunkLength + lookedForSize;

      // there can be an end of line JMI
      if (theString [currentPosition] == '\n')
        currentPosition++;

#ifdef DEBUG_SPLITTING
      // set remainder
      remainder =
        theString.substr (
          currentPosition);

      gLogIOstream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "found = " << found <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl <<
        endl;      
#endif
    }
  } // while
}

//______________________________________________________________________________
string baseName (const string &filename)
{
  if (! filename.size ()) {
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

//______________________________________________________________________________
string makeSingleWordFromString (const string& theString)
{
  string result;

  for (
    string::const_iterator i = theString.begin ();
    i != theString.end ();
    i++) {
    if (isalnum (*i)) {
      result.push_back ((*i));
    }
  } // for
  
  return result;
}

//______________________________________________________________________________
/* JMI
#ifdef WIN32
  // JMI
#else
/ * JMI
IConv::IConv (const char* to, const char* from) 
  : fIconvDescriptor (iconv_open (to, from))
{
  fInputBufferSize = 1024;
  fInputBuffer = new char [fInputBufferSize];
  
  fOutputBufferSize = 1024;
  fOutputBuffer = new char [fOutputBufferSize];
  for (unsigned int i = 0; i < fOutputBufferSize; i++) {
    fOutputBuffer [i] = '\0';
  } // for
}
    
IConv::~IConv ()
{
  iconv_close (fIconvDescriptor);
}

bool IConv::convert (char* input, char* output, size_t& outputSize)
{
  size_t saveInputSize = strlen (input);

  bool doTrace = false;

  size_t inBytesLeft  = saveInputSize;
  size_t outBytesLeft = fOutputBufferSize;

  if (doTrace) {
    gLogIOstream <<
      "saveInputSize = " << saveInputSize <<
      endl <<
      "fInputBufferSize = " << fInputBufferSize <<
      endl <<
      "input = *" << input << "*" <<
      endl <<
      "inBytesLeft = " << inBytesLeft <<
      endl <<
      "outBytesLeft = " << outBytesLeft <<
      endl <<
      endl;
  }

  // is there room enough in fInputBufferSize?
  if (saveInputSize >= fInputBufferSize) {
    const size_t newSize = saveInputSize + 1;
    
    if (doTrace) {
      gLogIOstream <<
        "Bringing fInputBuffer from " <<
        fInputBufferSize <<
        " to " <<
        newSize <<
        " characters" <<
        endl;
    }

    delete [] fInputBuffer;

    fInputBufferSize = newSize;
    fInputBuffer = new char [fInputBufferSize];
  }

  // copy input to fInputBufferSize
  strcpy (fInputBuffer, input);
                                       
  // heuristic to have enough room in fOutputBuffer
  size_t twiceAsMuch = saveInputSize * 2;
  
  if (fOutputBufferSize < twiceAsMuch) {
    if (doTrace) {
      gLogIOstream <<
        "Bringing fOutputBuffer from " <<
        fOutputBufferSize <<
        " to " <<
        twiceAsMuch <<
        " characters" <<
        endl;
    }
    
    delete [] fOutputBuffer;

    fOutputBufferSize = twiceAsMuch;
    fOutputBuffer = new char [fOutputBufferSize];
    for (unsigned int i = 0; i < fOutputBufferSize; i++) {
      fOutputBuffer [i] = '\0';
    } // for
  }

  if (doTrace) {
    gLogIOstream <<
      "saveInputSize = " << saveInputSize <<
      endl <<
      "fInputBufferSize = " << fInputBufferSize <<
      endl <<
      "input = *" << input << "*" <<
      endl <<
      "inBytesLeft = " << inBytesLeft <<
      endl <<
      "outBytesLeft = " << outBytesLeft <<
      endl <<
      endl;
  }

  // do the conversion
  if (doTrace) {
    gLogIOstream <<
      "Doing the encoding conversion" <<
      endl <<
      endl;
  }

  // we can't pass fInputBuffer and fOutputBuffer to iconv(),
  // since that would modify them, hence:
  char* inputBufferAux  = fInputBuffer;
  char* outputBufferAux = fOutputBuffer;
  
  size_t
    iconvResult =
      iconv (
        fIconvDescriptor,
        &inputBufferAux, &inBytesLeft,
        &outputBufferAux, &outBytesLeft);

  if (doTrace) {
    gLogIOstream <<
      "saveInputSize = " << saveInputSize <<
      endl <<
      "fInputBuffer = *" << fInputBuffer << "*" <<
      endl <<
      "inputBufferAux = *" << inputBufferAux << "*" <<
      endl <<
      "inBytesLeft = " << inBytesLeft <<
      endl <<
      "fOutputBuffer = *" << fOutputBuffer << "*" <<
      endl <<
       "outputBufferAux = *" << outputBufferAux << "*" <<
      endl <<
     "outBytesLeft = " << outBytesLeft <<
      endl <<
      endl;
  }

  // terminate the resulting string  
  fOutputBuffer [fOutputBufferSize - outBytesLeft + 1] = '\0';

  // copy the result to output
  strcpy (output, fOutputBuffer);
  
  // return boolean result
  return iconvResult != (size_t)(-1);
}

bool IConv::convert (std::string& input, std::string& output)
{
  size_t saveInputSize     = input.size ();

  bool doTrace = false;

  // is there room enough in fInputBufferSize?
  if (saveInputSize >= fInputBufferSize) {
    delete [] fInputBuffer;

    fInputBufferSize = saveInputSize + 1;
    fInputBuffer = new char [fInputBufferSize];
  }

  // copy input to fInputBufferSize
  strcpy (fInputBuffer, (char *) input.c_str ());
                                       
  // heuristic to have enough room in fOutputBuffer
  size_t twiceAsMuch = saveInputSize * 2;
  
  if (fOutputBufferSize < twiceAsMuch) {
    if (doTrace) {
      gLogIOstream <<
        "Bringing fOutputBuffer from " <<
        fOutputBufferSize <<
        " to " <<
        twiceAsMuch <<
        " characters" <<
        endl;
    }
    
    delete [] fOutputBuffer;

    fOutputBufferSize = twiceAsMuch;
    fOutputBuffer = new char [fOutputBufferSize];
    for (unsigned int i = 0; i < fOutputBufferSize; i++) {
      fOutputBuffer [i] = '\0';
    } // for
  }

  if (doTrace) {
    gLogIOstream <<
      "input = *" << input << "*" <<
      endl <<
      "saveInputSize = " << saveInputSize <<
      endl <<
      "fInputBuffer = *" << fInputBuffer << "*" <<
      endl <<
      "fOutputBufferSize = *" << fOutputBufferSize << "*" <<
      endl <<
      "fOutputBuffer = *" << fOutputBuffer << "*" <<
      endl <<
      endl;
  }
  
  size_t inBytesLeft  = saveInputSize;
  size_t outBytesLeft = fOutputBufferSize;

  // do the conversion

  // we can't pass fInputBuffer and fOutputBuffer to iconv(),
  // since that would modify them, hence:
  char* inputBufferAux  = fInputBuffer;
  char* outputBufferAux = fOutputBuffer;
  
  size_t
    iconvResult =
      iconv (
        fIconvDescriptor,
        &inputBufferAux, &inBytesLeft,
        &outputBufferAux, &outBytesLeft);

  
  if (doTrace) {
    gLogIOstream <<
      "fInputBuffer = *" << fInputBuffer << "*" <<
      endl <<
      "inputBufferAux = *" << inputBufferAux << "*" <<
      endl <<
      "inBytesLeft = " << inBytesLeft <<
      endl <<
      "fOutputBuffer = *" << fOutputBuffer << "*" <<
      endl <<
      "outputBufferAux = *" << outputBufferAux << "*" <<
      endl <<
      "outBytesLeft = " << outBytesLeft <<
      endl <<
      endl;
  }

  // terminate the resulting string  
  fOutputBuffer [fOutputBufferSize - outBytesLeft + 1] = '\0';
  
  // copy the result to output
  output = std::string (fOutputBuffer);
  
  // return boolean result
  return iconvResult != (size_t)(-1);
}
* /
#endif
*/

//______________________________________________________________________________
#ifdef WIN32
  // JMI
#else
inline IFdStreambuf::IFdStreambuf(int fd)
:
  d_fd (fd)
{
  setg (d_buffer, d_buffer + 1, d_buffer + 1);
}

inline int IFdStreambuf::underflow ()
{
  if (read (d_fd, d_buffer, 1) <= 0) {
    return EOF;
  }

  setg (d_buffer, d_buffer, d_buffer + 1);
  
  return static_cast<unsigned char> (*gptr ());
}
#endif

//______________________________________________________________________________
#ifdef WIN32
  // JMI
#else
S_IFdNStreambuf IFdNStreambuf::create ()
{
  IFdNStreambuf* o =
    new IFdNStreambuf ();
  assert(o!=0);
  return o;
}

S_IFdNStreambuf IFdNStreambuf::create (int fd, size_t bufsize)
{
  IFdNStreambuf* o =
    new IFdNStreambuf (
      fd, bufsize);
  assert(o!=0);
  return o;
}

inline IFdNStreambuf::IFdNStreambuf ()
  :
    d_bufsize (0),
    d_buffer (0)
{}


inline IFdNStreambuf::IFdNStreambuf (int fd, size_t bufsize)
{
  open (fd, bufsize);
}

IFdNStreambuf::~IFdNStreambuf ()
{
  if (d_bufsize) {
    close (d_fd);
    delete [] d_buffer;
  }
}

void IFdNStreambuf::open(int fd, size_t bufsize)
{
  d_fd = fd;
  d_bufsize = bufsize;
  
  d_buffer = new char [d_bufsize];
  
  setg (d_buffer, d_buffer + d_bufsize, d_buffer + d_bufsize);
}

int IFdNStreambuf::underflow()
{
  if (gptr() < egptr ()) {
    return *gptr ();
  }

  int nread = read (d_fd, d_buffer, d_bufsize);

  if (nread <= 0) {
    return EOF;
  }

  setg (d_buffer, d_buffer, d_buffer + nread);
  
  return static_cast<unsigned char> (*gptr ());
}

std::streamsize IFdNStreambuf::xsgetn (char *dest, std::streamsize n)
{
  int nread = 0;

  while (n) {
    if (!in_avail ()) {
      if (underflow () == EOF)
        break;
    }

    int avail = in_avail();

    if (avail > n) {
      avail = n;
    }

    memcpy (dest + nread, gptr(), avail);
    gbump (avail);

    nread += avail;
    n -= avail;
  }

  return nread;
}
#endif

//______________________________________________________________________________
#ifdef WIN32
  // JMI
#else
OFdnStreambuf::OFdnStreambuf ()
  :
    d_bufsize (0),
    d_buffer (0)
{}

OFdnStreambuf::OFdnStreambuf (int fd, size_t bufsize)
{
  open(fd, bufsize);
}


OFdnStreambuf::~OFdnStreambuf ()
{
  if (d_buffer) {
    sync ();
    delete [] d_buffer;
  }
}

void OFdnStreambuf::open (int fd, size_t bufsize)
{
  d_fd = fd;
  d_bufsize = bufsize == 0 ? 1 : bufsize;

  d_buffer = new char [d_bufsize];
  
  setp (d_buffer, d_buffer + d_bufsize);
}

int OFdnStreambuf::sync ()
{
  if (pptr () > pbase ()) {
    write (d_fd, d_buffer, pptr () - pbase ());
    setp (d_buffer, d_buffer + d_bufsize);
  }
  
  return 0;
}

int OFdnStreambuf::overflow (int c)
{
  sync ();
  
  if (c != EOF) {
    *pptr () = c;
    pbump (1);
  }
  return c;
}
#endif


}

/* JMI
#ifdef WIN32
  // JMI
#else

int main(int argc, char **argv)
{
  IFdStreambuf fds(STDIN_FILENO);
  istream      is(&fds);

  cout << is.rdbuf();
}

int main(int argc, char **argv)
{
                              // internally: 30 char buffer
  IFdNStreambuf fds(STDIN_FILENO, 30);

  char buf[80];               // main() reads blocks of 80
                              // chars
  while (true)
  {
    size_t n = fds.sgetn(buf, 80);
    if (n == 0)
        break;
    cout.write(buf, n);
  }
}

int main(int argc, char **argv)
{
  OFdnStreambuf   fds(STDOUT_FILENO, 500);
  ostream         os(&fds);

  switch (argc)
  {
    case 1:
      for (string  s; getline(cin, s); )
        os << s << '\n';
      os << "COPIED cin LINE BY LINE\n";
    break;

    case 2:
      cin >> os.rdbuf();      // Alternatively, use:  cin >> &fds;
      os << "COPIED cin BY EXTRACTING TO os.rdbuf()\n";
    break;

    case 3:
      os << cin.rdbuf();
      os << "COPIED cin BY INSERTING cin.rdbuf() into os\n";
    break;
  }
}



  if (true) {
    const size_t BUF_SIZE = 1024;
  
    char flute  [BUF_SIZE] = "Flöte buffer";
    char result [BUF_SIZE] = "something else";
    
    IConv iConverter ("ISO-8859-1", "UTF8");
  
    size_t outsize = BUF_SIZE; // you will need it
          
    if (iConverter.convert (flute, result, outsize)) {
      gLogIOstream <<
        "Buffer iconv result = |" << result << "|";
    }
    else {
      gLogIOstream <<
        "ERROR in icon()";
    }
    
    gLogIOstream <<
      endl <<
      endl;
  }

  if (true) {
    string flute = "Flöte string eins";
    string result;

    IConv iConverter ("ISO-8859-1", "UTF8");
      
    if (iConverter.convert (flute, result)) {
      gLogIOstream <<
        "String iconv result = |" << result << "|";
    }
    else {
      gLogIOstream <<
        "ERROR in icon()";
    }
    
    gLogIOstream <<
      endl <<
      endl;
  }

  if (true) {
    string flute = "Flöte string zwei";
    string result;

    IConv iConverter ("ISO-8859-1", "UTF8");

    if (iConverter.convert (flute, result)) {
      gLogIOstream <<
        "String iconv result = |" << result << "|";
    }
    else {
      gLogIOstream <<
        "ERROR in icon()";
/ * JMI
      if (errno == EINVAL)
        error (0, 0, "conversion from '%s' to wchar_t not available",
               charset);
      else
        perror ("iconv_open");
* /
    }
    
    gLogIOstream <<
      endl <<
      endl;
  }
#endif
*/
