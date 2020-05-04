/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsr2BrailleOah___
#define ___bsr2BrailleOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


//#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsr2BrailleOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsr2BrailleOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<bsr2BrailleOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeBsr2BrailleOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsr2BrailleOah (
      S_oahHandler handlerUpLink);

    virtual ~bsr2BrailleOah ();

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

    void                  printBsr2BrailleOahHelp ();

    void                  printBsr2BrailleOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<bsr2BrailleOah> S_bsr2BrailleOah;
EXP ostream& operator<< (ostream& os, const S_bsr2BrailleOah& elt);

EXP extern S_bsr2BrailleOah gBsr2BrailleOah;
EXP extern S_bsr2BrailleOah gBsr2BrailleOahUserChoices;
EXP extern S_bsr2BrailleOah gBsr2BrailleOahWithDetailedTrace;

//______________________________________________________________________________
void initializeBsr2BrailleOahHandling (
  S_oahHandler handler);


}


#endif
