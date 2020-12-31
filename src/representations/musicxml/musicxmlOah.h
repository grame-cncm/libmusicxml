/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___musicxmlOah___
#define ___musicxmlOah___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP musicxmlOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<musicxmlOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicxmlOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    musicxmlOahGroup ();

    virtual ~musicxmlOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    // specific trace
    // --------------------------------------

    // encoding
    void                  setTraceEncoding ()
                              { fTraceEncoding = true; }
    bool                  getTraceEncoding () const
                              { return fTraceEncoding; }

    // divisions
    void                  setTraceDivisions ()
                              { fTraceDivisions = true; }
    bool                  getTraceDivisions () const
                              { return fTraceDivisions; }

    // backup
    void                  setTraceBackup ()
                              { fTraceBackup = true; }
    bool                  getTraceBackup () const
                              { return fTraceBackup; }

    // forward
    void                  setTraceForward ()
                              { fTraceForward = true; }
    bool                  getTraceForward () const
                              { return fTraceForward; }
#endif

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMusicxmlTraceOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMusicxmlHelp ();

    void                  printMusicxmlValues (unsigned int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    // specific trace
    // --------------------------------------

    // encoding
    bool                  fTraceEncoding;

    // divisions
    bool                  fTraceDivisions;

    // backup
    bool                  fTraceBackup;

    // forward
    bool                  fTraceForward;
#endif
};
typedef SMARTP<musicxmlOahGroup> S_musicxmlOahGroup;
EXP ostream& operator<< (ostream& os, const S_musicxmlOahGroup& elt);

EXP extern S_musicxmlOahGroup gGlobalMusicxmlOahGroup;

//______________________________________________________________________________
EXP S_musicxmlOahGroup createGlobalMusicxmlOahGroup ();


}


#endif
