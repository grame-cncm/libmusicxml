/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

 // #include "unistd.h"

#include <iostream>
#include <sstream>
#include <cassert>

#include <vector>
#include <map>

#include <algorithm>    // for_each

#include <iomanip>      // setw, ...

#include "rational.h"
#include "utilities.h"

#include "messagesHandling.h"

#include "oahOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
//#define DEBUG_INDENTER

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
  assert (o != nullptr);
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
  string                     activity,
  string                     description,
  timingItem::timingItemKind kind,
  clock_t                    startClock,
  clock_t                    endClock)
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

void timing::doPrint (ostream& os) const
{
  // printing the details
  const int
    activityWidth     =  8,
    descriptionWidth  = 54,
    kindWidth         =  9,
    secondsWidth      =  9,
    secondsPrecision  = secondsWidth - 4; // to leave room for large numbers

  clock_t
    totalClock          = 0,
    totalMandatoryClock = 0,
    totalOptionalClock  = 0;

  os << left <<
    setw (activityWidth) << "Activity" << "  " <<
    setw (descriptionWidth) << "Description" << "  " <<
    setw (kindWidth)     << "Kind" << "  " <<
    setw (secondsWidth)  << "CPU (sec)" << endl <<
    setw (activityWidth) << replicateString ("-", activityWidth) << "  " <<
    setw (descriptionWidth) << replicateString ("-", descriptionWidth) << "  " <<
    setw (kindWidth) << replicateString ("-", kindWidth) << "  " <<
    setw (secondsWidth) << replicateString ("-", secondsWidth) <<
    endl << endl;

  for (
    list<S_timingItem>::const_iterator i=fTimingItemsList.begin ();
    i!=fTimingItemsList.end ();
    ++i
  ) {
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
      right << fixed <<
      setprecision (secondsPrecision) <<

      setw (secondsWidth) <<
      float(timingItemClock) / CLOCKS_PER_SEC <<
      endl;
  } // for
  os << endl;

  // printing the totals
  const int
    totalClockWidth          =  9,
    totalMandatoryClockWidth =  9,
    totalOptionalClockWidth  =  9,
    totalsPrecision          =  secondsPrecision;

  os << left <<
    setw (totalClockWidth)            << "Total (sec)" <<
    "  " <<
    setw (totalMandatoryClockWidth)   << "Mandatory" <<
    "  " <<
    setw (totalOptionalClockWidth)    << "Optional" <<
    endl <<

    setw (totalClockWidth) <<
    replicateString ("-", totalClockWidth) <<
    "  " <<
    setw (totalMandatoryClockWidth) <<
    replicateString ("-", totalMandatoryClockWidth) <<
    "  " <<
    setw (secondsWidth) <<
    replicateString ("-", secondsWidth) <<
    endl <<

    fixed <<
    setprecision (totalsPrecision) <<

    setw (totalClockWidth) <<
    float(totalClock) / CLOCKS_PER_SEC <<
    "  " <<
    setw (totalMandatoryClockWidth) <<
    float(totalMandatoryClock) / CLOCKS_PER_SEC <<
    "  " <<
    setw (totalOptionalClockWidth) <<
    float(totalOptionalClock) / CLOCKS_PER_SEC <<
    endl << endl;
}

void timing::printWithContext (
  string   context,
  ostream& os) const
{
  os << left <<
    "Timing information for " << context << ":" <<
    endl << endl;

  doPrint (os);
}

void timing::print (ostream& os) const
{
  os << left <<
    "Timing information:" <<
    endl << endl;

  doPrint (os);
}

ostream& operator<< (ostream& os, const timing& tim) {
  tim.print(os);
  return os;
}

timing timing::gGlobalTiming;

//______________________________________________________________________________
outputIndenter outputIndenter::gGlobalOStreamIndenter;

outputIndenter::outputIndenter (string spacer)
{
  fIndent = 0;
  fSpacer = spacer;
}

outputIndenter::~outputIndenter ()
{}

/*
  // Declare prefix and postfix decrement operators.
  Point& operator--();       // Prefix decrement operator.
  Point operator--(int);     // Postfix decrement operator.
*/

// increase the indentation by 1, prefix operator
outputIndenter& outputIndenter::operator++ ()
{
  ++fIndent;

#ifdef DEBUG_INDENTER
  gLogStream <<
    "% INDENTER: " << fIndent <<
    endl;
#endif

  return *this;
}

