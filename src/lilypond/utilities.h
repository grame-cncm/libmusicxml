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
std::ostream& operator<< (std::ostream& os, const timing& tim);

//______________________________________________________________________________
class EXP indenter
{
  public:

    indenter (std::string spacer = "  ");
    virtual ~indenter ();

    // get the indent
    int                   getIndent () const  { return fIndent; }
                         
    // increase the indentation by 1
    indenter&             operator++ (const int value);
    
    // decrease the indentation by 1
    indenter&             operator-- (const int value);

    indenter&             increment (int value);
    indenter&             decrement (int value);

    // reset the indentation
    void                  resetToZero ()    { fIndent = 0; }

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
    std::string                indentMultiLineString (std::string value);
    
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
class EXP indentedOstream: public std::ostream
{
/*
Reference:
 https://stackoverflow.com/questions/2212776/overload-handling-of-stdendl

Usage:
  indentedOstream myStream (std::cout);
   
  myStream <<
    1 << 2 << 3 << std::endl <<
    5 << 6 << std::endl <<
    7 << 8 << std::endl;
*/
 
  // a stream buffer that prefixes each line
  // with the current indentation
  class indentedStreamBuf: public std::stringbuf
  {
    private:
    
      std::ostream& fOutput;
      indenter&     fIndenter;

    public:
    
      // constructor
      indentedStreamBuf (
        std::ostream& str,
        indenter&     idtr)
        : fOutput (str),
          fIndenter (idtr)
          {}

      // flush
      void flush ()
          {
            fOutput.flush ();
          }
    
      // When we sync the stream with fOutput:
      // 1) output the indentation then the buffer
      // 2) reset the buffer
      // 3) flush the actual output stream we are using.
      virtual int sync ()
          {
            fOutput << fIndenter << str ();
            str ("");
            fOutput.flush ();
            return 0;
          }
  };

  private:
    // indentedOstream just uses a version of indentedStreamBuf
    indentedStreamBuf     fIndentedStreamBuf;
  
  public:
  
    // constructor
    indentedOstream (
      std::ostream&  str,
      indenter&      idtr)
      : std::ostream (&fIndentedStreamBuf),
        fIndentedStreamBuf (
          str, idtr)
      {}

    // destructor
    virtual ~indentedOstream ()
        {};

    // flush
    void flush ()
        {
          fIndentedStreamBuf.flush ();
        }
    
    // global variables for general use
    static indentedOstream
                          gOutputIndentedOstream; 
    static indentedOstream
                          gLogIndentedOstream; 
    static indentedOstream
                          gNullIndentedOstream; 
};

// useful shortcut macros
#define gOutputIOstream indentedOstream::gOutputIndentedOstream
#define gLogIOstream    indentedOstream::gLogIndentedOstream
#define gNullIOstream   indentedOstream::gNullIndentedOstream

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
  int    times);

//______________________________________________________________________________
std::string int2EnglishWord (int n);

//______________________________________________________________________________
std::string stringNumbersToEnglishWords (std::string str);

//______________________________________________________________________________
std::set<int> decipherNumbersSetSpecification (
  std::string theSpecification,
  bool   debugMode = false);

//______________________________________________________________________________
std::list<int> extractNumbersFromString (
  std::string theString, // can contain "1, 2, 17"
  bool   debugMode = false);

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
  char   separator,
  bool   debugMode = false);

//______________________________________________________________________________
std::string quoteStringIfNonAlpha (
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
void optionError (std::string errorMessage);

//______________________________________________________________________________
std::string escapeQuotes (std::string s);

//______________________________________________________________________________
void convertHTMLEntitiesToPlainCharacters (std::string& s);

//______________________________________________________________________________
void splitRegularStringContainingEndOfLines (
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

//______________________________________________________________________________
/* JMI
#ifdef WIN32
  // JMI
#else
class IConv {
  // see https://stackoverflow.com/questions/8104154/iconv-only-works-once

  public:
  
    IConv (const char* to, const char* from);
        
    ~IConv ();

    bool                  convert (char* input, char* output, size_t& outputSize);

    bool                  convert (std::string& input, std::string& output);

  private:
  
    iconv_t               fIconvDescriptor;

    size_t                fInputBufferSize;
    char*                 fInputBuffer;

    size_t                fOutputBufferSize;
    char*                 fOutputBuffer;
};
#endif
*/

//______________________________________________________________________________
#ifdef WIN32
  // JMI
#else
// see: http://www.icce.rug.nl/documents/cplusplus/
class IFdStreambuf: public std::streambuf, public smartable
{
  protected:
  
    int                   d_fd;
    char                  d_buffer [1];
      
  public:
  
    IFdStreambuf (int fd);
      
  private:
  
    int                   underflow ();
};
typedef SMARTP<IFdStreambuf> S_IFdStreambuf;
#endif

//______________________________________________________________________________
#ifdef WIN32
  // JMI
#else
// see: http://www.icce.rug.nl/documents/cplusplus/
class IFdNStreambuf: public std::streambuf, public smartable
{
  protected:
  
    int                   d_fd;
    size_t                d_bufsize;
    char*                 d_buffer;
      
  public:
  
    static SMARTP<IFdNStreambuf> create ();

    static SMARTP<IFdNStreambuf> create (
      int fd, size_t bufsize = 1);

    IFdNStreambuf ();
    IFdNStreambuf (
      int fd, size_t bufsize = 1);
    
    virtual ~IFdNStreambuf ();
    
    void                  open (int fd, size_t bufsize = 1);
      
  private:
  
    virtual int           underflow ();
    
    virtual std::streamsize
                          xsgetn (char *dest, std::streamsize n);
};
typedef SMARTP<IFdNStreambuf> S_IFdNStreambuf;
#endif

//______________________________________________________________________________
#ifdef WIN32
  // JMI
#else
// see: http://www.icce.rug.nl/documents/cplusplus/
class OFdnStreambuf: public std::streambuf, public smartable
{
  private:
  
    size_t                d_bufsize;
    int                   d_fd;
    char*                 d_buffer;

  public:
  
    OFdnStreambuf ();
    OFdnStreambuf (int fd, size_t bufsize = 1);
    
    virtual ~OFdnStreambuf ();
    
    void                  open (int fd, size_t bufsize = 1);
      
  private:
  
    virtual int           sync ();
    virtual int           overflow (int c);
};
typedef SMARTP<OFdnStreambuf> S_OFdnStreambuf;
#endif


} // namespace MusicXML2


#endif
