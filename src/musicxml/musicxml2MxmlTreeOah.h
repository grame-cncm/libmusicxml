/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___musicxml2MxmlTreeOah___
#define ___musicxml2MxmlTreeOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class musicxml2MxmlTreeOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<musicxml2MxmlTreeOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<musicxml2MxmlTreeOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicxml2MxmlTree (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    musicxml2MxmlTreeOah (
      S_oahHandler handlerUpLink);

    virtual ~musicxml2MxmlTreeOah ();

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
    void                  initializeMxmlTreeTraceOah (
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

    void                  printMusicxml2MxmlTreeHelp ();

    void                  printMusicxml2MxmlTreeValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

#ifdef TRACE_OAH
    // specific trace
    // --------------------------------------

    // visitors
    bool                  fTraceMusicXMLTreeVisitors;
#endif
};
typedef SMARTP<musicxml2MxmlTreeOah> S_musicxml2MxmlTreeOah;
EXP ostream& operator<< (ostream& os, const S_musicxml2MxmlTreeOah& elt);

EXP extern S_musicxml2MxmlTreeOah gMusicxml2MxmlTreeOah;
EXP extern S_musicxml2MxmlTreeOah gMusicxml2MxmlTreeOahUserChoices;
EXP extern S_musicxml2MxmlTreeOah gMusicxml2MxmlTreeOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMusicxml2MxmlTreeOahHandling (
  S_oahHandler handler);


}


#endif
