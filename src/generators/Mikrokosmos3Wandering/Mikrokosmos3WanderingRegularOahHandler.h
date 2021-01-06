/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___Mikrokosmos3WanderingRegularOahHandler___
#define ___Mikrokosmos3WanderingRegularOahHandler___

#include "oahRegularHandlers.h"

#include "Mikrokosmos3WanderingInsiderOahHandler.h"

namespace MusicXML2
{
//_______________________________________________________________________________
class EXP Mikrokosmos3WanderingRegularOahHandler : public oahRegularOahHandler
/*
  An Mikrokosmos3Wandering regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2gmnInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<Mikrokosmos3WanderingRegularOahHandler> create (
      string               executableName,
      string               executableAboutInformation,
      string               handlerHeader,
      S_Mikrokosmos3WanderingInsiderOahHandler
                           insiderOahHandler,
      mkkGenerateCodeKind theGenerateCodeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    Mikrokosmos3WanderingRegularOahHandler (
      string               executableName,
      string               executableAboutInformation,
      string               handlerHeader,
      S_Mikrokosmos3WanderingInsiderOahHandler
                           insiderOahHandler,
      mkkGenerateCodeKind theGenerateCodeKind);

    virtual ~Mikrokosmos3WanderingRegularOahHandler ();

  public:

    // set and get
    // ------------------------------------------------------

    // generate code kind
    void                  setGenerateCodeKind (mkkGenerateCodeKind value)
                              { fGenerateCodeKind = value; }
    mkkGenerateCodeKind  getGenerateCodeKind () const
                              { return fGenerateCodeKind; }

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

    void                  createAPIRegularGroup ();

    void                  createGenerateCodeRegularGroup ();

    void                  createOutputRegularGroup ();

    void                  createGuidoRegularGroup ();

    void                  createFilesRegularGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

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

  private:

    // private fields
    // ------------------------------------------------------

    // generate code kind
    mkkGenerateCodeKind     fGenerateCodeKind;
};
typedef SMARTP<Mikrokosmos3WanderingRegularOahHandler> S_Mikrokosmos3WanderingRegularOahHandler;
EXP ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingRegularOahHandler& elt);


}


#endif
