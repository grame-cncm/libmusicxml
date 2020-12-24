/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___utilities___
#define ___utilities___

#include <string>
#include <cassert>

#include <string.h>

#include <iostream>
#include <sstream>

#include <ctime>

#include <algorithm>    // for_each JMI
#include <functional>

#include <set>
#include <list>

/* JMI
#ifdef WIN32
  // JMI
#else
  #include <iconv.h>
#endif
*/

#include "smartpointer.h"
#include "basevisitor.h"
#include "exports.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class timingItem : public smartable
{
  public:
    enum timingItemKind {
      kMandatory, kOptional
    };

    static SMARTP<timingItem> createTimingItem (
      string    activity,
      string    description,
      timingItemKind kind,
      clock_t   startClock,
      clock_t   endClock);

    timingItem (
      string    activity,
      string    description,
      timingItemKind kind,
      clock_t   startClock,
      clock_t   endClock);

    string           fActivity;
    string           fDescription;
    timingItemKind        fKind;
    clock_t               fStartClock;
    clock_t               fEndClock;
};

typedef SMARTP<timingItem> S_timingItem;

class EXP timing {
  public:
       timing ();
    virtual ~timing ();

    // global variable for general use
    static timing gGlobalTiming;

    // add an item
    void                  appendTimingItem (
                            string    activity,
                            string    description,
                            timingItem::timingItemKind
                                           kind,
                            clock_t        startClock,
                            clock_t        endClock);

    // print
    void                  print (ostream& os) const;

    void                  printWithContext (
                            string   context,
                            ostream& os) const;

  private:

    // private services
    void                  doPrint (ostream& os) const;

  private:

    // private fields
    list<S_timingItem>    fTimingItemsList;
};
EXP ostream& operator<< (ostream& os, const timing& tim);

//______________________________________________________________________________
class EXP outputIndenter
{
  public:

    outputIndenter (string spacer = "  ");
    virtual ~outputIndenter ();

    // set the indent
    void                  setIndent (int indent)
                              { fIndent = indent; }

    // get the indent
    int                   getIndent () const
                              { return fIndent; }

    // increase the indentation by 1
    outputIndenter&        operator++ (const int value);

    // decrease the indentation by 1
    outputIndenter&       operator-- (const int value);

    outputIndenter&       increment (int value);
    outputIndenter&       decrement (int value);

    // reset the indentation
    void                  resetToZero ()
                              { fIndent = 0; }

    // compare indentation value
    bool                  operator == (const int &value) const
                              { return fIndent == value; }
    bool                  operator != (const int &value) const
                              { return fIndent != value; }

    // output as much space as specified
    void                  print (ostream& os) const;

    // get a spacer for adhoc uses, without increasing the indentation
    string                getSpacer () const
                              { return fSpacer; }

    // indent a multiline 'R"(...)"' string
    string                indentMultiLineString (string theString);

    // global variables for general use
    static outputIndenter gGlobalOStreamIndenter;

  private:
    int                   fIndent;
    string                fSpacer;
};

EXP ostream& operator<< (ostream& os, const outputIndenter& theIndenter);

// useful shortcut macros
#define gIndenter outputIndenter::gGlobalOStreamIndenter
#define gTab      outputIndenter::gGlobalOStreamIndenter.getSpacer ()

//______________________________________________________________________________
// a stream buffer that prefixes each line
// with the current indentation, i.e. spaces

/*
endl declaration:

  endl for ostream
  ostream& endl (ostream& os);

  basic template
  template <class charT, class traits>
  basic_ostream<charT,traits>& endl (basic_ostream<charT,traits>& os);

  Insert newline and flush
  Inserts a new-line character and flushes the stream.

  Its behavior is equivalent to calling os.put('\n') (or os.put(os.widen('\n')) for character types other than char), and then os.flush().

--

Reference for this class:
  https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl
*/

class indentedStreamBuf: public stringbuf
{
  private:

    ostream&         fOutputSteam;
    outputIndenter&       fOutputIndenter;

  public:

    // constructor
    indentedStreamBuf (
      ostream&   outputStream,
      outputIndenter& theIndenter)
      : fOutputSteam (
          outputStream),
        fOutputIndenter (
          theIndenter)
        {}

