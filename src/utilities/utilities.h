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

#include <iostream>
#include <sstream>

#include <ctime>

#include <set>
#include <list>

#include <functional>
#include <algorithm>

#include <string.h>

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


namespace MusicXML2
{

//______________________________________________________________________________
class EXP timingItem : public smartable
{
  public:
    enum timingItemKind { kMandatory, kOptional };

    static SMARTP<timingItem> createTimingItem (
      std::string    activity,
      std::string    description,
      timingItemKind kind,
      std::clock_t   startClock,
      std::clock_t   endClock);

    timingItem (
      std::string    activity,
      std::string    description,
      timingItemKind kind,
      std::clock_t   startClock,
      std::clock_t   endClock);

    std::string           fActivity;
    std::string           fDescription;
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
    static timing gTiming;

    // add an item
    void                  appendTimingItem (
                            std::string    activity,
                            std::string    description,
                            timingItem::timingItemKind
                                           kind,
                            clock_t        startClock,
                            clock_t        endClock);

    // print
    void                  print (std::ostream& os) const;

  private:

    std::list<S_timingItem>
                          fTimingItemsList;
};
EXP std::ostream& operator<< (std::ostream& os, const timing& tim);

//______________________________________________________________________________
class EXP indenter
{
  public:

    indenter (std::string spacer = "  ");
    virtual ~indenter ();

    // get the indent
    int                   getIndent () const
                              { return fIndent; }

    // increase the indentation by 1
    indenter&             operator++ (const int value);

    // decrease the indentation by 1
    indenter&             operator-- (const int value);

    indenter&             increment (int value);
    indenter&             decrement (int value);

    // reset the indentation
    void                  resetToZero ()
                              { fIndent = 0; }

    // check indentation value
    bool                  operator == (const int &value) const
                              { return fIndent == value; }
    bool                  operator != (const int &value) const
                              { return fIndent != value; }

    // output as much space as specified
    void                  print (std::ostream& os) const;

    // get a spacer for adhoc uses, without increasing the indentation
    std::string           getSpacer () const
                              { return fSpacer; }

    // indent a multiline 'R"(...)"' std::string
    std::string           indentMultiLineString (std::string value);

    // global variable for general use
    static indenter       gIndenter;

  private:
    int                   fIndent;
    std::string           fSpacer;
};

EXP std::ostream& operator<< (std::ostream& os, const indenter& idtr);

// useful shortcut macros
#define gIndenter indenter::gIndenter
#define gTab      indenter::gIndenter.getSpacer ()

//______________________________________________________________________________
// a stream buffer that prefixes each line
// with the current indentation, i.e. spaces

/*
std::endl declaration:

  std::endl for ostream
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

class indentedStreamBuf: public std::stringbuf
{
  private:

    std::ostream&         fOutputSteam;
    indenter&             fIndenter;

  public:

    // constructor
    indentedStreamBuf (
      std::ostream& outputStream,
      indenter&     idtr)
      : fOutputSteam (outputStream),
        fIndenter (idtr)
        {}

    // indentation
    indenter&             getIndenter ()
                              { return fIndenter; }

    // flush
    void                  flush ()
                              { fOutputSteam.flush (); }

    virtual int           sync ();
};

//______________________________________________________________________________
class EXP indentedOstream: public std::ostream
{
/*
Reference for this class:
  https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl

Usage:
  indentedOstream myStream (std::cout);

  myStream <<
    1 << 2 << 3 << std::endl <<
    5 << 6 << std::endl <<
    7 << 8 << std::endl;
*/

  private:
    // indentedOstream just uses an indentedStreamBuf
    indentedStreamBuf     fIndentedStreamBuf;

  public:

    // constructor
    indentedOstream (
      std::ostream&  str,
      indenter&      idtr)
      : std::ostream (&fIndentedStreamBuf),
        fIndentedStreamBuf (str, idtr)
        {}

    // destructor
    virtual ~indentedOstream ()
        {};

    // flush
    void                  flush ()
                              { fIndentedStreamBuf.flush (); }

    // indentation
    indenter&             getIndenter ()
                              { return fIndentedStreamBuf.getIndenter (); }

    void                  incrIdentation ()
                              { fIndentedStreamBuf.getIndenter ()++; }

