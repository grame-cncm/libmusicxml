/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyRegularOahHandler___
#define ___xml2lyRegularOahHandler___

#include "oahRegularHandlers.h"

#include "xml2lyInsiderOahHandler.h"


namespace MusicXML2
{
//_______________________________________________________________________________
class EXP xml2lyRegularOahHandler : public oahRegularOahHandler
/*
  An xml2ly regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2lyInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyRegularOahHandler> create (
      string      executableName,
      string      handlerHeader,
      S_xml2lyInsiderOahHandler
                  insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyRegularOahHandler (
      string      executableName,
      string      handlerHeader,
      S_xml2lyInsiderOahHandler
                  insiderOahHandler);

    virtual ~xml2lyRegularOahHandler ();

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

    void                  createPartsRegularGroup ();
    void                  createStavesRegularGroup ();
    void                  createVoicesRegularGroup ();

    void                  createTranspositionsRegularGroup ();

    void                  createClefsRegularGroup ();
    void                  createKeysRegularGroup ();
    void                  createTimesRegularGroup ();

    void                  createRepeatsRegularGroup ();

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

    void                  createStringsRegularGroup ();

    void                  createTablaturesRegularGroup ();

    void                  createHarmoniesRegularGroup ();
    void                  createFiguredBassesRegularGroup ();

    void                  createBookRegularGroup ();
    void                  createHeaderRegularGroup ();
    void                  createPaperRegularGroup ();
    void                  createLayoutRegularGroup ();

    void                  createOutputRegularGroup ();

    void                  createMidiRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyRegularOahHandler> S_xml2lyRegularOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2lyRegularOahHandler& elt);


}


#endif
