/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___musicXMLOah___
#define ___musicXMLOah___

#include "oahBasicTypes.h"
#include "exports.h"


namespace MusicXML2
{

//______________________________________________________________________________
class musicXMLOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<musicXMLOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<musicXMLOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicXMLOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    musicXMLOah (
      S_oahHandler handlerUpLink);

    virtual ~musicXMLOah ();

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

    void                  initializeMusicXMLWorkTitleOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLClefsKeysTimesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLDynamicsandWedgesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMusicXMLCombinedOptionsOptions (
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

    void                  printMusicXMLOahHelp ();

    void                  printMusicXMLOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // trace
    bool                  fTraceMusicXMLTreeVisitors;

    // worktitle

    bool                  fUseFilenameAsWorkTitle;

    // dynamics and wedges

    bool                  fAllDynamicsBelow;
    bool                  fAllWedgesBelow;

    // clefs, keys, times

    bool                  fIgnoreRedundantClefs;
    bool                  fIgnoreRedundantKeys;
    bool                  fIgnoreRedundantTimes;

    S_oahBooleanAtom      fIgnoreRedundantClefsAtom;
    S_oahBooleanAtom      fIgnoreRedundantKeysAtom;
    S_oahBooleanAtom      fIgnoreRedundantTimesAtom;

    // combined options, cubase

    bool                  fCubase;
    bool                  fNoCubase;

#ifdef TRACE_OAH
    // specific trace JMI move to traceOah

    // encoding
    bool                  fTraceEncoding;

    // divisions
    bool                  fTraceDivisions;

    // backup
    bool                  fTraceBackup;

    // forward
    bool                  fTraceForward;
#endif

    // to do JMI
    bool                  fLoopToMusicXML;
};
typedef SMARTP<musicXMLOah> S_musicXMLOah;
EXP ostream& operator<< (ostream& os, const S_musicXMLOah& elt);

EXP extern S_musicXMLOah gMusicXMLOah;
EXP extern S_musicXMLOah gMusicXMLOahUserChoices;
EXP extern S_musicXMLOah gMusicXMLOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMusicXMLOahHandling (
  S_oahHandler handler);


}


#endif
