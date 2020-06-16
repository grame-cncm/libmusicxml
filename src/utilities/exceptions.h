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
      : fExceptionDescription (exceptionDescription),
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

    // services
    // ------------------------------------------------------

    virtual const char*   what () const throw ()
      {
        stringstream s;

        s <<
          "msrException: " <<
          fExceptionDescription;

        string theString    = s.str ();
        int    thetringSize = theString.size ();

        char * result = new char [thetringSize + 1];
        assert (result != nullptr);

        strcpy (result, theString.c_str ());
        result [thetringSize] = '\0';

        return result;
      }

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

//______________________________________________________________________________
class mxmlTreeToMsrException: public msrException
{
  public:
    mxmlTreeToMsrException (string const& exceptionDescription) throw ()
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

//______________________________________________________________________________
class lpsrMusicXMLException: public msrException
{
  public:
    lpsrMusicXMLException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class msrScoreToLpsrScoreException: public msrException
{
  public:
    msrScoreToLpsrScoreException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class lpsrScoreToLilypondException : public msrException
{
  public:
    lpsrScoreToLilypondException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class msrScoreToMusicXMLScoreException : public msrException
{
  public:
    msrScoreToMusicXMLScoreException (string const& exceptionDescription) throw ()
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

//______________________________________________________________________________
class bsrInternalException: public msrException
{
  public:
    bsrInternalException (string const& exceptionDescription) throw ()
      : msrException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class bsrScoreToFinalizedBsrScoreException : public msrException
{
  public:
    bsrScoreToFinalizedBsrScoreException (string const& exceptionDescription) throw ()
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

//______________________________________________________________________________
class bsrScoreToBrailleTextException : public msrException
{
  public:
    bsrScoreToBrailleTextException (string const& exceptionDescription) throw ()
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
