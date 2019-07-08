/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___musicXMLOptions___
#define ___musicXMLOptions___

#include "oahBasicTypes.h"
#include "exports.h"


namespace MusicXML2
{

//______________________________________________________________________________
class musicXMLOptions : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<musicXMLOptions> create (
      S_oahHandler oahHandler);

    SMARTP<musicXMLOptions>        createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicXMLOptions (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    musicXMLOptions (
      S_oahHandler oahHandler);

    virtual ~musicXMLOptions ();

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

    void                  setAllMusicXMLTraceOptions (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

    virtual S_oahValuedAtom
                          handleOptionsItem (
                            ostream&      os,
                            S_oahAtom item);

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OPTIONS
    void                  initializeMusicXMLTraceOptions (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeMusicXMLWorkOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLClefsKeysTimesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLCombinedOptionsOptions (
                            bool boolOptionsInitialValue);

  public:

    // print
    // ------------------------------------------------------

    void                  printMusicXMLOptionsHelp ();

    void                  printMusicXMLOptionsValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // trace
    bool                  fTraceMusicXMLTreeVisitors;

    // worktitle

    bool                  fUseFilenameAsWorkTitle;

    // clefs, keys, times

    bool                  fIgnoreRedundantClefs;
    bool                  fIgnoreRedundantKeys;
    bool                  fIgnoreRedundantTimes;

    S_oahBooleanAtom  fIgnoreRedundantClefsItem;
    S_oahBooleanAtom  fIgnoreRedundantKeysItem;
    S_oahBooleanAtom  fIgnoreRedundantTimesItem;

    // combined options, cubase

    bool                  fCubase;
    bool                  fNoCubase;

#ifdef TRACE_OPTIONS
    // specific trace

    // encoding
    bool                  fTraceEncoding;

    // divisions
    bool                  fTraceDivisions;

    // backup
    bool                  fTraceBackup;
#endif

    // to do JMI
    bool                  fLoopToMusicXML;
};
typedef SMARTP<musicXMLOptions> S_musicXMLOptions;
EXP ostream& operator<< (ostream& os, const S_musicXMLOptions& elt);

EXP extern S_musicXMLOptions gMusicXMLOptions;
EXP extern S_musicXMLOptions gMusicXMLOptionsUserChoices;
EXP extern S_musicXMLOptions gMusicXMLOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeMusicXMLOptionsHandling (
  S_oahHandler oahHandler);


}


#endif
