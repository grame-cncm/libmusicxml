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

#include "exports.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
EXP void displayException (exception& e, ostream& os);

//______________________________________________________________________________
class EXP msgException: public exception
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msgException (
                            string const& exceptionDescription = "",
                            int           exceptionNumber = 0,
                            int           exceptionLevel = 0
                          ) throw ()
                            : fExceptionDescription ("msgException: " + exceptionDescription),
                              fExceptionNumber (exceptionNumber),
                              fExceptionLevel (exceptionLevel)
                          {}

    virtual               ~msgException () throw ()
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
class EXP msgAssertException: public msgException
{
  public:
    msgAssertException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgOahException: public msgException
{
  public:
    msgOahException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgMusicXMLException: public msgException
{
  public:
    msgMusicXMLException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMxmlTreeToMsrException: public msgException
{
  public:
    msgMxmlTreeToMsrException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgScoreToMusicXMLScoreException : public msgException
{
  public:
    msgScoreToMusicXMLScoreException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgMsrException: public msgException
{
  public:
    msgMsrException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMsrInternalException: public msgException
{
  public:
    msgMsrInternalException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMsrUnsupportedException: public msgException
{
  public:
    msgMsrUnsupportedException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgScoreToMsrScoreException: public msgException
{
  public:
    msgScoreToMsrScoreException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgLpsrLilypondException: public msgException
{
  public:
    msgLpsrLilypondException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgScoreToLpsrScoreException: public msgException
{
  public:
    msgScoreToLpsrScoreException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgLpsrScoreToLilypondException : public msgException
{
  public:
    msgLpsrScoreToLilypondException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgLpsrMusicXMLException: public msgException
{
  public:
    msgLpsrMusicXMLException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgMsrScoreToBsrScoreException: public msgException
{
  public:
    msgMsrScoreToBsrScoreException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgBsrInternalException: public msgException
{
  public:
    msgBsrInternalException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgBsrScoreToFinalizedBsrScoreException : public msgException
{
  public:
    msgBsrScoreToFinalizedBsrScoreException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgBsrScoreToBrailleTextException : public msgException
{
  public:
    msgBsrScoreToBrailleTextException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgMsrScoreToGuidoException: public msgException
{
  public:
    msgMsrScoreToGuidoException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgMusicXMLFile2mxmlTreeException : public msgException
{
  public:
    msgMusicXMLFile2mxmlTreeException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMsrScoreToMusicXMLException : public msgException
{
  public:
    msgMsrScoreToMusicXMLException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgMsdrException : public msgException
{
  public:
    msgMsdrException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMsdrInternalException: public msgException
{
  public:
    msgMsdrInternalException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMsdlException : public msgException
{
  public:
    msgMsdlException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMsdlInternalException: public msgException
{
  public:
    msgMsdlInternalException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgBmmlException : public msgException
{
  public:
    msgBmmlException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgBmmlInternalException: public msgException
{
  public:
    msgBmmlInternalException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgBmmlFile2bmmlTreeException : public msgException
{
  public:
    msgBmmlFile2bmmlTreeException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgBmmlTreeToMsrException : public msgException
{
  public:
    msgBmmlTreeToMsrException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMsrScoreToBmmlScoreException : public msgException
{
  public:
    msgMsrScoreToBmmlScoreException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
class EXP msgMeiException : public msgException
{
  public:
    msgMeiException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMeiInternalException: public msgException
{
  public:
    msgMeiInternalException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMeiFile2meiTreeException : public msgException
{
  public:
    msgMeiFile2meiTreeException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMeiTreeToMsrException : public msgException
{
  public:
    msgMeiTreeToMsrException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

class EXP msgMsrScoreToMeiScoreException : public msgException
{
  public:
    msgMsrScoreToMeiScoreException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};

//______________________________________________________________________________
/*
class EXP msgStreamsException: public msgException
{
  public:
    msgStreamsException (string const& exceptionDescription) throw ()
      : msgException (exceptionDescription)
    {}
};
*/

}

#endif