// decrease the indentation by 1, prefix operator
outputIndenter& outputIndenter::operator-- ()
{
  --fIndent;

  if (fIndent < 0) {
    gLogStream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndent <<
      endl << endl;

#ifdef DEBUG_INDENTER
    msgAssert (
      __FILE__, __LINE__,
      false,
      "indentation has become negative");
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLogStream <<
      "% INDENTER: " << fIndent <<
      endl;
  }
#endif

  return *this;
}

// increase the indentation by 1, postfix operator
outputIndenter outputIndenter::outputIndenter::operator++ (int)
{
  ++fIndent;

#ifdef DEBUG_INDENTER
  gLogStream <<
    "% INDENTER: " << fIndent <<
    endl;
#endif

  return *this;
}

// decrease the indentation by 1, postfix operator
outputIndenter outputIndenter::outputIndenter::operator-- (int)
{
  --fIndent;

  if (fIndent < 0) {
    gLogStream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndent <<
      endl << endl;

#ifdef DEBUG_INDENTER
    msgAssert (
      __FILE__, __LINE__,
      false,
      "indentation has become negative");
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLogStream <<
      "% INDENTER: " << fIndent <<
      endl;
  }
#endif

  return *this;
}

outputIndenter& outputIndenter::increment (int value)
{
  fIndent += value;

  if (fIndent < 0) {
    gLogStream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndent <<
      endl << endl;

#ifdef DEBUG_INDENTER
    msgAssert (
      __FILE__, __LINE__,
      false,
      "indentation has become negative");
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLogStream <<
      "% INDENTER: " << fIndent <<
      endl;
  }
#endif

  return *this;
}

outputIndenter& outputIndenter::decrement (int value)
{
  fIndent -= value;

  if (fIndent < 0) {
    gLogStream <<
      endl <<
      "% ### Indentation has become negative: " <<  fIndent <<
      endl << endl;

#ifdef DEBUG_INDENTER
    msgAssert (
      __FILE__, __LINE__,
      false,
      "indentation has become negative");
#endif
  }

#ifdef DEBUG_INDENTER
  else {
    gLogStream <<
      "% INDENTER: " << fIndent <<
      endl;
  }
#endif

  return *this;
}

string outputIndenter::indentMultiLineString (const string& theString)
{
  // add indentation ahead of all lines inside 'theString'
  istringstream inputStream (theString);
  string        line;
  stringstream  s;

  while (getline (inputStream, line)) {
    this->print (s);
    s << line;

    if (inputStream.eof ()) break;

    s << endl;
  } // while

  return s.str ();
}

void outputIndenter::print (ostream& os) const
{
  int i = fIndent;

  while (i-- > 0) os << fSpacer;
}

ostream& operator<< (ostream& os, const outputIndenter& theIndenter) {
  theIndenter.print(os);
  return os;
}

//______________________________________________________________________________
int indentedStreamBuf::sync ()
{
  // When we sync the stream with fOutputSteam:
  // 1) output the indentation then the buffer
  // 2) reset the buffer
  // 3) flush the actual output stream we are using.

  unsigned int strSize = str ().size ();

  // fetch the last non-space character in the buffer
  // caution: the '\n' is present as the last character!
  size_t found = str ().find_last_not_of (' ', strSize - 2);

  // this can be uncommented to see low level informations
  // fOutputSteam << "% strSize: " << strSize << ", found: " << found << '\n';

  // output the indenter
  fOutputSteam << fOutputIndenter;

  // output the buffer
  if (found == strSize - 3) {
    // don't output the trailing spaces, but output the end of line
    fOutputSteam << str ().substr (0, found + 1) << '\n';
  }
  else {
    // output the whole buffer
    fOutputSteam << str ();
  }

  // reset the buffer
  str ("");

  // flush the output stream
  fOutputSteam.flush ();

  return 0;
}

//______________________________________________________________________________
// the global indented streams
S_indentedOstream gGlobalOutputIndentedOstream;
S_indentedOstream gGlobalLogIndentedOstream;

