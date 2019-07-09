/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahBasicOptions___
#define ___oahBasicOptions___

#include <set>

#include "exports.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class oahBasicOptions : public oahGroup
{
  public:

    static SMARTP<oahBasicOptions> create (
      string       executableName,
      S_oahHandler handler);

    SMARTP<oahBasicOptions>        createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeOahBasicOptions (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahBasicOptions (
      string       executableName,
      S_oahHandler handler);

    virtual ~oahBasicOptions ();

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

    virtual S_oahValuedAtom
                          handleAtom (
                            ostream&  os,
                            S_oahAtom item);

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeOahBasicHelpOptions (
                            bool boolOptionsInitialValue);

    void                  initializeOahBasicOptionsAndArgumentsOptions (
                            bool boolOptionsInitialValue);

#ifdef TRACE_OPTIONS
    void                  initializeOptionsTraceAndDisplayOptions (
                            bool boolOptionsInitialValue);
#endif

  public:

    // print
    // ------------------------------------------------------

    void                  printOahBasicOptionsHelp ();

    void                  printOahBasicOptionsValues (int valueFieldWidth);

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

#ifdef TRACE_OPTIONS
    bool                  fTraceOptions;
    bool                  fTraceOptionsDetails;

    bool                  fDisplayOptionsValues;
    bool                  fDisplayOptionsHandler;
#endif
};
typedef SMARTP<oahBasicOptions> S_oahBasicOptions;
EXP ostream& operator<< (ostream& os, const S_oahBasicOptions& elt);

EXP extern S_oahBasicOptions gOahBasicOptions;
EXP extern S_oahBasicOptions gOahBasicOptionsUserChoices;
EXP extern S_oahBasicOptions gOahBasicOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeOahBasicOptionsHandling (
  string       executableName,
  S_oahHandler handler);


}


#endif