    // indentation
    outputIndenter&       getOutputIndenter () const
                              { return fOutputIndenter; }

    // flush
    void                  flush ()
                              { fOutputSteam.flush (); }

    virtual int           sync ();
};

//______________________________________________________________________________
class EXP indentedOstream: public ostream, public smartable
{
/*
Reference for this class:
  https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl

Usage:
  indentedOstream myStream (cout);

  myStream <<
    1 << 2 << 3 << endl <<
    5 << 6 << endl <<
    7 << 8 << endl;
*/

  private:
    // indentedOstream just uses an indentedStreamBuf
    indentedStreamBuf     fIndentedStreamBuf;

  public:

    static SMARTP<indentedOstream> create (
      ostream&        theOStream,
      outputIndenter& theIndenter)
    {
      indentedOstream* o = new indentedOstream (
        theOStream,
        theIndenter);
      assert (o!=0);

      return o;
    }

    // constructor
    indentedOstream (
      ostream&        theOStream,
      outputIndenter& theIndenter)
      : ostream (
          & fIndentedStreamBuf),
        fIndentedStreamBuf (
          theOStream,
          theIndenter)
        {}

    // destructor
    virtual ~indentedOstream () {};

    // flush
    void                  flush ()
                              { fIndentedStreamBuf.flush (); }

    // indentation
    outputIndenter&       getIndenter () const
                              { return fIndentedStreamBuf.getOutputIndenter (); }

    void                  incrIdentation ()
                              { fIndentedStreamBuf.getOutputIndenter ()++; }

    void                  decrIdentation ()
                              { fIndentedStreamBuf.getOutputIndenter ()--; }
};
typedef SMARTP<indentedOstream> S_indentedOstream;

//______________________________________________________________________________
// the global log indented stream
EXP extern S_indentedOstream gGlobalOutputIndentedOstream;
EXP extern S_indentedOstream gGlobalLogIndentedOstream;

#define gOutputStream *gGlobalOutputIndentedOstream
#define gLogStream    *gGlobalLogIndentedOstream

EXP extern void createTheGlobalIndentedOstreams (
  ostream& theOutputStream,
  ostream& theLogStream);

//______________________________________________________________________________
struct stringQuoteEscaper
{
  /* usage:
      string dest = "";
      for_each( source.begin (), source.end (), stringQuoteEscaper (dest));
  */

  string&            target;

  explicit                stringQuoteEscaper (string& t)
                            : target (t)
                              {}

  void                    operator() (char ch) const
                              {
                                 if( ch == '"') {
                                   // or switch on any character that
                                   // needs escaping like '\' itself
                                    target.push_back ('\\');
                                 }
                                 target.push_back (ch);
                              }
};

//______________________________________________________________________________
struct stringSpaceRemover
{
  /* usage:
      string dest = "";
      for_each (
        source.begin (),
        source.end (),
        stringSpaceRemover (dest));
  */

  string&            target;

  explicit                stringSpaceRemover (string& t)
                            : target (t)
                              {}

  void                    operator() (char ch) const
                              {
                                if (ch != ' ') {
                                  target.push_back (ch);
                                }
                              }
};

//______________________________________________________________________________
struct stringSpaceReplacer
{
  /* usage:
      string dest = "";
      for_each (
        source.begin (),
        source.end (),
        stringSpaceReplacer (dest, ersatz));
  */

  string&            target;
  char                    ersatz;

  explicit                stringSpaceReplacer (string& t, char ch)
                            : target (t), ersatz (ch)
                              {}

  void                    operator() (char ch) const
                              {
                                if (ch == ' ')
                                  target.push_back (ersatz);
                                else
                                  target.push_back (ch);
                              }
};

//______________________________________________________________________________
string replicateString (
  string str,
  int    times);

//______________________________________________________________________________
string int2EnglishWord (int n);

//______________________________________________________________________________
string stringNumbersToEnglishWords (string str);

//______________________________________________________________________________
set<int> decipherNaturalNumbersSetSpecification (
  string theSpecification,
  bool        debugMode = false);