void createTheGlobalIndentedOstreams (
  ostream& theOutputStream,
  ostream& theLogStream)
{
  gGlobalOutputIndentedOstream =
    indentedOstream::create (
      theOutputStream,
      outputIndenter::gGlobalOStreamIndenter);

  gGlobalLogIndentedOstream =
    indentedOstream::create (
      theLogStream,
      outputIndenter::gGlobalOStreamIndenter);
}

//______________________________________________________________________________
// code taken from:
// http://comp.lang.cpp.moderated.narkive.com/fylLGJgp/redirect-output-to-dev-null
template<typename Ch, typename Traits = char_traits<Ch> >
struct basic_nullbuf : basic_streambuf<Ch, Traits>
{
  typedef basic_streambuf<Ch, Traits> base_type;
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

ostream cnull  (& cnull_obj);
//wostream wcnull (& wcnull_obj);

//______________________________________________________________________________
string replicateString (
  string str,
  int    times)
{
  string result;

  for (int i = 0; i < times; ++i)
    result += str;

  return result;
}

//______________________________________________________________________________
/* JMI
string replaceSubstringInString (
  string str,
  string subString,
  string ersatz)
{
  string result = str;

  size_t found = result.find (subString);

  if (found != string::npos) {
    result.replace (found, subString.size (), ersatz);
  }

  return result;
}
*/

//______________________________________________________________________________
string int2EnglishWord (int n)
{
  if (n == -9) {
    assert (false); // JMI
  }

  stringstream s;

  if (n < 0) {
    s << "Minus_";
    n = -n;
  }

  if (n >= 1000) {
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

  for (unsigned int i = 0; i < chunks.size (); ++i) {
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
  bool                    debugMode)
{
  string::const_iterator cursor = theStringIterator;
  int    result = 0;

  if (! isdigit (*cursor)) {
    gLogStream <<
      "consumeDecimalNumber (" << *cursor <<
      "), " << *cursor << " is no decimal digit!" <<
      endl;
    }

  while (isdigit (*cursor)) {
    if (debugMode) {
      gLogStream <<
        "--> consumeDecimalNumber: cursor = |" <<
        *cursor <<
        "|" <<
        endl;
    }

    result = result*10 + (*cursor-'0');

    ++cursor;
  } // while

  remainingStringIterator = cursor;

  if (debugMode) {
    gLogStream <<
      "--> consumeDecimalNumber: result = " << result <<
      ", *remainingStringIterator = |" << *remainingStringIterator <<
      "|" <<
      endl;
  }

  return result;
}

//______________________________________________________________________________
set<int> decipherNaturalNumbersSetSpecification (
  string theString,
  bool   debugMode)
{
  // A naturalNumbersSetSpecification sample is: "7,15-19,^16-17"

  set<int> result;

  if (debugMode) {
    gLogStream <<
      "--> decipherNaturalNumbersSetSpecification, theString = |" << theString <<
      "|" <<
      endl;
  }

  if (theString.size ()) {
    string::const_iterator
      cursor = theString.begin ();

    while (1) {
      if (debugMode) {
        gLogStream <<
          "--> decipherNaturalNumbersSetSpecification: cursor = |" <<
          *cursor << "|" <<
          endl;
      }

      int negated = 0;

      if (*cursor == '^') {
        ++cursor;
        negated = 1;
      }

      int
        intervalStartNumber =
          consumeDecimalNumber (cursor, cursor, debugMode),
        intervalEndNumber;

      if (*cursor == '-') {
        ++cursor;

        if (debugMode) {
          gLogStream <<
            "--> decipherNaturalNumbersSetSpecification after '-' : cursor = |" <<
            *cursor <<
            "|" <<
            endl << endl;
        }

        intervalEndNumber =
          consumeDecimalNumber (cursor, cursor, debugMode);
      }

      else {
        intervalEndNumber = intervalStartNumber;
      }

      if (debugMode) {
        gLogStream <<
          "--> decipherNaturalNumbersSetSpecification" <<
          ", intervalStartNumber = " << intervalStartNumber <<
          ", intervalEndNumber = " << intervalEndNumber <<
          ": *cursor = |" << *cursor << "|" <<
          endl;
      }

      for (int i = intervalStartNumber; i <= intervalEndNumber; ++i) {
        if (negated) {
          result.erase (i);
        }
        else {
          result.insert (i);
        }
      } // for

      if (*cursor != ',') {
        if (debugMode) {
          gLogStream <<
            "--> decipherNaturalNumbersSetSpecification, after non ',' : cursor = |" <<
            *cursor <<
            "|" <<
            endl << endl;
        }
        break;
      }

      ++cursor;

      if (debugMode) {
        gLogStream <<
          "--> decipherNaturalNumbersSetSpecification after ',' : cursor = |" <<
          *cursor <<
          "|"
          << endl <<
          endl;
      }
    } // while

    if (* cursor != '\0') {
      gLogStream <<
        "--> Extraneous characters |" << *cursor <<
        "| in numbers spec" <<
        endl << endl;
    }
  }

  return result;
}

//______________________________________________________________________________
string consumeString (
  string::const_iterator  theStringIterator,
  string::const_iterator& remainingStringIterator,
  bool                    debugMode)
{
  string result;

/* JMI
  string::const_iterator cursor = theStringIterator;

  while ((*cursor) != ',') {
    if (debugMode) {
      gLogStream <<
        "--> consumeString: cursor = |" <<
        *cursor <<
        "|" <<
        endl;
    }

    result += (*cursor);

    if (++cursor == theString.end ()) break;
  } // while

  remainingStringIterator = cursor;

  if (debugMode) {
    gLogStream <<
      "--> consumeString: result = " << result <<
      ", *remainingStringIterator = |" << *remainingStringIterator <<
      "|" <<
      endl;
  }
*/

  return result;
}

//______________________________________________________________________________
set<string> decipherStringsSetSpecification (
  string theString,
  bool   debugMode)
{
  // A integersSetSpecification sample is: "FOO,159,haLLo"

  set<string> result;

/* JMI
  if (debugMode) {
    gLogStream <<
      "--> decipherStringsSetSpecification, theString = |" << theString <<
      "|" <<
      endl;
  }

  string::const_iterator
    cursor = theString.begin ();

  while (1) {
    if (debugMode) {
      gLogStream <<
        "--> decipherStringsSetSpecification: cursor = |" <<
        *cursor << "|" <<
        endl;
    }

    string
      currentString =
        consumeString (cursor, theString.end (), debugMode);

    if (debugMode) {
      gLogStream <<
        "--> decipherStringsSetSpecification" <<
        ", currentString = " << currentString <<
        ": *cursor = |" << *cursor << "|" <<
        endl;
    }

    result.insert (currentString);

    if (*cursor != ',') {
      if (debugMode) {
        gLogStream <<
          "--> decipherStringsSetSpecification, after non ',' : cursor = |" <<
          *cursor <<
          "|" <<
          endl << endl;
      }
      break;
    }

    if (++cursor == theString.end ()) break;

    if (debugMode) {
      gLogStream <<
        "--> decipherStringsSetSpecification after ',' : cursor = |" <<
        *cursor <<
        "|"
        << endl <<
        endl;
    }
  } // while

  if (* cursor != '\0') {
    gLogStream <<
      "--> Extraneous characters |" << *cursor <<
      "| in numbers spec" <<
      endl << endl;
  }
  */

  return result;
}

//______________________________________________________________________________
list<int> extractNumbersFromString (
  string theString, // can contain "1, 2, 17"
  bool   debugMode)
{
  list<int> foundNumbers;

  if (debugMode) {
    gLogStream <<
      "--> extractNumbersFromString, theString = |" << theString <<
      "|" <<
      endl;
  }

  if (theString.size ()) {
    string::const_iterator
      cursor = theString.begin ();

    while (1) {
      if (cursor == theString.end ())
        break;

      if (debugMode) {
        gLogStream <<
          "--> extractNumbersFromString: cursor = |" <<
          *cursor << "|" <<
          endl;
      }

      if (isdigit (*cursor)) {
        // consume a decimal number
        int n = 0;
        while (isdigit (*cursor)) {
          n = n * 10 + (*cursor - '0');
          ++cursor;
        } // while

        // append the number to the list
        foundNumbers.push_back (n);
      }
      else {
        ++cursor;
      }
    } // while
  }

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
    gLogStream <<
      "--> extractNamesPairFromString, theString = |" << theString <<
      "|" <<
      endl;
  }

  if (theString.size ()) {
    string::const_iterator
      cursor = theString.begin ();

    // fetch name1
    while (1) {
      if (cursor == theString.end ())
        break;

      if (debugMode) {
        gLogStream <<
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
      ++cursor;
    } // while

    name1 = trim (name1);
    if (! name1.size ()) {
      // found an empty name1
      gLogStream <<
        "### ERROR: the first name before the " << separator <<
        " separator is empty in '" << theString << "'" <<
        endl;
    }

    if (cursor == theString.end ())
      gLogStream <<
        "### ERROR: the " << separator <<
        " separator is missing in string '" <<
        theString << "'" <<
        endl;
    else
      // overtake the separator
      ++cursor;

    // fetch name2
    while (1) {
      if (cursor == theString.end ())
        break;

      if (debugMode) {
        gLogStream <<
          "--> extractNamesPairFromString: cursor = |" <<
          *cursor << "|" <<
          endl;
      }

      if ((*cursor) == '=') {
        // found the separator
        gLogStream <<
          "### ERROR: the " << separator <<
          " separator occurs more than once in string '" <<
          theString << "'" <<
          endl;
        break;
      }

      // append the character to name2
      name2 += *cursor;
      ++cursor;
    } // while

    name2 = trim (name2);
    if (! name2.size ()) {
      // found an empty name2
      gLogStream <<
        "### ERROR: the second name after the " << separator <<
        " separator is empty in '" << theString << "'" <<
        endl;
    }
  }

  return make_pair (name1, name2);
}

//______________________________________________________________________________
string doubleQuoteStringIfNonAlpha (
  const string& theString)
{
  string result;

  bool   stringShouldBeDoubleQuoted = false;

  if (theString.size ()) {
    for (
      string::const_iterator i = theString.begin ();
      i != theString.end ();
      ++i
    ) {

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
          result += ' '; // TEMP JMI
        else
          result += (*i);

        stringShouldBeDoubleQuoted = true;
      }
    } // for
  }

  if (stringShouldBeDoubleQuoted) {
    return "\"" + result + "\"";
  }
  else {
    return result;
  }
}

//______________________________________________________________________________
string quoteStringIfNonAlpha (
  const string& theString)
{
  string result;

  bool   stringShouldBeQuoted = false;

  if (theString.size ()) {
    for (
      string::const_iterator i = theString.begin ();
      i != theString.end ();
      ++i
    ) {

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
          result += ' '; // TEMP JMI
        else
          result += (*i);

        stringShouldBeQuoted = true;
      }
    } // for
  }

  if (stringShouldBeQuoted) {
    return "'" + result + "'";
  }
  else {
    return result;
  }
}

//______________________________________________________________________________
string doubleQuoteString (
  const string& theString)
{
  string result;

  if (theString.size ()) {
    for (
      string::const_iterator i = theString.begin ();
      i != theString.end ();
      ++i
    ) {

      if (
        ((*i) >= 'a' && (*i) <= 'z')
          ||
        ((*i) >= 'A' && (*i) <= 'Z')) {
        // (*i) is a letter
        result += (*i);
      }

      else {
        // (*i) is not a letter
        if ((*i) == ' ') {
          result += ' '; // TEMP JMI
        }
        else if ((*i) == '"') {
          result += "\\\"";
        }
        else {
          result += (*i);
        }
      }
    } // for
  }

  return "\"" + result + "\"";
}

//______________________________________________________________________________
string quoteString (
  const string& theString)
{
  string result;

  if (theString.size ()) {
    for (
      string::const_iterator i = theString.begin ();
      i != theString.end ();
      ++i
    ) {

      if (
        ((*i) >= 'a' && (*i) <= 'z')
          ||
        ((*i) >= 'A' && (*i) <= 'Z')) {
        // (*i) is a letter
        result += (*i);
      }

      else {
        // (*i) is not a letter
        if ((*i) == ' ') {
          result += ' '; // TEMP JMI
        }
        else if ((*i) == '\'') {
          result += "\\\'";
        }
        else {
          result += (*i);
        }
      }
    } // for
  }

  return "'" + result + "'";
}

//______________________________________________________________________________
string stringToLowerCase (
  const string& theString)
{
  string result = theString;

  for_each (
    result.begin(),
    result.end(),
    [] (char & c) {
      c = ::tolower (c);
      }
    );

  return result;
}

string stringToUpperCase (
  const string& theString)
{
  string result = theString;

  for_each (
    result.begin(),
    result.end(),
    [] (char & c) {
      c = ::toupper (c);
      }
    );

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
    number << ' ';

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
string escapeDoubleQuotes (string s)
{
  string result;

  for_each (
    s.begin (),
    s.end (),
    stringQuoteEscaper (result));

  // replace occurrences of '\\"' by '\"',
  // in case there were already double quotes in string
  string
    lookedFor = "\\\\\"",
    ersatz    = "\\\"";

  for ( ; ; ) {
    size_t found = result.find (lookedFor);

    if (found == string::npos)
      break;

    result.replace (found, lookedFor.size (), ersatz);
  } // for

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
void splitStringIntoChunks (
  string        theString,
  string        theSeparator,
  list<string>& chunksList)
{
  //#define DEBUG_SPLITTING

#ifdef DEBUG_SPLITTING
  gLogStream <<
    "---> splitting |" << theString << "|" <<
    endl << endl;
#endif

  unsigned int theStringSize = theString.size ();

  unsigned int currentPosition = 0;

#ifdef DEBUG_SPLITTING
  string remainder = theString;
#endif

  unsigned int theSeparatorSize = theSeparator.size ();

  map<string, string>::const_iterator i;

  while (1) {
    size_t found =
      theString.find (theSeparator, currentPosition);

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
      gLogStream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl << endl;
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
        chunkLength + theSeparatorSize;

      // there can be an end of line JMI
      if (theString [currentPosition] == '\n')
        ++currentPosition;

#ifdef DEBUG_SPLITTING
      // set remainder
      remainder =
        theString.substr (
          currentPosition);

      gLogStream <<
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
        endl << endl;
#endif
    }
  } // while
}

//______________________________________________________________________________
void splitRegularStringAtEndOfLines (
  string        theString,
  list<string>& chunksList)
{
//#define DEBUG_SPLITTING

#ifdef DEBUG_SPLITTING
  gLogStream <<
    "---> splitting |" << theString << "|" <<
    endl << endl;
#endif

  splitStringIntoChunks (
    theString,
    "\n",
    chunksList);

    /* JMI
  unsigned int theStringSize = theString.size ();

  unsigned int currentPosition = 0;

#ifdef DEBUG_SPLITTING
  string remainder = theString;
#endif

  string lookedFor     = "\n";
  unsigned int    lookedForSize = lookedFor.size ();

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
      gLogStream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl << endl;
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
        ++currentPosition;

#ifdef DEBUG_SPLITTING
      // set remainder
      remainder =
        theString.substr (
          currentPosition);

      gLogStream <<
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
        endl << endl;
#endif
    }
  } // while
  */
}

//______________________________________________________________________________
void splitHTMLStringContainingEndOfLines (
  string        theString,
  list<string>& chunksList)
{
//#define DEBUG_SPLITTING

#ifdef DEBUG_SPLITTING
  gLogStream <<
    "---> splitting |" << theString << "|" <<
    endl << endl;
#endif

  unsigned int theStringSize = theString.size ();

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
  string       lookedFor     = "\n";
  unsigned int lookedForSize = lookedFor.size ();

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
      gLogStream <<
        "theStringSize = " << theStringSize <<
        endl <<
        "currentPosition = " << currentPosition <<
        endl <<
        "remainder = |" << remainder << "|" <<
        endl <<
        "chunkLength = " << chunkLength <<
        endl <<
        "chunk = \"" << chunk << "\"" <<
        endl << endl;
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
        ++currentPosition;

#ifdef DEBUG_SPLITTING
      // set remainder
      remainder =
        theString.substr (
          currentPosition);

      gLogStream <<
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
        endl << endl;
#endif
    }
  } // while
}

