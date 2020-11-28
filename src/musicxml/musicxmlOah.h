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
class musicxmlOahGroup : public oahGroup
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

    virtual void          checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    // specific trace
    // --------------------------------------

    // encoding
    bool                  getTraceEncoding () const
                              { return fTraceEncoding; }

    // divisions
    bool                  getTraceDivisions () const
                              { return fTraceDivisions; }

    // backup
    bool                  getTraceBackup () const
                              { return fTraceBackup; }

    // forward
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

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMusicxmlHelp ();

    void                  printMusicxmlValues (int fieldWidth);

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
S_musicxmlOahGroup createGlobalMusicxmlOahGroup ();


}


#endif