    void                  decrIdentation ()
                              { fIndentedStreamBuf.getIndenter ()--; }

    // global variables for general use
    static indentedOstream
                          gOutputIndentedOstream;
    static indentedOstream
                          gLogIndentedOstream;
    static indentedOstream
                          gNullIndentedOstream;
};

// useful shortcut macros
#define gOutputOstream indentedOstream::gOutputIndentedOstream
#define gLogOstream    indentedOstream::gLogIndentedOstream
#define gNullOstream   indentedOstream::gNullIndentedOstream

//______________________________________________________________________________
struct stringQuoteEscaper
{
  /* usage:
      string dest = "";
      for_each( source.begin (), source.end (), stringQuoteEscaper (dest));
  */

  std::string&            target;

  explicit                stringQuoteEscaper (std::string& t)
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
      std::string dest = "";
      for_each (
        source.begin (),
        source.end (),
        stringSpaceRemover (dest));
  */

  std::string&            target;

  explicit                stringSpaceRemover (std::string& t)
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
      std::string dest = "";
      for_each (
        source.begin (),
        source.end (),
        stringSpaceReplacer (dest, ersatz));
  */

  std::string&            target;
  char                    ersatz;

  explicit                stringSpaceReplacer (std::string& t, char ch)
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
std::string replicateString (
  std::string str,
  int         times);

//______________________________________________________________________________
std::string int2EnglishWord (int n);

//______________________________________________________________________________
std::string stringNumbersToEnglishWords (std::string str);

//______________________________________________________________________________
std::set<int> decipherNaturalNumbersSetSpecification (
  std::string theSpecification,
  bool        debugMode = false);

//______________________________________________________________________________
std::set<std::string> decipherStringsSetSpecification (
  std::string theSpecification,
  bool        debugMode = false);

//______________________________________________________________________________
std::list<int> extractNumbersFromString (
  std::string theString, // can contain "1, 2, 17"
  bool        debugMode = false);

//______________________________________________________________________________
// from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim string from start
inline std::string &ltrim (std::string &s) {
  std::function <int (int)>
    checkSpace =
      [] (int x) { return isspace (x); };

  s.erase (
    s.begin (),
    find_if (
      s.begin (),
      s.end (),
      std::not1 (checkSpace)
      )
    );

  return s;
}

// trim string from end
inline std::string &rtrim (std::string &s) {
  std::function <int (int)>
    checkSpace =
      [] (int x) { return isspace (x); };

  s.erase (
    find_if (
      s.rbegin (),
      s.rend (),
      std::not1 (checkSpace)
      ).base(),
    s.end ()
    );

  return s;
}

// trim string from both ends
inline std::string &trim (std::string &s) {
  return ltrim (rtrim (s));
}

//______________________________________________________________________________
std::pair<std::string, std::string> extractNamesPairFromString (
  std::string theString, // may contain "P1 = Bassoon"
  char        separator,
  bool        debugMode = false);

//______________________________________________________________________________
std::string doubleQuoteStringIfNonAlpha (
  std::string theString);

std::string quoteStringIfNonAlpha (
  std::string theString);

std::string doubleQuoteString (
  std::string theString);

std::string quoteString (
  std::string theString);

//______________________________________________________________________________
std::string booleanAsString (bool value);

//______________________________________________________________________________
std::string singularOrPlural (
  int number, std::string singularName, std::string pluralName);

std::string singularOrPluralWithoutNumber (
  int number, std::string singularName, std::string pluralName);

//______________________________________________________________________________
std::string escapeDoubleQuotes (std::string s);

//______________________________________________________________________________
void convertHTMLEntitiesToPlainCharacters (std::string& s);

//______________________________________________________________________________
void splitStringIntoChunks (
  std::string             theString,
  std::string             theSeparator,
  std::list<std::string>& chunksList);

void splitRegularStringAtEndOfLines (
  std::string             theString,
  std::list<std::string>& chunksList);

void splitHTMLStringContainingEndOfLines ( // JMI
  std::string             theString,
  std::list<std::string>& chunksList);

//______________________________________________________________________________
std::string baseName (const std::string &filename);
  // wait until c++17 for a standard library containing basename()...

//______________________________________________________________________________
std::string makeSingleWordFromString (const std::string& theString);


} // namespace MusicXML2


#endif