//______________________________________________________________________________
string baseName (const string& filename)
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
    --len;
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

  if (theString.size ()) {
    for (
      string::const_iterator i = theString.begin ();
      i != theString.end ();
      ++i
    ) {
      if (isalnum (*i)) {
        result.push_back ((*i));
      }
    } // for
  }

  return result;
}


}

/* JMI
//______________________________________________________________________________
class segmentedLinesOstream
{
/ * JMI NOT DONE
  // in order to avoid spaces at the end of a line,
  // an end of segment causes a space to be output later,
  // by the next '<<' operator

--
*
Reference for this class:
  https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl

Usage:
  segmentedLinesOstream myStream (cout);

  myStream <<
    1 << 2 << 3 << endl <<
    5 << 6 << endl <<
    7 << 8 << endl;
* /

  private:
    // segmentedLinesOstream just uses an indentedOstream
    indentedOstream&      fIndentedOstream;

    // an end of segment causes a space to be output by the next '<<' operator
    bool                  fAtEndOfSegment;

  public:

    // constructor
                          segmentedLinesOstream (
                            indentedOstream& indentedOstream)
                            : fIndentedOstream (indentedOstream)
                              { fAtEndOfSegment = false; }

    // destructor
    virtual               ~segmentedLinesOstream ()
        {};

    // flush
    void                  flush ()
                              { fIndentedOstream.flush (); }

    // set and get
    indentedOstream&      getIndentedOstream ()
                              { return fIndentedOstream; }

    // indentation
    outputIndenter&       getIndenter ()
                              { return fIndentedOstream.getIndenter (); }

    void                  incrIdentation ()
                              { fIndentedOstream.incrIdentation (); }

    void                  decrIdentation ()
                              { fIndentedOstream.decrIdentation (); }

    // segments
    void                  setAtEndOfSegment (bool value)
                              { fAtEndOfSegment = value; }
    bool                  getAtEndOfSegment ()
                              { return fAtEndOfSegment; }
};

// '<<' operators to implement segments
EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, char ch);
EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, int i);
EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, unsigned int i);
EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, float f);
EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, const string& str);
EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, char * str);

// the manipulators
segmentedLinesOstream& endline (segmentedLinesOstream& os);
segmentedLinesOstream& endseg (segmentedLinesOstream& os);
*/


