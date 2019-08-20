/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahOah___
#define ___oahOah___

#include <set>

#include "exports.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class oahOah : public oahGroup
{
  public:

    static SMARTP<oahOah> create (
      string       executableName,
      S_oahHandler handlerUpLink);

    SMARTP<oahOah> createCloneWithTrueValues (); // JMI

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeOahOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOah (
      string       executableName,
      S_oahHandler handlerUpLink);

    virtual ~oahOah ();

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

    void                  initializeOahOahAndArgumentsOptions (
                            bool boolOptionsInitialValue);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printOahOahHelp ();

    void                  printOahOahValues (int valueFieldWidth);

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

    // option name
    // --------------------------------------

    string                fOptionName; // JMI

    // options and help display
    // --------------------------------------

    bool                  fDisplayOahValues;
    bool                  fDisplayOptionsHandler;
};
typedef SMARTP<oahOah> S_oahOah;
EXP ostream& operator<< (ostream& os, const S_oahOah& elt);

EXP extern S_oahOah gOahOah;
EXP extern S_oahOah gOahOahUserChoices;
EXP extern S_oahOah gOahOahWithDetailedTrace;

//______________________________________________________________________________
void initializeOahOahHandling (
  string       executableName,
  S_oahHandler handler);


}


#endif
