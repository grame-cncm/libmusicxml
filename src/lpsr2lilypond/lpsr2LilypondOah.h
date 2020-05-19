/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsr2LilyPondOah___
#define ___lpsr2LilyPondOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


//#include "msrBasicTypes.h"


namespace MusicXML2
{
//______________________________________________________________________________
class lpsr2LilyPondOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lpsr2LilyPondOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<lpsr2LilyPondOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLpsr2LilyPondOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsr2LilyPondOah (
      S_oahHandler handlerUpLink);

    virtual ~lpsr2LilyPondOah ();

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

    void                  setAllMusicXMLTraceOah (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeMusicXMLTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeMusicXMLHeaderOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLClefsKeysTimesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMMeasuresOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLWordsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLDynamicsAndWedgesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLCombinedOptionsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLLoopOptions (
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

    void                  printLpsr2LilyPondOahHelp ();

    void                  printLpsr2LilyPondOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<lpsr2LilyPondOah> S_lpsr2LilyPondOah;
EXP ostream& operator<< (ostream& os, const S_lpsr2LilyPondOah& elt);

EXP extern S_lpsr2LilyPondOah gLpsr2LilyPondOah;
EXP extern S_lpsr2LilyPondOah gLpsr2LilyPondOahUserChoices;
EXP extern S_lpsr2LilyPondOah gLpsr2LilyPondOahWithDetailedTrace;

//______________________________________________________________________________
void initializeLpsr2LilyPondOahHandling (
  S_oahHandler handler);


}


#endif