//______________________________________________________________________________
set<string> decipherStringsSetSpecification (
  string theSpecification,
  bool        debugMode = false);

//______________________________________________________________________________
list<int> extractNumbersFromString (
  string theString, // can contain "1, 2, 17"
  bool        debugMode = false);

//______________________________________________________________________________
// from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim string from start
EXP inline string& ltrim (string& s) {
  function <int (int)>
    checkSpace =
      [] (int x) { return isspace (x); };

  s.erase (
    s.begin (),
    find_if (
      s.begin (),
      s.end (),
      not1 (checkSpace)
      )
    );

  return s;
}

// trim string from end
EXP inline string& rtrim (string& s) {
  function <int (int)>
    checkSpace =
      [] (int x) { return isspace (x); };

  s.erase (
    find_if (
      s.rbegin (),
      s.rend (),
      not1 (checkSpace)
      ).base(),
    s.end ()
    );

  return s;
}

// trim string from both ends
EXP inline string& trim (string& s) {
  return ltrim (rtrim (s));
}

//______________________________________________________________________________
pair<string, string> extractNamesPairFromString (
  string theString, // may contain "P1 = Bassoon"
  char        separator,
  bool        debugMode = false);

//______________________________________________________________________________
string doubleQuoteStringIfNonAlpha (
  string theString);

string quoteStringIfNonAlpha (
  string theString);

string doubleQuoteString (
  string theString);

string quoteString (
  string theString);

string stringToLowerCase (
  string theString);

string stringToUpperCase (
  string theString);

//______________________________________________________________________________
string booleanAsString (bool value);

//______________________________________________________________________________
string singularOrPlural (
  int number, string singularName, string pluralName);

string singularOrPluralWithoutNumber (
  int number, string singularName, string pluralName);

//______________________________________________________________________________
string escapeDoubleQuotes (string s);

//______________________________________________________________________________
void convertHTMLEntitiesToPlainCharacters (string& s);

//______________________________________________________________________________
void splitStringIntoChunks (
  string             theString,
  string             theSeparator,
  list<string>& chunksList);

void splitRegularStringAtEndOfLines (
  string             theString,
  list<string>& chunksList);

void splitHTMLStringContainingEndOfLines ( // JMI
  string             theString,
  list<string>& chunksList);

//______________________________________________________________________________
string  baseName (const string& filename);
  // wait until c++17 for a standard library containing basename()...

//______________________________________________________________________________
string  makeSingleWordFromString (const string& theString);


} // namespace MusicXML2


#endif

/* JMI worth it?
//______________________________________________________________________________
class EXP indentedSstream: public stringstream, public smartable
{
/ *
  explicit stringstream (ios_base::openmode which = ios_base::in | ios_base::out);

    Constructs a stringstream object with an empty sequence as content.
    Internally, its iostream base constructor is passed a pointer to
    a stringbuf object constructed with which as argument.
* /

  private:
    // indentedSstream just uses an indentedStreamBuf
    indentedStreamBuf     fIndentedStreamBuf;

  public:

    static SMARTP<indentedSstream> create (
      stringstream&   theStringStream,
      outputIndenter& theIndenter)
    {
      indentedSstream* o = new indentedSstream (
        theStringStream,
        theIndenter);
      assert (o!=0);

      return o;
    }

    // constructor
    indentedSstream (
      stringstream&   theStringStream,
      outputIndenter& theIndenter)
      : stringstream (
          & fIndentedStreamBuf),
        fIndentedStreamBuf (
          theStringStream,
          theIndenter)
        {}

    // destructor
    virtual ~indentedSstream () {};

    // flush
    void                  flush ()
                              { fIndentedStreamBuf.flush (); }

    // indentation
    outputIndenter&       getIndenter () const
                              { return fIndentedStreamBuf.getOutputIndenter (); }

    void                  incrIdentation ()
                              { fIndentedStreamBuf.getOutputIndenter ()++; }

    void                  decrIdentation ()
                              { fIndentedStreamBuf.getOutputIndenter ()--; }
};
typedef SMARTP<indentedSstream> S_indentedSstream;
*/

