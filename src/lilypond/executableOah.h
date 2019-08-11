/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___executableOah___
#define ___executableOah___

#include <set>

#include "exports.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class executableOah : public oahGroup
{
  public:

    static SMARTP<executableOah> create (
      string       executableName,
      S_oahHandler handlerUpLink);

    SMARTP<executableOah> createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeExecutableOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    executableOah (
      string       executableName,
      S_oahHandler handlerUpLink);

    virtual ~executableOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeOahBasicHelpOptions (
                            bool boolOptionsInitialValue);

    void                  initializeExecutableOahAndArgumentsOptions (
                            bool boolOptionsInitialValue);

#ifdef TRACE_OAH
    void                  initializeOptionsTraceAndDisplayOptions (
                            bool boolOptionsInitialValue);
#endif

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printExecutableOahHelp ();

    void                  printExecutableOahValues (int valueFieldWidth);

  public:

    // command line
    // --------------------------------------

    string                fHandlerExecutableName;

    string                fInputSourceName;

    // options and arguments
    // --------------------------------------

    bool                  fShowOptionsAndArguments;

    string                fCommandLineWithShortOptionsNames;
    string                fCommandLineWithLongOptionsNames;

    // options and arguments
    // --------------------------------------

    string                fOptionName;

    // options trace and display
    // --------------------------------------

#ifdef TRACE_OAH
    bool                  fTraceOah;
    bool                  fTraceOahDetails;

    bool                  fTraceOahVisitors;

    bool                  fDisplayOahValues;
    bool                  fDisplayOptionsHandler;
#endif
};
typedef SMARTP<executableOah> S_executableOah;
EXP ostream& operator<< (ostream& os, const S_executableOah& elt);

EXP extern S_executableOah gExecutableOah;
EXP extern S_executableOah gExecutableOahUserChoices;
EXP extern S_executableOah gExecutableOahWithDetailedTrace;

//______________________________________________________________________________
void initializeExecutableOahHandling (
  string       executableName,
  S_oahHandler handler);


}


#endif
