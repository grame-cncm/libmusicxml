/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2guidoRegularOahHandler___
#define ___msr2guidoRegularOahHandler___

#include "oahRegularHandlers.h"

#include "msr2guidoInsiderOahHandler.h"

namespace MusicXML2
{
//_______________________________________________________________________________
class EXP msr2guidoRegularOahHandler : public oahRegularOahHandler
/*
  An msr2guido regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalMsr2guidoInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2guidoRegularOahHandler> create (
      string      executableName,
      string      handlerHeader,
      S_msr2guidoInsiderOahHandler
                  insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2guidoRegularOahHandler (
      string      executableName,
      string      handlerHeader,
      S_msr2guidoInsiderOahHandler
                  insiderOahHandler);

    virtual ~msr2guidoRegularOahHandler ();

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArgumentsFromArgcAndArgv () const override;

    string                fetchOutputFileNameFromTheOptions () const override
                              {
                                return
                                  fInsiderOahHandler->
                                    fetchOutputFileNameFromTheOptions ();
                              }

    // quiet mode
    void                  enforceHandlerQuietness () override
                              {
                                fInsiderOahHandler->
                                  checkHandlerOptionsConsistency ();
                              }

    // consistency check
    void                  checkHandlerOptionsConsistency () override
                              {
                                fInsiderOahHandler->
                                  checkHandlerOptionsConsistency ();
                              }

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    // regular handler groups creation

    void                  createRegularHandlerGroups () override;

    // the groups themselves

    void                  createInformationsRegularGroup ();

    void                  createFilesRegularGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

    void                  createInputRegularGroup ();

    void                  createPresentationRegularGroup ();

    void                  createPartsRegularGroup ();
    void                  createStavesRegularGroup ();
    void                  createVoicesRegularGroup ();

    void                  createClefsRegularGroup ();
    void                  createKeysRegularGroup ();
    void                  createTimesRegularGroup ();

    void                  createMeasuresRegularGroup ();

    void                  createRestsRegularGroup ();
    void                  createNotesRegularGroup ();

    void                  creatBeamsRegularGroup ();

    void                  createArticulationsRegularGroup ();

    void                  createOrnamentsRegularGroup ();

    void                  createGraceNotesRegularGroup ();

    void                  createChordsRegularGroup ();

    void                  createTiesRegularGroup ();
    void                  createSlursRegularGroup ();
    void                  createLigaturesRegularGroup ();

    void                  createDynamicsRegularGroup ();
    void                  createWedgesRegularGroup ();

    void                  createTupletsRegularGroup ();

    void                  createLyricsRegularGroup ();

    void                  createHarmoniesRegularGroup ();
    void                  createFiguredBassesRegularGroup ();

    void                  createOutputRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msr2guidoRegularOahHandler> S_msr2guidoRegularOahHandler;
EXP ostream& operator<< (ostream& os, const S_msr2guidoRegularOahHandler& elt);


}


#endif