/* JMI
  segmentedLinesOstream
    testSegmentedLinesOstream (fLogOutputStream);

  fLogOutputStream <<
    "getAtEndOfSegment: " <<
    booleanAsString (
      testSegmentedLinesOstream.getAtEndOfSegment ()) <<
    endl;

  testSegmentedLinesOstream.setAtEndOfSegment (true);

  fLogOutputStream <<
    "getAtEndOfSegment: " <<
    booleanAsString (
      testSegmentedLinesOstream.getAtEndOfSegment ()) <<
    endl;

  testSegmentedLinesOstream <<
    "FOO JMI" << endl; // <<
 //   endline;

  testSegmentedLinesOstream.getIndentedOstream () << flush;
  */


/* JMI
//______________________________________________________________________________
// the manipulators
segmentedLinesOstream& endline (segmentedLinesOstream& os)
{
  if (! os.getAtEndOfSegment ()) {
    // don't output multiple spaces after a segment
    os.setAtEndOfSegment (true);
  }

  os.getIndentedOstream () << endl;

  return os;
}

segmentedLinesOstream& endseg (segmentedLinesOstream& os)
{
  if (! os.getAtEndOfSegment ()) {
    // don't output multiple spaces after a segment
    os.setAtEndOfSegment (true);
  }

  return os;
}

// '<<' operators to implement segments
EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, char ch)
{
  if (os.getAtEndOfSegment ()) {
    os << ' ';
    os.setAtEndOfSegment (false);
  }

  os.getIndentedOstream () << ch;

  return os;
}

EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, int i)
{
  if (os.getAtEndOfSegment ()) {
    os << ' ';
    os.setAtEndOfSegment (false);
  }

  os.getIndentedOstream () << i;

  return os;
}

EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, unsigned int i)
{
  if (os.getAtEndOfSegment ()) {
    os << ' ';
    os.setAtEndOfSegment (false);
  }

  os.getIndentedOstream () << i;

  return os;
}

EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, float f)
{
  if (os.getAtEndOfSegment ()) {
    os << ' ';
    os.setAtEndOfSegment (false);
  }

  os.getIndentedOstream () << f;

  return os;
}

EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, const string& str)
{
  if (os.getAtEndOfSegment ()) {
    os << ' ';
    os.setAtEndOfSegment (false);
  }

  os.getIndentedOstream () << str;

  return os;
}

EXP segmentedLinesOstream& operator<< (segmentedLinesOstream& os, char * str)
{
  if (os.getAtEndOfSegment ()) {
    os << ' ';
    os.setAtEndOfSegment (false);
  }

  os.getIndentedOstream () << str;

  return os;
}
*/

