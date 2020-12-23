/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __exceptions__
#define __exceptions__

#include <string>
#include <sstream>
#include <exception>

#include <cassert>
#include <cstring>


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class msrException: public exception
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrException (
      string const& exceptionDescription = "",
      int           exceptionNumber = 0,
      int           exceptionLevel = 0
    ) throw ()
      : fExceptionDescription ("msrException: " + exceptionDescription),
        fExceptionNumber (exceptionNumber),
        fExceptionLevel (exceptionLevel)
    {}

    virtual ~msrException () throw ()
      {}

  public:

    // set and get
    // ------------------------------------------------------

    int                   getExceptionNumber () const throw ()
                              { return fExceptionNumber; }

    int                   getExceptionLevel () const throw ()
                              { return fExceptionLevel; }

  public:

    // public services
    // ------------------------------------------------------

    virtual const char*   what () const throw ()
                              { return fExceptionDescription.c_str (); }

  private:

    string                fExceptionDescription;

    int                   fExceptionNumber;
    int                   fExceptionLevel;
};

//______________________________________________________________________________
class msrAssertException: public msrException
{
  public:
    msrAssertException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class msrOahException: public msrException
{
  public:
    msrOahException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class msrMusicXMLException: public msrException
{
  public:
    msrMusicXMLException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class mxmlTreeToMsrException: public msrException
{
  public:
    mxmlTreeToMsrException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class msrScoreToMusicXMLScoreException : public msrException
{
  public:
    msrScoreToMusicXMLScoreException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class msrMsrException: public msrException
{
  public:
    msrMsrException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class msrMsrInternalException: public msrException
{
  public:
    msrMsrInternalException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class msrMsrUnsupportedException: public msrException
{
  public:
    msrMsrUnsupportedException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class msrScoreToMsrScoreException: public msrException
{
  public:
    msrScoreToMsrScoreException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class lpsrLilypondException: public msrException
{
  public:
    lpsrLilypondException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class msrScoreToLpsrScoreException: public msrException
{
  public:
    msrScoreToLpsrScoreException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class lpsrScoreToLilypondException : public msrException
{
  public:
    lpsrScoreToLilypondException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class lpsrMusicXMLException: public msrException
{
  public:
    lpsrMusicXMLException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class msrScoreToBsrScoreException: public msrException
{
  public:
    msrScoreToBsrScoreException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class bsrInternalException: public msrException
{
  public:
    bsrInternalException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class bsrScoreToFinalizedBsrScoreException : public msrException
{
  public:
    bsrScoreToFinalizedBsrScoreException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class bsrScoreToBrailleTextException : public msrException
{
  public:
    bsrScoreToBrailleTextException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class musicXMLFile2mxmlTreeException : public msrException
{
  public:
    musicXMLFile2mxmlTreeException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class msrScoreToMusicXMLException : public msrException
{
  public:
    msrScoreToMusicXMLException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class msplException : public msrException
{
  public:
    msplException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class msplInternalException: public msrException
{
  public:
    msplInternalException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class bmmlException : public msrException
{
  public:
    bmmlException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class bmmlInternalException: public msrException
{
  public:
    bmmlInternalException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class bmmlFile2bmmlTreeException : public msrException
{
  public:
    bmmlFile2bmmlTreeException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class bmmlTreeToMsrException : public msrException
{
  public:
    bmmlTreeToMsrException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class msrScoreToBmmlScoreException : public msrException
{
  public:
    msrScoreToBmmlScoreException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class meiException : public msrException
{
  public:
    meiException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class meiInternalException: public msrException
{
  public:
    meiInternalException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class meiFile2meiTreeException : public msrException
{
  public:
    meiFile2meiTreeException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class meiTreeToMsrException : public msrException
{
  public:
    meiTreeToMsrException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

class msrScoreToMeiScoreException : public msrException
{
  public:
    msrScoreToMeiScoreException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
/*
class msrStreamsException: public msrException
{
  public:
    msrStreamsException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};
*/

}

#endif
